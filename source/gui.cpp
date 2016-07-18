#include <sfil.h>

#include <cmath>
#include <ctime>
#include <chrono>
#include <algorithm>
#include <cstdlib>
#include <cstdio>
#include <sstream>
#include <dirent.h> 
#include <fstream>

#include "gui.h"
#include "input.h"
#include "tinyxml2/tinyxml2.h"

using namespace tinyxml2;

// https://gbatemp.net/threads/wip-ebook-reader-read-ebooks-on-your-3ds-alpha-1.434453/
// https://www.reddit.com/r/3dshacks/comments/4sze79/wip_ebook_reader_alpha_1_release/
// https://github.com/litehtml/litehtml/wiki/How-to-use-litehtml
// https://github.com/litehtml/litehtml/blob/master/containers/linux/container_linux.h
// https://github.com/litehtml/litehtml/blob/master/containers/linux/container_linux.cpp
// https://github.com/litehtml/litebrowser-linux/blob/master/src/html_widget.h
// https://github.com/litehtml/litebrowser-linux/blob/master/src/html_widget.cpp
// https://github.com/litehtml/litebrowser-linux/blob/master/src/main.cpp



// Thanks to http://stackoverflow.com/a/6417908
std::string Gui::remove_extension(const std::string& filename)
{
    size_t lastdot = filename.find_last_of(".");
    if (lastdot == std::string::npos) return filename;
    return filename.substr(0, lastdot); 
}

std::string Gui::get_extension(const std::string& filename)
{
    return filename.substr(filename.find_last_of(".")); 
}

template<typename T>
std::string to_string(const T& value)
{
    std::ostringstream oss;
    oss << value;
    return oss.str();
}

void Gui::Load()
{
	m_TextFont = sftd_load_font_file("romfs:/font/SourceCodePro-Regular.ttf");
	m_Font = sftd_load_font_file("romfs:/font/LiberationSans-Regular.ttf");
	m_Next = sfil_load_PNG_file("romfs:/NextFM.png", SF2D_PLACE_RAM);
	m_Prev = sfil_load_PNG_file("romfs:/PrevFM.png", SF2D_PLACE_RAM);
	m_Top = sfil_load_PNG_file("romfs:/top.png", SF2D_PLACE_RAM);
	m_Bottom = sfil_load_PNG_file("romfs:/bottom.png", SF2D_PLACE_RAM);
	m_Controls = sfil_load_PNG_file("romfs:/controls.png", SF2D_PLACE_RAM);
	m_Exit = sfil_load_PNG_file("romfs:/exit.png", SF2D_PLACE_RAM);
	m_Charging = sfil_load_PNG_file("romfs:/BatteryCharge.png", SF2D_PLACE_RAM);
	m_About = sfil_load_PNG_file("romfs:/about.png", SF2D_PLACE_RAM);
	m_TextBG = sfil_load_PNG_file("romfs:/text.png", SF2D_PLACE_RAM);
	m_BookmarkedBG = sfil_load_PNG_file("romfs:/bookmarked.png", SF2D_PLACE_RAM);
	m_Back = sfil_load_PNG_file("romfs:/back.png", SF2D_PLACE_RAM);

	m_BatteryLevels.push_back(sfil_load_PNG_file("romfs:/BatteryEmpty.png", SF2D_PLACE_RAM));
	m_BatteryLevels.push_back(sfil_load_PNG_file("romfs:/BatteryLowest.png", SF2D_PLACE_RAM));
	m_BatteryLevels.push_back(sfil_load_PNG_file("romfs:/BatteryLow.png", SF2D_PLACE_RAM));
	m_BatteryLevels.push_back(sfil_load_PNG_file("romfs:/BatteryMid.png", SF2D_PLACE_RAM));
	m_BatteryLevels.push_back(sfil_load_PNG_file("romfs:/BatteryHigh.png", SF2D_PLACE_RAM));
	m_BatteryLevels.push_back(sfil_load_PNG_file("romfs:/BatteryFull.png", SF2D_PLACE_RAM));

	DIR *dir;
	struct dirent *ent;
	
	dir = opendir("/books");

	while ((ent = readdir (dir)) != nullptr	)
	{
	    if (get_extension(ent->d_name) == ".epub")
	    {
	    	files.push_back(ent->d_name);
	    }
	}
	
	closedir (dir);
}

void Gui::Close()
{
	sftd_free_font(m_Font);
	sftd_free_font(m_TextFont);
	sf2d_free_texture(m_Next);
	sf2d_free_texture(m_Prev);
	sf2d_free_texture(m_Controls);
	sf2d_free_texture(m_Top);
	sf2d_free_texture(m_Bottom);
	sf2d_free_texture(m_Exit);
	sf2d_free_texture(m_Charging);
	sf2d_free_texture(m_About);
	sf2d_free_texture(m_TextBG);
	sf2d_free_texture(m_BookmarkedBG);
	sf2d_free_texture(m_Back);

	for(auto& v : m_BatteryLevels) {
		sf2d_free_texture(v);
	}
}

void Gui::HandleEventsMenu(Input& input, Renderer& ren)
{
	if (loading)
	{
		if (selected != "")
		{
			OpenBook(selected);
			loading = false;
			input.curMode = 1;
			drawAbout = false;
		}
	}

	if (input.m_kDown & KEY_UP) { m_Index--; }
	if (input.m_kDown & KEY_DOWN) { m_Index++; }
	if (input.m_kDown & KEY_LEFT) { m_curPage--; m_Index = 0; }
	if (input.m_kDown & KEY_RIGHT) { m_curPage++; m_Index = 0; }
	
	// correct values
	if (m_Index < 0) m_Index = 0;
	if (m_Index > 6) m_Index = 6;
	if (m_curPage < 0) m_curPage = 0;

	if (input.m_kDown & KEY_X) { RemoveBook(files[m_Index+(7*m_curPage)]); }

	if (input.m_kDown & KEY_A) { 
		selected = files[m_Index+(7*m_curPage)]; 
		if (selected != "") {
			loading = true;
		}
	}

	if (input.m_PosX >= 159 && input.m_PosX <= 320 && input.m_PosY >= 217 && input.m_PosY <= 241) {
		if (drawAbout == true) {
			drawAbout = false;
		} else {
			drawAbout = true;
		}
	}

	if (input.m_PosX >= 295 && input.m_PosX <= 320 && input.m_PosY >= 65 && input.m_PosY <= 137) {
		m_curPage++; 
		m_Index = 0;
	}

	if (input.m_PosX >= 9 && input.m_PosX <= 31 && input.m_PosY >= 65 && input.m_PosY <= 137) {
		m_curPage--; 
		m_Index = 0;
	}

	if (input.m_PosX >= 0 && input.m_PosX <= 158 && input.m_PosY >= 217 && input.m_PosY <= 241) {
		input.running = false;
	}

	if (input.m_PosX >= 1 && input.m_PosX <= 20 && input.m_PosY >= 1 && input.m_PosY <= 20) {
		if (ren.draw3D == true) {
			ren.draw3D = false;
		} else {
			ren.draw3D = true;
		}
	}
}

void Gui::HandleEventsBook(Input& input)
{
	if (!showBookmarks)
	{
		if (bookmarked)
		{
			if ((input.m_PosX >= 20 && input.m_PosX <= 300 && input.m_PosY >= 20 && input.m_PosY <= 220) || (input.m_kDown & KEY_SELECT) ) {
				bookmarked = false;
			}
		}
		else
		{
			if (input.m_kDown & KEY_LEFT) { m_BookPage--; }
			if (input.m_kDown & KEY_RIGHT) { m_BookPage++; }

			if (m_BookPage < 0) { m_BookPage = 0; }

			if (input.m_PosX >= 20 && input.m_PosX <= 103 && input.m_PosY >= 18 && input.m_PosY <= 185) {
				input.curMode = 0;
				CloseBook();
				selected = "";
				m_BookPage = 0;
			}

			if (input.m_PosX >= 120 && input.m_PosX <= 203 && input.m_PosY >= 18 && input.m_PosY <= 185) {
				LoadBookmark();
			}

			if ((input.m_PosX >= 219 && input.m_PosX <= 302 && input.m_PosY >= 18 && input.m_PosY <= 185) || (input.m_kDown & KEY_SELECT) ) {
				bookmarked = true;
				SaveBookmark();
			}
		}	
	}
	else
	{
		if (input.m_kDown & KEY_UP) { m_IndexBookmark--; }
		if (input.m_kDown & KEY_DOWN) { m_IndexBookmark++; }
		if (input.m_kDown & KEY_LEFT) { m_curPageBookmark--; m_IndexBookmark = 0; }
		if (input.m_kDown & KEY_RIGHT) { m_curPageBookmark++; m_IndexBookmark = 0; }
		
		// correct values
		if (m_IndexBookmark < 0) m_IndexBookmark = 0;
		if (m_IndexBookmark > 6) m_IndexBookmark = 6;
		if (m_curPageBookmark < 0) m_curPageBookmark = 0;

		if (input.m_kDown & KEY_X) { RemoveBookmark(m_IndexBookmark+(7*m_curPageBookmark)); }

		if (input.m_kDown & KEY_A) { 
			m_BookPage = bookmarkedPages[m_IndexBookmark+(7*m_curPageBookmark)]; 
		}

		if (input.m_PosX >= 159 && input.m_PosX <= 320 && input.m_PosY >= 217 && input.m_PosY <= 241) {
			if (drawAbout == true) {
				drawAbout = false;
			} else {
				drawAbout = true;
			}
		}

		if (input.m_PosX >= 295 && input.m_PosX <= 320 && input.m_PosY >= 65 && input.m_PosY <= 137) {
			m_curPageBookmark++; 
			m_IndexBookmark = 0;
		}

		if (input.m_PosX >= 9 && input.m_PosX <= 31 && input.m_PosY >= 65 && input.m_PosY <= 137) {
			m_curPageBookmark--; 
			m_IndexBookmark = 0;
		}

		if (input.m_PosX >= 0 && input.m_PosX <= 158 && input.m_PosY >= 217 && input.m_PosY <= 241) {
			showBookmarks = false;
		}
	}
}

void Gui::Update()
{
	PTMU_GetBatteryLevel(&batteryLevel);
	PTMU_GetBatteryChargeState(&charging);
}

void Gui::DrawFileSelect(Renderer& ren)
{
	sf2d_draw_texture(m_Bottom, 0, 0);
	sf2d_draw_texture(m_Exit, 0, 217);

	if (!loading)
	{
		int pos = 20;
	
		if (m_curPage == 0) {
			begin = 0;
			end = 7; 
		} else {
			begin = (7*m_curPage);
			end = (7*m_curPage) + 6;
		}

		if (end > files.size()) {
			end = files.size();
		}

		for (begin = begin; begin < end; ++begin) {
			sftd_draw_text(m_Font, 57, pos, RGBA8(0, 0, 0 ,255), 12, files[begin].c_str());
			pos += 20;
		}

		sftd_draw_text(m_Font, 45, (m_Index + 1) * 20, RGBA8(0, 0, 0, 255), 12, "->");
		sf2d_draw_texture(m_Next, 295, 65);

		if (m_curPage > 0) {
			sf2d_draw_texture(m_Prev, 9, 65);
		}
	}
	else
	{
		sftd_draw_text(m_Font, (320 / 2) - ( sftd_get_text_width(m_Font, 12, "Loading...") / 2), 100, RGBA8(0, 0, 0, 255), 12, "Loading...");
	}

	if (ren.draw3D == true)
	{
		sftd_draw_text(m_Font, 0, 0, RGBA8(255, 0, 0, 255), 14, "3D");
	}
	else
	{
		sftd_draw_text(m_Font, 0, 0, RGBA8(0, 0, 0, 255), 14, "3D");
	}
}

std::string Gui::getSelected()
{
	return selected;
}

void Gui::DrawTopBackground()
{
	if (drawAbout) {
		sf2d_draw_texture(m_About, 0, 0);
	} else {
		sf2d_draw_texture(m_Top, 0, 0);
	}
}

void Gui::DrawStatusScreen()
{
	// Battery icon
	if (charging)
		sf2d_draw_texture(m_Charging, 0, 0);
	else 
		sf2d_draw_texture(m_BatteryLevels[batteryLevel], 0, 0);

	// clock
	sftd_draw_text (m_Font, 299, 2, RGBA8(0, 0, 0, 255), 12, clock().c_str());

	// Title - Name of the book!
    if (!drawAbout) {
    	sftd_draw_text (m_Font, 20, 2, RGBA8(0, 0, 0, 255), 12, remove_extension(selected).c_str());
    }
}

void Gui::OpenBook(const std::string& bookName)
{
	std::string fullBook = "/books/"+bookName;

	book.LoadBook(fullBook.c_str());
}

void Gui::CloseBook()
{
	book.CloseBook();
}

void Gui::DrawTextBG()
{
	sf2d_draw_texture(m_TextBG, 0, 0);
}

void Gui::DrawBook(Gui& gui, Renderer& ren)
{
	if (!drawAbout)
	{
		book.Reader(gui, ren);
	}
	else
	{
		sf2d_draw_texture(m_About, 0, 0);
	}
}

void Gui::DrawControls()
{
	if (bookmarked)
	{
		sf2d_draw_texture(m_BookmarkedBG, 0, 0);
	}
	else if (showBookmarks)
	{
		sf2d_draw_texture(m_Bottom, 0, 0);
		sf2d_draw_texture(m_Back, 0, 217);

		int pos = 20;

		if (m_curPageBookmark == 0) {
			beginBookmark = 0;
			endBookmark = 7; 
		} else {
			beginBookmark = (7*m_curPageBookmark);
			endBookmark = (7*m_curPageBookmark) + 6;
		}

		if (endBookmark > bookmarkedPages.size()) {
			endBookmark = bookmarkedPages.size();
		}

		for (beginBookmark = beginBookmark; beginBookmark < endBookmark; ++beginBookmark) {
			std::string bob = "Page " + to_string<int>(bookmarkedPages[beginBookmark]); 
			sftd_draw_text(m_Font, 57, pos, RGBA8(0, 0, 0 ,255), 12, bob.c_str());
			pos += 20;
		}

		sftd_draw_text(m_Font, 45, (m_IndexBookmark + 1) * 20, RGBA8(0, 0, 0, 255), 12, "->");
		sf2d_draw_texture(m_Next, 295, 65);

		if (m_curPageBookmark > 0) {
			sf2d_draw_texture(m_Prev, 9, 65);
		}
	}
	else
	{
		sf2d_draw_texture(m_Controls, 0, 0);
	}
}

void Gui::LoadBookmark()
{
	bookmarkedPages.clear();

	std::ifstream in("/books/bookmarks.xml");
	std::string line, text;

	if (!in.fail())
	{
		while(std::getline(in, line))
		{
			text += line + "\n";
		}

		in.close();

		XMLDocument doc;
		doc.Parse(text.c_str());

		// get root bookmarks element
		XMLElement* root = doc.FirstChildElement("bookmarks");

		for (XMLElement* e = root->FirstChildElement("bookmark"); e != nullptr; e = e->NextSiblingElement("bookmark"))
		{
			if (e->Attribute("book") == selected)
			{
				bookmarkedPages.push_back(e->IntAttribute("page"));
			}
		}

		showBookmarks = true;
	}
	else
	{
		in.close();
	}
}

void Gui::SaveBookmark()
{	
	std::ifstream in("/books/bookmarks.xml");
	std::string line, text;

	if (!in.fail())
	{
		while(std::getline(in, line))
		{
			text += line + "\n";
		}

		in.close();

		XMLDocument doc;
		doc.Parse(text.c_str());

		// get root bookmarks element
		XMLElement* root = doc.FirstChildElement("bookmarks");

		// create a new element with the name of the book
		XMLElement* bookmarkElement = doc.NewElement("bookmark");
		bookmarkElement->SetAttribute("book", selected.c_str());

		// insert bookmark
		bookmarkElement->SetAttribute("page", m_BookPage);
		root->InsertEndChild(bookmarkElement);

		doc.SaveFile("/books/bookmarks.xml");
	}
	else
	{
		in.close();

		XMLDocument doc;

		// create a new document.
		XMLDeclaration* dec = doc.NewDeclaration();
		XMLNode* root = doc.NewElement("bookmarks");

		doc.InsertFirstChild(dec);
		doc.InsertEndChild(root);

		// create a new element with the name of the book
		XMLElement* bookmarkElement = doc.NewElement("bookmark");
		bookmarkElement->SetAttribute("book", selected.c_str());

		// insert bookmark
		bookmarkElement->SetAttribute("page", m_BookPage);
		root->InsertEndChild(bookmarkElement);

		doc.SaveFile("/books/bookmarks.xml");
	}
}

sftd_font* Gui::getTextFont()
{
	return m_TextFont;
}

int Gui::getBookPage()
{
	return m_BookPage;
}

std::string Gui::clock()
{
	std::time_t time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
	std::string temp = std::ctime(&time);
	
	// Remove pesky newlines
	temp.erase(std::remove(temp.begin(), temp.end(), '\n'), temp.end());

	// Trim string info down to MON JAN 01 HH:MM format
	temp = temp.substr(0, 16);

	return temp;
}

void Gui::RemoveBook(const std::string& file)
{
	// remove book from sd card
	std::string path = "/books/" + file;
	remove(path.c_str());

	// erase book from vector
	files.erase(std::find(files.begin(), files.end(), file));
	files.shrink_to_fit();
}

void Gui::RemoveBookmark(int element)
{
	bookmarkedPages.erase(bookmarkedPages.begin() + element);
	bookmarkedPages.shrink_to_fit();

	std::ifstream in("/books/bookmarks.xml");
	std::string line, text;

	if (!in.fail())
	{
		while(std::getline(in, line))
		{
			text += line + "\n";
		}

		in.close();

		XMLDocument doc;
		doc.Parse(text.c_str());

		// get root bookmarks element
		XMLElement* root = doc.FirstChildElement("bookmarks");

		for (XMLElement* e = root->FirstChildElement("bookmark"); e != nullptr; e = e->NextSiblingElement("bookmark"))
		{
			if (e->Attribute("book") == selected)
			{
				if (e->IntAttribute("page") == bookmarkedPages[element])
				{
					doc.DeleteNode(e);
				}
			}
		}

		doc.SaveFile("/books/bookmarks.xml");
	}
	else
	{
		in.close();
	}

}
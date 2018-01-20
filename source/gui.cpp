///
/// GUI.cpp
/// UI management class
///

#include <cmath>
#include <ctime>
#include <chrono>
#include <cstdio>
#include <sstream>
#include <fstream>
#include <cstdlib>
#include <dirent.h> 
#include <algorithm>

void GUI::load()
{
	m_next = new Texture(1, "romfs:/NextFM.png");
	m_prev = new Texture(2, "romfs:/PrevFM.png");
	m_top = new Texture(3, "romfs:/top.png");
	m_bottom = new Texture(4, "romfs:/bottom.png");
	m_controls = new Texture(5, "romfs:/controls.png");
	m_exit = new Texture(6, "romfs:/exit.png");
	m_charging = new Texture(7, "romfs:/BatteryCharge.png");
	m_about = new Texture(8, "romfs:/about.png");
	m_textBG = new Texture(9, "romfs:/text.png");
	m_bookmarkedBG = new Texture(10, "romfs:/bookmarked.png");
	m_back = new Texture(11, "romfs:/back.png");

	m_batteryLevels.push_back(new Texture(12, "romfs:/BatteryEmpty.png"));
	m_batteryLevels.push_back(new Texture(13, "romfs:/BatteryLowest.png"));
	m_batteryLevels.push_back(new Texture(14, "romfs:/BatteryLow.png"));
	m_batteryLevels.push_back(new Texture(15, "romfs:/BatteryMid.png"));
	m_batteryLevels.push_back(new Texture(16, "romfs:/BatteryHigh.png"));
	m_batteryLevels.push_back(new Texture(17, "romfs:/BatteryFull.png"));

	DIR *dir;
	struct dirent *ent;
	
	dir = opendir("/books");

	while ((ent = readdir (dir)) != nullptr	)
	{
	    if (get_extension(ent->d_name) == ".epub")
	    {
	    	m_files.push_back(ent->d_name);
	    }
	}
	
	closedir (dir);
}

void GUI::close()
{
	delete m_next;
	delete m_prev;
	delete m_controls;
	delete m_top;
	delete m_bottom;
	delete m_exit;
	delete m_charging;
	delete m_about;
	delete m_textBG;
	delete m_bookmarkedBG;
	delete m_back;

	for(auto& tex : m_batteryLevels)
	{
		delete tex;
	}
}

void Gui::HandleEventsMenu(Input& input, Renderer& ren)
{
	if (m_loading)
	{
		if (m_selected != "")
		{
			OpenBook(m_selected);
			m_loading = false;
			input.SetCurMode(AppState::Text);
			m_drawAbout = false;
		}
	}

	if (input.getKeyDown() & KEY_UP) { m_index--; }
	if (input.getKeyDown() & KEY_DOWN) { m_index++; }
	if (input.getKeyDown() & KEY_LEFT) { m_curPage--; m_index = 0; }
	if (input.getKeyDown() & KEY_RIGHT) { m_curPage++; m_index = 0; }
	
	// correct values
	if (m_index < 0) m_index = 0;
	if (m_index > 6) m_index = 6;
	if (m_curPage < 0) m_curPage = 0;

	if (input.getKeyDown() & KEY_X) { RemoveBook(m_files[m_index+(7*m_curPage)]); }

	if (input.getKeyDown() & KEY_A) { 
		m_selected = m_files[m_index+(7*m_curPage)]; 
		if (m_selected != "") {
			m_loading = true;
		}
	}

	if (input.getPosX() >= 159 && input.getPosX() <= 320 && input.getPosY() >= 217 && input.getPosY() <= 241) {
		if (m_drawAbout == true) {
			m_drawAbout = false;
		} else {
			m_drawAbout = true;
		}
	}

	if (input.getPosX() >= 295 && input.getPosX() <= 320 && input.getPosY() >= 65 && input.getPosY() <= 137) {
		m_curPage++; 
		m_index = 0;
	}

	if (input.getPosX() >= 9 && input.getPosX() <= 31 && input.getPosY() >= 65 && input.getPosY() <= 137) {
		m_curPage--; 
		m_index = 0;
	}

	if (input.getPosX() >= 0 && input.getPosX() <= 158 && input.getPosY() >= 217 && input.getPosY() <= 241) {
		input.SetRunning(false);
	}
}

void Gui::HandleEventsBook(Input& input)
{
	if (!m_showBookmarks)
	{
		if (m_bookmarked)
		{
			if ((input.getPosX() >= 20 && input.getPosX() <= 300 && input.getPosY() >= 20 && input.getPosY() <= 220) || (input.getKeyDown() & KEY_SELECT) ) {
				m_bookmarked = false;
			}
		}
		else
		{
			if (input.getKeyDown() & KEY_L) { m_bookPage--; }
			if (input.getKeyDown() & KEY_R) { m_bookPage++; }
            if (input.getKeyDown() & KEY_LEFT) { m_bookPage--; }
            if (input.getKeyDown() & KEY_RIGHT) { m_bookPage++; }

			if (m_bookPage < 0) { m_bookPage = 0; }

			if (input.getPosX() >= 20 && input.getPosX() <= 103 && input.getPosY() >= 18 && input.getPosY() <= 185) {
				input.SetCurMode(AppState::Menu);
				CloseBook();
				m_selected = "";
				m_bookPage = 0;
			}

			if (input.getPosX() >= 120 && input.getPosX() <= 203 && input.getPosY() >= 18 && input.getPosY() <= 185) {
				LoadBookmark();
			}

			if ((input.getPosX() >= 219 && input.getPosX() <= 302 && input.getPosY() >= 18 && input.getPosY() <= 185) || (input.getKeyDown() & KEY_SELECT) ) {
				m_bookmarked = true;
				SaveBookmark();
			}
		}	
	}
	else
	{
		if (input.getKeyDown() & KEY_UP) { m_indexBookmark--; }
		if (input.getKeyDown() & KEY_DOWN) { m_indexBookmark++; }
		if (input.getKeyDown() & KEY_LEFT) { m_curPageBookmark--; m_indexBookmark = 0; }
		if (input.getKeyDown() & KEY_RIGHT) { m_curPageBookmark++; m_indexBookmark = 0; }
		
		// correct values
		if (m_indexBookmark < 0) m_indexBookmark = 0;
		if (m_indexBookmark > 6) m_indexBookmark = 6;
		if (m_curPageBookmark < 0) m_curPageBookmark = 0;

		if (input.getKeyDown() & KEY_X) { RemoveBookmark(m_indexBookmark+(7*m_curPageBookmark)); }

		if (input.getKeyDown() & KEY_A) { 
			m_bookPage = m_bookmarkedPages[m_indexBookmark+(7*m_curPageBookmark)]; 
		}

		if (input.getPosX() >= 159 && input.getPosX() <= 320 && input.getPosY() >= 217 && input.getPosY() <= 241) {
			if (m_drawAbout == true) {
				m_drawAbout = false;
			} else {
				m_drawAbout = true;
			}
		}

		if (input.getPosX() >= 295 && input.getPosX() <= 320 && input.getPosY() >= 65 && input.getPosY() <= 137) {
			m_curPageBookmark++; 
			m_indexBookmark = 0;
		}

		if (input.getPosX() >= 9 && input.getPosX() <= 31 && input.getPosY() >= 65 && input.getPosY() <= 137) {
			m_curPageBookmark--; 
			m_indexBookmark = 0;
		}

		if (input.getPosX() >= 0 && input.getPosX() <= 158 && input.getPosY() >= 217 && input.getPosY() <= 241) {
			m_showBookmarks = false;
		}
	}
}

void Gui::Update()
{
	PTMU_GetBatteryLevel(&m_batteryLevel);
	PTMU_GetBatteryChargeState(&m_chargingValue);
}

void Gui::DrawFileSelect(Renderer& ren)
{
	sf2d_draw_texture(m_bottom, 0, 0);
	sf2d_draw_texture(m_exit, 0, 217);

	if (!m_loading)
	{
		int pos = 20;
	
		if (m_curPage == 0) {
			m_begin = 0;
			m_end = 7; 
		} else {
			m_begin = (7*m_curPage);
			m_end = (7*m_curPage) + 7;
		}

		if (m_end > m_files.size()) {
			m_end = m_files.size();
		}

        for (; m_begin < m_end; ++m_begin) {
			sftd_draw_text(m_font, 57, pos, RGBA8(0, 0, 0 ,255), 12, m_files[m_begin].c_str());
			pos += 20;
		}

		sftd_draw_text(m_font, 45, (m_index + 1) * 20, RGBA8(0, 0, 0, 255), 12, "->");
		sf2d_draw_texture(m_next, 295, 65);

		if (m_curPage > 0) {
			sf2d_draw_texture(m_prev, 9, 65);
		}
	}
	else
	{
		sftd_draw_text(m_font, (320 / 2) - ( sftd_get_text_width(m_font, 12, "Loading...") / 2), 100, RGBA8(0, 0, 0, 255), 12, "Loading...");
	}
}

std::string Gui::getSelected()
{
	return m_selected;
}

void Gui::DrawTopBackground()
{
	if (m_drawAbout) {
		sf2d_draw_texture(m_about, 0, 0);
	} else {
		sf2d_draw_texture(m_top, 0, 0);
	}
}

void Gui::DrawStatusScreen()
{
	// Battery icon
	if (m_charging)
		sf2d_draw_texture(m_charging, 0, 0);
	else 
		sf2d_draw_texture(m_batteryLevels[m_batteryLevel], 0, 0);

	// clock
	sftd_draw_text (m_font, 299, 2, RGBA8(0, 0, 0, 255), 12, clock().c_str());

	// Title - Name of the book!
    if (!m_drawAbout) {
    	sftd_draw_text (m_font, 20, 2, RGBA8(0, 0, 0, 255), 12, remove_extension(m_selected).c_str());
    }
}

void Gui::OpenBook(const std::string& bookName)
{
	std::string fullBook = "/books/"+bookName;
	
	m_book.LoadBook(fullBook.c_str());
}

void Gui::CloseBook()
{
	m_book.CloseBook();
}

void Gui::DrawTextBG()
{
	sf2d_draw_texture(m_textBG, 0, 0);
}

void Gui::DrawBook(Gui& gui)
{
	if (!m_drawAbout)
	{
		m_book.Reader(gui);
	}
	else
	{
		sf2d_draw_texture(m_about, 0, 0);
	}
}

void Gui::DrawControls()
{
	if (m_bookmarked)
	{
		sf2d_draw_texture(m_bookmarkedBG, 0, 0);
	}
	else if (m_showBookmarks)
	{
		sf2d_draw_texture(m_bottom, 0, 0);
		sf2d_draw_texture(m_back, 0, 217);

		int pos = 20;

		if (m_curPageBookmark == 0) {
			m_beginBookmark = 0;
			m_endBookmark = 7; 
		} else {
			m_beginBookmark = (7*m_curPageBookmark);
			m_endBookmark = (7*m_curPageBookmark) + 7;
		}

		if (m_endBookmark > m_bookmarkedPages.size()) {
			m_endBookmark = m_bookmarkedPages.size();
		}

		for (; m_beginBookmark < m_endBookmark; ++m_beginBookmark) {
			std::string bob = "Page " + to_string<int>(m_bookmarkedPages[m_beginBookmark]); 
			sftd_draw_text(m_font, 57, pos, RGBA8(0, 0, 0 ,255), 12, bob.c_str());
			pos += 20;
		}

		sftd_draw_text(m_font, 45, (m_indexBookmark + 1) * 20, RGBA8(0, 0, 0, 255), 12, "->");
		sf2d_draw_texture(m_next, 295, 65);

		if (m_curPageBookmark > 0) {
			sf2d_draw_texture(m_prev, 9, 65);
		}
	}
	else
	{
		sf2d_draw_texture(m_controls, 0, 0);
	}
}

void Gui::LoadBookmark()
{
	m_bookmarkedPages.clear();

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
			if (e->Attribute("book") == m_selected)
			{
				m_bookmarkedPages.push_back(e->IntAttribute("page"));
			}
		}

		m_showBookmarks = true;
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
		bookmarkElement->SetAttribute("book", m_selected.c_str());

		// insert bookmark
		bookmarkElement->SetAttribute("page", m_bookPage);
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
		bookmarkElement->SetAttribute("book", m_selected.c_str());

		// insert bookmark
		bookmarkElement->SetAttribute("page", m_bookPage);
		root->InsertEndChild(bookmarkElement);

		doc.SaveFile("/books/bookmarks.xml");
	}
}

sftd_font* Gui::getTextFont()
{
	return m_textFont;
}

int Gui::getBookPage()
{
	return m_bookPage;
}

void Gui::RemoveBook(const std::string& file)
{
	// remove book from sd card
	std::string path = "/books/" + file;
	remove(path.c_str());

	// erase book from vector
	m_files.erase(std::find(m_files.begin(), m_files.end(), file));
	m_files.shrink_to_fit();
}

void Gui::RemoveBookmark(int element)
{
	m_bookmarkedPages.erase(m_bookmarkedPages.begin() + element);
	m_bookmarkedPages.shrink_to_fit();

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
			if (e->Attribute("book") == m_selected)
			{
				if (e->IntAttribute("page") == m_bookmarkedPages[element])
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
std::string Gui::to_string(const T& value)
{
    std::ostringstream oss;
    oss << value;
    return oss.str();
}
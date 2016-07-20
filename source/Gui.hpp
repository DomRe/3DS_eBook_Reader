// Gui.hpp
// UI management class

#ifndef EBOOK_GUI_HPP_
#define EBOOK_GUI_HPP_

#include <3ds.h>

#include <sf2d.h>
#include <sftd.h>

#include "Book.hpp"

class Input;

class Gui
{
public:
	void Load();
	void Close();

	void HandleEventsMenu(Input& input, Renderer& ren);
	void HandleEventsBook(Input& input);
	void Update();

	// Top Screen
	void DrawTopBackground();
	void DrawStatusScreen();
	void DrawTextBG();

	// Bottom Screen
	void DrawFileSelect(Renderer& ren);

	void OpenBook(const std::string& bookName, Renderer& ren);
	void CloseBook();

	// Top Screen
	void DrawBook(Gui& gui, Renderer& ren);

	// Bottom Screen
	void DrawControls();

	std::string getSelected();

	sftd_font* getTextFont();

	int getBookPageY();
	int getBookVectorPos();

private:
	Book book;

	int m_Index = 0;
	int m_curPage = 0;
	int m_IndexBookmark = 0;
	int m_curPageBookmark = 0;
	int m_BookPageY = 10;
	int m_bookVectorPos = 0;
	unsigned int begin = 0;
	unsigned int end = 7;
	unsigned int beginBookmark = 0;
	unsigned int endBookmark = 7;
	bool drawAbout = false;
	bool loading = false;
	bool bookmarked = false;
	bool showBookmarks = false;

	sf2d_texture* m_Next;
	sf2d_texture* m_Prev;
	sf2d_texture* m_Top;
	sf2d_texture* m_Bottom;
	sf2d_texture* m_Exit;
	sf2d_texture* m_About;
	sf2d_texture* m_Controls;
	sf2d_texture* m_Charging;
	sf2d_texture* m_TextBG;
	sf2d_texture* m_BookmarkedBG;
	sf2d_texture* m_Back;
	std::vector<sf2d_texture*> m_BatteryLevels;

	sftd_font* m_Font;
	sftd_font* m_TextFont;

	std::vector<std::string> files;
	std::vector<int> bookmarkedPages;
	std::string selected = "";

	u32 wifiStatus = 0;
	u8 batteryLevel = 5;
	u8 charging = 0;

private:
	std::string clock();
	std::string remove_extension(const std::string& filename);
	std::string get_extension(const std::string& filename);
	
	void RemoveBook(const std::string& file);
	void LoadBookmark();
	void SaveBookmark();
	void RemoveBookmark(int element);
};

#endif
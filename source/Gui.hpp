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
	/*
	* IMPORTS: none
	* EXPORTS: none
	* PURPOSE: Load the gui. Basically the constructor. Loads resources, etc.
	*/
	void Load();

	/*
	* IMPORTS: none
	* EXPORTS: none
	* PURPOSE: Cleanup the gui.
	*/
	void Close();

	/*
	* IMPORTS: Input - input management object. Renderer - rendering manager object.
	* EXPORTS: none
	* PURPOSE: Handle events in the menu.
	*/
	void HandleEventsMenu(Input& input, Renderer& ren);
	
	/*
	* IMPORTS: Input - input management object.
	* EXPORTS: none
	* PURPOSE: Handle events in the book reader.
	*/
	void HandleEventsBook(Input& input);

	/*
	* IMPORTS: none
	* EXPORTS: none
	* PURPOSE: Update the gui and its events.
	*/
	void Update();

	/*
	* IMPORTS: none
	* EXPORTS: none
	* PURPOSE: Draw the top screen background.
	*/
	void DrawTopBackground();

	/*
	* IMPORTS: none
	* EXPORTS: none
	* PURPOSE: Draw the gui status bar.
	*/
	void DrawStatusScreen();

	/*
	* IMPORTS: none
	* EXPORTS: none
	* PURPOSE: Draw the background for the book reader.
	*/
	void DrawTextBG();

	/*
	* IMPORTS: none
	* EXPORTS: none
	* PURPOSE: Draw the file selection screen.
	*/
	void DrawFileSelect(Renderer& ren);

	/*
	* IMPORTS: none
	* EXPORTS: none
	* PURPOSE: Close book. Close the open book.
	*/
	void CloseBook();

	/*
	* IMPORTS: Gui -> the ui object. (It is passed to another function)
	* EXPORTS: none
	* PURPOSE: Draw the actual book to the screen.
	*/
	void DrawBook(Gui& gui, Renderer& ren);

	/*
	* IMPORTS: none
	* EXPORTS: none
	* PURPOSE: Draw the book controls to the bottom screen.
	*/
	void DrawControls();

	/*
	* IMPORTS: none
	* EXPORTS: std::string containing book path.
	* PURPOSE: Get the selected books location.
	*/
	std::string getSelected();

	/*
	* IMPORTS: none
	* EXPORTS: none
	* PURPOSE: Y position of book page.
	*/
	int getBookPageY();

	/*
	* IMPORTS: none
	* EXPORTS: none
	* PURPOSE: vector position
	*/
	int getBookVectorPos();

private:
	Book m_book;

	int m_index = 0;
	int m_curPage = 0;
	int m_indexBookmark = 0;
	int m_curPageBookmark = 0;
	int m_bookPageY = 10;
	int m_bookVectorPos = 0;
	unsigned int m_begin = 0;
	unsigned int m_end = 7;
	unsigned int m_beginBookmark = 0;
	unsigned int m_endBookmark = 7;
	bool m_drawAbout = false;
	bool m_loading = false;
	bool m_bookmarked = false;
	bool m_showBookmarks = false;

	sf2d_texture* m_next;
	sf2d_texture* m_prev;
	sf2d_texture* m_top;
	sf2d_texture* m_bottom;
	sf2d_texture* m_exit;
	sf2d_texture* m_about;
	sf2d_texture* m_controls;
	sf2d_texture* m_charging;
	sf2d_texture* m_textBG;
	sf2d_texture* m_bookmarkedBG;
	sf2d_texture* m_back;
	std::vector<sf2d_texture*> m_batteryLevels;

	sftd_font* m_font;

	std::vector<std::string> m_files;
	std::vector<int> m_bookmarkedPages;
	std::string m_selected = "";

	u32 m_wifiStatus = 0;
	u8 m_batteryLevel = 5;
	u8 m_chargingValue = 0;

private:
	/*
	* IMPORTS: none
	* EXPORTS: std::string
	* PURPOSE: Return T (type) as a std::string.
	*/
	template<typename T>
	std::string to_string(const T& value);

	/*
	* IMPORTS: none
	* EXPORTS: std::string
	* PURPOSE: Return current time as a std::string
	*/
	std::string clock();

	/*
	* IMPORTS: none
	* EXPORTS: std::string
	* PURPOSE: Remove the extension from the string.
	*/
	std::string remove_extension(const std::string& filename);

	/*
	* IMPORTS: none
	* EXPORTS: none
	* PURPOSE: Return the extension from a string.
	*/
	std::string get_extension(const std::string& filename);
	
    /*
	* IMPORTS: bookName - Name of the book to open. Renderer - renderer object.
	* EXPORTS: none
	* PURPOSE: Open a book to read. Used internally.
	*/
	void OpenBook(const std::string& bookName, Renderer& ren);

	/*
	* IMPORTS: file - std::string containing the path of the book
	* EXPORTS: none
	* PURPOSE: Delete a book from the SD card.
	*/
	void RemoveBook(const std::string& file);

	/*
	* IMPORTS: none
	* EXPORTS: none
	* PURPOSE: Load the bookmarks
	*/
	void LoadBookmark();

	/*
	* IMPORTS: none
	* EXPORTS: none
	* PURPOSE: Saves current page as a bookmark.
	*/
	void SaveBookmark();

	/*
	* IMPORTS: element - element of bookmark
	* EXPORTS: none
	* PURPOSE: removes a bookmark from the book
	*/
	void RemoveBookmark(int element);
};

#endif
///
/// GUI.hpp
/// UI management class
///

#ifndef EBOOK_GUI_HPP_
#define EBOOK_GUI_HPP_

#include <3ds.h>

#include "Book.hpp"
#include "Input.hpp"
#include "Texture.hpp"

class GUI
{
public:
	///
	/// Loads books, ui, etc.
	///
	void init();

	///
	/// Cleans up memory usage.
	///
	void exit();

	///
	/// Process events. Boolean to see if processing menu or book events.
	///
	void event(Input& input, bool* isBookMode);

	///
	/// Update ui and stuff.
	///
	void update();

	///
	/// Draw the top background.
	///
	void drawTopBackground();

	///
	/// Draw status bar on top of screen e.g. battery.
	///
 	void drawStatusBar();

 	///
 	/// Draw the file select screen.
 	///
 	void drawFileSelect();

 	///
 	/// Remove a book from the filesystem.
 	/// 
 	void removeBook(const std::string& file);

private:
	Texture* m_next;
	Texture* m_prev;
	Texture* m_top;
	Texture* m_bottom;
	Texture* m_exit;
	Texture* m_about;
	Texture* m_controls;
	Texture* m_charging;
	Texture* m_textBG;
	Texture* m_bookmarkedBG;
	Texture* m_back;

	Book* m_book = nullptr;

	std::vector<Texture*> m_batteryLevels;
	std::vector<std::string> m_files;

	u8 m_batteryLevel = 5;
	u8 m_chargingValue = 0;

	std::string m_selected = "";
	const char* m_loadText = "Loading...";

	bool m_drawAbout = false;
	bool m_loading = false;

	int m_curFilePage = 1;
	int m_index = 0;

private:
	///
	/// Return current time as a std::string.
	///
	std::string clock();

	///
	/// Remove the extension from the string.
	///
	std::string removeExtension(const std::string& filename);

	///
	/// Return the extension from a string.
	///
	std::string getExtension(const std::string& filename);
};

#endif

/*

	* PURPOSE: Handle events in the menu.
	void HandleEventsMenu(Input& input, Renderer& ren);
	
	* PURPOSE: Handle events in the book reader.
	void HandleEventsBook(Input& input);

	* PURPOSE: Update the gui and its events.
	void Update();

	* PURPOSE: Draw the top screen background.
	void DrawTopBackground();

	* PURPOSE: Draw the gui status bar.
	void DrawStatusScreen();

	* PURPOSE: Draw the background for the book reader.
	void DrawTextBG();

	* PURPOSE: Draw the file selection screen.
	void DrawFileSelect(Renderer& ren);

	* PURPOSE: Close book. Close the open book.
	void CloseBook();

	* PURPOSE: Draw the actual book to the screen.
	void DrawBook(Gui& gui);

	* PURPOSE: Draw the book controls to the bottom screen.
	void DrawControls();

	* PURPOSE: Get the selected books location.
	std::string getSelected();

	* PURPOSE: get book page
	int getBookPage();

private:
	int m_index = 0;
	int m_curPage = 0;
	int m_indexBookmark = 0;
	int m_curPageBookmark = 0;
	int m_bookPage = 0;
	unsigned int m_begin = 0;
	unsigned int m_end = 7;
	unsigned int m_beginBookmark = 0;
	unsigned int m_endBookmark = 7;
	
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

	
	std::vector<int> m_bookmarkedPages;
	std::string m_selected = "";

	

private:
	* PURPOSE: Return T (type) as a std::string.
	template<typename T>
	std::string to_string(const T& value);

	

	
	
	* PURPOSE: Open a book to read. Used internally.
	void OpenBook(const std::string& bookName);

	* PURPOSE: Delete a book from the SD card.
	void RemoveBook(const std::string& file);

	* PURPOSE: Load the bookmarks
	void LoadBookmark();

	* PURPOSE: Saves current page as a bookmark.
	void SaveBookmark();

	* PURPOSE: removes a bookmark from the book
	void RemoveBookmark(int element);

*/
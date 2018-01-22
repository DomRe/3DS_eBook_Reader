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
 	/// Draw book screen top.
 	///
 	void drawBookTop();

 	///
 	/// Draw book controls UI.
 	///
 	void drawBookControls();

 	///
 	/// Remove a book from the filesystem.
 	/// 
 	void removeBook(const std::string& file);

private:
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

	int m_curBookPage = 0;
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
///
/// GUI.cpp
/// UI management class
///

#include <cmath>
#include <ctime>
#include <chrono>
#include <cstdio>
#include <fstream>
#include <cstdlib>
#include <dirent.h> 
#include <algorithm>

#include "GUI.hpp"

#define SCALE 0.5f

void GUI::init()
{
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
	
	mkdir("sdmc:/books", 777);
	dir = opendir("/books");

	while ((ent = readdir (dir)) != nullptr	)
	{
	    if (getExtension(ent->d_name) == ".epub")
	    {
	    	m_files.push_back(ent->d_name);
	    }
	}
	
	closedir (dir);
}

void GUI::exit()
{
	if (m_book != nullptr)
	{
		delete m_book;
		m_book = nullptr;
	}

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

void GUI::event(Input& input, bool* isBookMode)
{
	if (!m_loading)
	{
		if (!(*isBookMode))
		{
			if (input.getKeyDown() & KEY_UP)
			{
				--m_index;
				if (m_index < 0)
				{
					m_index = 0;
				}
			}

			if (input.getKeyDown() & KEY_DOWN)
			{ 
				++m_index;
				if (m_index > 7)
				{
					m_index = 7;
				}

				int correctPos = m_index + ((8 * m_curFilePage) - 8);
				if (correctPos >= m_files.size())
				{
					correctPos = m_files.size() - 1;
					m_index = correctPos - ((8 * m_curFilePage) - 8);
				}
			}

			if ((input.getKeyDown() & KEY_LEFT) || (input.getKeyDown() & KEY_L))
			{
				--m_curFilePage;
				m_index = 0;
			}
			
			if ((input.getKeyDown() & KEY_RIGHT) || (input.getKeyDown() & KEY_R))
			{ 
				++m_curFilePage;
				m_index = 0;
			}

			if (input.getKeyDown() & KEY_X) 
			{ 
				removeBook(m_files[m_index + ((8 * m_curFilePage) - 8)]);
			}

			if (input.getKeyDown() & KEY_A) 
			{ 
				m_loading = true;
				m_selected = m_files[m_index + ((8 * m_curFilePage) - 8)];
			}

			if (input.getPosX() >= 159 && input.getPosX() <= 320 && input.getPosY() >= 217 && input.getPosY() <= 241)
			{
				if (m_drawAbout == true)
				{
					m_drawAbout = false;
				}
				else
				{
					m_drawAbout = true;
				}
			}

			if (input.getPosX() >= 0 && input.getPosX() <= 158 && input.getPosY() >= 217 && input.getPosY() <= 241)
			{
				input.m_running = false;
			}
		}
		else
		{	
			if (!m_showBookmarks)
			{
				// BOOK EVENTS

				if (m_drawBookmarked)
				{
					if ((input.getPosX() >= 20 && input.getPosX() <= 300 && input.getPosY() >= 20 && input.getPosY() <= 220) || (input.getKeyDown() & KEY_A) )
					{
						m_drawBookmarked = false;
					}
				}
				else
				{
					if ((input.getKeyDown() & KEY_B)  || (input.getPosX() >= 20 && input.getPosX() <= 103 && input.getPosY() >= 18 && input.getPosY() <= 185))
					{ 
						*isBookMode = false;
						m_selected = "";
						m_curFilePage = 1;
						m_index = 0;
						delete m_book;
						m_book = nullptr;
					}

					if ((input.getKeyDown() & KEY_LEFT) || (input.getKeyDown() & KEY_L))
					{
						--m_curBookPage;
						if (m_curBookPage < 0)
						{
							m_curBookPage = 0;
						}
					}
					
					if ((input.getKeyDown() & KEY_RIGHT) || (input.getKeyDown() & KEY_R))
					{ 
						++m_curBookPage;
						if (m_curBookPage > m_book->getBookText().size())
						{
							m_curBookPage = m_book->getBookText().size();
						}
					}

					if (input.getPosX() >= 120 && input.getPosX() <= 203 && input.getPosY() >= 18 && input.getPosY() <= 185)
					{
						m_showBookmarks = true;
						m_bookmarks.loadBookmarks(m_selected);
					}

					if ((input.getPosX() >= 219 && input.getPosX() <= 302 && input.getPosY() >= 18 && input.getPosY() <= 185) || (input.getKeyDown() & KEY_SELECT) )
					{
						m_drawBookmarked = true;
						m_bookmarks.saveBookmark(m_selected, m_curBookPage);
						m_showBookmarks = false;
					}
				}
			}
			else
			{
				// BOOKMARK MENU
				
				if (input.getKeyDown() & KEY_UP)
				{ 
					m_indexBookmark--;
				}
				
				if (input.getKeyDown() & KEY_DOWN)
				{
				 	m_indexBookmark++; 
				}
				
				if (input.getKeyDown() & KEY_LEFT) 
				{ 
					m_curPageBookmark--; 
					m_indexBookmark = 0; 
				}
				
				if (input.getKeyDown() & KEY_RIGHT) 
				{ 
					m_curPageBookmark++; 
					m_indexBookmark = 0; 
				}
				
				if (m_indexBookmark < 0) m_indexBookmark = 0;
				if (m_indexBookmark > 6) m_indexBookmark = 6;
				if (m_indexBookmark >= m_bookmarks.getBookmarkedPages().size()) m_indexBookmark = (m_bookmarks.getBookmarkedPages().size() - 1);
				if (m_curPageBookmark < 0) m_curPageBookmark = 0;

				if (input.getKeyDown() & KEY_X)
				{ 
					int bkmkelem = m_indexBookmark + (7 * m_curPageBookmark);
				    if (!(bkmkelem >= m_bookmarks.getBookmarkedPages().size()))
				    {
				    	m_bookmarks.removeBookmark(m_selected, bkmkelem);
				    }
				}

				if (input.getKeyDown() & KEY_A)
				{ 
					int bkmkelemtwo = m_indexBookmark + (7 * m_curPageBookmark);
					if (!(bkmkelemtwo >= m_bookmarks.getBookmarkedPages().size()))
					{
						m_curBookPage = m_bookmarks.getBookmarkedPages()[bkmkelemtwo];
					}
				}

				if (input.getPosX() >= 159 && input.getPosX() <= 320 && input.getPosY() >= 217 && input.getPosY() <= 241) 
				{
					if (m_drawAbout == true)
					{
						m_drawAbout = false;
					}
					else
					{
						m_drawAbout = true;
					}
				}

				if ((input.getPosX() >= 0 && input.getPosX() <= 158 && input.getPosY() >= 217 && input.getPosY() <= 241) || (input.getKeyDown() & KEY_B))
				{
					m_showBookmarks = false;
				}
			}
		}
	}
	else
	{
		m_book = new Book(m_selected);
		m_loading = false;
		*isBookMode = true;
		m_drawAbout = false;
	}
}

void GUI::update()
{
	PTMU_GetBatteryLevel(&m_batteryLevel);
	PTMU_GetBatteryChargeState(&m_chargingValue);
}

void GUI::drawTopBackground()
{
	if (m_drawAbout) 
	{
		m_about->draw(0, 0);
	} 
	else 
	{
		m_top->draw(0, 0);
	}
}

void GUI::drawStatusBar()
{
	if (m_chargingValue != static_cast<u8>(1))
	{
		m_batteryLevels[m_batteryLevel]->draw(0, 0);
	}
	else 
	{
		m_charging->draw(0, 0);
	}

	std::string time = clock();
	float xClockPosStatusBar = ((float)TOP_WIDTH - pp2d_get_text_width(time.c_str(), SCALE, SCALE) - 6.0f);
	pp2d_draw_text(xClockPosStatusBar, 2.5f, SCALE, SCALE, RGBA8(0, 0, 0, 255), time.c_str());

    if (!m_drawAbout)
    {
    	pp2d_draw_text(20.0f, 2.5f, SCALE, SCALE, RGBA8(0, 0, 0, 255), removeExtension(m_selected).c_str());
    }
}

void GUI::drawFileSelect()
{
	pp2d_draw_rectangle(0, 217, 320, 24, RGBA8(156, 156, 156, 255));
	m_bottom->draw(0, 0);
	
	if (!m_loading)
	{
		m_exit->draw(0, 217);

		float y = 20.0f;
		auto result = std::div(m_files.size(), 8);
		int pageCount;

		if (result.quot == 0)
		{
			pageCount = 1;
		}
		else if (result.quot != 0 && result.rem == 0)
		{
			pageCount = result.quot;
		}
		else if (result.quot != 0 && result.rem != 0)
		{
			pageCount = result.quot + 1;
		}

		if (m_curFilePage < 1)
		{
			m_curFilePage = 1;
		}
		else if (m_curFilePage > pageCount)
		{
			m_curFilePage = pageCount;
		}

		int begin = (m_curFilePage * 8) - 8;
		int end = begin + 8;

		if (begin < 0)
		{
			begin = 0;
		}

		if (end > m_files.size())
		{
			end = m_files.size();
		}

		for (int i = begin; i < end; ++i)
		{
			pp2d_draw_text(30.0f, y, SCALE, SCALE, RGBA8(0, 0, 0, 255), m_files[i].c_str());
			y += 20.0f;
		}

		float indexYPos = 20.0f + (m_index * 20);
		pp2d_draw_text((30.0f - pp2d_get_text_width("->", SCALE, SCALE)) - 2.0f, indexYPos, SCALE, SCALE, RGBA8(0, 0, 0, 255), "->");
	}
	else
	{
		float xLoad = (static_cast<float>(BOTTOM_WIDTH) / 2.0f) - (pp2d_get_text_width(m_loadText, 1.0f, 1.0f) / 2.0f);
		float yLoad = ((static_cast<float>(SCREEN_HEIGHT) / 2.0f) - (pp2d_get_text_height(m_loadText, 1.0f, 1.0f) / 2.0f)) - 20.0f;
		pp2d_draw_text(xLoad, yLoad, 1.0f, 1.0f, RGBA8(0, 0, 0, 255), m_loadText);
	}
}

void GUI::drawBookTop()
{
	m_textBG->draw(0, 0);

	pp2d_draw_text_wrap(10.0f, 25.0f, SCALE, SCALE, RGBA8(0, 0, 0, 255), 380.0f, m_book->getBookText()[m_curBookPage].c_str());
}

void GUI::drawBookControls()
{
	if (m_drawBookmarked)
	{
		m_bookmarkedBG->draw(0, 0);
	}
	else if (m_showBookmarks)
	{
		pp2d_draw_rectangle(0, 217, 320, 24, RGBA8(156, 156, 156, 255));

		m_bottom->draw(0, 0);
		m_back->draw(0, 217);

		float pos = 20.0f;

		if (m_curPageBookmark == 0)
		{
			m_beginBookmark = 0;
			m_endBookmark = 7; 
		}
		else 
		{
			m_beginBookmark = (7 * m_curPageBookmark);
			m_endBookmark = (7 * m_curPageBookmark) + 6;
		}

		if (m_endBookmark > m_bookmarks.getBookmarkedPages().size())
		{
			m_endBookmark = m_bookmarks.getBookmarkedPages().size();
		}

		for (m_beginBookmark = m_beginBookmark; m_beginBookmark < m_endBookmark; ++m_beginBookmark)
		{
			std::string bob = "Page " + to_string<int>(m_bookmarks.getBookmarkedPages()[m_beginBookmark]); 
			pp2d_draw_text(30.0f, pos, SCALE, SCALE, RGBA8(0, 0, 0, 255), bob.c_str());
			pos += 20.0f;
		}

		pp2d_draw_text((30.0f - pp2d_get_text_width("->", SCALE, SCALE)) - 2.0f, 20.0f + (m_indexBookmark * 20), SCALE, SCALE, RGBA8(0, 0, 0, 255), "->");
	}
	else
	{
		m_controls->draw(0, 0);
	}
}

void GUI::removeBook(const std::string& file)
{
	std::string path = "sdmc:/books/" + file;
	remove(path.c_str());

	m_files.erase(std::find(m_files.begin(), m_files.end(), file));
	m_files.shrink_to_fit();
}

std::string GUI::clock()
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
std::string GUI::removeExtension(const std::string& filename)
{
    size_t lastdot = filename.find_last_of(".");
    if (lastdot == std::string::npos) return filename;
    return filename.substr(0, lastdot); 
}

std::string GUI::getExtension(const std::string& filename)
{
    return filename.substr(filename.find_last_of(".")); 
}
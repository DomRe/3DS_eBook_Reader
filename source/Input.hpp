// Input.hpp
// Input manager

#ifndef EBOOK_INPUT_HPP_
#define EBOOK_INPUT_HPP_

#include <3ds.h>

#include "AppState.hpp"

class Input
{
public:
	/*
	* IMPORTS: none
	* EXPORTS: none
	* PURPOSE: Construct class.
	*/
	Input();

	/*
	* IMPORTS: none
	* EXPORTS: none
	* PURPOSE: Process events from 3ds.
	*/
	void HandleEvents();

	/*
	* IMPORTS: boolean
	* EXPORTS: none
	* PURPOSE: Set if the application still running.
	*/
	void SetRunning(bool isRunning);

	/*
	* IMPORTS: none
	* EXPORTS: boolean. True if running.
	* PURPOSE: Is the application still running.
	*/
	bool IsRunning() const;

	/*
	* IMPORTS: AppState - enum of app states
	* EXPORTS: none
	* PURPOSE: Change the current app state.
	*/
	void SetCurMode(AppState as);

	/*
	* IMPORTS: none
	* EXPORTS: boolean. True if running.
	* PURPOSE: Is the application still running.
	*/
	AppState CurMode() const;

	/*
	* IMPORTS: none
	* EXPORTS: u16 - X pos of touch on screen
	* PURPOSE: ^
	*/
	u16 getPosX() const;
	
	/*
	* IMPORTS: none
	* EXPORTS: u16 - y pos of touch on screen.
	* PURPOSE: ^
	*/
	u16 getPosY() const;

	/*
	* IMPORTS: none
	* EXPORTS: u32 - keypress
	* PURPOSE: Get the keys pressed.
	*/
	u32 getKeyDown() const;

private:
	u16 m_posX;
	u16 m_posY;
	u32 m_kDown;
	bool m_running;
	AppState m_as;
};

#endif
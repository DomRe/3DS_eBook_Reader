// Input.cpp
// Input manager

#include "Input.hpp"

Input::Input()
:m_kDown(0), m_running(true)
{
}

void Input::checkForEvents()
{
	hidScanInput();
	m_kDown = hidKeysDown();

	hidTouchRead(&m_touch);
}

u16 Input::getPosX() const
{
	return m_touch.px;
}

u16 Input::getPosY() const
{
	return m_touch.py;
}

u32 Input::getKeyDown() const
{
	return m_kDown;
}
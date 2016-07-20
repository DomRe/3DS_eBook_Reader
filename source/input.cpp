// Input.cpp
// Input manager

#include "Input.hpp"

Input::Input()
{
	m_running = true;
	m_as = AppState::Menu;
}

void Input::HandleEvents()
{
	hidScanInput();
	m_kDown = hidKeysDown();

	touchPosition touch;
	hidTouchRead(&touch);

	m_PosX = touch.px;
	m_PosY = touch.py;
}

void Input::SetRunning(bool isRunning)
{
	m_running = isRunning;
}

bool Input::IsRunning() const
{
	return m_running;
}

void Input::SetCurMode(AppState& as)
{
	m_as = as;
}

AppState& Input::CurMode() const
{
	return m_as;
}

u16 Input::getPosX() const
{
	return m_posX;
}

u16 Input::getPosY() const
{
	return m_posY;
}

u32 Input::getKeyDown() const
{
	return m_kDown;
}
///
/// Input.hpp
/// Input manager
///

#ifndef EBOOK_INPUT_HPP_
#define EBOOK_INPUT_HPP_

#include <3ds.h>

class Input
{
public:
	///
	/// Constructor.
	///
	Input();

	///
	/// Checks 3DS for events and stores them.
	///
	void checkForEvents();
	
	///
	/// Get xPos of touch.
	///
	u16 getPosX() const;
	
	///
	/// Get yPos of touch.
	///
	u16 getPosY() const;
	
	///
	/// Get key(s) down.
	///
	u32 getKeyDown() const;

private:
	u32 m_kDown;
	touchPosition m_touch;

public:
	bool m_running;
};

#endif
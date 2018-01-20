///
/// App.hpp
/// Main application class.
///

#ifndef EBOOK_APP_HPP_
#define EBOOK_APP_HPP_

#include "Input.hpp"

class App
{
public:
	///
	/// Init application.
	///
	App();

	///
	/// Cleanup any used memory.
	///
	~App();

	///
	/// Main loop.
	///
	void run();

	///
	/// Check for events.
	///
	void event();

	///
	/// Update application data.
	///
	void update();

	///
	/// Render everything to screen.
	///
	void render();

private:
	bool m_isBookMode;

	Input m_input;
};

#endif
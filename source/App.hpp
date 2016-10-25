// App.hpp
// Main application class

#ifndef EBOOK_APP_HPP_
#define EBOOK_APP_HPP_

#include "Gui.hpp"
#include "Input.hpp"
#include "Renderer.hpp"

class App
{
public:
	/*
	* IMPORTS: none
	* EXPORTS: none
	* PURPOSE: Initialise application.
	*/
	void Init();

	/*
	* IMPORTS: none
	* EXPORTS: none
	* PURPOSE: Process event loop.
	*/
	void Event();

	/*
	* IMPORTS: none
	* EXPORTS: none
	* PURPOSE: Process app updates.
	*/
	void Update();

	/*
	* IMPORTS: none
	* EXPORTS: none
	* PURPOSE: Process draw requests.
	*/
	void Render();

	/*
	* IMPORTS: none
	* EXPORTS: none
	* PURPOSE: Cleanup and shutdown application.
	*/
	void End();

	/*
	* IMPORTS: none
	* EXPORTS: 0 on sucess
	* PURPOSE: Runs the application.
	*/
	int Run();

	/*
	* IMPORTS: none
	* EXPORTS: Input
	* PURPOSE: Get input managing object.
	*/
	Input& getInput() const;

	/*
	* IMPORTS: none
	* EXPORTS: Renderer
	* PURPOSE: Get drawing managing object.
	*/
	Renderer& getRenderer() const;

	/*
	* IMPORTS: none
	* EXPORTS: Gui
	* PURPOSE: Get ui managing object.
	*/
	Gui& getGui() const;

private:
	Input m_input;
	Renderer m_ren;
	Gui m_gui;

public:
	std::string m_masterCSS = "";
};

#endif
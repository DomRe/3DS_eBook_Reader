// Renderer.hpp
// Render manager

#ifndef EBOOK_RENDERER_HPP_
#define EBOOK_RENDERER_HPP_

#include "Litehtml3DSContainer.hpp"

class Renderer
{
public:
	/*
	* IMPORTS: none
	* EXPORTS: none
	* PURPOSE: Start drawing on top screen.
	*/
	void StartDrawingTop();
	
	/*
	* IMPORTS: none
	* EXPORTS: none
	* PURPOSE: Start drawing on bottom screen.
	*/
	void StartDrawingBottom();

	/*
	* IMPORTS: none
	* EXPORTS: none
	* PURPOSE: Stop drawing to a screen. Pair with StartDrawing.
	*/
	void StopDrawing();

	/*
	* IMPORTS: none
	* EXPORTS: none
	* PURPOSE: Render everything to the screen.
	*/
	void Render();

// normally these are private and we use getters but im getting some wierd errors when passing to litehtml through functions
// so for now they are public
// doesnt really matter for the purposes of the application, but still bad coding nonetheless.
public:
	container_3ds m_c3ds;
	litehtml::context m_htmlContext;
};

#endif
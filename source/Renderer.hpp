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

	/*
	* IMPORTS: none
	* EXPORTS: litehtml 3ds container
	* PURPOSE: get internal litehtml 3ds container
	*/
	container_3ds& getC3DS() const;

	/*
	* IMPORTS: none
	* EXPORTS: litehtml context
	* PURPOSE: get internal litehtml context
	*/
	litehtml::context& getContext() const;	

private:
	container_3ds m_c3ds;
	litehtml::context m_htmlContext;
};

#endif
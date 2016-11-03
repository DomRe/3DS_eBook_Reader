// Renderer.hpp
// Render manager

#ifndef EBOOK_RENDERER_HPP_
#define EBOOK_RENDERER_HPP_

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
};

#endif
// Renderer.cpp
// Render manager

#include <sf2d.h>

#include "Renderer.hpp"

void Renderer::StartDrawingTop()
{
	sf2d_start_frame(GFX_TOP, GFX_LEFT);
}

void Renderer::StartDrawingBottom()
{
	sf2d_start_frame(GFX_BOTTOM, GFX_LEFT);
}

void Renderer::StopDrawing()
{
	sf2d_end_frame();
}

void Renderer::Render()
{
	sf2d_swapbuffers();
}
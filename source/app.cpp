///
/// App.cpp
/// Main application class.
///

#include <unistd.h>

#include "pp2d/pp2d.h"

#include "App.hpp"

App::App()
:m_isRunning(true), m_isBookMode(false)
{
	romfsInit();
	aptInit();
	ptmuInit();
	hidInit();

	pp2d_init();
	pp2d_set_3D(0);
	pp2d_set_texture_filter(GPU_LINEAR, GPU_NEAREST);
}

App::~App()
{
	pp2d_exit();

	hidExit();
	ptmuExit();
	aptExit();
	romfsExit();
}

int App::run()
{
	while (m_input.m_running)
	{
		aptMainLoop();
		
		event();
		update();
		render();
	}

	return 0;
}

void App::event()
{
	m_input.checkForEvents();

	if (m_input.getKeyDown() & KEY_SELECT)
	{
		m_input.m_running = false;
	}

	if (m_isBookMode)
	{
		// book events
	}
	else
	{
		// menu events
	}
}

void App::update()
{
	// update gui?
}

void App::render()
{
	if (m_isBookMode)
	{
		// draw book crap
		pp2d_begin_draw(GFX_TOP, GFX_LEFT);

		pp2d_end_draw();


		pp2d_begin_draw(GFX_BOTTOM, GFX_LEFT);

		pp2d_end_draw();
	}
	else
	{
		// draw menu crap
		pp2d_begin_draw(GFX_TOP, GFX_LEFT);

		pp2d_end_draw();


		pp2d_begin_draw(GFX_BOTTOM, GFX_LEFT);

		pp2d_end_draw();
	}
}

/*

void App::Render()
{
	switch (m_input.CurMode())
	{
		case AppState::Menu:
			m_ren.StartDrawingTop();

				m_gui.DrawTopBackground();
				m_gui.DrawStatusScreen();

			m_ren.StopDrawing();
			m_ren.StartDrawingBottom();

				m_gui.DrawFileSelect(m_ren);

			m_ren.StopDrawing();
			m_ren.Render();
		break;

		case AppState::Text:
			m_ren.StartDrawingTop();
			
				m_gui.DrawTextBG();
				m_gui.DrawBook(m_gui);
				m_gui.DrawStatusScreen();
			
			m_ren.StopDrawing();

			m_ren.StartDrawingBottom();
			
				m_gui.DrawControls();
				
			m_ren.StopDrawing();
			m_ren.Render();
		break;
	}
}
*/
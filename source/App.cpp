///
/// App.cpp
/// Main application class.
///

#include <unistd.h>

#include "pp2d/pp2d.h"

#include "App.hpp"

App::App()
:m_isBookMode(false)
{
	sdmcInit();
	romfsInit();
	aptInit();
	ptmuInit();
	hidInit();

	pp2d_init();
	pp2d_set_3D(0);
	pp2d_set_texture_filter(GPU_LINEAR, GPU_NEAREST);

	m_gui.init();
}

App::~App()
{
	m_gui.exit();

	pp2d_exit();

	hidExit();
	ptmuExit();
	aptExit();
	romfsExit();
	sdmcExit();
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

	if (m_input.getKeyDown() & KEY_START)
	{
		m_input.m_running = false;
	}

	m_gui.event(m_input, &m_isBookMode);
}

void App::update()
{
	m_gui.update();
}

void App::render()
{
	if (m_isBookMode == true)
	{
		pp2d_begin_draw(GFX_TOP, GFX_LEFT);
			m_gui.drawBookTop();
			m_gui.drawStatusBar();

		pp2d_end_draw();
		
		pp2d_begin_draw(GFX_BOTTOM, GFX_LEFT);
			m_gui.drawBookControls();
		pp2d_end_draw();
	}
	else
	{
		pp2d_begin_draw(GFX_TOP, GFX_LEFT);
			m_gui.drawTopBackground();
			m_gui.drawStatusBar();
		pp2d_end_draw();


		pp2d_begin_draw(GFX_BOTTOM, GFX_LEFT);
			m_gui.drawFileSelect();
		pp2d_end_draw();
	}
}
// App.cpp
// Main application class

#include <unistd.h>

#include <string>
#include <sstream>
#include <fstream>

#include <sf2d.h>
#include <sftd.h>

#include "App.hpp"
#include "Time.hpp"

#ifndef EXIT_SUCCESS
	#define EXIT_SUCCESS 0
#endif

void App::Init()
{
	// Initialize services
	romfsInit();
	aptInit();
	ptmuInit();
	hidInit();

	// Initialize sf2d
	sf2d_init();
	sf2d_set_clear_color(RGBA8(0, 0, 0, 255));
	sf2d_set_vblank_wait(0);
	
	sftd_init();
	
	m_gui.Load();

	m_input.SetCurMode(AppState::Menu);

	std::ifstream in("romfs:/master.css");
	std::string master_css(static_cast<std::stringstream const&>(std::stringstream() << in.rdbuf()).str());

	m_ren.html_context.load_master_stylesheet(master_css.c_str());
}

void App::Event()
{
	m_input.HandleEvents();

	if (m_input.getKeyDown() & KEY_START) {
		m_input.SetRunning(false);
	}

	switch (m_input.CurMode())
	{
		case AppState::Menu:
			m_gui.HandleEventsMenu(m_input, m_ren);	
		break;

		case AppState::Text:
			m_gui.HandleEventsBook(m_input);
		break;
	}
}

void App::Update()
{
	m_gui.Update();
}

void App::m_render()
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
			m_ren.m_render();
		break;

		case AppState::Text:
			m_ren.StartDrawingTop();
			
				m_gui.DrawTextBG();
				m_gui.DrawBook(m_gui, m_ren);
				m_gui.DrawStatusScreen();
			
			m_ren.StopDrawing();

			m_ren.StartDrawingBottom();
			
				m_gui.DrawControls();
				
			m_ren.StopDrawing();
			m_ren.m_render();
		break;
	}
}

void App::End()
{
	m_ren.getC3DS().Close();
	m_gui.Close();

	sftd_fini();
	sf2d_fini();

	hidExit();
	ptmuExit();
	aptExit();
	romfsExit();
}

void App::Run()
{
	uint64_t lastTime = re::NanoTime();
		
	double delta = 0.0;
	const double ns = 1000000000.0 / 60.0;
	uint64_t timer = re::MillisTime();
	int frames = 0;
	int updates = 0;

	Init();

	while (m_input.IsRunning())
	{
		uint64_t now = re::NanoTime();
		delta += (now - lastTime) / ns;
		lastTime = now;
		
		aptMainLoop();
		if (delta >= 1.0)
		{		
			Event();
			Update();
			
			updates++;
			delta--;
		}

		m_render();
		frames++;

		if ((re::MillisTime() - timer) > 1000)
		{
			timer += 1000;

			updates = 0;
			frames = 0;
		}
	}

	End();

	return EXIT_SUCCESS;
}
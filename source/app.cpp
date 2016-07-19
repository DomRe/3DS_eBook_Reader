#include <sstream>
#include <fstream>
#include <string>
#include <unistd.h>

#include <sf2d.h>
#include <sftd.h>

#include "app.h"

#define MENU 0
#define TEXT 1

int MilliToMicro(int millis)
{
	return millis * 1000;
}

void App::Init()
{
	// Initialize services
	romfsInit();
	aptInit();
	ptmuInit();
	hidInit();

	sf2d_init();
	sf2d_set_clear_color(RGBA8(0, 0, 0, 255));
	sf2d_set_vblank_wait(0);
	
	sftd_init();
	
	gui.Load();

	input.curMode = MENU;

	std::ifstream in("romfs:/master.css");
	std::string master_css(static_cast<std::stringstream const&>(std::stringstream() << in.rdbuf()).str());

	ren.html_context.load_master_stylesheet(master_css.c_str());
}

void App::Event()
{
	input.HandleEvents();

	// delay program to allow 3ds hardware to catch up.
	usleep(MilliToMicro(50));

	if (input.m_kDown & KEY_START) {
		input.running = false;
	}

	switch (input.curMode)
	{
		case MENU:
			gui.HandleEventsMenu(input, ren);	
		break;

		case TEXT:
			gui.HandleEventsBook(input);
		break;
	}
}

void App::Update()
{
	gui.Update();
}

void App::Render()
{
	switch (input.curMode)
	{
		case MENU:
			ren.StartDrawingTop();

				gui.DrawTopBackground();
				gui.DrawStatusScreen();
				
			ren.StopDrawing();
			ren.StartDrawingBottom();

				gui.DrawFileSelect(ren);

			ren.StopDrawing();
			ren.Render();
		break;

		case TEXT:
			ren.StartDrawingTop();
			
				gui.DrawTextBG();
				gui.DrawBook(gui, ren);
				gui.DrawStatusScreen();
			
			ren.StopDrawing();

			ren.StartDrawingBottom();
			
				gui.DrawControls();
				
			ren.StopDrawing();
			ren.Render();
		break;
	}
}

void App::End()
{
	ren.c3ds.Close();
	gui.Close();

	sftd_fini();
	sf2d_fini();

	hidExit();
	ptmuExit();
	aptExit();
	romfsExit();
}
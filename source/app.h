#ifndef APP_H
#define APP_H

#include "gui.h"
#include "input.h"
#include "rendering.h"

class App
{
public:
	void Init();

	void Event();
	void Update();
	void Render();

	void End();

private:
	Input input;
	Renderer ren;
	Gui gui;
};

#endif
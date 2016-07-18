#ifndef RENDERING_H
#define RENDERING_H

#include "container_3ds.h"

class Renderer
{
public:
	void StartDrawingTop();
	void StartDrawingTopRight();
	void StartDrawingBottom();
	void StopDrawing();
	void Render();

	bool draw3D = false;

	litehtml::context html_context;
	container_3ds c3ds;
	litehtml::document::ptr m_html;
};

#endif
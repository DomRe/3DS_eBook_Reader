///
/// Texture.cpp
/// Manages a texture object lifetime.
///

#include "Texture.hpp"

Texture::Texture(size_t id, const char* path)
:m_id(id)
{
	pp2d_load_texture_png(m_id, path);
}

Texture::~Texture()
{
	pp2d_free_texture(m_id);
}

void Texture::draw(int x, int y)
{
	pp2d_draw_texture(m_id, x, y);
}
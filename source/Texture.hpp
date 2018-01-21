///
/// Texture.hpp
/// Manages a texture object lifetime.
///

#ifndef EBOOK_TEXTURE_HPP_
#define EBOOK_TEXTURE_HPP_

#include "pp2d/pp2d.h"

class Texture
{
public:
	///
	/// Load a texture with id and at path.
	/// ONLY SUPPORTS PNG!
	///
	Texture(size_t id, const char* path);

	///
	/// Cleanup memory used by texture.
	///
	~Texture();

	///
	/// Draw the texture at coords.
	///
	void draw(int x, int y);

private:
	size_t m_id;
};

#endif
#include <algorithm>
#include <string> 
#include <fstream>
#include <streambuf>

#include <sf2d.h>

#include <ft2build.h>
#include FT_CACHE_H
#include FT_FREETYPE_H
#include FT_GLYPH_H

#include "container_3ds.h"

#define PPI_3DS 132.1

#define FT_CEIL(X)  (((X + 63) & -64) / 64)

void container_3ds::Close()
{
	if(m_Font.font)
	{
		sftd_free_font(m_Font.font);
	}
}

litehtml::uint_ptr container_3ds::create_font(const litehtml::tchar_t* faceName, int size, int weight, litehtml::font_style italic, unsigned int decoration, litehtml::font_metrics* fm)
{
	// very simple support for fonts in early stage, so we dont give a damn about the type of font it is...
	m_Font.font = sftd_load_font_file("romfs:/font/LiberationSans-Regular.ttf");

	FTC_FaceID face_id = (FTC_FaceID)m_Font.font;
	FT_Face face;
	FTC_Manager_LookupFace(sftd_get_manager(m_Font.font), face_id, &face);
	m_Font.size = size;

	int scale = face->size->metrics.y_scale;

	FT_Load_Glyph(face, FT_Get_Char_Index(face, 'x'), FT_LOAD_DEFAULT);

	int ascent = FT_CEIL(FT_MulFix(face->ascender, scale));
	int descent = FT_CEIL(FT_MulFix(face->descender, scale));

	fm->ascent = ascent;
	fm->descent = descent;
	fm->height = ascent - descent + 1;
	fm->x_height = face->glyph->metrics.height;

	m_Font.strikeout = false;
	m_Font.underline = false;

	return (litehtml::uint_ptr)(&m_Font);
}

void container_3ds::delete_font(litehtml::uint_ptr hFont)
{
	if (m_Font.font)
	{
		sftd_free_font(m_Font.font);
	}
}

int container_3ds::text_width(const litehtml::tchar_t* text, litehtml::uint_ptr hFont)
{
	sftd_font_extended* fnt = (sftd_font_extended*) hFont;

	return sftd_get_text_width(fnt->font, fnt->size, text);
}

void container_3ds::draw_text(litehtml::uint_ptr hdc, const litehtml::tchar_t* text, litehtml::uint_ptr hFont, litehtml::web_color color, const litehtml::position& pos)
{
	sftd_font_extended* fnt = (sftd_font_extended*) hFont;

	sftd_draw_text(fnt->ifont, pos.x, pos.y, RGBA8(color.red, color.blue, color.green, color.alpha), fnt->size, text);
}

int container_3ds::pt_to_px(int pt)
{
	return pt / 72 * PPI_3DS;
}

int container_3ds::get_default_font_size() const
{
	return 14;
}

const litehtml::tchar_t* container_3ds::get_default_font_name() const
{
	// unused, because we define only one kind of font.
	return "LiberationSans-Regular";
}

void container_3ds::draw_list_marker(litehtml::uint_ptr hdc, const litehtml::list_marker& marker)
{
	// unused, becuase we don't need to draw bullet points in a book.
}

void container_3ds::load_image(const litehtml::tchar_t* src, const litehtml::tchar_t* baseurl, bool redraw_on_ready)
{
	// current version of ebook reader does not support images.
}

void container_3ds::get_image_size(const litehtml::tchar_t* src, const litehtml::tchar_t* baseurl, litehtml::size& sz)
{
	// current version of ebook reader does not support images.
}

void container_3ds::draw_background(litehtml::uint_ptr hdc, const litehtml::background_paint& bg)
{
	// not needed, no support for images yet, and backgroud is being drawn by app.
}

void container_3ds::draw_borders(litehtml::uint_ptr hdc, const litehtml::borders& borders, const litehtml::position& draw_pos, bool root)
{
	// unused, dont need borders, only text
}

void container_3ds::set_caption(const litehtml::tchar_t* caption)
{
	// not needed, our app handles this already
}

void container_3ds::set_base_url(const litehtml::tchar_t* base_url)
{
	// not needed
}

void container_3ds::link(const std::shared_ptr<litehtml::document>& doc, const litehtml::element::ptr& el)
{
	// not needed
}

void container_3ds::on_anchor_click(const litehtml::tchar_t* url, const litehtml::element::ptr& el)
{
	// not needed
}

void container_3ds::set_cursor(const litehtml::tchar_t* cursor)
{
	// not needed
}

void container_3ds::transform_text(litehtml::tstring& text, litehtml::text_transform tt)
{
	std::string temp = text;

	if (tt == litehtml::text_transform_capitalize) {
		temp[0] = toupper(temp[0]);

	} else if (tt == litehtml::text_transform_uppercase) {
		std::transform(temp.begin(), temp.end(), temp.begin(), ::toupper);

	} else if (tt == litehtml::text_transform_lowercase) {
		std::transform(temp.begin(), temp.end(), temp.begin(), ::tolower);
	}

	text = temp.c_str();
}

void container_3ds::import_css(litehtml::tstring& text, const litehtml::tstring& url, litehtml::tstring& baseurl)
{
	std::string temp = "";

	std::ifstream readcss(url);
	
	readcss.seekg(0, std::ios::end);
	temp.reserve(readcss.tellg());
	readcss.seekg(0, std::ios::beg);

	temp.assign((std::istreambuf_iterator<char>(readcss)),
            std::istreambuf_iterator<char>());

	text = temp;
}

void container_3ds::set_clip(const litehtml::position& pos, const litehtml::border_radiuses& bdr_radius, bool valid_x, bool valid_y)
{
	// not needed, images not supported yet
}

void container_3ds::del_clip()
{
	// not needed, images not supported yet
}

void container_3ds::get_client_rect(litehtml::position& client) const
{
	client.x = 0;
	client.y = 0;
	client.width = 400;
	client.height = 240;
}

std::shared_ptr<litehtml::element>	container_3ds::create_element(const litehtml::tchar_t *tag_name, const litehtml::string_map &attributes, const std::shared_ptr<litehtml::document> &doc)
{
	// unused
	return 0;
}

void container_3ds::get_media_features(litehtml::media_features& media) const
{
	// unused
}

void container_3ds::get_language(litehtml::tstring& language, litehtml::tstring & culture) const
{
	language = _t("en");
	culture = _t("");
}
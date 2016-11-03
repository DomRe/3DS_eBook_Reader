// Litehtml3DSContainer.hpp
// Layer between litehtml and nintendo 3ds

#include <string> 
#include <fstream>
#include <streambuf>
#include <algorithm>

#include <sf2d.h>

#include <ft2build.h>
#include FT_CACHE_H
#include FT_FREETYPE_H
#include FT_GLYPH_H

#include "Litehtml3DSContainer.hpp"

#define PPI_3DS 95.5

#define FT_CEIL(X)  (((X + 63) & -64) / 64)

typedef struct texture_atlas texture_atlas;

typedef enum {
   SFTD_LOAD_FROM_FILE,
   SFTD_LOAD_FROM_MEM
} sftd_font_load_from;

struct sftd_font {
   sftd_font_load_from from;
   union {
       char *filename;
       struct {
           const void *font_buffer;
           unsigned int buffer_size;
       };
   };
   FTC_Manager ftcmanager;
   FTC_CMapCache cmapcache;
   FTC_ImageCache imagecache;
   texture_atlas *tex_atlas;
};

void container_3ds::Close()
{
	if(m_Font.font)
	{
		sftd_free_font(m_Font.font);
	}
}

void container_3ds::SetCSS(const std::string& cssText)
{
	css = cssText;
}

litehtml::uint_ptr container_3ds::create_font(const litehtml::tchar_t* faceName, int size, int weight, litehtml::font_style italic, unsigned int decoration, litehtml::font_metrics* fm)
{
	if(m_Font.font == nullptr)
	{
        // very simple support for fonts in early stage, so we dont give a damn about the type of font it is...
		m_Font.font = sftd_load_font_file("romfs:/font/LiberationSans-Regular.ttf");
 
        FTC_FaceID face_id = (FTC_FaceID)m_Font.font;
        FT_Face face;
        FTC_Manager_LookupFace(m_Font.font->ftcmanager, face_id, &face);
        m_Font.size = 12; // = size;
 
        FTC_ScalerRec scaler;
        scaler.face_id = face_id;
        scaler.width = 0;
        scaler.height = m_Font.size << 6;
        scaler.pixel = 0;
        scaler.x_res = 72;
        scaler.y_res = 72;
 
        FT_Size font_size;
        FTC_Manager_LookupSize(m_Font.font->ftcmanager, &scaler, &font_size);
        FT_Load_Glyph(face, FT_Get_Char_Index(face, 'x'), FT_LOAD_NO_BITMAP);
 
        fm->ascent = face->size->metrics.ascender >> 6;
		fm->descent = face->size->metrics.descender >> 6;
		fm->height = fm->ascent - fm->descent + 1;
		fm->x_height = face->glyph->metrics.height >> 6;

        m_Font.strikeout = false;
        m_Font.underline = false;
    }

	return (litehtml::uint_ptr)(&m_Font);
}

void container_3ds::delete_font(litehtml::uint_ptr hFont)
{

}

int container_3ds::text_width(const litehtml::tchar_t* text, litehtml::uint_ptr hFont)
{
	return sftd_get_text_width(m_Font.font, m_Font.size, (const char*)text);
}

void container_3ds::draw_text(litehtml::uint_ptr hdc, const litehtml::tchar_t* text, litehtml::uint_ptr hFont, litehtml::web_color color, const litehtml::position& pos)
{
	std::string texts((const char*)text);
	sftd_draw_text(m_Font.font, pos.x, pos.y, RGBA8(0, 0, 0, 255), m_Font.size, texts.c_str());
}

int container_3ds::pt_to_px(int pt)
{
	return (int)((double)pt * PPI_3DS / 72.0);
}

int container_3ds::get_default_font_size() const
{
	return 12;
}

const litehtml::tchar_t* container_3ds::get_default_font_name() const
{
	// We say this is the default font, but its irrelevant.
	return "LiberationSans-Regular";
}

void container_3ds::draw_list_marker(litehtml::uint_ptr hdc, const litehtml::list_marker& marker)
{
	// current version of ebook reader does not support bullet points.
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
	text = css;
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
	return nullptr;
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

litehtml::tstring container_3ds::resolve_color(const litehtml::tstring& color) const
{
	return litehtml::tstring();
}
///
/// DocumentContainer.hpp
///

#ifndef EBOOK_DOCUMENTCONTAINER_HPP_
#define EBOOK_DOCUMENTCONTAINER_HPP_

#include "litehtml/litehtml.h"

class eBookContainer : public litehtml::document_container
{
public:
	litehtml::uint_ptr create_font(const litehtml::tchar_t* faceName, int size, int weight, litehtml::font_style italic, unsigned int decoration, litehtml::font_metrics* fm) override;
};

#endif
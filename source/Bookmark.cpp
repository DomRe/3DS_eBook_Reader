///
/// Bookmark.cpp
/// Manages the eBook bookmarks.
///

#include "tinyxml2/tinyxml2.h"

#include "Bookmark.hpp"

Bookmark::~Bookmark()
{

}

void Bookmark::loadBookmark(const std::string& book)
{
    m_bookmarkedPages.clear();

    XMLDocument doc;
    doc.LoadFile("sdmc:/books/bookmarks.xml");

    XMLElement* root = doc.FirstChildElement("bookmarks");

    if (root)
    {
        for (XMLElement* e = root->FirstChildElement("bookmark"); e != nullptr; e = e->NextSiblingElement("bookmark"))
        {
            if (e->Attribute("book") == selected)
            {
                bookmarkedPages.push_back(e->IntAttribute("page"));
            }
        }
    }
}

void Bookmark::saveBookmark(const std::string& book)
{

}
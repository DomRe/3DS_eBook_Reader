///
/// Bookmark.cpp
/// Manages the eBook bookmarks.
///

#include "tinyxml2/tinyxml2.h"

#include "Bookmark.hpp"

using namespace tinyxml2;

Bookmark::~Bookmark()
{
	m_bookmarkedPages.clear();
}

void Bookmark::loadBookmarks(const std::string& book)
{
    m_bookmarkedPages.clear();

    XMLDocument doc;
    doc.LoadFile("sdmc:/books/bookmarks.xml");

    XMLElement* root = doc.FirstChildElement("bookmarks");

    if (root)
    {
        for (XMLElement* e = root->FirstChildElement("bookmark"); e != nullptr; e = e->NextSiblingElement("bookmark"))
        {
            if (e->Attribute("book") == book)
            {
                m_bookmarkedPages.push_back(e->IntAttribute("page"));
            }
        }
    }
}

void Bookmark::saveBookmark(const std::string& book, int page)
{
    XMLDocument doc;
    doc.LoadFile("sdmc:/books/bookmarks.xml");
    
    XMLElement* root = doc.FirstChildElement("bookmarks");

    XMLElement* bookmarkElement = doc.NewElement("bookmark");
    bookmarkElement->SetAttribute("book", book.c_str());
    
    bookmarkElement->SetAttribute("page", page);
    root->InsertEndChild(bookmarkElement);

    doc.SaveFile("sdmc:/books/bookmarks.xml");
}

void Bookmark::removeBookmark(const std::string& book, int element)
{
    m_bookmarkedPages.erase(m_bookmarkedPages.begin() + element);
    m_bookmarkedPages.shrink_to_fit();

    XMLDocument doc;
    doc.LoadFile("sdmc:/books/bookmarks.xml");

    XMLElement* root = doc.FirstChildElement("bookmarks");

    if (root)
    {
        for (XMLElement* e = root->FirstChildElement("bookmark"); e != nullptr; e = e->NextSiblingElement("bookmark"))
        {
            if (e->Attribute("book") == book)
            {
                if (e->IntAttribute("page") == m_bookmarkedPages[element])
                {
                    doc.DeleteNode(e);
                }
            }
        }
    }

    doc.SaveFile("sdmc:/books/bookmarks.xml");
}

const std::vector<int>& Bookmark::getBookmarkedPages() const
{
	return m_bookmarkedPages;
}
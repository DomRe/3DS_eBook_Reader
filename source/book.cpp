// Book.hpp
// Manages the ebook.

#include <cmath>
#include <numeric>
#include <fstream>
#include <iostream>
#include <algorithm>

#include "Gui.hpp" // includes book.h
#include "tinyxml2/tinyxml2.h"

using namespace tinyxml2;

Book::~Book()
{
    m_manifest.clear();
    m_spine.clear();
}

void Book::CloseBook()
{
    m_manifest.clear();
    m_spine.clear();

    m_book = "";
    m_opf = ""; 
}

void Book::LoadBook(const std::string& epub, Renderer& ren)
{
    m_book = epub;
    m_zipfile = std::shared_ptr<BLUnZip>(new BLUnZip(m_book));

    ParseContainer();
    ParseOPF(ren);

    // basically screen width
    m_bookpos.x = 0;
    m_bookpos.y = 10;
    m_bookpos.width = 400;
    m_bookpos.height = 240;

    m_curpage = 0;
    ParsePage(m_curpage, ren);
}

void Book::ParseContainer()
{
    std::string unclean( m_zipfile->ExtractToString("META-INF/container.xml") );

    XMLDocument doc;
    doc.Parse( unclean.c_str() );

    XMLElement* container = doc.FirstChildElement( "container" );
    XMLElement* rootfiles = container->FirstChildElement( "rootfiles" );
    XMLElement* rootfile = rootfiles->FirstChildElement( "rootfile" );

    m_opf = rootfile->Attribute("full-path");
}

void Book::ParseOPF(Renderer& ren)
{
    std::string unclean( m_zipfile->ExtractToString( m_opf ) );
    
    XMLDocument doc;
    doc.Parse( unclean.c_str() );

    XMLElement* package = doc.FirstChildElement("package");
    XMLElement* manifest_ = package->FirstChildElement("manifest");
    XMLElement* item = manifest_->FirstChildElement("item");

    for(XMLElement* rfe = item; rfe != nullptr; rfe = rfe->NextSiblingElement("item"))
    {
        m_manifest.emplace(rfe->Attribute("id"), rfe->Attribute("href"));
    }

    XMLElement* spine_ = package->FirstChildElement("spine");
    XMLElement* itemref = spine_->FirstChildElement("itemref");

    for (XMLElement* rfe = itemref; rfe != nullptr; rfe = rfe->NextSiblingElement("itemref"))
    {
        m_spine.push_back(rfe->Attribute("idref"));
    }

    ren.m_c3ds.SetCSS(m_zipfile->ExtractToString(m_manifest["css"]));
}

void Book::ParsePage(unsigned int pagenum, Renderer& ren)
{
    std::string page(m_zipfile->ExtractToString(m_manifest[m_spine[pagenum]]));
    m_content = litehtml::document::createFromString(page.c_str(), &ren.m_c3ds, &ren.m_htmlContext);
    
    int best_width = m_content->render(400);
    if(best_width < 400)
    {
        m_content->render(best_width);
    }
}

unsigned int Book::GetPageCount() const
{
    return m_spine.size();
}

std::string Book::GetBook()
{
    return m_book;
}

void Book::Reader(Gui& gui, Renderer& ren)
{   
    if((int)gui.getBookVectorPos() != m_curpage)
    {
        m_curpage = gui.getBookVectorPos();
        ParsePage(m_curpage, ren);
    }

    m_content->draw(nullptr, 0, -gui.getBookPageY(), &m_bookpos);
} 
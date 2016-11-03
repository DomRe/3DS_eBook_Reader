// Book.hpp
// Manages the ebook.

#include <cmath>
#include <numeric>
#include <fstream>
#include <iostream>
#include <algorithm>

#include "Gui.hpp" // includes book.h
#include "TextVisitor.hpp"

using namespace tinyxml2;

Book::~Book()
{
    m_manifest.clear();
    m_spine.clear();
    m_alltext.clear();
}

void Book::CloseBook()
{
    m_manifest.clear();
    m_spine.clear();

    m_book = "";
    m_opf = ""; 
}

void Book::LoadBook(const std::string& epub)
{
    m_book = epub;
    BLUnZip zp(m_book);

    ParseContainer(zp);
    ParseOPF(zp);
    ParsePages(zp);
}

void Book::ParseContainer(BLUnZip& zipfile)
{
    std::string data( zipfile.ExtractToString("META-INF/container.xml") );

    XMLDocument doc;
    doc.Parse( data.c_str() );

    XMLElement* container = doc.FirstChildElement( "container" );
    XMLElement* rootfiles = container->FirstChildElement( "rootfiles" );
    XMLElement* rootfile = rootfiles->FirstChildElement( "rootfile" );

    m_opf = rootfile->Attribute("full-path");
}

void Book::ParseOPF(BLUnZip& zipfile)
{
    std::string data( zipfile.ExtractToString( m_opf ) );
    
    XMLDocument doc;
    doc.Parse( data.c_str() );

    XMLElement* package = doc.FirstChildElement("package");
    XMLElement* manifest_ = package->FirstChildElement("manifest");
    XMLElement* item = manifest_->FirstChildElement("item");

    for(XMLElement* rfe = item; rfe != nullptr; rfe = rfe->NextSiblingElement("item"))
    {
       m_manifest.emplace(rfe->Attribute("id"), rfe->Attribute("href"));
    }

    XMLElement* m_spine_ = package->FirstChildElement("spine");
    XMLElement* itemref = m_spine_->FirstChildElement("itemref");

    for (XMLElement* rfe = itemref; rfe != nullptr; rfe = rfe->NextSiblingElement("itemref"))
    {
        m_spine.push_back(rfe->Attribute("idref"));
    }
}

void Book::ParsePages(BLUnZip& zipfile)
{
    std::vector<char> filter(std::numeric_limits<unsigned char>::max(), 1);
    for (unsigned char c : m_valid)
    {
        filter[c] = 0;
    }

    // m_spine.size(); or 7 for debug
    for (unsigned int i = 0; i != 7; i++)
    {
        TextVisitor tv;

        std::string page ( zipfile.ExtractToString( m_manifest[m_spine[i]]) );

        XMLDocument doc;
        doc.Parse(page.c_str());
        XMLElement* body = doc.FirstChildElement("html")->FirstChildElement("body");
        body->Accept(&tv);

        for (auto& v : tv.GetText())
        {
            m_alltext.push_back(v);
        }

        // clean up text, remove any random / corrupt characters
        // https://github.com/dietmarkuehl/cputube/blob/master/cpu/test/replace.cpp
        for (auto& text : m_alltext)
        {
            std::replace_if(text.begin(), text.end(), [&](unsigned char c) { return filter[c]; }, '\'');
        }

        // now we have scrubbed the text, we need to remove weird comma thingys.
        for (auto& text : m_alltext)
        {
            std::string bad = "'''";
            std::string good = "'";

            std::string::size_type n = 0;
            while ( ( n = text.find( bad, n ) ) != std::string::npos )
            {
                text.replace( n, bad.size(), good );
                n += good.size();
            }
        }
    }
}

std::string Book::GetBook()
{
    return m_book;
}

void Book::Reader(Gui& gui)
{   
    int ypos = 20;  
    
    // 57 character limit using fixed width font, start at y = 20, 12 pixels spacing per line..., 18 lines per page top screen, max looping is 236
    // bottom screen is same, except only 46 characters per line.
    for (int i = (gui.getBookPage() * 18); i < ((gui.getBookPage() * 18) + 18); i++)
    {
        sftd_draw_text(gui.getTextFont(), 0, ypos, RGBA8(0, 0, 0, 255), 12, m_alltext[i].c_str());
        ypos += 12;
    }
} 
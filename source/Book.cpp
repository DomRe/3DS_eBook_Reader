///
/// Book.cpp
/// Manages the eBook.
///

#include <cmath>
#include <numeric>
#include <fstream>
#include <iostream>
#include <algorithm>

#include "tinyxml2/tinyxml2.h"

#include "Book.hpp"

using namespace zipper;
using namespace tinyxml2;

Book::Book(const std::string& epub)
{
    m_book = epub;
    Unzipper zip(epub);

    parseContainer(zip);
    parseOPF(zip);
    parsePages(zip);

    zip.close();
}

Book::~Book()
{
    m_manifest.clear();
    m_spine.clear();
    m_text.clear();

    m_book = "";
    m_opf = ""; 
    m_title = "";
    m_author = "";
    m_container = "";
}    

void Book::parseContainer(Unzipper& zip)
{
    std::vector<unsigned char> buffer;
    zip.extractEntryToMemory("META-INF/container.xml", buffer);
    
    // We do this extra step to ensure null termination and proper c string structure.
    std::string data(buffer.begin(), buffer.end());

    XMLDocument doc;
    doc.Parse(data.c_str());

    XMLElement* container = doc.FirstChildElement("container");
    XMLElement* rootfiles = container->FirstChildElement("rootfiles");
    XMLElement* rootfile = rootfiles->FirstChildElement("rootfile");

    m_opf = rootfile->Attribute("full-path");

    auto pos = m_opf.find("content.opf");
    if (pos == 0)
    {
        m_container = "";
    }
    else
    {
        m_container = m_opf.substr(0, pos);
    }
}

void Book::parseOPF(Unzipper& zip)
{
    std::vector<unsigned char> buffer;
    zip.extractEntryToMemory(m_opf, buffer);
    
    // We do this extra step to ensure null termination and proper c string structure.
    std::string data(buffer.begin(), buffer.end());
    
    XMLDocument doc;
    doc.Parse(data.c_str());

    XMLElement* package = doc.FirstChildElement("package");
    XMLElement* metadata = package->FirstChildElement("metadata");
    XMLElement* manifest = package->FirstChildElement("manifest");
    XMLElement* item = manifest->FirstChildElement("item");

    m_title = metadata->FirstChildElement("dc:title")->GetText();
    m_author = metadata->FirstChildElement("dc:creator")->GetText();

    for(XMLElement* rfe = item; rfe != nullptr; rfe = rfe->NextSiblingElement("item"))
    {
       m_manifest.emplace(rfe->Attribute("id"), rfe->Attribute("href"));
    }

    XMLElement* spine = package->FirstChildElement("spine");
    XMLElement* itemref = spine->FirstChildElement("itemref");

    for (XMLElement* it = itemref; it != nullptr; it = it->NextSiblingElement("itemref"))
    {
        m_spine.emplace_back(it->Attribute("idref"));
    }
}

void Book::parsePages(Unzipper& zip)
{
    for (size_t i = 0; i < m_spine.size(); ++i)
    {
        std::vector<unsigned char> buffer;
        zip.extractEntryToMemory(m_container + m_manifest[m_spine[i]], buffer);
    
        // We do this extra step to ensure null termination and proper c string structure.
        std::string data(buffer.begin(), buffer.end());

        XMLDocument doc;
        doc.Parse(data.c_str());

        XMLElement* body = doc.FirstChildElement("body");
        for (XMLElement* elem = body; elem != nullptr; elem = elem->NextSiblingElement())
        {
            const char* text = elem->GetText();
            if (text != 0)
            {
                m_text.emplace_back(text);
            }
        }
    }
}

std::string Book::getBook() const
{
    return m_book;
}

const std::vector<std::string>& Book::getBookText() const
{
    return m_text;
}
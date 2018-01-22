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

using namespace tinyxml2;

Book::Book(const std::string& epub)
{
    m_book = "sdmc:/books/" + epub;
    BLUnZip zip(m_book);
    
    parseContainer(zip);
    parseOPF(zip);
    parsePages(zip);

    zip.Close();
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

void Book::parseContainer(BLUnZip& zip)
{
    std::string data = zip.ExtractToString("META-INF/container.xml");

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

void Book::parseOPF(BLUnZip& zip)
{
    std::string data = zip.ExtractToString(m_opf);
    
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
        m_spine.push_back(it->Attribute("idref"));
    }
}

void Book::parsePages(BLUnZip& zip)
{
    for (size_t i = 0; i < m_spine.size(); ++i)
    {
        std::string data = zip.ExtractToString(m_container + m_manifest[m_spine[i]]);

        XMLDocument doc;
        doc.Parse(data.c_str());

        XMLElement* html = doc.FirstChildElement("html");
        XMLElement* body = html->FirstChildElement("body");

        for (XMLElement* next = body->FirstChildElement(); next != nullptr; next = next->NextSiblingElement())
        {
            m_text.push_back(next->GetText());
        }
    }
}

const std::vector<std::string>& Book::getBookText() const
{
    return m_text;
}
// http://ideveloperworld.blogspot.com.au/2011/02/epub-reader.html

// #include "tidy.h"
// #include "tidybuffio.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <cmath>
#include <numeric>

#include "gui.h" // includes book.h
#include "TextVisitor.h"

using namespace tinyxml2;

Book::~Book()
{
	manifest.clear();
	spine.clear();
	alltext.clear();
}

void Book::CloseBook()
{
	manifest.clear();
	spine.clear();
	alltext.clear();

	book = "";
	opf = "";	
}

void Book::LoadBook(const std::string& epub)
{
	book = epub;
	BLUnZip zp(book);

	ParseContainer(zp);
	ParseOPF(zp);
	ParsePages(zp);

	bookpos.x = 0;
	bookpos.y = 0;
	bookpos.width = 400;
	bookpos.height = 240;
}

void Book::ParseContainer(BLUnZip& zipfile)
{
	std::string unclean( zipfile.ExtractToString("META-INF/container.xml") );

	XMLDocument doc;
    doc.Parse( unclean.c_str() );

    XMLElement* container = doc.FirstChildElement( "container" );
    XMLElement* rootfiles = container->FirstChildElement( "rootfiles" );
    XMLElement* rootfile = rootfiles->FirstChildElement( "rootfile" );

    opf = rootfile->Attribute("full-path");
}

void Book::ParseOPF(BLUnZip& zipfile)
{
	std::string unclean( zipfile.ExtractToString( opf ) );
	
	XMLDocument doc;
    doc.Parse( unclean.c_str() );

    XMLElement* package = doc.FirstChildElement("package");
    XMLElement* manifest_ = package->FirstChildElement("manifest");
    XMLElement* item = manifest_->FirstChildElement("item");

	for(XMLElement* rfe = item; rfe != nullptr; rfe = rfe->NextSiblingElement("item"))
	{
    	manifest.emplace(rfe->Attribute("id"), rfe->Attribute("href"));
	}

	XMLElement* spine_ = package->FirstChildElement("spine");
	XMLElement* itemref = spine_->FirstChildElement("itemref");

	for (XMLElement* rfe = itemref; rfe != nullptr; rfe = rfe->NextSiblingElement("itemref"))
	{
		spine.push_back(rfe->Attribute("idref"));
	}
}

void Book::ParsePages(BLUnZip& zipfile)
{
	std::vector<char> filter(std::numeric_limits<unsigned char>::max(), 1);
	for (unsigned char c : valid)
	{
    	filter[c] = 0;
	}

	// spine.size(); or 7
	for (unsigned int i = 0; i != 3; i++)
	{
		TextVisitor tv;

		std::string page ( zipfile.ExtractToString( manifest[spine[i]]) );

		XMLDocument doc;
		doc.Parse(page.c_str());
		XMLElement* body = doc.FirstChildElement("html")->FirstChildElement("body");
		body->Accept(&tv);

		for (auto& v : tv.GetText())
		{
			alltext.push_back(v);
		}

		// clean up text, remove any random / corrupt characters
		// https://github.com/dietmarkuehl/cputube/blob/master/cpu/test/replace.cpp
		for (auto& text : alltext)
		{/*
			text.erase(std::remove_if(text.begin(), text.end(),
									  [&](unsigned char c)
									  { return filter[c]; }
									 ), text.end());
									 */
			std::replace_if(text.begin(), text.end(), [&](unsigned char c) { return filter[c]; }, '\'');
		}
	}
}

std::string Book::GetBook()
{
	return book;
}
/*
void Book::Reader(Gui& gui)
{	
	int ypos = 20;	
	
	// 57 character limit using fixed width font, start at y = 20, 12 pixels spacing per line..., 18 lines per page top screen, max looping is 236
	// bottom screen is same, except only 46 characters per line.
	for (int i = (gui.getBookPage() * 18); i < ((gui.getBookPage() * 18) + 18); i++)
	{
		sftd_draw_text(gui.getTextFont(), 0, ypos, RGBA8(0, 0, 0, 255), 12, alltext[i].c_str());
		ypos += 12;
	}
}	
*/

// create a vector of litehtml::document::ptr's. create once only during parsePages...


void Book::Reader(Gui& gui, Renderer& ren)
{	
 	ren.m_html->render(400);
 	ren.m_html->draw(nullptr, 0, 10, &bookpos);
}    
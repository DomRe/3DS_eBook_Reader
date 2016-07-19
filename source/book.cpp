// http://ideveloperworld.blogspot.com.au/2011/02/epub-reader.html

// #include "tidy.h"
// #include "tidybuffio.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <cmath>
#include <numeric>

#include "gui.h" // includes book.h
#include "tinyxml2/tinyxml2.h"

using namespace tinyxml2;

Book::~Book()
{
	manifest.clear();
	spine.clear();
	content.clear();
}

void Book::CloseBook()
{
	manifest.clear();
	spine.clear();
	content.clear();

	book = "";
	opf = "";	
}

void Book::LoadBook(const std::string& epub, Renderer& ren)
{
	book = epub;
	BLUnZip zp(book);

	ParseContainer(zp);
	ParseOPF(zp);
	ParsePages(zp, ren);

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

void Book::ParsePages(BLUnZip& zipfile, Renderer& ren)
{
	content.clear(); // ensure its empty.

	ren.c3ds.SetCSS(zipfile.ExtractToString(manifest["css"]));

	// spine.size()
	for (unsigned int i = 0; i != 5; i++)
	{
		std::string page ( zipfile.ExtractToString( manifest[spine[i]]) );
		litehtml::document::ptr html = litehtml::document::createFromUTF8(page.c_str(), &ren.c3ds, &ren.html_context);
		content.push_back(html.get());
	}	
}

std::string Book::GetBook()
{
	return book;
}

void Book::Reader(Gui& gui, Renderer& ren)
{	
	content[gui.getBookVectorPos()]->render(400);
 	content[gui.getBookVectorPos()]->draw(nullptr, 0, gui.getBookPageY(), &bookpos);
}    
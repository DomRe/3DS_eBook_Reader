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
}

void Book::CloseBook()
{
	manifest.clear();
	spine.clear();

	book = "";
	opf = "";	
}

void Book::LoadBook(const std::string& epub, Renderer& ren)
{
	book = epub;
	zipfile = std::shared_ptr<BLUnZip>(new BLUnZip(book));

	ParseContainer();
    ParseOPF(ren);

	bookpos.x = 0;
	bookpos.y = 0;
	bookpos.width = 400;
	bookpos.height = 240;

	curpage = 0;
    ParsePage(curpage, ren);
}

void Book::ParseContainer()
{
	std::string unclean( zipfile->ExtractToString("META-INF/container.xml") );

	XMLDocument doc;
    doc.Parse( unclean.c_str() );

    XMLElement* container = doc.FirstChildElement( "container" );
    XMLElement* rootfiles = container->FirstChildElement( "rootfiles" );
    XMLElement* rootfile = rootfiles->FirstChildElement( "rootfile" );

    opf = rootfile->Attribute("full-path");
}

void Book::ParseOPF(Renderer& ren)
{
	std::string unclean( zipfile->ExtractToString( opf ) );
	
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

	ren.c3ds.SetCSS(zipfile->ExtractToString(manifest["css"]));
}

void Book::ParsePage(unsigned int pagenum, Renderer& ren)
{
    std::string page(zipfile->ExtractToString(manifest[spine[pagenum]]));
    content = litehtml::document::createFromUTF8(page.c_str(), &ren.c3ds, &ren.html_context);
    content->render(400);
}

unsigned int Book::GetPageCount() const
{
	return spine.size();
}

std::string Book::GetBook()
{
	return book;
}

void Book::Reader(Gui& gui, Renderer& ren)
{	
	if((int)gui.getBookVectorPos() != curpage)
	{
		curpage = gui.getBookVectorPos();
		ParsePage(curpage, ren);
	}

	content->draw(0, 0, -gui.getBookPageY(), &bookpos);
}    
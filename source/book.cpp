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

/*
#include "tidy/tidy.h"
#include "tidy/tidyenum.h"
#include "tidy/tidybuffio.h"
#include "tidy/tidyplatform.h"

std::string CleanHTML(const std::string &html)
{
    // Init tidy document.
    TidyDoc tdoc = tidyCreate();
    TidyBuffer output = {0};
    TidyBuffer err = {0};

    bool ok;
    int rc = -1;

    // Configure tidy.
    ok = ( tidyOptSetBool(tdoc, TidyHtmlOut, yes)
      && tidyOptSetBool(tdoc, TidyQuiet, yes)
      && tidyOptSetBool(tdoc, TidyNumEntities, yes)
      && tidyOptSetBool(tdoc, TidyShowWarnings, no)
      && tidyOptSetBool(tdoc, TidyStrictTagsAttr, yes)
      && tidyOptSetBool(tdoc, TidyWrapLen, yes)
      && tidyOptSetBool(tdoc, TidyCoerceEndTags, yes)
      && tidyOptSetBool(tdoc, TidyQuoteMarks, yes)
      && tidyOptSetBool(tdoc, TidyWrapAttVals, yes)
      && tidyOptSetBool(tdoc, TidyFixBackslash, yes)
      && tidyOptSetBool(tdoc, TidyIndentAttributes, yes));

    // Parse input.
    if (ok)
    {
    	tidySetErrorBuffer(tdoc, &err);
    }

    if(rc >= 0)
    {
    	rc = tidyParseString(tdoc, html.c_str());
    }

    // Clean & Repair.
	if (rc >= 0)
	{
	 	rc = tidyCleanAndRepair(tdoc);
	}
    
	if (rc >= 0)
	{
	  	// Check for errors.
	    rc = tidyRunDiagnostics(tdoc);
	}
    
    if (rc > 1)
    {
    	rc = (tidyOptSetBool(tdoc, TidyForceOutput, yes) ? rc : -1);
    }

    if (rc >= 0)
    {
    	rc = tidySaveBuffer(tdoc, &output);
    }

    std::string result = "";

    if (rc >= 0)
    {
    	if (rc > 0)
    	{
    		result = (char*)output.bp;
    	}
    }
    else
    {
    	result = "error";
    }
    
    tidyBufFree(&output);
    tidyBufFree(&err);
    tidyRelease(tdoc);

    return result;
}
*/
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

	m_content->draw(0, 0, -gui.getBookPageY(), &m_bookpos);
}    
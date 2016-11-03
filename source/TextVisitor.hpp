// Implementation of the tinyxml2 visitor class based off of XMLPrinter.

#ifndef TEXTVISITOR_H
#define TEXTVISITOR_H

#include <vector>
#include <string>

#include "tinyxml2/tinyxml2.h"

using namespace tinyxml2;

class TextVisitor : public XMLVisitor
{
public:
	// Visit a Document
	virtual bool VisitEnter(const XMLDocument& /*doc*/) override;
	virtual bool VisitExit(const XMLDocument& /*doc*/) override;

	// Visit an Element
	virtual bool VisitEnter(const XMLElement& element, const XMLAttribute* attribute) override;
	virtual bool VisitExit(const XMLElement& element) override;

	// Visit a declaration.
	virtual bool Visit(const XMLDeclaration& declaration) override;

	// Visit a text node. THIS IS THE ONE WE WILL BE USING!
	virtual bool Visit(const XMLText& text) override;

	// Visit a comment node.
	virtual bool Visit(const XMLComment& comment) override;

	// Visit an unknown node.
	virtual bool Visit(const XMLUnknown& unknown) override;

	// Get the text we parsed out of the document.
	std::vector<std::string>& GetText();

private:
	std::vector<std::string> m_bookText;
};

#endif
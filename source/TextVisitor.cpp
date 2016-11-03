#include "TextVisitor.hpp"

bool TextVisitor::VisitEnter(const XMLDocument& doc)
{
	return true;
}

bool TextVisitor::VisitExit(const XMLDocument& doc)
{
	return true;
}

bool TextVisitor::VisitEnter(const XMLElement& element, const XMLAttribute* attribute)
{
	return true;
}

bool TextVisitor::VisitExit(const XMLElement& element)
{
	return true;
}

bool TextVisitor::Visit(const XMLDeclaration& declaration)
{
	return true;
}

bool TextVisitor::Visit(const XMLText& text)
{
	std::string t = " ";
	t = text.Value();	

	// break text strings into 57 character lengths to fit on the screen.
	for (unsigned int i = 0; i < t.length(); i += 57)
	{
		m_bookText.push_back(t.substr(i, 57));
	}
	
	return true;
}

bool TextVisitor::Visit(const XMLComment& comment)
{
	return true;
}

bool TextVisitor::Visit(const XMLUnknown& unknown)
{
	return true;
}

std::vector<std::string>& TextVisitor::GetText()
{
	return m_bookText;
}
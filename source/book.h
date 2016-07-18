#ifndef BOOK_H
#define BOOK_H

#include <vector>
#include <string>
#include <unordered_map>

#include "zip/BLUnZip.h"

#include "rendering.h"

// Forward dec
class Gui;

class Book
{
public:
	~Book();

	void LoadBook(const std::string& epub);

	void ParseContainer(BLUnZip& zipfile);
	void ParseOPF(BLUnZip& zipfile);
	void ParsePages(BLUnZip& zipfile);

	void Reader(Gui& gui, Renderer& ren);
	void CloseBook();

	std::string GetBook();
private:
	std::string book = "";
	std::string opf = "";

	// supports everything but the '\' character.
	std::string valid = " abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ`1234567890-=~!@#$^&*()\"_+[];,./{}|:<>'?%";

public:
	// id, href
	std::unordered_map<std::string, std::string> manifest;

	// order of book
	std::vector<std::string> spine;

	// All the text in the book.
	std::vector<std::string> alltext;

	//litehtml::document::ptr m_html;
	litehtml::position bookpos;
};

#endif
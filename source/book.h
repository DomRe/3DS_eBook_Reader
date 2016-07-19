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

	void LoadBook(const std::string& epub, Renderer& ren);

	void ParseContainer(BLUnZip& zipfile);
	void ParseOPF(BLUnZip& zipfile);
	void ParsePages(BLUnZip& zipfile, Renderer& ren);

	void Reader(Gui& gui, Renderer& ren);
	void CloseBook();

	std::string GetBook();
private:
	std::string book = "";
	std::string opf = "";

public:
	// id, href
	std::unordered_map<std::string, std::string> manifest;

	// order of book
	std::vector<std::string> spine;

	// The html pages in the book
	std::vector<litehtml::document*> content;
	litehtml::position bookpos;
};

#endif
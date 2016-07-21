// Book.hpp
// Manages the ebook.

#ifndef EBOOK_BOOK_HPP_
#define EBOOK_BOOK_HPP_

#include <vector>
#include <string>
#include <unordered_map>

#include "Renderer.hpp"
#include "zip/BLUnZip.h"

class Gui;

class Book
{
public:
	/*
	* IMPORTS: none
	* EXPORTS: none
	* PURPOSE: Cleanup book when application closes.
	*/
	~Book();

	/*
	* IMPORTS: epub - std::string containing path to book. ren - rendering object.
	* EXPORTS: none
	* PURPOSE: Load and parse the book.
	*/
	void LoadBook(const std::string& epub, Renderer& ren);

	/*
	* IMPORTS: none
	* EXPORTS: none
	* PURPOSE: Extract the book and parse the meta xml.
	*/
	void ParseContainer();

	/*
	* IMPORTS: none
	* EXPORTS: none
	* PURPOSE: Parse the OPF containing all the information about the book.
	*/
	void ParseOPF(Renderer& ren);

	/*
	* IMPORTS: none
	* EXPORTS: none
	* PURPOSE: Parse the pages of the book.
	*/
	void ParsePage(unsigned int pagenum, Renderer& ren);

	/*
	* IMPORTS: gui - the GUI object. Ren - the renderer object.
	* EXPORTS: none
	* PURPOSE: Read the book.
	*/
	void Reader(Gui& gui, Renderer& ren);

	/*
	* IMPORTS: none
	* EXPORTS: none
	* PURPOSE: Clean up the book in preperation of the object loading another book.
	*/
	void CloseBook();

	/*
	* IMPORTS: none
	* EXPORTS: int - page number.
	* PURPOSE: Get the current page of the book.
	*/
	unsigned int GetPageCount() const;

	/*
	* IMPORTS: none
	* EXPORTS: std::string of book
	* PURPOSE: To get the std::string of the path of the book.
	*/
	std::string GetBook();
private:
	std::string m_book = "";
	std::string m_opf = "";

	// ebook unzipped to memory
	std::shared_ptr<BLUnZip> m_zipfile;

	// id, href
	std::unordered_map<std::string, std::string> m_manifest;

	// order of book, THIS IS ORDERED DONT MESS WITH ORDER
	std::vector<std::string> m_spine;

	// The html pages in the book
	litehtml::document::ptr m_content;
	litehtml::position m_bookpos;

	// curpage
	int m_curpage;
};

#endif
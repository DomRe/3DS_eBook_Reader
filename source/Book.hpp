// Book.hpp
// Manages the ebook.

#ifndef EBOOK_BOOK_HPP_
#define EBOOK_BOOK_HPP_

#include <vector>
#include <string>
#include <unordered_map>

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
	void LoadBook(const std::string& epub);

	/*
	* IMPORTS: zipfile
	* EXPORTS: none
	* PURPOSE: Extract the book and parse the meta xml.
	*/
	void ParseContainer(BLUnZip& zipfile);

	/*
	* IMPORTS: zipfile, renderer
	* EXPORTS: none
	* PURPOSE: Parse the OPF containing all the information about the book.
	*/
	void ParseOPF(BLUnZip& zipfile);

	/*
	* IMPORTS: zipfile, renderer
	* EXPORTS: none
	* PURPOSE: Parse the pages of the book.
	*/
	void ParsePages(BLUnZip& zipfile);

	/*
	* IMPORTS: gui - the GUI object. Ren - the renderer object.
	* EXPORTS: none
	* PURPOSE: Read the book.
	*/
	void Reader(Gui& gui);

	/*
	* IMPORTS: none
	* EXPORTS: none
	* PURPOSE: Clean up the book in preperation of the object loading another book.
	*/
	void CloseBook();

	/*
	* IMPORTS: none
	* EXPORTS: std::string of book
	* PURPOSE: To get the std::string of the path of the book.
	*/
	std::string GetBook();
	
private:
	std::string m_book = "";
	std::string m_opf = "";

	std::string m_valid = " abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ`1234567890-=~!@#$^&*()\"_\\+[];,./{}|:<>'?%";

	// id, href
	std::unordered_map<std::string, std::string> m_manifest;

	// order of book, THIS IS ORDERED DONT MESS WITH ORDER
	std::vector<std::string> m_spine;

	// All the text in the book.
	std::vector<std::string> m_alltext;
};

#endif
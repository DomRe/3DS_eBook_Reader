///
/// Bookmark.hpp
/// Manages the eBook bookmarks.
///

#ifndef EBOOK_BOOKMARK_HPP_
#define EBOOK_BOOKMARK_HPP_

#include <string>
#include <vector>

class Bookmark
{
public:
	///
	/// Clean up any used memory.
	///
	~Bookmark();

	///
	/// Load bookmarks for a book.
	///
	void loadBookmark(const std::string& book);

	///
	/// Save bookmarks for a book.
	///
	void saveBookmark(const std::string& book);

private:
	std::vector<int> m_bookmarkedPages;
};

#endif
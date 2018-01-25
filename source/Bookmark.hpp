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
	void loadBookmarks(const std::string& book);

	///
	/// Save a bookmark for the book.
	///
	void saveBookmark(const std::string& book, int page);

	///
	/// Remove a bookmark.
	///
	void removeBookmark(const std::string& book, int element);
	
	///
	/// Get the bookmarked pages.
	///
	const std::vector<int>& getBookmarkedPages() const;

private:
	std::vector<int> m_bookmarkedPages;
};

#endif
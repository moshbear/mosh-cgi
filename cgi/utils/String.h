/* cgi/utils/String.h - string functions
 *
 *  Copyright (C) 1996 - 2007 GNU Cgicc team
 *                2011 Andrey Vul
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 3 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110, USA
 */

#ifndef _CGI_UTILS_STRING_H_
#define _CGI_UTILS_STRING_H_ 1

/*! \file String.h
 * \brief String utility functions
 *
 * String utilities used internally by the cgi library.
 */

#include <string>
#include <utility>
#include <cgi/utils/algo.h>
#include <cgi/utils/Unicode.h>
#include <cgi/utils/string/CaseInsensitiveComparator.h>

namespace cgi { namespace string {

/*!
 * \brief Compare two strings for equality, ignoring case.
 *
 * For case-sensitive comparison, use (s1 == s2);
 * \tparam charT (deduced from input)
 * \param s1 The first string to compare
 * \param s2 The second string to compare
 * \return \c true if the strings are equal, \c false if they are not
 */
template <class charT>
bool equality(const std::basic_string<charT>& s1, const std::basic_string<charT>& s2) {
	return std::equal(s1.begin(), s1.end(), s2.begin(), CaseInsensitiveComparator<charT>());
}
/*!
 * \brief Compare two strings for equality, ignoring case.
 *
 * For case-sensitive comparison, use (s1 == s2);
 * \tparam charT (deduced from input)
 * \param s1 The first string to compare
 * \param s2 The second string to compare
 * \param n The number of characters to compare.
 * \return \c true if the strings are equal, \c false if they are not
 */
template <class charT>
bool equality(const std::basic_string<charT>& ss1, const std::basic_string<charT>& ss2, size_t n)
{
	return std::equal(ss1.begin(), ss1.begin() + n, ss2.begin(), CaseInsensitiveComparator<charT>());
}

/*!
 * Template class to partially specialize extractBetween for different char types.
 * \tparam charT output char type
 */
template <class charT>
struct extractor {
	
	/*!
	 * \brief Extract a substring contained within two separators (iterator version).
	 *
	 * For example, after the call
	 * \code
	 * std::string data = "11foo22";
	 * std::string res;
	 * res = extractor<charT>::extractBetween(data.begin(), data.end(), "11", "22");
	 * \endcode
	 * \c res will be "foo".
	 * \param _Begin Iterator to beginning of data.
	 * \param _End Iterator to end of data.
	 * \param sep1 The first logical separator.
	 * \param sep2 The second logical separator.
	 * \return The substring between the separators.
	 */
	template <class InputIterator>
	static std::basic_string<charT> extractBetween(InputIterator _Begin, InputIterator _End,
							const std::string& sep1, const std::string& sep2);
	template <class InputIterator>
	static inline std::basic_string<charT> extractBetween(InputIterator _Begin, InputIterator _End,
								const std::string& sep) {
		return extractBetween(_Begin, _End, sep, sep);
	}
	static inline std::basic_string<charT> extractBetween(const std::string& data,
								const std::string& sep1, const std::string& sep2) {
		return extractBetween(data.begin(), data.end(), sep1, sep2);
	}
	static inline std::basic_string<charT> extractBetween(const std::string& data,
							const std::string& sep) {
		return extractBetween(data, sep, sep);
	}
};

// specialization for [charT = char]
// base case
template<> 
struct extractor<char> {
	template <class InputIterator>
	static std::string extractBetween(InputIterator _Begin, InputIterator _End, const std::string& sep1, const std::string& sep2) {
		InputIterator __Start = utils::searcher<InputIterator>::search(_Begin, _End, sep1.begin(), sep1.end());
		if (__Start == _End)
			return std::string();
		for (size_t i = 0; i < sep1.size(); ++i, ++__Start);
		InputIterator __End = utils::searcher<InputIterator>::search(__Start, _End, sep2.begin(), sep2.end());
		if (__End == _End)
			return std::string();
		return std::string(__Start, __End);
	}
};

// specialization for [charT = wchar_t]
template<>
struct extractor<wchar_t> {
	template <class InputIterator>
	static inline std::wstring extractBetween(InputIterator _Begin, InputIterator _End, const std::string& sep1, const std::string& sep2) {
		return unicode::utfIn<wchar_t>(extractor<char>::extractBetween(_Begin, _End, sep1, sep2));
	}
};

} } // namespace string // namespace cgi

#endif				/* ! _CGI_UTILS_STRING_H_ */

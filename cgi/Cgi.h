
/* -*-mode:c++; c-file-style: "gnu";-*- */
/*
 *  $Id: Cgicc.h,v 1.19 2009/01/03 17:12:07 sebdiaz Exp $
 *
 *  Copyright (C) 1996 - 2004 Stephen F. Booth <sbooth@gnu.org>
 *                       2007 Sebastien DIAZ <sebastien.diaz@gmail.com>
 *  Part of the GNU cgi library, http://www.gnu.org/software/cgi
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

#ifndef _CGI_CGI_H_
#define _CGI_CGI_H_ 1

/*! \file Cgi.h
 * \brief The main header file for the GNU %cgi library
 */

/*
 * M0shbear's fork of
 * 	The GNU cgi library, by Stephen F. Booth <sbooth@gnu.org>
 * 	http://www.cgi.org
 *
 * This fork is against 3.2.9.
 */

#include <string>
#include <boost/optional.hpp>
#include <boost/ref.hpp>

#include <cgi/utils/Search_AutoRange.h>
#include <cgi/utils/StringUtils.h>
#include <fastcgi++/http.hpp>
#include <cgi/Types.h>
#include <cgi/config.h>
#include <cgi/form/Entry.h>
#include <cgi/form/File.h>
#include <cgi/form/MultipartHeader.h>
#include <cgi/Environment.h>
#include <cgi/Exceptions.h>

namespace cgi {

// ============================================================
// Class Cgicc
// ============================================================

/*! \class Cgicc Cgicc.h cgi/Cgicc.h
 * \brief The main class of the GNU %cgi library
 *
 * Cgicc is used to retrieve information on specific HTML form elements
 * (such as checkboxes, radio buttons, and text fields), on uploaded files,
 * and to save, restore, and retrieve information on the CGI
 * environment.
 *
 * Normally, you will instantiate an object of this type in
 * \c main():
 * \code
 * int
 * main(int argc, char **argv) {
 *   try {
 *     cgi::Cgicc cgi;
 *     // do something with cgi
 *   }
 *
 *   catch(const exception& e) {
 *    //handle the error
 *   }
 * }
 * \endcode
 */
template <class charT>
class Cgi {
public:

	// ============================================================

	/*! \name Constructors and Destructor */
	//@{

	/*!
	 * \brief Constructor
	 *
	 * If you are using %cgi with FastCGI, you will need to pass
	 * a \c CgiInput subclass that %cgi will use to read input.  If
	 * \c input is omitted, standard input and environment
	 * variables will be used.
	 * \param input A CgiInput object to use for reading input
	 */
	Cgi(Fastcgipp::Http::Session<charT> const& fcgiHttpSession)
	: environment(fcgiHttpSession) {
		parseQuery(environment.envData()["QUERY_STRING"]);
		if (environment.postData().size() > 0)
			parsePost(environment.postData(), environment.envData()["CONTENT_TYPE"]);
	}
	/*!
	 * \brief Destructor
	 *
	 * Delete this Cgicc object
	 */
	~Cgi() {
	}
	//@}

	// ============================================================

	/*! \name Overloaded Operators */
	//@{

	/*!
	 * \brief Compare two Cgiccs for equality.
	 *
	 * Cgiccs are equal if they represent the same environment.
	 * \param cgi The Cgicc to compare to this one.
	 * \return \c true if the two Cgiccs are equal, \c false otherwise.
	 */
	inline bool operator==(const Cgi<charT>&  cgi) const {
		return &this->environment == &cgi.environment;
	}
	/*!
	 * \brief Compare two Cgiccs for inequality.
	 *
	 * Cgiccs are equal if they represent the same environment.
	 * \param cgi The Cgicc to compare to this one.
	 * \return \c false if the two Cgiccs are equal, \c true otherwise.
	 */ inline bool
	operator!=(const Cgi<charT>& cgi) const {
		return !operator==(cgi);
	}
	//@}


	// ============================================================

	/*! \name Library Information
	 * Information on this installation of %cgi
	 */
	//@{

	/*!
	 * \brief Get the date on which this library was compiled.
	 *
	 This is a string of the form <TT>mmm dd yyyy</TT>.
	 * \return The compile date
	 */
	const char* getCompileDate() const;

	/*!
	 * \brief Get the time at which this library was compiled.
	 *
	 * This is a string of the form \c hh:mm:ss in 24-hour time.
	 * \return The compile time
	 */
	const char* getCompileTime() const;

	/*!
	 * \brief Get the version number of cgi.
	 *
	 * The version number is a string of the form \c #.#.
	 * \return The version number
	 */
	const char* getVersion() const;

	/*!
	 * \brief Get the platform for which Cgicc was configured.
	 *
	 * The host is a string of the form \c processor-manufacturer-os
	 * return The host triplet.
	 */
	const char* getHost() const;
	//@}

	// ============================================================

	/*! \name Form Element Access
	 * Information on submitted form elements
	 */
	//@{

	/*!
	 * \brief Find a radio button in a radio group, or a selected list item.
	 *
	 * \param name The name of the radio button or list item to find.
	 * \tparam wantarray Return the singular value or the list of values
	 * \return An iterator referring to the desired element, if found.
	 * \throw NotFound item was not found
	 * \throw IsList wantarray was \c false and the element has a list of values
	 */
	#define getEntry(WantArray) getEntry##$##WantArray
	form::Entry<charT>& getEntry(0)(const std::basic_string<charT>&);
	cgi::list<boost::ref<form::Entry<charT> > >::type getEntry(1)(const std::basic_string<charT>&);

	/*!
	 * \brief Find a radio button in a radio group, or a selected list item.
	 *
	 * \param name The name of the radio button or list item to find.
	 * \return An iterator referring to the desired element, if found.
	 */
	inline form::Entry<charT>& operator[] (const std::basic_string<charT>& name) {
		return getEntry(0)(name);
	}

	inline cgi::list<boost::ref<form::Entry<charT> > > operator[] (const std::basic_string<charT>& name, bool dummy_wantarray) {
		return getEntry(1)(name);
	}
	std::list<boost::ref<form::Entry<charT> > > getEntry(1)(const std::basic_string<charT>& name) {
		if (entries.find(name) == entries.end())
			throw NotFound("entry " + common::utfOut(name) + " not found");
		auto iterators = entries.equal_range(name);
		std::list<boost::ref<form::Entry<charT> > > > vals;
		for (auto a = iterators.first; a != iterators.second; ++a)
			vals.push_back(boost::ref(a.second));
		return vals;
	}
	form::Entry<charT>& getEntry(0)(const std::basic_string<charT>& name) {
		try {
			auto l = getEntry(1)(name);
			if (l.size() > 1)
				throw IsList("entry " + common::utfOut(name) + " is a list");
			return *(l.begin());
		} catch (...) {
			throw;
		}
	}

	/*!
	 * \brief Get all the submitted form entries, excluding files.
	 *
	 * \return A vector containing all the submitted elements.
	 */
	inline decltype((entries)) operator*() const {
		return entries;
	}
	//@}
	// ============================================================
	/*! \name Save and Restore */
	//@{
	/*!
	*\brief Save the current CGI environment to a file.
	* A wrapper for Fastcgipp::Http::Session::dumpToFile.
	* Use the associated program to parse to human-readable values.
	* \param filename The name of the file to which to save.
	*/
	inline void dumpEnvToFile(const std::string&  filename)	const {
		environment.dumpToFile(filename);
	}

	//@}
	//! \brief The form entries
	map<std::basic_string<charT>, form::Entry<charT> >::mm_type entries;
	//! \brief The form files
	map<std::basic_string<charT>, form:::File<charT> >::type files;
private:
	Environment<charT> environment;

	// parse standard content (application/x-www-formurlencoded)
	template <class InputIterator>
	static void parseQuery(InputIterator _Begin, InputIterator _End) {
		while (_Begin != _End) {
			std::basic_string<charT> name;
			std::basic_string<charT> value;
			// Find the '=' separating the name from its value
			InputIterator _Sep = std::find(_Begin, _End, '=');
			// If no '=', we're finished
			if (_Sep == _End)
				break;
			// Decode the name
			name = form::utils::urldecode<charT>(_Begin, _Pos);
			_Begin = ++_Sep;
			// Find the '&' separating subsequent name/value pairs
			_Sep = std::find(_Begin, _End, '&');
			value = form::utils::urldecode<charT>(_Begin, _Pos);
			// Store the pair
			entries.insert(std::make_pair(name, form::Entry<charT>(name, value)));
			if (_Sep == _End)
				break;
			// Update parse position
			_Begin = ++_Sep;
		}
	}
	static void parseQuery(std::basic_string<charT>&& str) {
		parseQuery(str.begin(), str.end());
	}
	// parse file upload (multipart/form-data)
	template <class InputIterator>
	static void parseMultipart(InputIterator _Begin, InputIterator _End, const std::string& cType) {
		std::string sep1("--");
		std::string sep2;
		// generate separators
		{
			std::string bType = "boundary=";
			std::string::const_iterator p = cgi::utils::search(cType, bType);
			sep1 += std::string(p, cType.end());
			sep2 = sep1 + "--";
			sep1 += "\r\n";
			sep2 += "\r\n";
		}
		// find data between separators
		_Begin = cgi::utils::search(_Begin, _End, sep1) + sep1.size();
		while (_Begin != _End) {
			InputIterator _Sep = cgi::utils::search(_Begin, _End, sep1);
			// sep1 not found => rest of data is item
			if (_Sep == _End)
				break;
			parseMime(_Begin, _Sep);
			_Begin = _Sep + bType.size();
		}
		// data is terminated by sep2
		InputIterator _Sep = cgi::utils::search(_Begin, _End, sep2);
		if (_Sep != _End)
			parseMIME(_Begin, _Sep);
	}
	// Parse POST data
	static void parsePost(const std::vector<char>&& post, const std::string& content_type = "application/x-www-form-urlencoded") {
		if (common::stringsAreEqual("application/x-www-form-urlencoded", content_type))
			parseQuery(post.begin(), post.end());
		else if (common::stringsAreEqual("multipart/form-data", content_type))
			parseMultipart(post.begin(), post.end(), content_type);
		else
			throw invalid_argument("Bad Content-Type: " + content_type);
	}

	
	// Parse a multipart/form-data header
	template <class InputIterator>
	static form::MultipartHeader<charT> parseHeader(InputIterator _Begin, InputIterator _End) {
		std::string data(_Begin, _End);
		return form::MultipartHeader<charT>(
			common::StringUtils::extractBetween<charT>(data, "Content-Disposition: ", ";"),
			common::StringUtils::extractBetween<charT>(data, "name=\"", "\""),
			form::utils::urldecode<charT>(common::StringUtils::extractBetween<char>(data, "filename=\"", "\"")),
			common::StringUtils::extractBetween<charT>(data, "Content-Type: ", "\r\n\r\n")
		);
	}
	template <class InputIterator>
	void parseMIME(InputIterator _Begin, InputIterator _End) {
		std::string hdrEnd = "\r\n\r\n"; // assume .size() == 4
		InputIterator _HeaderEnd = utils::search(_Begin, _End, hdrEnd);
		if (_HeaderEnd == _End)
			throw std::runtime_error("Malformed input (end-of-header marker not found");
		InputIterator _DataBegin = _HeaderEnd + 4;
		boost::shared_ptr<std::vector<char> >value(new vector<char>(_DataBegin, _End));
		form::MultipartHeader<charT> head = parseHeader(_Begin, _DataBegin);
		// not a file, i.e., form entry
		if (head.value.empty())
			entries.insert(std::make_pair(head.name, form::Entry<charT>(name, std::basic_string<charT>(_DataBegin, _End))));
		// a file
		else
			files.insert(make_pair(head.name, form::File<charT>(head.name, head.value, head.contentType, value)));
	}
#undef getEntry
};

}				// namespace cgi

#endif				/* ! _CGICC_H_ */

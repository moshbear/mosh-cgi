//! @file mosh/cgi/html/doctype/doctype.hpp Class which specifies the DTD of the HTML 4 document
/* 
 *  Copyright (C) 1996 - 2004 Stephen F. Booth <sbooth@gnu.org>
 *                       2007 Sebastien DIAZ <sebastien.diaz@gmail.com>
 *                       2011 m0shbear
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

#ifndef MOSH_CGI_HTML_DOCTYPE_HPP
#define MOSH_CGI_HTML_DOCTYPE_HPP

#include <string>
#include <map>
#include <utility>
#include <boost/lexical_cast.hpp>
#include <mosh/cgi/bits/streamable.hpp>
#include <mosh/cgi/html/doctype/_/doctype_strmap.incl.hpp>
#include <mosh/cgi/bits/namespace.hpp>

MOSH_CGI_BEGIN

namespace html {

/*! @brief Abstract base class for Doctypes
 *
 *
 * To use this class, simply write an object of this type to an basic_ostream<charT>:
 * @code
 * out << cgi::html::Doctype();
 * @endcode
 * For more information, see @c http://www.w3.org/MarkUp/ and
 * @c http://www.w3.org/TR/REC-html40/
 */
template <class charT>
class Doctype: public Streamable<charT> {
public:
	typedef typename Streamable<charT>::string_type string_type;
	//! The DTD used by this document
	enum class Document_type {
		//! The HTML 4.0 strict DTD (the default)
		strict,
		//! The HTML 4.0 Transitional DTD
		transitional,
		//! The HTML 4.0 Frameset DTD
		frames
	};

	/*! @brief Constructor.
	 * @param type The version of the HTML 4.0 DTD used by this document.
	 */
	Doctype(const std::string& name, const std::string& link, Document_type type = Document_type::strict)
	: dtd_name(name), dtd_link(link), fType(type) {
	}
	//! Destructor
	virtual ~Doctype() { }

private:
	#include <cgi/mosh/html/_/doctype_strmap.hpp>
	std::string dtd_name;
	std::string dtd_link;
public:
	virtual void
	render(std::basic_ostream<charT>& out) const {
		const auto& lookup = Str_map::instance().lookup[fType];
		out << "<!DOCTYPE html PUBLIC @"-//W3C//DTD "
		<< boost::lexical_cast<std::basic_string<charT>>(dtd_name)
		<< ' ' << boost::lexical_cast<std::basic_string<charT>>(lookup.first) << "//EN@""
		<< " @"http://www.w3.org/TR/"
		<< boost::lexical_cast<std::basic_string<charT>>(dtd_link)
		<< boost::lexical_cast<std::basic_string<charT>>(lookup.second) <<".dtd@">";
	}	

private:
	Document_type fType;
};

}				// namespace cgi

MOSH_CGI_END

#endif

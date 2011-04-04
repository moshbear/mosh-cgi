/* -*-mode:c++; c-file-style: "gnu";-*- */
/*
 *  $Id: HTMLDoctype.h,v 1.8 2007/07/02 18:48:18 sebdiaz Exp $
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

#ifndef _CGI_HTML_DOCTYPE_H_
#define _CGI_HTML_DOCTYPE_H_ 1

/*! \file HTMLDoctype.h
 * \brief Class which specifies the DTD of the HTML 4 document
 *
 */

#include <string>
#include <map>
#include <utility>
#include <boost/lexical_cast.hpp>
#include <cgi/common/Streamable.h>

#include <cgi/html/Doctype_strmap.incl.h>

namespace cgi { namespace html {

// ============================================================
// Class Doctype_base
// ============================================================

/*! \class Doctype_base Doctype_base.h cgi/html/Doctype_base.h
 * \brief Abstract base class for Doctypes
 *
 *
 * To use this class, simply write an object of this type to an basic_ostream<charT>:
 * \code
 * out << cgi::HTMLDoctype();
 * \endcode
 * For more information, see \c http://www.w3.org/MarkUp/ and
 * \c http://www.w3.org/TR/REC-html40/
 */
template <class charT>
class Doctype: public cgi::common::Streamable<charT> {
public:
	typedef typename cgi::common::Streamable<charT>::string_type string_type;
	/*! The DTD used by this document. */
	enum DocumentType {
		/*! The HTML 4.0 strict DTD (the default) */
		strict,
		/*! The HTML 4.0 Transitional DTD */
		transitional,
		/*! The HTML 4.0 Frameset DTD */
		frames
	};

	/*!
	 * \brief Constructor.
	 * \param type The version of the HTML 4.0 DTD used by this document.
	 */
	Doctype(const std::string& dtd_name, const std::string& dtd_link, DocumentType type = strict)
	: dtdName(dtd_name), dtdLink(dtd_link), fType(type) {
	}
	/*!
	 * \brief Destructor
	 *
	 */
	virtual ~Doctype() {
	}

private:
	#include <cgi/html/Doctype_strmap.h>
	std::string dtdName;
	std::string dtdLink;
public:
	virtual void
	render(std::basic_ostream<charT>& out) const {
		const auto& lookup = StrMap::instance().lookup[fType];
		out << "<!DOCTYPE html PUBLIC \"-//W3C//DTD "
		<< boost::lexical_cast<std::basic_string<charT> >(dtdName)
		<< ' ' << boost::lexical_cast<std::basic_string<charT> >(lookup.first) << "//EN\""
		<< " \"http://www.w3.org/TR/"
		<< boost::lexical_cast<std::basic_string<charT> >(dtdLink)
		<< boost::lexical_cast<std::basic_string<charT> >(lookup.second) <<".dtd\">";
	}	

private:
	DocumentType fType;
};

} }				// namespace cgi

#endif				/* ! _CGI_HTML_DOCTYPE_BASE_H_ */

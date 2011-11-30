//! @file mosh/cgi/http/header/redirect_header.hpp Class for redirecting the client to a different URI
/*
 *  Copyright (C) 1996 - 2004 Stephen F. Booth <sbooth@gnu.org>
 *                       2007 Sebastien DIAZ <sebastien.diaz@gmail.com>
 *                       2011 m0shbear
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

#ifndef MOSH_CGI_HTTP_REDIRECTHEADER_HPP
#define MOSH_CGI_HTTP_REDIRECTHEADER_HPP

#include <boost/lexical_cast.hpp>
#include <mosh/cgi/bits/t_string.hpp>
#include <map>
#include <ostream>
#include <string>
#include <mosh/cgi/http/header/header.hpp>
#include <mosh/cgi/http/header/_/redirect_header_statusmap.incl.hpp>
#include <mosh/cgi/bits/namespace.hpp>

MOSH_CGI_BEGIN

namespace http {

template <class charT>
class Redirect_header: public Header<charT> {
	typedef Header<charT> base_type;
	typedef Redirect_header<charT> this_type;
public:
	typedef typename base_type::string_type string_type;
private:
	#include <cgi/http/header/_/redirect_header_statusmap.h>
public:
	//! Create a new redirection header
	Redirect_header(const string_type& url)
	: Redirect_header(url, -1)
	{ }
	//! Create a new redirection header with status
	Redirect_header(const string_type& url, bool permanent)
	: Redirect_header(url, permanent ? 301 : 307)
	{ }
	//! Create a new redirection header with code
	Redirect_header(const string_type& url, int status_)
	: base_type(url), status(status_)
	{ }
	//! Destructor
	virtual ~Redirect_header() { }

	virtual void render(typename base_type::ostream_type& out) const {
		std::map<int, string_type>&& code = StatusMap::instance().lookup;
		if (code.find(status) != code.end()) {
			out << T_STRING(charT, "Status: ")
			    << boost::lexical_cast<string_type>(code[status]) << std::endl;
		}
		out << T_STRING(charT, "Location: ") << this->data << std::endl;
		Header::render(out);
	}
	
private:
	Redirect_header();
	int status;
};

}

MOSH_CGI_END

#endif

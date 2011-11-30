//! @file mosh/cgi/http/header/content_header.hpp HTTP content header class
/* 				
 * Copyright (C) 1996-2007 GNU Cgicc team
 * 	     (C) 2011 m0shbear
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110, USA 
 */

#ifndef MOSH_CGI_HTTP_CONTENTHEADER_HPP
#define MOSH_CGI_HTTP_CONTENTHEADER_HPP

#include <ostream>
#include <mosh/cgi/bits/t_string.hpp>
#include <mosh/cgi/http/header/header.hpp>
#include <mosh/cgi/bits/namespace.hpp>

MOSH_CGI_BEGIN

namespace http {

template <class charT>
class Content_header : public Header<charT> {
	// for future use, these typedefs may come in very handy
	typedef Header<charT> base_type;
	typedef Content_header<charT> this_type;
public:
	// public to allow typename Content_header<T>::string_type usage
	typedef typename base_type::string_type string_type; 

	Content_header(const string_type& mime_type)
	: base_type(mime_type)
	{ }

	virtual ~Content_header() { }
	
	virtual void render(typename base_type::ostream_type& out) const {
		out << T_STRING(charT, "Content-Type: ") << this->data << std::endl;
		Header::render(out);
	}
private:
	Content_header();
};
}

MOSH_CGI_END

#endif

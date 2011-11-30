//! @file mosh/cgi/http/content_header_classes.hpp Default HTTP Content-Type classses
/*
 * Copyright (C) 1996-2007 GNU Cgicc team
 * Copyright (C)  2011 m0shbear
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
 *
 */

#ifndef MOSH_CGI_HTTP_CONTENTHEADER_CLASSES_HPP
#define MOSH_CGI_HTTP_CONTENTHEADER_CLASSES_HPP

#include <mosh/cgi/http/header/content_header.hpp>
#include <mosh/cgi/bits/namespace.hpp>

#define CONTENT_HEADER(Type,Name) \
	template <class charT> \
	class Type##Header : public Content_header<charT> { \
	public:\
		Type##Header() : Content_header<charT>(Name) { } \
		virtual ~Type##Header() { } \
	}

MOSH_CGI_BEGIN

namespace http {

CONTENT_HEADER(HTML, "text/html");
CONTENT_HEADER(Plain, "text/plain");
CONTENT_HEADER(XHTML, "application/xhtml+xml");

}

MOSH_CGI_END

#undef CONTENT_HEADER

#endif

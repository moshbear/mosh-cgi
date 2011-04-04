/* cgi/http/ContentHeader_classes.h - default HTTP Content-Type classes
 *
 * Copyright (C) 1996-2007 GNU Cgicc team
 * Copyright (C)  2011 Andrey Vul
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

#ifndef _CGI_HTTP_CONTENTHEADER_CLASSES_H_
#define _CGI_HTTP_CONTENTHEADER_CLASSES_H_ 1

#include <cgi/http/ContentHeader.h>

#define CONTENT_HEADER(Type,Name) \
	template <class charT> \
	class Type##Header : public ContentHeader<charT> { \
	public:\
		Type##Header() : ContentHeader<charT>(Name) { } \
		virtual ~Type##Header() { } \
	}

namespace cgi { namespace http {
CONTENT_HEADER(HTML, "text/html");
CONTENT_HEADER(Plain, "text/plain");
CONTENT_HEADER(XHTML, "application/xhtml+xml");
} } // namespace http // namespace cgi

#endif				/* ! _CGI_HTTP_CONTENTHEADER_CLASSES_H_ */

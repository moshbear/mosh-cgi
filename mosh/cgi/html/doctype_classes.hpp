//! @file mosh/cgi/html/doctype_classes.hpp Doctypes
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

#ifndef MOSH_CGI_HTML_DOCTYPE_CLASSES_HPP
#define MOSH_CGI_HTML_DOCTYPE_CLASSES_HPP

#include <string>
#include <mosh/cgi/html/doctype/doctype.hpp>
#include <mosh/cgi/bits/namespace.hpp>

#define DOCTYPE(DType, DTDName, DTDLink) \
    template <class charT> \
    class Doctype##DType : public Doctype<charT> { \
	typedef typename Doctype<charT>::Document_type Document_type;\
	public: Doctype##DType(Document_type type = Document_type::strict) \
		: Doctype<charT>(DTDName, DTDLink, type) {\
		}\
		virtual ~Doctype##DType() { }\
	}

MOSH_CGI_BEGIN

namespace html {

DOCTYPE(4, "HTML 4.0", "REC-html40/");
DOCTYPE(X, "XHTML 1.0", "xhtml1/DTD/xhtml1-");

}

MOSH_CGI_END

#endif

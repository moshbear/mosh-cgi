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

#ifndef _CGI_HTML_DOCTYPE_CLASSES_H_
#define _CGI_HTML_DOCTYPE_CLASSES_H_ 1

/*! \file HTMLDoctype_classes.h
 * \brief Doctypes
 *
 */

#include <string>
#include <cgi/html/Doctype.h>

#define DOCTYPE(DType, DTDName, DTDLink) \
    template <class charT> \
    class Doctype##DType : public Doctype<charT> { \
	typedef typename Doctype<charT>::DocumentType DocumentType;\
	public: Doctype##DType(DocumentType type = DocumentType::strict) \
		: Doctype<charT>(DTDName, DTDLink, type) {\
		}\
		virtual ~Doctype##DType() { }\
	}

namespace cgi { namespace html {

DOCTYPE(4, "HTML 4.0", "REC-html40/");
DOCTYPE(X, "XHTML 1.0", "xhtml1/DTD/xhtml1-");

} }				// namespace cgi

#endif				/* ! _CGI_HTML_DOCTYPE_CLASSES_H_ */

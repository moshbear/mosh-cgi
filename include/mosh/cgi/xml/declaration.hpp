//! @file mosh/cgi/xml/declaration.hpp Class that abstracts a XML declaration
/*
 *  Copyright (C) 1996 - 2004 Stephen F. Booth <sbooth@gnu.org>
 *                       2007 David Roberts
		     2007 Sebastien DIAZ <sebastien.diaz@gmail.com>
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
#ifndef MOSH_CGI_XML_DECLARATION_HPP
#define MOSH_CGI_XML_DECLARATION_HPP

#include <string>
#include <mosh/cgi/xml/pi.hpp>
#include <mosh/cgi/bits/t_string.hpp>
#include <mosh/cgi/bits/namespace.hpp>

MOSH_CGI_BEGIN

namespace xml {

template <class charT>
class Declaration: public PI<charT> {
	typedef PI<charT> base_type;
public:
	typedef typename base_type::string_type string_type;
	Declaration(string_type& version = T_STRING(charT, "1.0"))
	: base_type(T_STRING(charT, "xml")) {
		this->attributes.push_back(typename base_type::attribute_type(T_STRING(charT, "version"), version));
	}
	virtual ~Declaration() {
	}
};

}

MOSH_CGI_END

#endif

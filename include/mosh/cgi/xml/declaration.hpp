//! @file mosh/cgi/xml/declaration.hpp Class that abstracts a XML declaration
/*
 *  Copyright (C) 1996 - 2004 Stephen F. Booth <sbooth@gnu.org>
 *                       2007 David Roberts
 *		         2007 Sebastien DIAZ <sebastien.diaz@gmail.com>
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
struct Declaration : public PI<charT> {
	Declaration(std::basic_string<charT>& version = wide_string<charT>("1.0"))
	: PI(wide_string<charT>("xml"))
	{
		this->attributes.insert({"version", version});
	}
	virtual ~Declaration()
	{ }
};

}

MOSH_CGI_END

#endif

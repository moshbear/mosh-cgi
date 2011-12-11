//! @file mosh/cgi/xml/pi.h Class that abstracts an XML Processing Instruction
/*
 *  Copyright (C) 1996 - 2004 Stephen F. Booth <sbooth@gnu.org>
 *                       2007 David Roberts
 *		         2007 Sebastien DIAZ <sebastien.diaz@gmail.com>
 *		         2011 m0shbear
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
#ifndef MOSH_CGI_XML_PI_H
#define MOSH_CGI_XML_PI_H

#include <string>
#include <sstream>
#include <map>

#include <mosh/cgi/bits/t_string.hpp>
#include <mosh/cgi/bits/namespace.hpp>

MOSH_CGI_BEGIN

namespace xml {

template <class charT>
struct PI {
	typedef typename std::basic_string<charT> string;
	std::map<std::string, string> attributes;
	string name;

	PI(const string& name_)
	: base_type(), attributes(), name(name_) {
	}
	virtual ~PI() {}
	
	PI<charT>& operator=(const PI<charT>& element) {
		if (this != &element) {
			attributes = element.attributes;
		}
		return *this;
	}
	
	PI<charT>& operator=(PI<charT>&& element) {
		if (this != &element) {
			attributes = std::move(element.attributes);
		}
		return *this;
	}

	virtual operator string () const {
		std::basic_stringstream<charT> s;
		s << wide_string<charT>("<?") << name;
		if (!attributes.empty()) {
			for (const auto& a : attributes) {
				s << wide_string<charT>(" " + a.first);
				s << wide_string<charT>("=\"") << a.second << wide_char<charT>('"');
			}
		}
		s << wide_string<charT>("?>");
		return s.str();
	}
};

}

MOSH_CGI_END

#endif

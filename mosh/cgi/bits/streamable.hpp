//! @file mosh/cgi/bits/streamable.hpp - ABC for output-streamable classes
/*
 *
 *  Copyright (C) 1996-2007 GNU Cgicc team
 *            (C) 2011 m0shbear
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

#ifndef MOSH_CGI_STREAMABLE_HPP
#define MOSH_CGI_STREAMABLE_HPP

#include <ostream>
#include <string>
#include <mosh/cgi/bits/namespace.hpp>

MOSH_CGI_BEGIN

template <class charT>
class Streamable;

/*!
 * Prototype for overloading streaming operator
 * @tparam charT The character type
 * @param out The basic_ostream to which to write
 * @param obj The Streamable object to write
 * @return A reference to @c out
 */
template <class charT>
std::basic_ostream<charT>& operator<<(std::basic_ostream<charT>& out, const Streamable<charT>& obj);

/*! @class Streamable streamable.hpp mosh/cgi/bits/streamable.hpp
 * @brief Mix-in streamable interface.
 *
 * Abstract mix-in class which makes classes streamable via
 * the @c << operator.
 *
 * Uses the visitor pattern.
 */
template <class charT>
struct Streamable {
	// declare the exportable types public so subclasses can import via typename
	typedef std::basic_ostream<charT> ostream_type;
	typedef std::basic_string<charT> string_type;
private:
	friend ostream_type& operator<< <>(ostream_type& out, const Streamable<charT>& obj);
public:
	Streamable() {	}
	virtual ~Streamable() { }
	virtual void render(ostream_type& out) const = 0;
};

template <class charT>
std::basic_ostream<charT>& operator<<(std::basic_ostream<charT>& out, const Streamable<charT>& obj)
{
	obj.render(out);
	return out;
}

MOSH_CGI_END

#endif

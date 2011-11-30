//@file cgi/html/element/atomic.hpp Template class for concrete atomic html::Element subclasses
/*
 *
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

#ifndef MOSH_CGI_HTML_ELEMENT_ATOMIC_HPP
#define MOSH_CGI_HTML_ELEMENT_ATOMIC_HPP

#include <boost/shared_ptr.hpp>
#include <boost/lexical_cast.hpp>
#include <map>
#include <mosh/cgi/html/element/element.hpp>
#include <mosh/cgi/bits/namespace.hpp>

MOSH_CGI_BEGIN

namespace html {
namespace element {

/*! @brief Template for concrete atomic html::Element subclasses
 *
 * An atomic HTML element is an element in which the opening and closing 
 * tags are combined.  For example, in the HTML code
 @verbatim
 <meta link="made" href="mailto:user@example.org" />
 @endverbatim
 * The @c meta tag is an atomic HTML element because the opening and closing
 * tags appear together.
 * @sa html::Element
 * @sa html::Boolean
 */

template <class charT, class Tag>
class Atomic: public Element<charT> {
public:
	//! Create a new empty atomic element
	Atomic()
	: Element<charT>(0, 0, 0, Element<charT>::Element_type::atomic)
	{ }

	//! Create a new atomic element from an existing copy
	Atomic(const Atomic<charT, Tag>& e)
	: Element<charT>(e)
	{ }

	//! Create a new atomic element from a temporary copy
	Atomic(Atomic<charT, Tag>&& e)
	: Element<charT>(e)
	{ }

	//! Create a new atomic element with given attributes
	Atomic(const std::map<std::basic_string<charT>, std::basic_string<charT>>& attributes)
	: Element<charT>(&attributes, 0, 0, Element<charT>::Element_type::atomic)
	{ }

	//! Create a new atomic element with given attributes
	Atomic(std::map<std::basic_string<charT>, std::basic_string<charT>>&& attributes)
	: Element<charT>(&attributes, 0, 0, Element<charT>::Element_type::atomic, Move_attributes::yes, Move_data::no)
	{ }


	virtual ~Atomic() { }

	//! Clone this element
	virtual boost::shared_ptr<Element<charT>> clone() const {
		return boost::shared_ptr<Element<charT>>(Atomic<charT, Tag>(*this));
	}
	//! Get this element's name
	virtual const std::basic_string<charT> name() const {
		return boost::lexical_cast<std::basic_string<charT>>(Tag::name());
	}
};

}
}

MOSH_CGI_END

#endif

//@file cgi/html/element/boolean.hpp Template class for concrete boolean Element subclasses
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

#ifndef MOSH_CGI_HTML_ELEMENT_BOOLEAN_HPP
#define MOSH_CGI_HTML_ELEMENT_BOOLEAN_HPP

#include <map>
#include <string>
#include <boost/shared_ptr.hpp>
#include <boost/lexical_cast.hpp>
#include <mosh/cgi/html/element/element.hpp>
#include <mosh/cgi/bits/namespace.hpp>

MOSH_CGI_BEGIN

namespace html { namespce element {

/* @brief Template for concrete boolean Element subclasses
 *
 * A boolean HTML element is an element having a boolean (open or closed)
 * state.  Most commonly used HTML tags are boolean elements:
 @verbatim
 <a href="http://www.example.org">GNU Project</a>
 @endverbatim
 * The @c a element is boolean, since it is either open or closed.  Boolean
 * elements are often additive:
 @verbatim
 <b>bold text<i>bold italic text</i></b>
 @endverbatim
 * Note than under the XHTML 1.0 standard, elements may not overlap; ie,
 * in the example above, it would be illegal to close the @c b tag before
 * the @c i tag.
 * @sa Element
 * @sa Atomic
 */
template <class charT, class Tag>
class Boolean: public Element<charT> {

	typedef string_typestring_type;
public:
	//! Create a new empty boolean element.
	Boolean()
	: Element<charT>(0, 0, 0, Element<charT>::Element_type::boolean) {
	}
	
	/*! @brief Create a new element, specifying the enclosed text.
	 * @param text The text within the element.
	 */
	Boolean(const string_type& text)
	: Element<charT>(0, 0, &text, Element<charT>::Element_type::boolean) {
	}

	/*! @brief Create a new element, specifying the enclosed text.
	 * @param text The text within the element.
	 */
	Boolean(string_type&& text)
	: Element<charT>(0, 0, &text, Element<charT>::Element_type::boolean, move_data)
	{ }

	/*! @brief Create a new element, specifying the HTMLAttribute objects.
	 * @param attributes The HTMLAttributes contained within the element.
	 */
	Boolean(const std::map<string_type, string_type>& attributes)
	: Element<charT>(&attributes, 0, 0, Element<charT>::Element_type::boolean)
	{ }

	//! @brief Create a new element, specifying the HTMLAttribute objects.
	Boolean(std::map<string_type, string_type>&& attributes)
	: Element<charT>(&attributes, 0, 0, Element<charT>::Element_type::boolean, move_attributes)
	{ }

	//! Create a new element, specifying an embedded element
	Boolean(const Element<charT>& e)
	: Element<charT>(0, 0, 0, Element<charT>::Element_type::boolean) {
		embedded.push_back(e);
	}
	
	//! Create a new element, specifying an embedded element
	Boolean(Element<charT>&& e)
	: Element<charT>(0, 0, 0, Element<charT>::Element_type::boolean) {
		embedded.push_back(std::move(e));
	}
	
	//! Create a new element, specifying an embedded element
	Boolean(const embedlist_type& e)
	: Element<charT>(0, &e, 0, Element<charT>::Element_type::boolean) {
	}
	
	/*!
	 * @brief Create a new element, specifying an embedded element
	 */
	//! Create a new element, specifying an embedded element
	Boolean(embedlist_type&& e)
	: Element<charT>(0, &e, 0, Element<charT>::Element_type::boolean, move_embedded) {
		embedded.push_back(e);
	}


	/*!
	 * @brief Create a new element, specifying the enclosed text and attributes
	 * @param attributes The HTMLAttributes contained within the element.
	 * @param text The text within the element.
	 */
	Boolean(const string_type& text, const std::map<string_type, string_type>& attributes)
	: Element<charT>(&attributes, 0, &text, Element<charT>::boolean) {
	}

	/*!
	 * @brief Create a new element, specifying the attributes and embedded elements.
	 * @param attributes The HTMLAttributes contained within the element.
	 * @param embed The HTMLElement embedded inside the element.
	 */
	Boolean(const std::map<string_type, string_type>& attributes, const Element<charT>& embed)
	: Element<charT>(&attributes, &embed, 0, Element<charT>::boolean) {
	}

	//! Destructor
	virtual ~Boolean() { }

	//! Clone this element
	virtual inline boost::shared_ptr<Element<charT>> clone() const {
		return boost::shared_ptr<Element<charT>>(new Boolean<charT,Tag>(*this));
	}
	
	//! Get the name of this element
	virtual inline const string_type name() const {
		return boost::lexical_cast<string_type>(Tag::name());
	}

	/*! @brief Swap the state of this boolean element
	* *A state of @c true indicates the element is currently open */
	virtual inline void swap_state() const {
		state = !state;
	}

	/*! @brief Get the state of this boolean element
	 * @return @c true if this element is open, @c false otherwise
	 */
	virtual inline bool get_state() const {
		return state;
	}
	/*!
	 * @brief Reset the state of this boolean element to closed
	 *
	 */
	static inline void reset() {
		state = false;
	}
	//@}

private:
	bool state;
};

}
}

//template <class charT, class Tag> bool html::Element::Boolean<charT, Tag>::state = false;

MOSH_CGI_END

#endif	

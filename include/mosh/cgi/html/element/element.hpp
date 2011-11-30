//! @file mosh/cgi/html/element/element.hpp Class dealing with HTML elements
/*
 *  Copyright (C) 1996 - 2004 Stephen F. Booth <sbooth@gnu.org>
 *                       2007 Sebastien DIAZ <sebastien.diaz@gmail.com>
 *                       2011 m0shbear
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

#ifndef MOSH_CGI_HTML_ELEMENT_H
#define MOSH_CGI_HTML_ELEMENT_H

#include <string>
#include <list>
#include <boost/shared_ptr.hpp>
#include <mosh/cgi/bits/streamable.hpp>
#include <mosh/cgi/bits/namespace.hpp>

MOSH_CGI_BEGIN

namespace html {
namespace element {

/*! @brief Class representing an HTML element.
 *
 * An HTML element is any entity enclosed in angle brackets (@< and @>)
 * interpreted as HTML, for example @c a, @c img, @c html, and @c body.
 *
 * This class is an abstract base class that defines the interface
 * for all Element subclasses.
 */
template <class charT>
class Element: public Streamable<charT> {
	typedef Streamable<charT> base_type;
	typedef Element<charT> this_type;
public:
	typedef typename boost::shared_ptr<this_type> eptr_type;
	typedef typename base_type::string_type string_type;
	typedef typename std::map<std::basic_string<charT>, std::basic_string<charT>> attrlist_type;
	typedef typename std::vector<eptr_type> embedlist_type;
	/*!
	 * @brief Possible types of Elements
	 *
	 * An Element is either atomic, meaning it has no corresponding
	 * closing tag (elements such as @c hr and @c br are atomic) or
	 * boolean (elements such as @c a and @c ol are boolean)
	 */
	enum class Element_type {
		/*! Atomic element, such as @c hr */
		atomic,
		/*! Boolean element, such as @c strong */
		boolean
	};
	//! List of element attributes.
	attrlist_type attributes;
	//! List of embedded elements, if any
	embedlist_type embedded;
	//! The data contained in this element. Useful only for boolean elements.
	std::basic_string<charT> data;
	//! Element type
	Element_type type;

	/*! @brief Copy constructor.
	 *
	 * Sets the name and internal state of this element to those of @c element
	 * @param element The Element to copy
	 */
	Element(const this_type& element)
	: Element(&element.attributes, &element.embedded, &element.data, element.type) { }

	/*! @brief Move constructor.
	 *
	 * Sets the name and internal state of this element to those of @c element
	 * @param element The Element to move
	 */
	Element(this_type&& element)
	: Element(&element.attributes, &element.embedded, &element.data, element.type, move_all)
	{ }

	//! Destructor
	virtual ~Element() { }

	/*! @brief Compare two Elements for equality.
	 *
	 * Elements are equal if they have the same name
	 * @param element The Element to compare to this one.
	 * @return @c true if the two Elements are equal, @c false otherwise.
	 */
	bool operator==(const this_type& element) const {
		return (name()== element.name()) && ((data.empty())==(element.data.empty()));
	}
	/*! @brief Compare two Elements for inequality.
	 *
	 * Elements are equal if they have the same name
	 * @param element The Element to compare to this one.
	 * @return @c false if the two Elements are equal, @c true otherwise.
	 */
	inline bool operator!=(const this_type& element) const {
		return !operator==(element);
	}
	
	/*! @brief Assignment operator
	 *
	 * Sets the name and internal state of this element to those of @c element
	 * @param element The Element to copy
	 * @return A reference to @c this
	 */
	this_type& operator=(const this_type& element) {
		if (this != &element) {
			attributes = element.attributes;
			embedded = element.embedded;
			data = element.data;
			type = element.type;
		}
		return *this;
	}
	/*! @brief Assignment operator
	 *
	 * Sets the name and internal state of this element to those of @c element
	 * @param element The Element to move
	 * @return A reference to @c this
	 */
	this_type& operator=(this_type&& element) {
		if (this != &element) {
			attributes = std::move(element.attributes);
			embedded = std::move(element.embedded);
			data = std::move(element.data);
			type = element.type;
		}
		return *this;
	}


	//! Get the name of this element.
	virtual const std::basic_string<charT>& name() const = 0;

	//! Clone this Element via deep copy
	virtual this_type* clone() const = 0;

	//! Swap the state of the boolean element
	virtual void swap_state() const { }

	//! Get the state of this boolean element
	virtual bool state() const {
		return false;
	}
	/*! *@brief Render this Element to an ostream
	 *This is used for output
	* @param out The ostream to which to write */
	virtual void render(std::basic_ostream < charT >& out) const {
		if (type == Element_type::boolean && data.empty()) {
			// no embedded elements
			if (embedded.empty()) {
				this->swap_state();
				// state() == true -> element is active
				if (this->get_state()) {
					out << '<' << this->name();
					// render attributes
					if (!attributes.empty()) {
						for (const auto& attr : attributes) {
							out << ' ' << attr.first << '=' << '"' << attr.second << '"';
						}
					}
					out << '>';
				} else
					out << '<' << '/' << this->name() << '>';
			} else { // embedded elements
				out << '<' << this->name();
				// render attributes
				if (!attributes.empty()) {
					out << ' ';
					for (const auto& attr : attributes) {
						out << ' ' << attr.first << '=' << '"' << attr.second << '"';
					}
				}
				out << '>';
				for (const auto& em, embedded) {
					em->render(out);
				}
				out << '<' << '/' << name() << '>';
			}
		} else { // non-boolean
			if (type == Element_type::atomic) {
				out << '<' << name();
				// render attributes
				if (!attributes.empty()) {
					for (const auto& attr : attributes) {
						out << ' ' << attr.first << '=' << '"' << attr.second << '"';
					}
				}					
				out << ' ' << '/' << '>';
			} else {
				out << '<' << name();
				// render attributes
				if (!attributes.empty()) {
					for (const auto& attr : attributes) {
						out << ' ' << attr.first << '=' << '"' << attr.second << '"';
					}
				}
				out << '>';
				if (!embedded.empty()) {
					for (const auto& em, embedded) {
						em->render(out);
					}
				else
					out << data;
				out << '<' << '/' << name() << '>';
			}
		}
	}

protected:

	//! Initialize this->data via std::move
	static const unsigned move_attributes = 1<<0;
	//! Initialize this->embedded via std::move
	static const unsigned move_embedded = 1<<1;
	//! Initialize this->data via std::move
	static const unsigned move_data = 1<<2;
	//! Initialize everything via std::move 
	static const unsigned move_all = move_attributes | move_embedded | move_data;

	/*! @brief Subclass constructor
	 *
	 * This allows the subclasses to fully specify all properties
	 * @param attributes A pointer to a map of attributes
	 * @param embedded A pointer to any array of embedded Elements, if any
	 * @param data A pointer to the data, if any
	 * @param type The type of element
	 * @param do_move a bitmask of move options
	 */
	Element(const attrlist_type* attributes_, const embedlist_type* embedded_, const string_type* data_, const Element_type type_,
		unsigned do_move = 0)
	: base_type(), attributes(), embedded(), data(), type(type_) {
		if (attributes_) {
			if (do_move & move_attributes) 
				attributes = std::move(*attributes_);
			else
				attributes = *attributes_;
		}
		if (embedded_) {
			if (do_move & move_embedded)
				embedded = std::move(*embedded_);
			else
				embedded = *embedded_;
		}
		if (data_) {
			if (do_move & move_data)
				data = std::move(*data_);
			else
				data = *data_;
		}
	}

	/*! @brief Subclass constructor
	 *
	 * This allows the subclasses to fully specify all properties
	 * @param attributes A pointer to a map of attributes
	 * @param embedded A pointer to any array of embedded Elements, if any
	 * @param data A pointer to the data, if any
	 * @param type The type of element
	 * @param do_move a bitmask of move options
	 */
	Element(std::list<attribute_type>* attributes_, embedlist_type* embedded_, string_type* data_, ElementType type_,
		unsigned do_move = 0)
	: base_type(), attributes(), embedded(), data(), type(type_) {
		if (attributes_) {
			if (do_move & move_attributes) 
				attributes = std::move(*attributes_);
			else
				attributes = *attributes_;
		}
		if (embedded_) {
			if (do_move & move_embedded)
				embedded = std::move(*embedded_);
			else
				embedded = *embedded_;
		}
		if (data_) {
			if (do_move & move_data)
				data = std::move(*data_);
			else
				data = *data_;
		}
	}
private:
	Element();
};

}
}

MOSH_CGI_END

#endif

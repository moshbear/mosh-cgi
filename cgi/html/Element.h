/* -*-mode:c++; c-file-style: "gnu";-*- */
/*
 *  $Id: HTMLElement.h,v 1.8 2007/07/02 18:48:18 sebdiaz Exp $
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

#ifndef _CGI_HTML_ELEMENT_H_
#define _CGI_HTML_ELEMENT_H_ 1

/*! \file HTMLElement.h
 * \brief Class dealing with HTML elements
 *
 * For example, \c a, \c img, \c html, and \c body, are all HTML elements.
 */

#include <string>
#include <list>
#include <boost/shared_ptr.hpp>
#include <util/STL_Foreach.h>
#include <cgi/common/Streamable.h>
#include <cgi/common/Kv.h>

namespace cgi { namespace html {

//template < class charT > class HTMLElementList;

// ============================================================
// Class HTMLElement
// ============================================================

/*! \class HTMLElement HTMLElement.h cgi/HTMLElement.h
 * \brief Class representing an HTML element.
 *
 * An HTML element is any entity enclosed in angle brackets (\< and \>)
 * interpreted as HTML, for example \c a, \c img, \c html, and \c body.
 *
 * This class is an abstract base class that defines the interface
 * for all HTMLElement subclasses.
 */
template <class charT>
class Element: public cgi::common::Streamable<charT> {
	typedef cgi::common::Streamable<charT> base_type;
	typedef cgi::html::Element<charT> this_type;
public:
	typedef typename boost::shared_ptr<this_type> element_r_type;
	typedef typename base_type::string_type string_type;
	typedef typename cgi::common::KV<charT, false, true> attribute_type;
	
	/*!
	 * \brief Possible types of HTMLElements
	 *
	 * An HTMLElement is either atomic, meaning it has no corresponding
	 * closing tag (elements such as \c hr and \c br are atomic) or
	 * boolean (elements such as \c a and \c ol are boolean)
	 */
	enum ElementType {
		/*! Atomic element, such as \c hr */
		atomic,
		/*! Boolean element, such as \c strong */
		boolean
	};
	/*! \brief List of element attributes.
	 */
	std::list<attribute_type> attributes;
	/*! \brief List of embedded elements, if any.
	 */
	std::list<element_r_type> embedded;
	/*! \brief The data contained in this element.
	 * Useful only for boolean elements.
	 */
	std::basic_string<charT> data;
	/*! \brief Element type
	 */
	ElementType type;
	// ============================================================

	/*! \name Constructors and Destructor */
	//@{

	/*!
	 * \brief Copy constructor.
	 *
	 * Sets the name and internal state of this element to those of \c element
	 * \param element The HTMLElement to copy.
	 */
	Element(const this_type& element)
	: Element(&element.attributes, &element.embedded, &element.data, element.type) { }
	/*!
	 * \brief Destructor
	 *
	 * Delete this HTMLElement object
	 */

	virtual ~Element() {
	}
	//@}

	// ============================================================

	/*!
	 * \brief Compare two HTMLElements for equality.
	 *
	 * HTMLElements are equal if they have the same name
	 * \param element The HTMLElement to compare to this one.
	 * \return \c true if the two HTMLElements are equal, \c false otherwise.
	 */
	bool operator==(const this_type& element) const {
		return (name()== element.name()) && ((data.empty())==(element.data.empty()));
	}
	/*!
	 * \brief Compare two HTMLElements for inequality.
	 *
	 * HTMLElements are equal if they have the same name
	 * \param element The HTMLElement to compare to this one.
	 * \return \c false if the two HTMLElements are equal, \c true otherwise.
	 */
	inline bool operator!=(const this_type& element) const {
		return !operator==(element);
	}
	/*!
	 * \brief Assignment operator
	 *
	 * Sets the name and internal state of this element to those of \c element
	 * \param element The HTMLElement to copy
	 * \return A reference to \c this
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
	this_type& operator=(this_type&& element) {
		if (this != &element) {
			attributes = std::move(element.attributes);
			embedded = std::move(element.embedded);
			data = std::move(element.data);
			type = element.type;
		}
		return *this;
	}


	// ============================================================

	/*! \name Accessor Methods
	 * Information on the element
	 */
	//@{

	/*!
	 * \brief Get the name of this element.
	 *
	 * For example, \c html or \c body.
	 * \return The name of this element.
	 */
	virtual const std::basic_string<charT>& name() const = 0;

	//@}
	// ============================================================
	//*! \name Mutator Methods 
	// * Set properties of the element
	//@{
	/*!
	 * \brief Clone this HTMLElement
	 *
	 * This performs a deep copy of the element
	 * \return A pointer to a newly-allocated copy of \c this.
	 */
	virtual this_type* clone() const = 0;

	//@}

	// ============================================================

	/*! \name Boolean element methods
	 * Methods specific to boolean elements
	 */
	//@{

	/*!
	 * \brief Swap the state of the boolean element
	 *
	 * A state of \c true means the element is active
	 */
	virtual void swapState() const { }
	/*!
	 * \brief Get the state of this boolean element
	 *
	 *
	 * \return \c true if the element is active, \c false otherwise
	 */
	virtual bool state() const {
		return false;
	}
	//@}
	/*! *\brief Render this HTMLElement to an ostream
	 *This is used for output
	* \param out The ostream to which to write */
	virtual void render(std::basic_ostream < charT >& out) const {
		if (type == boolean && data.empty()) {
			// no embedded elements
			if (embedded.empty()) {
				// state() == true => element is active
				if (state()) {
					out << '<' << name();
					// render attributes
					if (!attributes.empty()) {
						out << ' ';
						FOREACH(auto const& iter, attributes, {
							iter->render(out);
							out << ' ';
						})
					}
					out << '>';
				} else
					out << '<' << '/' << name() << '>';
			} else { // embedded elements
				out << '<' << name();
				// render attributes
				if (!attributes.empty()) {
					out << ' ';
					FOREACH(auto const& iter, attributes, {
						iter->render(out);
						out << ' ';
					})
				}
				out << '>';
				FOREACH(auto const& iter, embedded, iter->get()->render(out);)
				out << '<' << '/' << name() << '>';
			}
		} else { // non-boolean
			if (type == atomic) {
				out << '<' << name();
				// render attributes
				if (!attributes.empty()) {
					out << ' ';
					FOREACH(auto const& iter, attributes, {
						iter->render(out);
						out << ' ';
					})
				}					
				out << ' ' << '/' << '>';
			} else {
				out << '<' << name();
				// render attributes
				if (!attributes.empty()) {
					out << ' ';
					FOREACH(auto const& iter, attributes, {
						iter->render(out);
						out << ' ';
					})
				}
				out << '>';
				if (!embedded.empty())
					FOREACH(auto const& iter, embedded, iter->get()->render(out);)
				else
					out << data;
				out << '<' << '/' << name() << '>';
			}
		}
	}

protected:

	/*!
	 * \brief Subclass constructor
	 *
	 * This allows the subclasses to fully specify all properties
	 * \param attributes A pointer to a std::list containing KV attributes, if any
	 * \param embedded A pointer to the embedded HTMLElement, if any
	 * \param data A pointer to the data, if any
	 * \param type The type of element
	 */
	Element(const std::list<attribute_type>* attributes_, const this_type* embedded_, const string_type* data_, ElementType type_)
	: base_type(), attributes(), embedded(), data(), type(type_) {
		if (attributes_)
			attributes = *attributes_;
		if (embedded_)
			embedded.push_back(element_r_type(embedded_->clone()));
		if (data_) 
			data = *data_;
	}
private:
	Element();
};

} } // namespace html // namespace cgi

#endif				/* ! _CGI_HTML_ELEMENT_H_ */

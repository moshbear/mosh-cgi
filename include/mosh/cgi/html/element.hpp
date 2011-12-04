//! @file mosh/cgi/html/element.hpp Class dealing with HTML elements
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

#ifndef MOSH_CGI_HTML_ELEMENT_HPP
#define MOSH_CGI_HTML_ELEMENT_HPP

#include <sstream>
#include <string>
#include <map>
#include <set>
#include <utility>
#include <stdexcept>
#include <mosh/cgi/bits/singleton.hpp>
#include <mosh/cgi/bits/t_string.hpp>
#include <mosh/cgi/bits/namespace.hpp>

MOSH_CGI_BEGIN

namespace html {

namespace element {

//! Attribute tagger. Use it to create std::pair<T1,T2>s representing element attributes.
template <typename charT>
std::pair<std::string, std::basic_string<charT>>
P(const std::string& s1, const std::basic_string<charT>& s2) {
	return std::make_pair(s1, s2);
}

std::pair<std::string, std::string> SP(const std::string& s1, const std::string& s2) {
	return P(s1, s2);
}

std::pair<std::string, std::wstring> WSP(const std::string& s1, const std::wstring& s2) {
	return P(s1, s2);
}

//! An HTML element
template <typename charT>
class Element  {
	typedef Element<charT> this_type;
public:
	//! Typedef for strings
	typedef typename std::basic_string<charT> string_type;
	//! Typedef for attributes
	typedef typename std::pair<std::string, string_type> attribute_type;
	//! Typedef for attribute lists
	typedef typename std::map<std::string, string_type> attrlist_type;
 
	//! Enumeration for encoding type
	enum class Type {
		//! Encodes an atomic <foo /> element; resulting tag will have attributes but no data
		atomic,
		//! Encodes a boolean <foo></foo> element; resulting tag will have both attributes and data
		boolean,
		//! Encodes a comment <!-- foo --> element; resulting tag will "have" data but no attributes
		comment
	};

	/*! @brief Create a new element with a given name and type
	 *  @param[in] name Element name
	 *  @param[in] type Element type
	 *  @sa Type_constants
	 */
	Element(const std::string& name_, Type type_)
	: name(name_), type(type_)
	{ }

	//! Destructor
	virtual ~Element() { }

	/*! @name Appenders
	 * These overloads append attributes and values to existing elements.
	 */
	//@{
	/*! @brief Add an attribute.
	 *  @param[in] _a attribute
	 */
	Element& operator () (const attribute_type& _a) {
		attributes.insert(_a);
		return *this;
	}

	/*! @brief Add attribute(s).
	 *  @param[in] _a-{} list of attributes
	 */
	Element& operator () (std::initializer_list<attribute_type> _a) {
		for (const auto& at : _a)
			attributes.insert(at);
		return *this;
	}
	
	/*! @brief Add a value.
	 *  @param[in] _v value
	 */
	Element& operator () (const string_type& _v) {
		data += _v;
		return *this;
	}

	/*! @brief Add value(s).
	 *  @param[in] _v {}-list of values
	 */
	Element& operator () (std::initializer_list<string_type> _v) {
		for (const string_type& vv : _v)
			data += vv;
		return *this;
	}

	/*! @brief Add an attribute and a value.
	 *  @param[in] _a attribute
	 *  @param[in] _v value
	 */
	Element& operator () (const attribute_type& _a, const string_type& _v) {
		attributes.insert(_a);
		data += _v;
		return *this;
	}
	
	/*! @brief Add an attribute and value(s).
	 *  @param[in] _a attribute
	 *  @param[in] _v {}-list of values
	 */
	Element& operator () (const attribute_type& _a, std::initializer_list<string_type> _v) {
		attributes.insert(_a);
		for (const string_type& vv : _v)
			data += vv;
		return *this;
	}

	/*! @brief Add attribute(s) and a value.
	 *  @param[in] _a {}-list of attributes
	 *  @param[in] _v value
	 */
	Element& operator () (std::initializer_list<attribute_type> _a, const string_type& _v) {
		for (const auto& at : _a)
			attributes.insert(at);
		data += _v;
		return *this;
	}

	/*! @brief Add attribute(s) and value(s).
	 *  @param[in] _a {}-list of attributes
	 *  @param[in] _v {}-list of values
	 */
	Element& operator () (std::initializer_list<attribute_type> _a, std::initializer_list<string_type> _v) {
		for (const auto& at : _a)
			attributes.insert(at);
		for (const string_type& vv : _v)
			data += vv;
		return *this;
	}

	/*! @brief Add an attribute.
	 *  @param[in] _a attribute
	 */
	Element& operator << (const attribute_type& _a) {
		attributes.insert(_a);
		return *this;
	}

	/*! @brief Add attribute(s).
	 * @param[in] _a {}-list of attributes
	 */
	Element& operator << (std::initializer_list<attribute_type> _a) {
		for (const auto& at : _a)
			attributes.insert(at);
		return *this;
	}

	/*! @brief Add a value.
	 * @param[in] _v value
	 */
	Element& operator << (const string_type& _v) {
		data += _v;
		return *this;
	}
	
	/*! @brief Add value(s).
	 * @param[in] _v {}-list of values
	 */
	Element& operator << (std::initializer_list<string_type> _v) {
		for (const auto& vv : _v)
			attributes.insert(vv);
		return *this;
	}

	//@}

	/*! @brief String cast operator
	 *  Renders the element, with attributes and pre-rendered data.
	 *  @warn No escaping is done.
	 */
	operator string_type() const {
		std::basic_stringstream<charT> s;
		s << wide_char<charT>('<');
		if (type == Type::comment) {
			s << wide_string<charT>("!--");
		} else {
			s << wide_string<charT>(name);
			for (const auto& a : attributes) {
				s << wide_char<charT>(' ');
				s << wide_string<charT>(a.first);
				s << wide_char<charT>('=');
				s << wide_char<charT>('"');
				s << wide_string<charT>(a.second);
				s << wide_char<charT>('"');
			}
		}
		if (type == Type::atomic) {
			s << wide_char<charT>(' ');
			s << wide_char<charT>('/');
		} else {
			if (type == Type::boolean) {
				s << wide_char<charT>('>');
			}
			s << data;
			if (type == Type::boolean) {
				s << wide_char<charT>('<');
				s << wide_char<charT>('/');
				s << wide_string<charT>(name);
			} else if (type == Type::comment) {
				s << wide_string<charT>("--");
			}
		}
		s << '>';

		return s.str();
	}

private:
	//! Element name
	std::string name;
	//! Element type
	Type type;
	/*! @brief List of attributes.
	 *  Unused for comment elements.
	 *  @warning Is not propagated via operator ()
	 */
	attrlist_type attributes;
	/*! @brief Pre-rendered embedded data
	 *  Unused for atomic elements.
	 *  @warning Is not propagated via operator ()
	 */
	string_type data;
	Element() = delete;
	Element(const std::string& name_, Type type_, const attrlist_type* _a, const string_type* _v)
	: name(name_), type(type_)
	{
		if (_a)
			attributes = *_a;
		if (_v)
			data = *_v;
	}

};

//! Singleton element generator
template <typename charT>
class Element_generator {
public:
	Element<charT> operator[](const std::string& name) const {
		return  _generator::instance()[name];
	}
private:
	class _generator : public Singleton<_generator> {
	public:
		//! Initialization
		_generator() {
			atomics = {
					"meta", "br", "col", "link",
					"base", "img", "param", "area",
					"hr", "frame", "input"
				};
			booleans = {
					"html", "head", "title", "style",
					"body", "div", "span", "h1",
					"h2", "h3", "h4", "h5",
					"h6", "address", "em", "strong",
					"cite",	"dfn",	"code",	"samp",	
					"kbd",	"var",	"abbr",	"acronym",
					"blockquote", "q", "sub", "sup",
					"p", "pre", "ins", "del",
					"bdo", "ul", "ol", "li",
					"dl", "dt", "dd", "table",
					"caption", "thead", "tfoot", "tbody",
					"colgroup", "tr", "th", "td",
					"a", "object", "map", "tt",
					"i", "b", "big", "small",
					"frameset", "noframes", "iframe", "form",
					"button", "select", "optgroup", "option",
					"textarea", "label", "fieldset", "legend",
					"script", "noscript"
				};
		}

		//! Generate an element
		Element<charT> operator [] (const std::string& name) const {
			if (name == "comment") {
				return Element<charT>("", Element<charT>::Type::comment);
			}
			if (atomics.find(name) != atomics.end()) {
				return Element<charT>(name, Element<charT>::Type::atomic);
			}
			if (booleans.find(name) != booleans.end()) {
				return Element<charT>(name, Element<charT>::Type::boolean);
			}
			throw std::invalid_argument("tag does not exist");
			
		}
	private:
		//! A list of atomic elements
		std::set<std::string> atomics;
		//! A list of boolean elements
		std::set<std::string> booleans;
	};
};

Element_generator<char> SElement;
Element_generator<wchar_t> WSElement;

}

}

MOSH_CGI_END


#endif

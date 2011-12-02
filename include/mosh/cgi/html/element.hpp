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

/*! @name Attribute taggers
 */
//@{
/*! @brief Attribute tagger
 * When creating inline attributes, enclose the key with Attr() and use << to pipe in the value
 * @code
 * 	{ "foo", "bar" } -> Attr("foo")<<"bar"
 * @endcode
 */
template <typename charT>
class Attr {	
public:
	//! Create an Attr with a given key
	Attr(const std::string& _str)
	: str(_str)
	{ }
	//! Destructor
	virtual ~Attr() { }
	//! Transform this Attr into a {K,V} pair
	std::pair<std::string, std::basic_string<charT>>
		operator<<(std::basic_string<charT>& str2) const
	{
		return std::make_pair(str, str2);
	}
private:
	Attr() = delete; // prevent default construction
	//! Key string
	std::string str;
	
};

/*! @brief Attribute list tagger
 * When creating inline attribute list, enclose the first pair with an Attr_list
 * and use << to pipe in addition attributes.
 * @code
 * 	{ { "foo", "bar" }, { "alice", "bob" } } ->
 * 		Attr_list(Attr("foo")<<"bar")<<(Attr("alice")<<"bob")
 * or 		Attr_list() << (Attr("foo")<<"bar")<<(Attr("alice")<<"bob")
 * @endcode
 */
template <typename charT>
class Attr_list {
	//! Typedef for ASCII strings (keys are always ascii)
	typedef std::string str1_type;
	//! Typedef for template-dependent strings
	typedef std::basic_string<charT> str2_type;
public:
	//! Destructor
	virtual ~Attr_list() { }
	//! Create an Attr_list with a given attribute pair
	Attr_list(const std::pair<str1_type, str2_type>& p)
	{
		attr.insert(p);
	}
	//! Add another attribute pair to the set
	Attr_list& operator << (const std::pair<str1_type, str2_type>& p)
	{
		attr.insert(p);
		return *this;
	}
	//! Get a const reference to the accumulated map
	operator const std::map<str1_type, str2_type>& () const {
		return attr;
	}
private:
	//! Set of accumulated attributes
	std::map<str1_type, str2_type> attr;
};
//@}
/*! @brief String piper
 * When creating joinable inline strings, enclose the first value with Pipable() and use
 * << to pipe in additional values.
 * @code
 * 	"foo" + "bar" ->
 * 		Pipable("foo")<<"bar"
 * or		Pipable() << "foo" << "bar"
 * @endcode
 */
template <typename charT>
class Pipable {
public:
	//! Create a Pipable with a given string
	Pipable(const std::basic_string<charT>& _str)
	: str(_str)
	{ }
	//! Destructor
	virtual ~Pipable() { }
	//! Add another value to the string
	Pipable& operator << (const std::basic_string<charT>& str2) {
		str += str2;
		return *this;
	}
	//! Get a const reference to the accumulated string
	operator const std::basic_string<charT>& () const {
		return str;
	}
private:
	//! The accumulated string
	std::basic_string<charT> str;
};

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
	 *  @param[in] _a Attribute
	 */
	Element& operator << (const attribute_type& _a) {
		attributes.insert(_a);
		return *this;
	}

	/*! @brief Add a given set of attributes.
	 * @param[in] _a Attributes
	 */
	Element& operator << (const attrlist_type& _a) {
		for (const auto& at : _a)
			attributes.insert(at);
		return *this;
	}

	/*! @brief Add a given embedded value
	 * @param[in] _v Embedded value
	 */
	Element& operator << (const string_type& _v) {
		data += _v;
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
			if (is_boolean(type)) {
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

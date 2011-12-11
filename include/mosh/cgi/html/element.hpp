//! @file mosh/cgi/html/element.hpp Class dealing with HTML elements
/*
 *  Copyright (C) 2011 m0shbear
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
#include <mosh/cgi/html/doctype.hpp>
#include <mosh/cgi/bits/t_string.hpp>
#include <mosh/cgi/bits/namespace.hpp>

MOSH_CGI_BEGIN

//! HTML classes
namespace html {

//! HTML element classes
namespace element {

//! Enumeration for encoding type
enum class Type {
	//! Encodes an unary <foo /> element; resulting tag will have attributes but no data
	unary,
	//! Encodes a binary <foo></foo> element; resulting tag will have both attributes and data
	binary,
	//! Encodes a DOCTYPE directive <!FOO >
	dtd,
	//! Encodes a comment <!-- foo -->
	comment
};

//! An HTML element
template <typename charT>
class Element  {
	typedef Element<charT> this_type;
public:
	//! Typedef for strings
	typedef typename std::basic_string<charT> string;
	//! Typedef for attributes
	typedef typename std::pair<std::string, string> attribute;
	//! Typedef for attribute lists
	typedef typename std::map<std::string, string> attr_list;
 
	/*! @brief Create a new element with a given name and type
	 *  @param[in] name_ Element name
	 *  @param[in] type_ Element type
	 *  @sa Type
	 */
	Element(const std::string& name_, Type type_)
	: name(name_), type(type_)
	{ }

	//! Copy constructor
	Element(const Element& e)
	: attributes(e.attributes), data(e.data), name(e.name), type(e.type)
	{ }

	//! Move constructor
	Element(Element&& e)
	: attributes(std::move(e.attributes)), data(std::move(e.data)), name(std::move(e.name)), type(e.type)
	{ }

	//! Destructor
	virtual ~Element() { }

	//! Assignment operator
	this_type& operator = (const this_type& e) {
		if (this != &e) {
			attributes = e.attributes;
			data = e.data;
			name = e.name;
			type = e.type;
		}
		return *this;
	}
	//! Assignment operator
	this_type operator = (this_type&& e) {
		if (this != &e) {
			attributes = std::move(e.attributes);
			data = std::move(e.data);
			name = std::move(e.name);
			type = e.type;
		}
		return *this;
	}

	/*! @name Clone and call
	 * These overloads create new elements which behave as if attributes and values 
	 * were appended to existing elements.
	 */
	//@{
	/*! @brief Create a copy of @c *this.
	 */
	this_type operator () () const {
		return this_type(*this);
	}

	/*! @brief Create a copy of @c *this with a given attribute.
	 *  @param[in] _a attribute
	 */
	this_type operator () (const attribute& _a) const {
		this_type e(*this);
		e += _a;
		return e;
	}

	/*! @brief Create a copy of @c *this with given attribute(s).
	 *  @param[in] _a-{} list of attributes
	 */
	this_type operator () (std::initializer_list<attribute> _a) const {
		this_type e(*this);
		e += _a;
		return e;
	}
	
	/*! @brief Create a copy of @c *this with a given value.
	 *  @param[in] _v value
	 */
	this_type operator () (const string& _v) const {
		this_type e(*this);
		e += _v;
		return e;
	}

	/*! @brief Create a copy of @c *this with given value(s).
	 *  @param[in] _v {}-list of values
	 */
	this_type operator () (std::initializer_list<string> _v) const {
		this_type e(*this);
		e += _v;
		return e;
	}

	/*! @brief Create a copy of @c this with a given attribute and value.
	 *  @param[in] _a attribute
	 *  @param[in] _v value
	 */
	this_type operator () (const attribute& _a, const string& _v) const {
		this_type e(*this);
		e += _a;
		e += _v;
		return e;
	}
	
	/*! @brief Create a copy of @c this with a given attribute and value(s).
	 *  @param[in] _a attribute
	 *  @param[in] _v {}-list of values
	 */
	this_type operator () (const attribute& _a, std::initializer_list<string> _v) const {
		this_type e(*this);
		e += _a;
		e += _v;
		return e;
	}

	/*! @brief Create a copy of @c this with given attribute(s) and a value.
	 *  @param[in] _a {}-list of attributes
	 *  @param[in] _v value
	 */
	this_type operator () (std::initializer_list<attribute> _a, const string& _v) const {
		this_type e(*this);
		e += _a;
		e += _v;
		return e;
	}

	/*! @brief Add attribute(s) and value(s).
	 *  @param[in] _a {}-list of attributes
	 *  @param[in] _v {}-list of values
	 */
	this_type operator () (std::initializer_list<attribute> _a, std::initializer_list<string> _v) const {
		this_type e(*this);
		e += _a;
		e += _v;
		return e;
	}
	//@}
	/*! @name Appenders
	 */
	//@{
	/*! @brief Add an attribute.
	 *  @param[in] _a attribute
	 */
	this_type& operator += (const attribute& _a) {
		attributes.insert(_a);
		post_insertion_hook(_a);
		return *this;
	}

	/*! @brief Add attribute(s).
	 * @param[in] _a {}-list of attributes
	 */
	this_type& operator += (std::initializer_list<attribute> _a) {
		for (const auto& at : _a) {
			attributes.insert(at);
			post_insertion_hook(at);
		}
		return *this;
	}

	/*! @brief Add a value.
	 * @param[in] _v value
	 */
	this_type& operator += (const string& _v) {
		data += _v;
		return *this;
	}
	
	/*! @brief Add value(s).
	 * @param[in] _v {}-list of values
	 */
	this_type& operator += (std::initializer_list<string> _v) {
		for (const auto& vv : _v)
			data += vv;
		return *this;
	}

	//@}

	/*! @brief String cast operator
	 *  Renders the element, with attributes and pre-rendered data.
	 *  @warn No escaping is done.
	 */
	virtual operator string() const {
		std::basic_stringstream<charT> s;
		s << wide_char<charT>('<');
		s << wide_string<charT>(name);
		for (const auto& a : this->attributes) {
			s << wide_char<charT>(' ');
			s << wide_string<charT>(a.first);
			s << wide_char<charT>('=');
			s << wide_char<charT>('"');
			s << a.second;
			s << wide_char<charT>('"');
		}
		if (type == Type::unary) {
			s << wide_char<charT>(' ');
			s << wide_char<charT>('/');
		} else {
			if (type == Type::binary) {
				s << wide_char<charT>('>');
			}
			s << this->data;
			if (type == Type::binary) {
				s << wide_char<charT>('<');
				s << wide_char<charT>('/');
				s << wide_string<charT>(name);
			} else if (type == Type::comment) {
				s << wide_string<charT>("--");
			}
		}
		s << wide_char<charT>('>');

		return s.str();
	}
	//! String cast operator
	virtual operator const charT* () const {
		return this->operator string().c_str();
	}
protected:
	//! Default constructor for derived classes
	Element() { }

	/*! @brief A post-insertion hook
	 * Define in derived classes in order to effect postconditions on insertions
	 * where the key is of an interesting value.
	 */
	virtual void post_insertion_hook(const attribute&) { }

	/*! @brief List of attributes.
	 *  Unused for comment elements.
	 */
	attr_list attributes;
	/*! @brief Pre-rendered embedded data
	 *  Unused for unary elements.
	 */
	string data;
private:
	//! Element name
	std::string name;
	//! Element type
	Type type;
};

/*! @name Concatenators
 * These overloads create new elements which behave as if 
 * attributes and values were appendedto existing elements.
 */
//@{
/*! @brief Concatenate an attribute.
 *  @param[in] _e element
 *  @param[in] _a attribute
 */
template <typename charT>
Element<charT> operator + (const Element<charT>& _e, const typename Element<charT>::attribute& _a) {
	Element<charT> e(_e);
	e += _a;
	return e;
}

/*! @brief Concatenate an attribute.
 *  @param[in] _e element
 *  @param[in] _a attribute
 */
template <typename charT>
Element<charT> operator + (Element<charT>&& _e, const typename Element<charT>::attribute& _a) {
	Element<charT> e(_e);
	e += _a;
	return std::move(e);
}

/*! @brief Concatenate attribute(s).
 *  @param[in] _e element
 *  @param[in] _a {}-list of attributes
 */
template <typename charT>
Element<charT> operator + (const Element<charT>& _e, std::initializer_list<typename Element<charT>::attribute> _a) {
	Element<charT> e(_e);
	e += _a;
	return e;
}

/*! @brief Concatenate attribute(s).
 *  @param[in] _e element
 *  @param[in] _a {}-list of attributes
 */
template <typename charT>
Element<charT> operator + (Element<charT>&& _e, std::initializer_list<typename Element<charT>::attribute> _a) {
	Element<charT> e(_e);
	e += _a;
	return std::move(e);
}

/*! @brief Concatenate a value.
 *  @param[in] _e element 
 *  @param[in] _v value
 */
template <typename charT>
Element<charT> operator + (const Element<charT>& _e, const std::basic_string<charT>& _v) {
	Element<charT> e(_e);
	e += _v;
	return e;
}
	
/*! @brief Concatenate a value.
 *  @param[in] _e element 
 *  @param[in] _v value
 */
template <typename charT>
Element<charT> operator + (Element<charT>&& _e, const std::basic_string<charT>& _v) {
	Element<charT> e(_e);
	e += _v;
	return std::move(e);
}
	
/*! @brief Concatenate value(s).
 *  @param[in] _e element
 *  @param[in] _v {}-list of values
 */
template <typename charT>
Element<charT> operator + (const Element<charT>& _e, std::initializer_list<std::basic_string<charT>> _v) {
	Element<charT> e(_e);
	e += _v;
	return e;
}

/*! @brief Concatenate value(s).
 *  @param[in] _e element
 *  @param[in] _v {}-list of values
 */
template <typename charT>
Element<charT> operator + (Element<charT>&& _e, std::initializer_list<std::basic_string<charT>> _v) {
	Element<charT> e(_e);
	e += _v;
	return std::move(e);
}
//@}


/*! @brief HTML begin class
 * This class outputs <!DOCTYPE ...><html ...> when cast to string.
 * @note For XHTML, it adds xmlns.
 * @note Data added is ignored
 */
template <typename charT>
class HTML_begin : public Element<charT> {
	typedef HTML_begin<charT> this_type;
public:
	//! Typedef for strings
	typedef typename std::basic_string<charT> string;
	//! Typedef for attributes
	typedef typename std::pair<std::string, string> attribute;
	//! Typedef for attribute lists
	typedef typename std::map<std::string, string> attr_list;
 
	/*! @brief Create a new HTML start with a given type
	 *  @param[in] type_ HTML type
	 *  @sa doctype::HTML_revision
	 */
	HTML_begin(doctype::HTML_revision type_ = doctype::HTML_revision::xhtml_10_transitional)
	: Element<charT>(), type(type_)
	{
		if (is_xhtml()) {
			this->attributes.insert(std::make_pair("xmlns", wide_string<charT>("http://www.w3.org/1999/xhtml")));
		}
	}

	//! Copy constructor
	HTML_begin(const HTML_begin& b)
	: Element<charT>(b), type(b.type)
	{ }

	//! Move constructor
	HTML_begin(HTML_begin&& b)
	: Element<charT>(std::move(b)), type(b.type)
	{ }

	//! Destructor
	virtual ~HTML_begin() { }

	/*! @brief String cast operator
	 *  Renders the element, with attributes and pre-rendered data.
	 *  @warn No escaping is done.
	 */
	virtual operator string() const {
		std::basic_stringstream<charT> s;
		doctype::HTML_doctype_generator<charT> dg;
		s << dg(type);
		s << wide_string<charT>("<html");
		for (const auto& a : this->attributes) {
			s << wide_char<charT>(' ');
			s << wide_string<charT>(a.first);
			s << wide_char<charT>('=');
			s << wide_char<charT>('"');
			s << a.second;
			s << wide_char<charT>('"');
		}
		s << wide_char<charT>('>');

		return s.str();
	}

protected:
	virtual void post_insertion_hook(const attribute& _a) {
		if (is_xhtml()) {
			if (_a.first == "lang") { // make use of lang attribute XHTML-conforming
				this->attributes.insert(std::make_pair("xml:lang", _a.second));
			}
		}
	}
private:
	doctype::HTML_revision type;
	bool is_xhtml() const {
		switch (type) {
		case doctype::HTML_revision::xhtml_10:
		case doctype::HTML_revision::xhtml_10_transitional:
		case doctype::HTML_revision::xhtml_10_frameset:
		case doctype::HTML_revision::xhtml_11:
		case doctype::HTML_revision::xhtml_basic_11:
			return true;
		default:
			return false;
		}
	}

};

//! This class prints </html>
template <typename charT>
struct HTML_end {
	virtual operator std::basic_string<charT> () const {
		return wide_string<charT>("</html>");
	}
	//! String cast operator
	virtual operator const charT* () const {
		return this->operator std::basic_string<charT>().c_str();
	}
};

/*! @brief body begin class
 * This class outputs <body ...> when cast to string.
 * @note Data added is ignored
 */
template <typename charT>
class Body_begin : public Element<charT> {
	typedef Body_begin<charT> this_type;
public:
	//! Typedef for strings
	typedef typename std::basic_string<charT> string;
	//! Typedef for attributes
	typedef typename std::pair<std::string, string> attribute;
	//! Typedef for attribute lists
	typedef typename std::map<std::string, string> attr_list;
 
 	//! Default constructor
	Body_begin()
	: Element<charT>()
	{ }
	//! Copy constructor
	Body_begin(const Body_begin& b)
	: Element<charT>(b)
	{ }

	//! Move constructor
	Body_begin(Body_begin&& b)
	: Element<charT>(std::move(b))
	{ }

	//! Destructor
	virtual ~Body_begin() { }

	/*! @brief String cast operator
	 *  Renders the element, with attributes and pre-rendered data.
	 *  @warn No escaping is done.
	 */
	virtual operator string() const {
		std::basic_stringstream<charT> s;
		s << wide_string<charT>("<body");
		for (const auto& a : this->attributes) {
			s << wide_char<charT>(' ');
			s << wide_string<charT>(a.first);
			s << wide_char<charT>('=');
			s << wide_char<charT>('"');
			s << a.second;
			s << wide_char<charT>('"');
		}
		s << wide_char<charT>('>');

		return s.str();
	}

protected:
	virtual void post_insertion_hook(const attribute&) {
	}
};

//! This class prints </body>
template <typename charT>
struct Body_end {
	virtual operator std::basic_string<charT> () const {
		return wide_string<charT>("</body>");
	}
	//! String cast operator
	virtual operator const charT* () const {
		return this->operator std::basic_string<charT>().c_str();
	}
};

#ifdef MOSH_CGI_USING_BYTE_ELEMENTS
#include <mosh/cgi/html/element/s.hpp>
#endif
#ifdef MOSH_CGI_USING_WIDE_ELEMENTS
#include <mosh/cgi/html/element/ws.hpp>
#endif

/*! @name Attribute taggers
 */
//@{
//! Attribute tagger. Use it to create std::pair<T1,T2>s representing element attributes.
template <typename charT>
std::pair<std::string, std::basic_string<charT>>
P(const std::string& s1, const std::basic_string<charT>& s2) {
	return std::make_pair(s1, s2);
}
//! Attribute tagger. Use it to create std::pair<T1,T2>s representing element attributes.
template <typename charT>
std::pair<std::string, std::basic_string<charT>>
P(std::string&& s1, std::basic_string<charT>&& s2) {
	return std::make_pair(std::move(s1), std::move(s2));
}
//! @c char specialization of P
std::pair<std::string, std::string>
SP(const std::string& s1, const std::string& s2) {
	return P(s1, s2);
}
//! @c char specialization of P
std::pair<std::string, std::string>
SP(std::string&& s1, std::string&& s2) {
	return P(std::move(s1), std::move(s2));
}
//! @c wchar_t specialization of P
std::pair<std::string, std::wstring>
WSP(const std::string& s1, const std::wstring& s2) {
	return P(s1, s2);
}
//! @c wchar_t specialization of P
std::pair<std::string, std::wstring>
WSP(std::string&& s1, std::wstring&& s2) {
	return P(std::move(s1), std::move(s2));
}
//@}

}

}

MOSH_CGI_END


#endif

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

#include <string>
#include <sstream>
#include <map>
#include <set>
#include <utility>
#include <stdexcept>
#include <ostream>
#include <type_traits>
#include <mosh/cgi/html/html_doctype.hpp>
#include <mosh/cgi/bits/t_string.hpp>
#include <mosh/cgi/bits/namespace.hpp>

MOSH_CGI_BEGIN

//! HTML classes
namespace html {

//! HTML element classes
namespace element {

//! Enumeration for encoding type
namespace Type {
	
	/* Guide to constants:
	 * Each value must be a power of 2
	 */

	//! Encodes an unary <foo /> element; resulting tag will have attributes but no data
	const unsigned unary = 1 << 0;
	//! Encodes a binary <foo></foo> element; resulting tag will have both attributes and data
	const unsigned binary = 1 << 1;
	//! Encodes a DOCTYPE directive <!FOO >
	const unsigned dtd = 1 << 2;
	//! Encodes a comment <!-- foo -->
	const unsigned comment = 1 << 3;
	
	inline void _validate(unsigned t) {
		if (t < (1 << 0) || t > (1 << 3))
			goto _throw_;
		if (__builtin_popcount(t) != 1)
			goto _throw_;
		return;
	_throw_:
		throw std::invalid_argument("MOSH_CGI::html::element::Type");
	}		
}

//! An HTML element
template <typename charT>
class Element  {
public:
	//! Typedef for strings
	typedef typename std::basic_string<charT> string;
	//! Typedef for attributes
	typedef typename std::pair<std::string, string> attribute;
	//! Typedef for attribute lists
	typedef typename std::map<std::string, string> attr_list;
private:
	typedef Element<charT> this_type;
public:
	/*! @brief Create a new element with a given name and type
	 *  @param[in] name_ Element name
	 *  @param[in] type_ Element type
	 *  @sa Type
	 */
	Element(unsigned type_, const std::string& name_)
	: type(type_), name(name_), attributes(), data()
	{
		Type::_validate(type_);
	}

	//! Copy constructor
	Element(const this_type& e)
	: type(e.type), name(e.name), attributes(e.attributes), data(e.data)
	{ }

	//! Move constructor
	Element(this_type&& e)
	: type(e.type), name(std::move(e.name)), attributes(std::move(e.attributes)),
	  data(std::move(e.data))
	{ }

	//! Destructor
	virtual ~Element() { }

	//! Assignment operator
	this_type& operator = (const this_type& e) {
		if (this != &e) {
			type = e.type;
			attributes = e.attributes;
			data = e.data;
			name = e.name;
		}
		return *this;
	}
	//! Assignment operator
	this_type operator = (this_type&& e) {
		if (this != &e) {
			type = e.type;
			attributes = std::move(e.attributes);
			data = std::move(e.data);
			name = std::move(e.name);
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
		if (this->attribute_addition_hook(_a))
			attributes.insert(_a);
		return *this;
	}

	/*! @brief Add attribute(s).
	 * @param[in] _a {}-list of attributes
	 */
	this_type& operator += (std::initializer_list<attribute> _a) {
		for (const auto& at : _a) {
			if (this->attribute_addition_hook(at))
				attributes.insert(at);
		}
		return *this;
	}

	/*! @brief Add a value.
	 * @param[in] _v value
	 */
	this_type& operator += (const string& _v) {
		if (this->data_addition_hook(_v))
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
		s << wide_string<charT>(this->name);
		for (const auto& a : this->attributes) {
			s << wide_char<charT>(' ');
			s << wide_string<charT>(a.first);
			s << wide_char<charT>('=');
			s << wide_char<charT>('"');
			s << a.second;
			s << wide_char<charT>('"');
		}
		if (this->type == Type::unary) {
			s << wide_char<charT>(' ');
			s << wide_char<charT>('/');
		} else {
			if (this->type == Type::binary) {
				s << wide_char<charT>('>');
			}
			s << this->data;		
			if (this->type == Type::binary) {
				s << wide_char<charT>('<');
				s << wide_char<charT>('/');
				s << wide_string<charT>(this->name);
			} else if (this->type == Type::comment) {
				s << wide_string<charT>("--");
			}
		}
		s << wide_char<charT>('>');
		return s.str();
	}

protected:
	//! Default constructor for derived classes
	Element()
	: type(0), name(), attributes(), data()
	{ }
	
	/*! @brief Type-exposing constructor for derived classes
	 *  @param[in] type_ Type
	 *  @sa Type
	 */
	Element(unsigned type_)
	: type(type_), name(), attributes(), data()
	{ }


	/*! @brief Hook for attribute addition 
	 *  To perform custon behavior (e.g. value checking) on attribute addition,
	 *  override this function in derived classes.
	 *  @retval @c true Add the attribute to the list
	 *  @retval @c false Don't add the attribute to the list
	 */
	virtual bool attribute_addition_hook(const attribute&) { return true; }
	/*! @brief Hook for data addition 
	 *  To perform custon behavior (e.g. value checking) on data addition,
	 *  override this function in derived classes.
	 *  @retval @c true Add the string to data
	 *  @retval @c false Don't add the string to data
	 */
	virtual bool data_addition_hook(const string&) { return true; }

	//! Element type
	unsigned type;
private:
	//! Element name
	std::string name;
protected:
	/*! @brief List of attributes.
	 *  Unused for comment elements.
	 */
	attr_list attributes;
	/*! @brief Pre-rendered embedded data
	 *  Unused for unary elements.
	 */
	string data;
	
};

/*! @name Concatenators
 * These overloads create new elements which behave as if 
 * attributes and values were appendedto existing elements.
 */
//@{
template <typename charT, typename T>
typename std::enable_if<std::is_base_of<Element<charT>, T>::value, T>::type
operator + (T&& e1, T&& e2) {
	T e(std::forward<T>(e1));
	e += static_cast<std::basic_string<charT>>(e2);
	return e;
}

template <typename charT, typename T>
typename std::enable_if<std::is_base_of<Element<charT>, T>::value, T>::type
operator + (T const& e1, T&& e2) {
	T e(e1);
	e += static_cast<std::basic_string<charT>>(e2);
	return ;
}

/*! @brief Concatenate an attribute.
 *  @param[in] _e element
 *  @param[in] _a attribute
 */
template <typename charT, typename T>
typename std::enable_if<std::is_base_of<Element<charT>, T>::value, T>::type
operator + (T const& _e, const typename Element<charT>::attribute& _a) {
	T e(_e);
	e += _a;
	return e;
}

/*! @brief Concatenate an attribute.
 *  @param[in] _e element
 *  @param[in] _a attribute
 */
template <typename charT, typename T>
typename std::enable_if<std::is_base_of<Element<charT>, T>::value, T>::type
operator + (T&& _e, const typename Element<charT>::attribute& _a) {
	T e(std::move(_e));
	e += _a;
	return std::move(e);
}

/*! @brief Concatenate attribute(s).
 *  @param[in] _e element
 *  @param[in] _a {}-list of attributes
 */
template <typename charT, typename T>
typename std::enable_if<std::is_base_of<Element<charT>, T>::value, T>::type
operator + (T const& _e, std::initializer_list<typename Element<charT>::attribute> _a) {
	T e(_e);
	e += _a;
	return e;
}

/*! @brief Concatenate attribute(s).
 *  @param[in] _e element
 *  @param[in] _a {}-list of attributes
 */
template <typename charT, typename T>
typename std::enable_if<std::is_base_of<Element<charT>, T>::value, T>::type
operator + (T&& _e, std::initializer_list<typename Element<charT>::attribute> _a) {
	T e(std::move(_e));
	e += _a;
	return std::move(e);
}

/*! @brief Concatenate a value.
 *  @param[in] _e element 
 *  @param[in] _v value
 */
template <typename charT, typename T>
typename std::enable_if<std::is_base_of<Element<charT>, T>::value, T>::type
operator + (T const& _e, const std::basic_string<charT>& _v) {
	T e(_e);
	e += _v;
	return e;
}
	
/*! @brief Concatenate a value.
 *  @param[in] _e element 
 *  @param[in] _v value
 */
template <typename charT, typename T>
typename std::enable_if<std::is_base_of<Element<charT>, T>::value, T>::type
operator + (T&& _e, const std::basic_string<charT>& _v) {
	T e(std::move(_e));
	e += _v;
	return std::move(e);
}

template <typename charT, typename T>
typename std::enable_if<std::is_base_of<Element<charT>, T>::value, std::basic_string<charT>>::type
operator + (std::basic_string<charT> const& _v, T&& _e) {
	return _v + static_cast<std::basic_string<charT>>(_e);
}

/*! @brief Concatenate value(s).
 *  @param[in] _e element
 *  @param[in] _v {}-list of values
 */
template <typename charT, typename T>
typename std::enable_if<std::is_base_of<Element<charT>, T>::value, T>::type
operator + (T const& _e, std::initializer_list<std::basic_string<charT>> _v) {
	T e(_e);
	e += _v;
	return e;
}

/*! @brief Concatenate value(s).
 *  @param[in] _e element
 *  @param[in] _v {}-list of values
 */
template <typename charT, typename T>
typename std::enable_if<std::is_base_of<Element<charT>, T>::value, T>::type
operator + (T&& _e, std::initializer_list<std::basic_string<charT>> _v) {
	T e(std::move(_e));
	e += _v;
	return std::move(e);
}

//@}

template <typename charT>
std::basic_ostream<charT>& operator << (std::basic_ostream<charT>& os, const Element<charT>& e) {
	os << static_cast<std::basic_string<charT>>(e);
	return os;
}

/*! @brief HTML begin class
 * This class outputs <!DOCTYPE ...><html ...> when cast to string.
 * @note For XHTML, it adds xmlns and <?xml ...?>
 * @note To add attributes to the <?xml ?>, add attributes in the form of { "xml=foo", "bar" }.
 * @note Data added is assumed to be valid SGML DTD
 *
 */
template <typename charT>
class HTML_begin : public virtual Element<charT> {
	typedef HTML_begin<charT> this_type;
	typedef Element<charT> base_type;
public:
	//! Typedef for strings
	typedef typename base_type::string string;
	//! Typedef for attributes
	typedef typename base_type::attribute attribute;
	//! Typedef for attribute lists
	typedef typename base_type::attr_list attr_list;
 
	/*! @brief Create a new HTML start with a given type
	 *  @param[in] type_ HTML type
	 *  @sa doctype::HTML_revision
	 */
	HTML_begin(unsigned type_ = html_doctype::html_revision::xhtml_10_strict)
	: Element<charT>(type_), doctype(html_doctype::html_doctype<charT>(type_)), xml_attributes()
	{
		if (is_xhtml()) {
			this->attributes.insert(std::make_pair("xmlns", wide_string<charT>("http://www.w3.org/1999/xhtml")));
		}
	}

	//! Copy constructor
	HTML_begin(const HTML_begin<charT>& b)
	: Element<charT>(b), doctype(b.doctype)
	{ }

	//! Move constructor
	HTML_begin(HTML_begin<charT>&& b)
	: Element<charT>(std::move(b)), doctype(std::move(b.doctype))
	{ }

	//! Destructor
	virtual ~HTML_begin() { }

	// Copy over all the overloads of () from Element because it's simply too much work to do in-class
	// type_traits magic.
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
	virtual operator string () const {
		std::basic_stringstream<charT> s;
		if (is_xhtml()) {
			s << wide_string<charT>("<?xml version=\"1\" ");
			for (const auto& a : this->xml_attributes) {
				s << wide_string<charT>(a.first + "=\"");
				s << a.second + wide_string<charT>("\" ");
			}
			s << wide_string<charT>("?>\r\n");
		}	
		s << this->doctype << wide_string<charT>("\r\n");
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
	virtual bool attribute_addition_hook(const attribute& _a) {
		if (is_xhtml()) {
			if (_a.first == "lang") { // make use of lang attribute XHTML-conforming
				this->attributes.insert(std::make_pair("xml:lang", _a.second));
			}
			if (!_a.first.compare(0, 4, "xml=")) {
				this->xml_attributes.insert(std::make_pair(_a.first.substr(4), _a.second));
				return false;
			}
		}
		return true;
	}
	virtual bool data_addition_hook(const string& _s) {
		doctype += _s;
		return false;
	}

	//! Doctype
	sgml_doctype::Doctype_declaration<charT> doctype;
	
	//! List of <?xml attributes.
	attr_list xml_attributes;

private:
	bool is_xhtml() const {
		using namespace html_doctype::html_revision;
		return (get_family(this->type) == static_cast<uint8_t>(Family::xhtml));
	}

};

//! This class prints </html>
template <typename charT>
struct HTML_end {

	HTML_end() { }
	virtual ~HTML_end() { }

	virtual operator std::basic_string<charT> () const {
		return wide_string<charT>("</html>");
	}
};

template <typename charT>
std::basic_ostream<charT>& operator << (std::basic_ostream<charT>& os, const HTML_end<charT>& e) {
	os << static_cast<std::basic_string<charT>>(e);
	return os;
}

/*! @brief body begin class
 * This class outputs <body ...> when cast to string.
 * @note Data added is discarded
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
	Body_begin(const this_type& b)
	: Element<charT>(b)
	{ }

	//! Move constructor
	Body_begin(this_type&& b)
	: Element<charT>(std::move(b))
	{ }

	//! Destructor
	virtual ~Body_begin() { }

	// Copy over all the overloads of () from Element because it's simply too much work to do in-class
	// type_traits magic.
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
	//@}
	/* @name Unavailable operators
	 * These operator overloads are disabled for semantic enforcement reasons.
	 */
	//@{
	this_type operator () (const string&) const = delete;
	this_type operator () (std::initializer_list<string>) const  = delete;
	this_type operator () (const attribute&, const string&) const = delete;
	this_type operator () (const attribute&, std::initializer_list<string>) const = delete;
	this_type operator () (std::initializer_list<attribute>, const string&) const = delete;
	this_type operator () (std::initializer_list<attribute>, std::initializer_list<string>) const = delete;
	//@}
	virtual operator string () const {
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
	// Don't append data
	virtual bool data_addition_hook(const string&) { return false; }
};

//! This class prints </body>
template <typename charT>
struct Body_end {

	Body_end() { }
	virtual ~Body_end() { }

	virtual operator std::basic_string<charT> () const {
		return wide_string<charT>("</body>");
	}
};

template <typename charT>
std::basic_ostream<charT>& operator << (std::basic_ostream<charT>& os, const Body_end<charT>& e) {
	os << static_cast<std::basic_string<charT>>(e);
	return os;
}

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
//@}
template <typename T, size_t N>
std::basic_string<T> S(const T (&s)[N]) {
	return std::basic_string<T>(s, N);
}

}

}

MOSH_CGI_END


#endif

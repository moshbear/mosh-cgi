/* cgi/common/Kv.h - templated container for name=value pairs
 *
 * Copyright (C) 2011 Andrey Vul
 *
 * This is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 * 
 * This is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 */

#ifndef _CGI_COMMON_KV_H_
#define _CGI_COMMON_KV_H_

#include <string>
#include <ostream>
#include <cgi/common/Streamable.h>
#include <cgi/utils/String.h>

namespace cgi { namespace common {
/*! \class KV Kv.h cgi/common/Kv.h
 * \brief Extensible name-value pair via K-V idiom
 *
 * A generic encapsulation for name-value pairs.
 * \tparam charT char type
 * \tparam quote_K escape the key portion with quotes
 * \tparam quote_V escape the value portion with quotes
 * \tparam nocase_K case-insensitive comparison of keys
 * \tparam nocase_V case-insensitive comparison of values
 */
template <class charT,
	  bool quote_K = false, bool quote_V = quote_K, // controls behaviour of ::render
	  bool nocase_K = true, bool nocase_V = nocase_K> // controls behaviour of ::operator==
struct KV : public virtual cgi::common::Streamable<charT> {
private:
	typedef KV<charT, quote_K, quote_V, nocase_K, nocase_V> this_type;
public:
	/*! The string type of the elements.
	 */
	typedef typename std::basic_string<charT> string_type;
	/*! The key part of the pair, i.e. the name.
	 */
	string_type name;
	/*! The value part of the pair.
	 */
	string_type value;
	/*! \brief Default constructor
	 */
	KV() : name(), value() { }
	/*! \brief Constructor for default pair.
	 *
	 * The default pair, where .name() == .value()
	 */
	KV(const string_type& name_) : KV(name_, name_) { }
	/*! \brief Constructor.
	 *
	 * \param name_ name to use
	 * \param value_ value to use
	 */
	KV(const string_type& name_, const string_type& value_) : name(name_), value(value_) { }
	/*! \brief Copy constructor.
	 */
	KV(const this_type& kv) : KV(kv.name, kv.value) { }
	/*! \brief Move constructor.
	 */
	KV(this_type&& kv) : KV("") {
		name = std::move(kv.name);
		value = std::move(kv.value);
	}
	virtual ~KV() { }
	/*! \brief Assignment
	 *
	 * \param kv KV to assign from
	 */
	this_type& operator=(const this_type& kv) {
		if (this == &kv)
			return *this;
		name = kv.name;
		value = kv.value;
		return *this;
	}
	/*! \brief Assignment with move semantics
	 *
	 * \param kv KV to assign from. Note: kv becomes useless after.
	 */
	this_type& operator=(this_type&& kv) {
		if (this == &kv)
			return *this;
		name  = std::move(kv.name);
		value = std::move(kv.value);
	}
	/*! \brief Check for equality
	 * Behaviour is governed by template parameters nocase_K and nocase_V.
	 * \return \c true if the KVs are equal
	 */
	bool operator==(const this_type& kv) const {
		if (this == &kv)
			return true;
		bool eq = true;
		if (nocase_K)
			eq &= string::equality(name, kv.name);
		else
			eq &= (name == kv.name);
		if (!eq)
			return false;
		if (nocase_V)
			eq &= string::equality(value, kv.value);
		else
			eq &= (value == kv.value);
		return eq;
	}
	/*! \brief Check for inequality
	 * \return \c true if the KVs are not equal
	 */
	bool operator!=(const this_type& kv) const { return !operator==(kv); }
	/*! \brief Render to a basic_ostream.
	 *
	 * Behaviour is governed by template parameters quote_K and quote_V.
	 */
	virtual void render(std::basic_ostream<charT>& out) const {
		const charT quoteT = '"';
		if (quote_K)
			out << quoteT;
		out << name;
		if (quote_K)
			out << quoteT;
		out << '=';
		if (quote_V)
			out << quoteT;
		out << value;
		if (quote_V)
			out << quoteT;
	}
};

} } // namespace common // namespace cgi

#endif


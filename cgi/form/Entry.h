/* -*-mode:c++; c-file-style: "gnu";-*- */
/*
 *  $Id: FormEntry.h,v 1.14 2007/07/02 18:48:18 sebdiaz Exp $
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

#ifndef _CGI_FORM_ENTRY_H_
#define _CGI_FORM_ENTRY_H_ 1

/*! \file FormEntry.h
 * \brief Class representing a single HTML form entry.
 *
 * FormEntry is an immutable class representing a single user entry
 * in an HTML form element such as a text field, radio button, or a
 * checkbox.  A FormEntry is essentially a name/value pair, where the
 * name is the name of the form element as specified in the HTML form
 * itself, and the value is the user-entered or user-selected value.
 */

#include <string>
#include <algorithm>
#include <climits>
#include <cfloat>

#include <cgi/common/Kv.h>

namespace cgi { namespace form {

// ============================================================
// Class form::Entry
// ============================================================

/*! \class form::Entry Entry.h cgi/form/Entry.h
 * \brief Class representing a single HTML form entry.
 *
 * Entry is an immutable class representing a single user entry
 * in an HTML form element such as a text field, radio button, or a
 * checkbox.  A FormEntry is essentially a name/value pair, where the
 * name is the name of the form element as specified in the HTML form
 * itself, and the value is the user-entered or user-selected value.
 *
 * If a \c QUERY_STRING contained the fragment \c cgi=yes the
 * corresponding FormEntry would have a name of \c cgi and a value
 * of \c yes
 *
 * \sa FormFile
 */
template <class charT>
struct Entry : public cgi::common::KV<charT> {
private:
	typedef cgi::common::KV<charT> KV;
public:
	// ============================================================

	/*! \name Constructors and Destructor */
	//@{
	/*! \brief Default constructor
	 */
	Entry() : cgi::common::KV<charT>() { }
	/*!
	 * \brief Create a new FormEntry
	 *
	 * This is usually not called directly, but by Cgicc.
	 * \param name The name of the form element
	 * \param value The value of the form element
	 */
	Entry(const std::basic_string<charT>& name, const std::basic_string<charT>& value)
	: KV(name, value),
	  valueN(makeString(value, -1, true)),
	  valueS(makeString(valueN, -1, false)) {
	}

	/*!
	 * \brief Copy constructor.
	 *
	 * Sets the name and value of this FormEntry to those of \c entry.
	 * \param entry The FormEntry to copy.
	 */
	inline Entry(const Entry<charT>& entry)
	: KV(entry.name, entry.value) {
		operator=(entry);
	}

	/*!
	 * \brief Destructor.
	 *
	 * Delete this FormEntry object
	 */
	inline ~Entry() { }
	//@}

	// ============================================================

	/*! \name Overloaded Operators */
	//@{

	/*!
	 * \brief Compare two FormEntrys for equality.
	 *
	 * FormEntrys are equal if they have the same name and value.
	 * \param entry The FormEntry to compare to this one.
	 * \return \c true if the two FormEntrys are equal, \c false otherwise.
	 */
	inline bool operator==(const Entry<charT>& entry) const {
		return (this == &entry) ||
			this->KV::operator==(entry);
	}
	/*!
	 * \brief Compare two FormEntrys for inequality.
	 *
	 * FormEntrys are equal if they have the same name and value.
	 * \param entry The FormEntry to compare to this one.
	 * \return \c false if the two FormEntrys are equal, \c true otherwise.
	 */
	inline bool operator!=(const Entry<charT>& entry) const {
		return !operator==(entry);
	}
	/*!
	 * \brief Assign one FormEntry to another.
	 *
	 * Sets the name and value of this FormEntry to those of \c entry.
	 * \param entry The FormEntry to copy.
	 * \return A reference to this.
	 */
	Entry<charT>& operator=(const Entry<charT>& entry) {
		if (this == &entry)
			return *this;
		this->KV::operator=(entry);
		valueN = makeString(this->value, -1, true);
		valueS = makeString(valueN, -1, false); 
	}
	//@}

	// ============================================================

	/*!
	 * \brief Get the value of the form element as a string
	 *
	 * The value returned may contain line breaks.
	 * \return The value of the form element.
	 */
	inline const std::basic_string<charT>& valueRaw() const {
		return this->value;
	}
	inline const std::basic_string<charT>& valueNormalized() const {
		return valueN;
	}
	inline const std::basic_string<charT>& valueStripped() const {
		return valueS;
	}
	/*!
	 * \brief Get the value of the form element as a string
	 *
	 * The value returned may contain line breaks.
	 * \return The value of the form element.
	 */
	inline const std::basic_string<charT>& operator*() const {
		return this->value;
	}
	
	inline typename std::basic_string<charT>::size_type length() const {
		return this->value.size();
	}
	/*!
	 * \brief Determine if this form element is empty
	 *
	 * In an empty form element, length() == 0.
	 * \return \c true if this form element is empty, \c false otherwise.
	 */
	inline bool isEmpty() const {
		return this->value.empty();
	}
	//@}
private:
	std::basic_string<charT> valueN;
	std::basic_string<charT> valueS;
private:
	static std::basic_string<charT> makeString(const std::basic_string<charT>& str,
							typename std::basic_string<charT>::size_type maxLen,
							bool allowNewlines)
	{
		typename std::basic_string<charT>::size_type len = 0;
		typename std::basic_string<charT>::size_type avail = maxLen;
		typename std::basic_string<charT>::size_type crCount = 0;
		typename std::basic_string<charT>::size_type lfCount = 0;
		typename std::basic_string<charT>::const_iterator src = str.begin();
		typename std::basic_string<charT>::const_iterator lim = str.end();
		std::basic_string<charT> dst;

		while (src != lim && len < avail) {
			// handle newlines
			if ('\r' == *src || '\n' == *src) {
				crCount = 0;
				lfCount = 0;

				// Count the number of each kind of line break ('\r' and '\n')
				while (('\r' == *src || '\n' == *src) && (src != lim)) {
					if ('\r' == *src)
						crCount++;
					else
						lfCount++;
					++src;
				}
				// if newlines are allowed, add them
				if (allowNewlines) {
					// output the larger value
					size_t lfsAdd = std::max(crCount, lfCount);
					dst.append(lfsAdd, '\n');
					len += lfsAdd;
				}
			}
			// just append all other characters
			else {
				dst.append(1, *src);
				++len;
				++src;
			}
		}
		return dst;
	}
};
} } // namespace form // namespace cgi

#endif /* ! _CGI_FORM_ENTRY_H_ */

/* -*-mode:c++; c-file-style: "gnu";-*- */
/*
 *  $Id: File.h,v 1.11 2007/07/02 18:48:18 sebdiaz Exp $
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

#ifndef _CGI_FORM_FILE_H_
#define _CGI_FORM_FILE_H_ 1

/*! \file File.h
 * \brief Class representing a file submitted via an HTML form.
 *
 * File is an immutable class reprenting a file uploaded via
 * the HTTP file upload mechanism.  If you are going to use file upload
 * in your CGI application, remember to set the ENCTYPE of the form to
 * \c multipart/form-data.
 */

#include <iostream>
#include <string>
#include <vector>
#include <boost/shared_ptr.hpp>
#include <cgi/common/Kv.h>

namespace cgi { namespace form {

// ============================================================
// Class form::File
// ============================================================

/*! \class form::File File.h cgi/form/File.h
 * \brief Class representing a file submitted via an HTML form.
 *
 * File is an immutable class reprenting a file uploaded via
 * the HTTP file upload mechanism.  If you are going to use file upload
 * in your CGI application, remember to set the ENCTYPE of the form to
 * \c multipart/form-data.
 * \verbatim
 <form method="post" action="http://change_this_path/cgi-bin/upload.cgi"
 enctype="multipart/form-data">
 \endverbatim
 * \sa FormEntry
 */
template <class charT>
struct File : public cgi::common::KV<charT> {
private:
	typedef cgi::common::KV<charT> KV;
public:
	/*! \name Constructors and Destructor */
	//@{

	/*!
	 * \brief Default constructor
	 *
	 * Included so that std::map<std::basic_string<charT>, File<charT> > []-lookup
	 * can be used.
	 * This shouldn't be explicitly called.
	 *
	 */
	inline File() { }
	/*!
	 * \brief Create a new File.
	 *
	 * This is usually not called directly, but by Cgicc.
	 * \param filename The \e filename of the file on the remote machine.
	 * \param dataType The MIME content type of the data, if specified, or 0.
	 * \param data The file data.
	 */
	File(const std::basic_string<charT>& name,
	             const std::basic_string<charT>& filename,
	             const std::basic_string<charT>& dataType_,
		     boost::shared_ptr<std::vector<char> >& data_)
	: KV(name, filename), dataType(dataType_), data(data_) {
		if (dataType.empty())
			dataType = "text/plain";
	}

	/*!
	 * \brief Copy constructor.
	 *
	 * Sets the filename, datatype, and data to those of \c file
	 * @param file The File to copy.
	 */
	inline File(const File<charT>& file)
	: KV(file.name, file.value) {
		operator=(file);
	}

	/*!
	 * \brief Destructor
	 *
	 * Delete this File object
	 */
	inline ~File() {
	}
	//@}

	// ============================================================

	/*! \name Overloaded Operators */
	//@{

	/*!
	 * \brief Compare two Files for equality.
	 *
	 * Files are equal if they have the same filename.
	 * @param file The File to compare to this one.
	 * @return \c true if the two Files are equal, \c false otherwise.
	 */
	bool operator==(const File<charT>& file) const {
		return (this==&file)||
			(this->KV::operator==(file)
			&& cgi::string::equality(dataType, file.datatype));
	}
	/*!
	 * \brief Compare two Files for inequality.
	 *
	 * Files are equal if they have the same filename.
	 * \param file The File to compare to this one.
	 * \return \c false if the two Files are equal, \c true otherwise.
	 */
	inline bool operator!=(const File<charT>& file) const {
		return !operator==(file);
	}
	/*!
	 * \brief Assign one File to another.
	 *
	 * Sets the filename, datatype, and data to those of \c file
	 * \param file The File to copy.
	 * \return A reference to this.
	 */
	File<charT>&
	operator=(const File<charT>& file) {
		if (&file == this)
			return *this;
		this->KV::operator=(file);
		dataType = file.dataType;
		data = file.data;
		return *this;
	}
	//@}

	// ============================================================

	/*! \name Accessor Methods
	 * Information on the uploaded file
	 */
	//@{

	/*!
	 * \brief Write this file data to the specified stream.
	 *
	 * This is useful for saving uploaded data to disk
	 * \param out The ostream to which to write.
	 */
	void
	writeToStream(std::basic_ostream<charT>& out) const {
		if (data.get())
			out.write(&data.get()->at(0), data->size());
	}
	
	/*!
	 * \brief Get the length of the file data
	 *
	 * The length of the file data is usually measured in bytes.
	 * \return The length of the file data, in bytes.
	 */
	inline size_t dataLength() const {
		if (data.get())
			return data->size();
		return 0;
	}
	//@}
	std::basic_string<charT> dataType;
	boost::shared_ptr<std::vector<char> > data;

};

} } // namespace form // namespace cgi

#endif				/* ! _CGI_FORM_FILE_H_ */

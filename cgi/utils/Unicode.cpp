/* cgi/common/Utils-utf.cpp - UTF-8 encoding/decoding
 * 
 * Copyright (C) 2011 Andrey Vul
 *
 * (adapted from libfcgi++)
 *
 * This is free software: you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as  published 
 * by the Free Software Foundation, either version 3 of the License, or (at 
 * your option) any later version.                                          
 * This is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or 
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
 * License for more details.                                             
 * 
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library. If not, see <http://www.gnu.org/licenses/>.
 */

#include <string>
#include <sstream>
#include <stdexcept>
#include <boost/scoped_ptr.hpp>

#include <cgi/utils/Unicode.h>

#include "utf8_codecvt.hpp"
#include "utf8_codecvt_facet.cpp"

template<> std::wstring cgi::unicode::utfIn<wchar_t>(const std::string&  s) {
	const size_t bufferSize = 512;
	wchar_t buffer[bufferSize];
	size_t s_size = s.size();
	const char* s_ptr = &(*s.begin());
	using namespace std;
	// temporary locale => temporary pointer should be used
	boost::scoped_ptr<utf8CodeCvt::utf8_codecvt_facet> facet(new utf8CodeCvt::utf8_codecvt_facet);
	wstring ret;
	if (s_size) {
		codecvt_base::result cr = codecvt_base::partial;
		while (cr == codecvt_base::partial) {
			wchar_t* it;
			const char* tmpData;
			mbstate_t conversionState = mbstate_t();
			cr = use_facet<codecvt<wchar_t, char, mbstate_t> >(locale(locale::classic(), facet.get()))
				.in(conversionState, s_ptr, s_ptr + s_size, tmpData, buffer, buffer + bufferSize, it);
			ret.append(buffer, it);
			s_size -= tmpData - s_ptr;
			s_ptr = tmpData;
		}
		if (cr == codecvt_base::error)
			throw runtime_error("codecvt_base::error");
		return ret;
	}
	return wstring();
}

template<> std::string cgi::unicode::utfOut<wchar_t>(const std::wstring& ws) {
	const size_t bufferSize = 512;
	char buffer[bufferSize];
	size_t ws_size = ws.size();
	const wchar_t* ws_ptr = &(*ws.begin());
	using namespace std;
	boost::scoped_ptr<utf8CodeCvt::utf8_codecvt_facet> facet(new utf8CodeCvt::utf8_codecvt_facet);
	string ret;
	if (ws_size) {
		codecvt_base::result cr = codecvt_base::partial;
		while (cr == codecvt_base::partial) {
			char* it;
			const wchar_t* tmpData;
			mbstate_t conversionState = mbstate_t();
			cr = use_facet<codecvt<wchar_t, char, mbstate_t> >(locale(locale::classic(), facet.get()))
				.out(conversionState, ws_ptr, ws_ptr + ws_size, tmpData, buffer, buffer + bufferSize, it);
			ret.append(buffer, it);
			ws_size -= tmpData - ws_ptr;
			ws_ptr = tmpData;
		}
		if (cr == codecvt_base::error)
			throw runtime_error("codecvt_base::error");
		return ret;
	}
	return string();
}

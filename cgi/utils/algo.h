/* cgi/utils/algo.h: template selector for find, search
 * Copyright (C) 2011 Andrey m0shbear
 * 
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110, USA
 */

#ifndef _CGI_UTILS_ALGO_H_
#define _CGI_UTILS_ALGO_H_ 1

#include <vector>
#include <string>
#include <algorithm>
#include <stxxl/vector>
#include <stxxl/algorithm>
#include <stxxl-mosh/algo/search.h>

namespace cgi { namespace utils {
template <typename It>
struct finder {
	template <typename Eq>
	static It find(It, It, const Eq&);
};
template <typename It>
struct searcher {
	template <typename It2>
	static It search(It, It, It2, It2);
};

#define CGIU_ALGO_FIND_STD(T) \
	template<> struct finder<T> { \
		template <typename Eq> \
		static inline T find(T _Begin, T _End, const Eq& _V) { \
			return std::find(_Begin, _End, _V); \
		} \
	}

#define CGIU_ALGO_FIND_STXXL(T) \
	template<> struct finder<T> {\
		template <typename Eq> \
		static inline T find(T _Begin, T _End, const Eq& _V) { \
			return stxxl::find(_Begin, _End, _V, 0); \
		} \
	}

#define CGIU_ALGO_SEARCH_STD(T) \
	template<> struct searcher<T> { \
		template <typename It2> \
		static inline T search(T _1_Begin, T _1_End, It2 _2_Begin, It2 _2_End) { \
			return std::search(_1_Begin, _1_End, _2_Begin, _2_End); \
		} \
	}

#define CGIU_ALGO_SEARCH_STXXL2(T) \
	template<> struct searcher<T> { \
		template <typename It2> \
		static inline T search(T _1_Begin, T _1_End, It2 _2_Begin, It2 _2_End) { \
			return stxxl_mosh::search2(_1_Begin, _1_End, _2_Begin, _2_End, 0); \
		} \
	}

CGIU_ALGO_FIND_STD(std::vector<char>::iterator);
CGIU_ALGO_FIND_STD(std::vector<char>::const_iterator);
CGIU_ALGO_FIND_STD(std::string::iterator);
CGIU_ALGO_FIND_STD(std::string::const_iterator);
CGIU_ALGO_FIND_STXXL(stxxl::vector<char>::iterator);
CGIU_ALGO_FIND_STXXL(stxxl::vector<char>::const_iterator);
CGIU_ALGO_SEARCH_STD(std::vector<char>::iterator);
CGIU_ALGO_SEARCH_STD(std::vector<char>::const_iterator);
CGIU_ALGO_SEARCH_STD(std::string::iterator);
CGIU_ALGO_SEARCH_STD(std::string::const_iterator);
CGIU_ALGO_SEARCH_STXXL2(stxxl::vector<char>::iterator);
CGIU_ALGO_SEARCH_STXXL2(stxxl::vector<char>::const_iterator);

#undef CGIU_ALGO_FIND_STD
#undef CGIU_ALGO_FIND_STXXL
#undef CGIU_ALGO_SEARCH_STD
#undef CGIU_ALGO_SEARCH_STXXL

} } // namespace utils // namespace cgi

#endif

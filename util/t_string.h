#ifndef _UTIL_T_STRING_H_
#define _UTIL_T_STRING_H_ 1

#include <string>
#include <boost/lexical_cast.hpp>

#define T_STRING(t, s) boost::lexical_cast<std::basic_string<t> >(s)

#endif

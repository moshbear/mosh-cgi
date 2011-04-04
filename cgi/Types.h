#ifndef _CGI_TYPES_H_
#define _CGI_TYPES_H_ 1
#include <cgi/config.h>

namespace stxxl {
template <class POD_Type> class vector;
template <class POD_K, class POD_V> class map;
}

namespace cgi {
template <class POD_Type> class BigVector {
	typedef stxxl::vector 
	
CGI_VECTOR_NS_BEGIN
template <class _V_type> CGI_VECTOR_BASE;
CGI_VECTOR_NS_END

CGI_RESTRICTED_VECTOR_NS_BEGIN
template <class _POD_V_type> CGI_RESTRICTED_VECTOR_BASE;
CGI_RESTRICTED_VECTOR_NS_END

template <class _POD_type> CGI_RESTRICTED_VECTOR;
namespace cgi {
template <class _V_type>
struct vector {
	typedef CGI_VECTOR<_V_type> type;
};
template <class _L_type>
struct list {
	typedef CGI_LIST<_L_type> type;
};
template <class _K_type, class _V_type> 
struct map {
	typedef CGI_MAP<_K_type, _V_type> type;
	typedef CGI_MMAP<_K_type, _V_type> mm_type;
};
typedef SIZE_TYPE size_t;

}

#endif

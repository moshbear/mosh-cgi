#ifndef _CGI_UTILS_SEARCH_AUTORANGE_H_
#define _CGI_UTILS_SEARCH_AUTORANGE_H_ 1
#include <util/N2930Iterator.h>
#include <algorithm>

namespace cgi { namespace utils {

/*!
 * \brief std::search, automatic range for both variables
 */
template <class ForwardIterable1, class ForwardIterable2>
auto search(ForwardIterable1&& _1, ForwardIterable2&& _2) -> decltype(std::begin(_1)) {
	return std::search(std::begin(_1), std::end(_1), std::begin(_2), std::end(_2));
}
/*!
 * \brief std::search, automatic range for both variables, custom predicate functor
 */
template <class ForwardIterable1, class ForwardIterable2, class BinaryPredicate>
auto search(ForwardIterable1&& _1, ForwardIterable2&& _2, BinaryPredicate _p) -> decltype(std::begin(_1)) {
	return std::search(std::begin(_1), std::end(_1), std::begin(_2), std::end(_2), _p);
}

template <class ForwardIterator1, class ForwardIterable2>
auto search(ForwardIterator1&& _1_begin, ForwardIterator1&& _1_end,
		ForwardIterable2 _2)
-> decltype(_1_begin) {
	return std::search(_1_begin, _1_end, std::begin(_2), std::end(_2));
}
template <class ForwardIterator1, class ForwardIterable2, class BinaryPredicate>
auto search(ForwardIterator1&& _1_begin, ForwardIterator1&& _1_end,
		ForwardIterable2&& _2, BinaryPredicate _p)
-> decltype(_1_begin) {
	return std::search(_1_begin, _1_end, std::begin(_2), std::end(_2), _p);
}

} } // namespace utils // namespace cgi
#endif /* !_CGI_UTILS_SEARCH_AUTORANGE_ */


// STXXL ALGO FIND WITH BINARY PREDICATE (C) 2011 M0SHBEAR, (C) [previous years] ORIGINAL CONTRIBUTORS
#ifndef MOSH__STXXL_ALGO_FIND_H
#define MOSH__STXXL_ALGO_FIND_H


#include <stxxl-mosh/namespace.h>
#include <stxxl/bits/mng/config.h>
#include <stxxl/bits/mng/buf_istream.h>

#include <stxxl/bits/algo/scan.h> // we want existing <T1, T2, T3> stxxl::find(T1, T1, T2, T3)  for <T1, T2> search(T1, T1, T2, T2)  

__MOSH_STXXL_BEGIN_NAMESPACE

// STXXL FIND WITH BINARY PREDICATE

//! \brief External equivalent of std::find with predicate functor
//! \remark The implementation exploits \c \<stxxl\> buffered streams (computation and I/O overlapped)
//! \param _begin object of model of \c ext_random_access_iterator concept
//! \param _end object of model of \c ext_random_access_iterator concept
//! \param _value value that is equality comparable to the _ExtIterator's value type
//! \param _pred binary predicate functor to use for comparison
//! \param nbuffers number of buffers (blocks) for internal use (should be at least 2*D )
//! \return first iterator \c i in the range [_begin,_end) such that *( \c i ) == \c _value, if no
//!         such exists then \c _end

template <typename _ExtIterator, typename _EqualityComparable, class _BinaryPredicate>
_ExtIterator find(_ExtIterator _begin, _ExtIterator _end, const _EqualityComparable& _value,
                  _BinaryPredicate _pred, stxxl::int_type nbuffers)
{
	using namespace stxxl;
	int_type nb = nbuffers ? nbuffers : (config::get_instance()->disks_number() * 2);
	if (_begin == _end)
		return _end;

	typedef buf_istream<typename _ExtIterator::block_type, typename _ExtIterator::bids_container_iterator> buf_istream_type;

	_begin.flush();     // flush container

	// create prefetching stream,
	buf_istream_type in(_begin.bid(), _end.bid() + ((_end.block_offset()) ? 1 : 0), nb);
-
	_ExtIterator _cur = _begin - _begin.block_offset();

	// skip part of the block before _begin untouched
	for ( ; _cur != _begin; ++_cur)
		++in;


	// search in the the range [_begin,_end)
	for ( ; _cur != _end; ++_cur) {
		typename _ExtIterator::value_type tmp;
		in >> tmp;
		if (_pred(tmp, _value))
			return _cur;
	}

	return _cur;
}

__MOSH_STXXL_END_NAMESPACE

#endif

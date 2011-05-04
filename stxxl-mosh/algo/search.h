// STXXL ALGO SEARCH and STXXL ALGO SEARCH WITH PREDICATE
// (C) 2011 Andrey m0shbear
// Portions (C) stxxl contributors
//
// TODO: more efficient buffering (project's responsibility)
/*
 * Copyright (c) 1994
 * Hewlett-Packard Company
 *
 * Permission to use, copy, modify, distribute and sell this software
 * and its documentation for any purpose is hereby granted without fee,
 * provided that the above copyright notice appear in all copies and
 * that both that copyright notice and this permission notice appear
 * in supporting documentation.  Hewlett-Packard Company makes no
 * representations about the suitability of this software for any
 * purpose.  It is provided "as is" without express or implied warranty.
 *
 *
 * Copyright (c) 1996
 * Silicon Graphics Computer Systems, Inc.
 *
 * Permission to use, copy, modify, distribute and sell this software
 * and its documentation for any purpose is hereby granted without fee,
 * provided that the above copyright notice appear in all copies and
 * that both that copyright notice and this permission notice appear
 * in supporting documentation.  Silicon Graphics makes no
 * representations about the suitability of this software for any
 * purpose.  It is provided "as is" without express or implied warranty.
 */
#ifndef MOSH__STXXL_ALGO_SEARCH_H
#define MOSH__STXXL_ALGO_SEARCH_H


#include <stxxl-mosh/namespace.h>

#include <stxxl/bits/mng/config.h>
#include <stxxl/bits/mng/buf_istream.h>

#include <stxxl/bits/algo/scan.h> // we want existing <T1, T2, T3> stxxl::find(T1, T1, T2, T3)  for <T1, T2> search(T1, T1, T2, T2)  

__MOSH_STXXL_BEGIN_NAMESPACE


// STXXL ALGO SEARCH

//! \brief External equivalent of std::search
//! \remark The implementation exploits \c \<stxxl\> buffered streans (computation and I/O overlapped)
//! \param _1_First object of model of \c ext_random_access_iterator concept
//! \param _1_Last object of model of \c ext_random_access_iterator concept
//! \param _2_First object of model of \c ext_random_access_iterator concept
//! \param _2_Last object of model of \c ext_random_access_iterator concept
//! \param nbuffers number of buffers (blocks) for internal use (should be at least 2*D )
//! \return first iterator \c i in the range [_1_First, _1_Last) such that [_2_First, _2_Last) is
//!		available at \c i; otherwise _1_Last

template <typename ForwardIterator1, typename ForwardIterator2>
ForwardIterator1 search(ForwardIterator1 _1_First, ForwardIterator1 _1_Last,
                        ForwardIterator2 _2_First, ForwardIterator2 _2_Last,
                        stxxl::int_type nbuffers)
{
	using namespace stxxl;
	// test for empty ranges
	if (_1_First == _1_Last || _2_First == _2_Last)
		return _1_First;
	typedef buf_istream<typename ForwardIterator1::block_type,
	        typename ForwardIterator1::bids_container_iterator>
	        buf_istream_1;
	typedef buf_istream<typename ForwardIterator2::block_type,
	        typename ForwardIterator2::bids_container_iterator>
	        buf_istream_2;
	typedef typename ForwardIterator1::value_type T1;
	typedef typename ForwardIterator2::value_type T2;

	_1_First.flush();
	_2_First.flush();
	int_type nb = nbuffers ? nbuffers : (config::get_instance()->disks_number() * 2);

	// Test for a pattern of length 1
	ForwardIterator2 _p1(_2_First);
	if (++_p1 == _2_Last) {
		// reset _1 buffer
		buf_istream_1 in1(_1_First.bid(), _1_Last.bid() + ((_1_Last.block_offset()) ? 1 : 0), nb);
		ForwardIterator1 _cur = _1_First - _1_First.block_offset();
		// skip part of the block before _begin untouched
		for ( ; _cur != _1_First; ++_cur)
			++in1;
		// search in the the range [_1_First, _1_Last)
		for ( ; _cur != _1_Last; ++_cur) {
			T1 tmp;
			in1 >> tmp;
			if (tmp == *_2_First)
				return _cur; // safe
		}
		return _cur;
	}

	// General case.
	ForwardIterator2 _p;
	ForwardIterator1 _Cur = _1_First;
	for (;;) {
		// reset _1's buffer
		// it SHOULD get smaller as _1_First approaches _1_Last
		buf_istream_1 in1(_1_First.bid(), _1_Last.bid() + ((_1_Last.block_offset()) ? 1 : 0), nb);
		ForwardIterator1 cur1 = _1_First - _1_First.block_offset();
		// skip part of the block before _begin untouched
		for ( ; cur1 != _1_First; ++cur1)
			++in1;
		// search in the range [_1_First, _1_Last)
		for ( ; cur1 != _1_Last; ++cur1) {
			typename ForwardIterator1::value_type tmp;
			in1 >> tmp;
			if (tmp == *_2_First)
				goto _found0; // 'return' unsafe; make do with 'goto'
		}
		if (cur1 == _1_Last)
			return _1_Last;
_found0:
		_1_First = cur1;

		// buffer _2
		buf_istream_2 in2(_2_First.bid(), _2_Last.bid() + ((_2_Last.block_offset()) ? 1 : 0), nb);
		// skip the part of the block before _2_First untouched
		ForwardIterator2 cur2 = _2_First - _2_First.block_offset();
		for ( ; cur2 != _2_First; ++cur2)
			++in2;
		_p = _p1;
		T2 tmp2;
		in2 >> tmp2;
		_Cur = _1_First;
		if (++_Cur == _1_Last)
			return _1_Last;
		T1 tmp1;
		in1 >> tmp1;
		for (; tmp1 == tmp2; in1 >> tmp1, in2 >> tmp2) {
			if (++_p == _2_Last)
				return _1_First;
			if (++_Cur == _1_Last)
				return _1_Last;
		}
		++_1_First;
	}
	return _1_First;
}

// STXXL ALGO SEARCH WITH NON-STXXL SECOND ARG
// 2 suffix added to differentiate between stxxl-stxxl search

//! \brief External equivalent of std::search
//! \remark The implementation exploits \c \<stxxl\> buffered streans (computation and I/O overlapped)
//! Preconditions: contiguous pages between _2_First and _2_Last
//! \param _1_First object of model of \c ext_random_access_iterator concept
//! \param _1_Last object of model of \c ext_random_access_iterator concept
//! \param _2_First standard iterator
//! \param _2_Last standard iterator
//! \param nbuffers number of buffers (blocks) for internal use (should be at least 2*D )
//! \return first iterator \c i in the range [_1_First, _1_Last) such that [_2_First, _2_Last) is
//!		available at \c i; otherwise _1_Last

template <typename ForwardIterator1, typename ForwardIterator2>
ForwardIterator1 search2(ForwardIterator1 _1_First, ForwardIterator1 _1_Last,
                        ForwardIterator2 _2_First, ForwardIterator2 _2_Last,
                        stxxl::int_type nbuffers)
{
	using namespace stxxl;
	// test for empty ranges
	if (_1_First == _1_Last || _2_First == _2_Last)
		return _1_First;
	typedef buf_istream<typename ForwardIterator1::block_type,
	        typename ForwardIterator1::bids_container_iterator>
	        buf_istream_1;
	typedef typename ForwardIterator1::value_type T1;
	typedef typename ForwardIterator2::value_type T2;

	_1_First.flush();
	int_type nb = nbuffers ? nbuffers : (config::get_instance()->disks_number() * 2);

	// Test for a pattern of length 1
	ForwardIterator2 _p1(_2_First);
	if (++_p1 == _2_Last) {
		// reset _1 buffer
		buf_istream_1 in1(_1_First.bid(), _1_Last.bid() + ((_1_Last.block_offset()) ? 1 : 0), nb);
		ForwardIterator1 _cur = _1_First - _1_First.block_offset();
		// skip part of the block before _begin untouched
		for ( ; _cur != _1_First; ++_cur)
			++in1;
		// search in the the range [_1_First, _1_Last)
		for ( ; _cur != _1_Last; ++_cur) {
			T1 tmp;
			in1 >> tmp;
			if (tmp == *_2_First)
				return _cur; // safe
		}
		return _cur;
	}

	// General case.
	ForwardIterator2 _p;
	ForwardIterator1 _Cur = _1_First;
	for (;;) {
		// reset _1's buffer
		// it SHOULD get smaller as _1_First approaches _1_Last
		buf_istream_1 in1(_1_First.bid(), _1_Last.bid() + ((_1_Last.block_offset()) ? 1 : 0), nb);
		ForwardIterator1 cur1 = _1_First - _1_First.block_offset();
		// skip part of the block before _begin untouched
		for ( ; cur1 != _1_First; ++cur1)
			++in1;
		// search in the range [_1_First, _1_Last)
		for ( ; cur1 != _1_Last; ++cur1) {
			typename ForwardIterator1::value_type tmp;
			in1 >> tmp;
			if (tmp == *_2_First)
				goto _found0; // 'return' unsafe; make do with 'goto'
		}
		if (cur1 == _1_Last)
			return _1_Last;
_found0:
		_1_First = cur1;

		_p = _p1;
		_Cur = _1_First;
		if (++_Cur == _1_Last)
			return _1_Last;
		T1 tmp1;
		in1 >> tmp1;
		for (; tmp1 == *_p; in1 >> tmp1) {
			if (++_p == _2_Last)
				return _1_First;
			if (++_Cur == _1_Last)
				return _1_Last;
		}
		++_1_First;
	}
	return _1_First;
}

// STXXL ALGO SEARCH WITH BINARY PREDICATE

//! \brief External equivalent of std::search
//! \remark The implementation exploits \c \<stxxl\> buffered streans (computation and I/O overlapped)
//! \param _1_First object of model of \c ext_random_access_iterator concept
//! \param _1_Last object of model of \c ext_random_access_iterator concept
//! \param _2_First object of model of \c ext_random_access_iterator concept
//! \param _2_Last object of model of \c ext_random_access_iterator concept
//! \param _pred binary predicate functor to use for comparison
//! \param nbuffers number of buffers (blocks) for internal use (should be at least 2*D )
//! \return first iterator \c i in the range [_1_First, _1_Last) such that [_2_First, _2_Last) is
//!		available at \c i; otherwise _1_Last
template <typename ForwardIterator1, typename ForwardIterator2, class _BinaryPredicate>
ForwardIterator1 search(ForwardIterator1 _1_First, ForwardIterator1 _1_Last,
                        ForwardIterator2 _2_First, ForwardIterator2 _2_Last,
			_BinaryPredicate _pred,
                        stxxl::int_type nbuffers)
{
	using namespace stxxl;
	// test for empty ranges
	if (_1_First == _1_Last || _2_First == _2_Last)
		return _1_First;
	typedef buf_istream<typename ForwardIterator1::block_type,
	        typename ForwardIterator1::bids_container_iterator>
	        buf_istream_1;
	typedef buf_istream<typename ForwardIterator2::block_type,
	        typename ForwardIterator2::bids_container_iterator>
	        buf_istream_2;
	typedef typename ForwardIterator1::value_type T1;
	typedef typename ForwardIterator2::value_type T2;

	_1_First.flush();
	_2_First.flush();
	int_type nb = nbuffers ? nbuffers : (config::get_instance()->disks_number() * 2);

	// Test for a pattern of length 1
	ForwardIterator2 _p1(_2_First);
	if (++_p1 == _2_Last) {
		// reset _1 buffer
		buf_istream_1 in1(_1_First.bid(), _1_Last.bid() + ((_1_Last.block_offset()) ? 1 : 0), nb);
		ForwardIterator1 _cur = _1_First - _1_First.block_offset();
		// skip part of the block before _begin untouched
		for ( ; _cur != _1_First; ++_cur)
			++in1;
		// search in the the range [_1_First, _1_Last)
		for ( ; _cur != _1_Last; ++_cur) {
			T1 tmp;
			in1 >> tmp;
			if (_pred(tmp, *_2_First))
				return _cur; // safe
		}
		return _cur;
	}

	// General case.
	ForwardIterator2 _p;
	ForwardIterator1 _Cur = _1_First;
	for (;;) {
		// reset _1's buffer
		// it SHOULD get smaller as _1_First approaches _1_Last
		buf_istream_1 in1(_1_First.bid(), _1_Last.bid() + ((_1_Last.block_offset()) ? 1 : 0), nb);
		ForwardIterator1 cur1 = _1_First - _1_First.block_offset();
		// skip part of the block before _begin untouched
		for ( ; cur1 != _1_First; ++cur1)
			++in1;
		// search in the range [_1_First, _1_Last)
		for ( ; cur1 != _1_Last; ++cur1) {
			T1 tmp;
			in1 >> tmp;
			if (_pred(tmp, *_2_First))
				goto _found0; // 'return' unsafe; make do with 'goto'
		}
		if (cur1 == _1_Last)
			return _1_Last;
_found0:
		_1_First = cur1;

		// buffer _2
		buf_istream_2 in2(_2_First.bid(), _2_Last.bid() + ((_2_Last.block_offset()) ? 1 : 0), nb);
		// skip the part of the block before _2_First untouched
		ForwardIterator2 cur2 = _2_First - _2_First.block_offset();
		for ( ; cur2 != _2_First; ++cur2)
			++in2;
		_p = _p1;
		T2 tmp2;
		in2 >> tmp2;
		_Cur = _1_First;
		if (++_Cur == _1_Last)
			return _1_Last;
		T1 tmp1;
		in1 >> tmp1;
		for (; _pred(tmp1, tmp2); in1 >> tmp1, in2 >> tmp2) {
			if (++_p == _2_Last)
				return _1_First;
			if (++_Cur == _1_Last)
				return _1_Last;
		}
		++_1_First;
	}
	return _1_First;
}

// STXXL ALGO SEARCH WITH NON-STXXL SECOND ARG AND BINARY PREDICATE
// 2 suffix version

//! \brief External equivalent of std::search
//! \remark The implementation exploits \c \<stxxl\> buffered streans (computation and I/O overlapped)
//! \param _1_First object of model of \c ext_random_access_iterator concept
//! \param _1_Last object of model of \c ext_random_access_iterator concept
//! \param _2_First standard iterator
//! \param _2_Last standard iterator
//! \param _pred binary predicate functor to use for comparison
//! \param nbuffers number of buffers (blocks) for internal use (should be at least 2*D )
//! \return first iterator \c i in the range [_1_First, _1_Last) such that [_2_First, _2_Last) is
//!		available at \c i; otherwise _1_Last
template <typename ForwardIterator1, typename ForwardIterator2, class _BinaryPredicate>
ForwardIterator1 search2(ForwardIterator1 _1_First, ForwardIterator1 _1_Last,
                        ForwardIterator2 _2_First, ForwardIterator2 _2_Last,
			_BinaryPredicate _pred,
                        stxxl::int_type nbuffers)
{
	using namespace stxxl;
	// test for empty ranges
	if (_1_First == _1_Last || _2_First == _2_Last)
		return _1_First;
	typedef buf_istream<typename ForwardIterator1::block_type,
	        typename ForwardIterator1::bids_container_iterator>
	        buf_istream_1;
	typedef typename ForwardIterator1::value_type T1;

	_1_First.flush();
	int_type nb = nbuffers ? nbuffers : (config::get_instance()->disks_number() * 2);

	// Test for a pattern of length 1
	ForwardIterator2 _p1(_2_First);
	if (++_p1 == _2_Last) {
		// reset _1 buffer
		buf_istream_1 in1(_1_First.bid(), _1_Last.bid() + ((_1_Last.block_offset()) ? 1 : 0), nb);
		ForwardIterator1 _cur = _1_First - _1_First.block_offset();
		// skip part of the block before _begin untouched
		for ( ; _cur != _1_First; ++_cur)
			++in1;
		// search in the the range [_1_First, _1_Last)
		for ( ; _cur != _1_Last; ++_cur) {
			T1 tmp;
			in1 >> tmp;
			if (_pred(tmp, *_2_First))
				return _cur; // safe
		}
		return _cur;
	}

	// General case.
	ForwardIterator2 _p;
	ForwardIterator1 _Cur = _1_First;
	for (;;) {
		// reset _1's buffer
		// it SHOULD get smaller as _1_First approaches _1_Last
		buf_istream_1 in1(_1_First.bid(), _1_Last.bid() + ((_1_Last.block_offset()) ? 1 : 0), nb);
		ForwardIterator1 cur1 = _1_First - _1_First.block_offset();
		// skip part of the block before _begin untouched
		for ( ; cur1 != _1_First; ++cur1)
			++in1;
		// search in the range [_1_First, _1_Last)
		for ( ; cur1 != _1_Last; ++cur1) {
			T1 tmp;
			in1 >> tmp;
			if (_pred(tmp, *_2_First))
				goto _found0; // 'return' unsafe; make do with 'goto'
		}
		if (cur1 == _1_Last)
			return _1_Last;
_found0:
		_1_First = cur1;

		_p = _p1;
		_Cur = _1_First;
		if (++_Cur == _1_Last)
			return _1_Last;
		T1 tmp1;
		in1 >> tmp1;
		for (; _pred(tmp1, *_p); in1 >> tmp1) {
			if (++_p == _2_Last)
				return _1_First;
			if (++_Cur == _1_Last)
				return _1_Last;
		}
		++_1_First;
	}
	return _1_First;
}

__MOSH_STXXL_END_NAMESPACE

#endif

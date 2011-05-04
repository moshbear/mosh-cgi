// Typedefs and type aliases for stxxl.
//
// v3 m0shbear
// license: boost
#ifndef BIG_TYPES_H
#define BIG_TYPES_H

#include <stxxl/types>
#include <stxxl/map>
#include <stxxl/deque>
#include <stxxl/vector>
#include <stxxl/algorithm>
#include <stxxl-mosh/algo/find.h>
#include <stxxl-mosh/algo/search.h>

BIG_TYPES_BEGIN

/*! \brief BigTypes Template Escape
 * \param _0 token to append '<' to
 * \param [ ... ] - template arguments
 * \note Self-closing: '>' not needed
 */
#define __BIGTYPES_TE(_0, ...) _0< __VA_ARGS__ >

/*! \brief BigTypes Templated Typename
 * \param _TemplateArgs list of template arguments.
 * 		Precondition(s): output of __BIGTYPES_TE
 * \param _Type Xxx part of 'BigXxx' name
 * \param _StxxlType type in stxxl namespace that this aliases.
 * 		Precondition(s) for _TemplateArgs apply here, too.
 */
#define __BIGTYPES_TT(_TemplateArgs, _Type, _StxxlType) \
	_TemplateArgs struct Big##_Type { \
		typedef stxxl::_StxxlType type; \
	}

/*! \brief BigTypes Templated Function
 * \param _TemplateArgs list of template arguments.
 * 		Precondition(s): output of __BIGTYPES_TE
 * \param _F_Ret Return type. The insertion of Big into the function name
 * 		prevents embedding of return type in _Func
 * \param _Func function name, with parameters. __BIGTYPES_TE not needed
 * 		because cpp recognizes (,...,)
 * \param _StxxlFunc function in stxxl namespace that this aliases.
 * 		Precondition(s) for _TemplateArgs apply here, too.
 *
 * \note appending semicolon not needed for this macro
 */
#define __BIGTYPES_TF(_TemplateArgs, _F_Ret, _Func, _StxxlFunc,...) \
	 _TemplateArgs _F_Ret Big##_Func { \
		return stxxl##__VA_ARGS__::_StxxlFunc ;\
	}

/*! \brief BigTypes TypeDef
 * \param _StxxlT type in stxxl namespace to alias
 * \param _MyT alias name (Big is automatically prefixed)
 */
#define __BIGTYPES_TD(_StxxlT, _MyT) typedef stxxl::_StxxlT Big##_MyT

// BigVector<T,...> => stxxl::vector<T,...>
__BIGTYPES_TT(__BIGTYPES_TE(template, typename _T, typename... _),
		Vector, __BIGTYPES_TE(vector, _T, _));
// BigMap<K,V,...> => stxxl::map<K,V,...>
__BIGTYPES_TT(__BIGTYPES_TE(template, typename _K, typename _V, typename... _),
		Map, __BIGTYPES_TE(map, _K, _V, _));
// BigList<T,...> => stxxl::deque<T,...>
__BIGTYPES_TT(__BIGTYPES_TE(template, typename _T, typename... _),
		List, __BIGTYPES_TE(deque, _T, _));

// BigFind<T1,T2>(_1,_2,_3,_4) => stxxl::find<T1,T2>(_1,_2,_3,_4)
__BIGTYPES_TF(__BIGTYPES_TE(template, typename _ExtIt, class _Cmp),
		_ExtIt, Find(_ExtIt _Beg, _ExtIt _End, _Cmp _Val, stxxl::int_type nbuf = 0),
		find(_Beg, _End, _Val, nbuf))
// BigFind<T1,T2,T3>(_1,_2,_3,_4,_5) => stxxl-mosh::find<T1,T2,T3>(_1,_2,_3,_4,_5)
__BIGTYPES_TF(__BIGTYPES_TE(template, typename _ExtIt, class _Cmp, class BPred),
		_ExtIt, Find(_ExtIt _Beg, _ExtIt _End, _Cmp _Val, BPred _Pred, stxxl::int_type nbuf = 0),
		find(_Beg, _End, _Val, _Pred, nbuf),_mosh)
// BigSearch<T1,T2>(_1,_2,_3,_4,_5) => stxxl-mosh::search<T1,T2>(_1,_2,_3,_4,_5)
__BIGTYPES_TF(__BIGTYPES_TE(template, typename _ExtIt1, typename _ExtIt2),
		_ExtIt1, Search(_ExtIt1 _1_Beg, _ExtIt1 _1_End,
				_ExtIt2 _2_Beg, _ExtIt2 _2_End,
				stxxl::int_type nbuf = 0),
		search(_1_Beg, _1_End, _2_Beg, _2_End, nbuf),_mosh)
// BigSearch<T1,T2,T3>(_1,_2,_3,_4,_5,_6) => stxxl-mosh::search<T1,T2,T3>(_1,_2,_3,_4,_5,_6)
__BIGTYPES_TF(__BIGTYPES_TE(template, typename _ExtIt1, typename _ExtIt2, class BPred),
		_ExtIt1, Search(_ExtIt1 _1_Beg, _ExtIt1 _1_End,
				_ExtIt2 _2_Beg, _ExtIt2 _2_End,
				BPred _Pred, stxxl::int_type nbuf = 0),
		search(_1_Beg, _1_End, _2_Beg, _2_End, _Pred, nbuf),_mosh)

// BigSize => stxxl::uint64
__BIGTYPES_TD(uint64, Size);

#undef __BIGTYPES_TE
#undef __BIGTYPES_TT
#undef __BIGTYPES_TF
#undef __BIGTYPES_TD

BIG_TYPES_END

#endif

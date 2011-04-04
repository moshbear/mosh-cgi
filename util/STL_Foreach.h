/* util/STL_Foreach.h - a macro emulating range-based for, as described in N2930:
 *  				"Range-Based For Loop Wording (Without Concepts)"
 *  
 * Copyright (C) 2011, Andrey Vul <andrey@moshbear.net>
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 	* Redistributions of source code must retain the above copyright
 *		notice, this list of conditions and the following disclaimer.
 * 	* Redistributions in binary form must reproduce the above copyright
 *		notice, this list of conditions and the following disclaimer in the
 *		documentation and/or other materials provided with the distribution.
 *      * The names of the contributors may not be used to endorse or promote products
 *        	derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL ANDREY VUL BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
 * TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
 * BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY
 * WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */


/* * BIG FAT WARNING: CONFORMING BEHAVIOR NOT IMPLEMENTED FOR std::pair, std::tuple,
 * *			std::valarray.
 * *			IT WILL NOT WORK IF _Expression IS NEITHER A CONTAINER NOR AN ARRAY!!
 * */
#ifndef STL_FOREACH
#define STL_FOREACH

#ifndef HAS_N2930_ITERATOR
#include <util/N2930Iterator.h>
#endif

/* 6.5.4 Range-based for statement [stmt.ranged]
 * The range-based for statement
 * 	for ( /for-range-declaration/ : /expression/ ) /statement/
 * is equivalent to
 *      {
 *      	auto && __range = ( /expression/ );
 *		for (auto __begin = /begin-expr/, __end = /end-expr/; __begin != __end; ++__begin ) {
 *      		/for-range-declaration/ = *__begin;
 *                      /statement/
 *              }
 *      }
 * where __range, __begin, and __end are variables defined for exposition only,
 * _RangeT is the type of the expression, and begin-expr and end-expr are determined as follows:
 * 	* If _RangeT is an array type, begin-expr and end-expr are __range and __range + __bound,
 * 	  respectively, where __bound is the array bound. If _RangeT is an array of unknown size
 * 	  or an array of incomplete type, the program is ill-formed.
 *      * Otherwise, begin-expr and end-expr are (std::)begin(__range) and (std::)end(__range),
 *        respectively, where begin and end are looked up with argument-dependent lookup
 *        ([basic.lookup.argdep]). For the purposes of this name lookup, namespace std is an
 *        associated namespace.
 *                     
 * Example:
 * 	int array[5] = { 1, 2, 3, 4, 5 };
 *      for (int& x : array)
 *               x *= 2;
 *	<=> FOREACH(int& x, array, x *= 2;)
 */
/* The macro implementing this for-loop contains 3 arguments:
 * _Declaration, the range declaration
 * _Expression, the expression containing begin(), end()
 * _Statement, the statement. Note, a semicolon must come before the ), as needed.
 */
#define FOREACH(_Declaration, _Expression, _Statement) \
	{\
		auto&& __range = (_Expression);\
		for (auto __begin = std::begin(__range),\
		                __end = std::end(__range);\
		                __begin != __end;\
		                ++__begin) {\
			_Declaration = *__begin;\
			_Statement\
		}\
	}

#endif	// STL_FOREACH

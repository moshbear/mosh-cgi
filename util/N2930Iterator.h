/* util/N2930Iterator.h - extensions to <iterator>, as defined in N2930:
 * 				"Range-Based For Loop Wording (Without Concepts)"
 *
 * Copyright (C) 2011, Andrey Vul <andrey@moshbear.net>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 	* Redistributions of source code must retain the above copyright
 * 		notice, this list of conditions and the following disclaimer.
 * 	* Redistributions in binary form must reproduce the above copyright
 *              notice, this list of conditions and the following disclaimer in the
 *              documentation and/or other materials provided with the distribution.
 *      * The names of the contributors may not be used to endorse or promote products
 *      	derived from this software without specific prior written permission.
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
 *
 */


#ifndef N2930_ITERATOR_INCLUDED
#define N2930_ITERATOR_INCLUDED

#include <cstddef>

namespace std {
/*   // 24.7 range access [iterator.range]
 *     template<typename C> auto begin(C& c) -> decltype(c.begin());
 *     template<typename C> auto begin(const C& c) -> decltype(c.begin());
 *     template<typename C> auto end(C& c) -> decltype(c.end());
 *     template<typename C> auto end(const C& c) -> decltype(c.end());
 *     template<typename T, size_t N> T* begin(T (&array)[N]);
 *     template<typename T, size_t N> T* end(T (&array)[N]);
 */

/* template<typename C> auto begin(C& c) -> decltype(c.begin());
 * template<typename C> auto begin(const C& c) -> decltype(c.begin());
 * 	Returns: c.begin()
 */
template<typename C>
auto begin(C& c) -> decltype(c.begin()) {
	return c.begin();
}
template<typename C>
auto begin(const C& c) -> decltype(c.begin()) {
	return c.begin();
}
/* template<typename C> auto end(C& c) -> decltype(c.end());
 * template<typename C> auto end(const C& c) -> decltype(c.end());
 * 	Returns: c.end()
 */
template<typename C>
auto end(C& c) -> decltype(c.end()) {
	return c.end();
}
template<typename C>
auto end(const C& c) -> decltype(c.end()) {
	return c.end();
}
/* template<typename T, size_t N> T* begin(T (&array)[N]);
 * 	Returns: array
 */
template<typename T, size_t N>
T* begin(T (&array)[N]) {
	return &array[0];
}
/* template<typename T, size_t N> T* end(T (&array)[N]);
 * 	Returns: array + N
 */
template<typename T, size_t N>
T* end(T (&array)[N]) {
	return &array[N];
}
} // namespace std

#endif //N2930_ITERATOR_INCLUDED


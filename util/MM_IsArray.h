#ifndef _UTIL_IS_ARRAY_H_
#define _UTIL_IS_ARRAY_H_ 1

#include <map>
#include <algorithm>
template <class K, class V>
bool multimapElementIsList(const std::multimap<K,V>& map, const K& key) {
	bool exists = (map.find(key) != map.end());
	if (!exists)
		return false;
	auto res = map.equal_range(key);
	size_t dist = static_cast<size_t>(std::distance(res.first, res.second));
	if (dist > 1)
		return true;
	return false;
}

#endif

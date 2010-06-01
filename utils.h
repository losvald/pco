/*
 * utils.h
 *
 * Copyright (C) 2010 Leo Osvald <leo.osvald@gmail.com>
 *
 * This file is part of Purchasable Cost Optimizer.
 *
 * Purchasable Cost Optimizer is free software: you can redistribute
 * it and/or modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation, either
 * version 3 of the License, or (at your option) any later version.
 *
 * Purchasable Cost Optimizer is distributed in the hope that it will
 * be useful, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Purchasable Cost Optimizer. If not, see
 * <http://www.gnu.org/licenses/>.
 */
/*
 * utils.h
 *
 *  Created on: Apr 10, 2010
 *      Author: losvald
 */

#ifndef UTILS_H_
#define UTILS_H_

#define FORC(it, c) for(__typeof(c.begin()) it = (c).begin(); it != (c).end(); ++it)

#include <iostream>
#include <map>

template<typename Tx, typename Ty>
std::ostream& operator<<(std::ostream& os, const std::pair<Tx, Ty>& pair) {
	os << "(" << pair.first << ", " << pair.second << ")";
	return os;
}

//template<typename Key, typename Value>
//class OccurenceMap : public std::map<Key, Value> {
//private:
//	Value zero_;
//public:
//	OccurenceMap() : zero_(0) { };
//	OccurenceMap(Value zero) : zero_(zero) { };
//	bool increment(Key key) {
//		if(count(key))
//			return false;
//		return true;
//	}
//};

#endif /* UTILS_H_ */

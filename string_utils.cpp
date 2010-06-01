/*
 * string_utils.cpp
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
 * string_utils.cpp
 *
 *  Created on: Apr 13, 2010
 *      Author: losvald
 */

#include "string_utils.h"

#include <sstream>
#include <vector>
#include <set>

/**
 * Razdvaja string na temelju znakova u nizu delimiters.
 * Npr. split("a,b,cd.efg,", ",.") -> "a", "b", "cd", "efg"
 */
std::vector<std::string> StringUtils::split(const std::string& s,
		const std::string& delimiters) {
	std::vector<std::string> tokens;
	std::set<char> delim(delimiters.begin(), delimiters.end());
	int last = 0;
	for ( int i = 0; i <= (int)s.size(); ++i ) {
		if ( i == (int)s.size() || delim.find(s[i]) != delim.end() ) {
			if(i-last >= 1)
				tokens.push_back(std::string(s.begin()+last, s.begin()+i));
			last = i+1;
		}
	}
	return tokens;
}

//std::string StringUtils::to_bitstring(int mask, int n) {
//	std::string ret;
//	for(int i = n-1; i >= 0; --i)
//		ret.push_back(((1<<i) & mask) ? '1' : '0');
//	return ret;
//}

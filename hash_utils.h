/*
 * hash_utils.h
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
 * hash_utils.h
 *
 *  Created on: Apr 25, 2010
 *      Author: losvald
 */

#ifndef HASH_UTILS_H_
#define HASH_UTILS_H_

#include <map>

namespace hash {

	template<typename Tx, typename Ty>
	class PerfectHasher {
	private:
		std::map<Tx, Ty> hash_;
		std::map<Ty, Tx> dehash_;
	public:
		void add(const Tx& object) {
			if(!isHashed(object)) {
				hash_[object] = compute_hash(object);
				dehash_[hash_[object]] = object;
			}
		}
		bool isHashed(const Tx& object) const {
			return hash_.count(object);
		}
		virtual Ty hash(const Tx& object) const {
			return hash_.at(object);
		}
		virtual Tx dehash(const Ty& hash) const {
			return dehash_.at(hash);
		}
		void clear() {
			hash_.clear();
			dehash_.clear();
		}
	protected:
		virtual Ty compute_hash(const Tx& object) const = 0;
	};


	template<typename T, typename RangeType>
	class RangeMapper : public PerfectHasher<T, RangeType> {
	private:
		RangeType from_;
		RangeType to_;
		RangeType inc_;
		mutable RangeType cur_ind_;
	public:
		RangeMapper(const RangeType& from, const RangeType& increment = 1)
		: from_(from), to_(from), inc_(increment), cur_ind_(from) { }
		int from() const { return from_; }
		int to() const { return to_; }
		int size() const { return this->cur_ind_ - this->from(); }
	protected:
		virtual RangeType compute_hash(const T& object) const {
			RangeType ret = cur_ind_;
			cur_ind_ += inc_;
			return ret;
		}
	};

}




#endif /* HASH_UTILS_H_ */

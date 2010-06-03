/*
 * constraint.cpp
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
 * constraint.cpp
 *
 *  Created on: Apr 11, 2010
 *      Author: losvald
 */

#include "constraint.h"

//bool Constraint::isMet(const Purchasable& purchasable) const {
//	return false;
//}
//
//ConstraintEqualsPurchasable::ConstraintEqualsPurchasable(const Purchasable&
//		equalsTo) {
//	other_ = equalsTo;
//}
//
//bool ConstraintEqualsPurchasable::isMet(const Purchasable& purchasable) const {
//	return purchasable == other_;
//}
//
//ComplexConstraint::ComplexConstraint(const Constraint& a,
//		UnaryConstraintModifier unaryConstraintModifier)
//: a_(&a), b_(NULL), c_(NULL), type(1), modifier_(unaryConstraintModifier) {
//}
//
//ComplexConstraint::ComplexConstraint(const Constraint& a, const Constraint& b,
//		BinaryConstraintModifier binaryConstraintModifier)
//: a_(&a), b_(&b), c_(NULL), type(2), modifier_(binaryConstraintModifier) {
//}
//
//ComplexConstraint::ComplexConstraint(const Constraint& a, const Constraint& b,
//		const Constraint& c,
//		TernaryConstraintModifier ternaryConstraintModifier)
//: a_(&a), b_(&b), c_(&c), type(3), modifier_(ternaryConstraintModifier) {
//}
//
//bool ComplexConstraint::isMet(const Purchasable& purchasable) const {
//	switch(modifier_) {
//	case NOT:
//		return !a_->isMet(purchasable);
//	case AND:
//		return a_->isMet(purchasable) && b_->isMet(purchasable);
//	case OR:
//		return a_->isMet(purchasable) || b_->isMet(purchasable);
//	case XOR:
//		return a_->isMet(purchasable) ^ b_->isMet(purchasable);
//	case IF:
//		return a_->isMet(purchasable) ? b_->isMet(purchasable)
//				: c_->isMet(purchasable);
//	default:
//		return false;
//	}
//}


/*
 * entity_io_utils.h
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
 * entity_io_utils.h
 *
 *  Created on: Apr 16, 2010
 *      Author: losvald
 */

#ifndef ENTITY_IO_UTILS_H_
#define ENTITY_IO_UTILS_H_

#include "purchasable_data.h"
#include "purchasable.h"
#include "currency.h"
#include "market.h"
#include "seller.h"
#include "purchasable_offer.h"
#include "buyer_request.h"

#include "entity_factory.h"

#include <string>
#include <vector>
#include <set>

class Currency;
class Market;
class PurchasableOffer;
class Seller;

class EntityIOUtils {
public:

	static bool loadExchangeRates(const std::string& filename);
	static bool savePurchasables(const std::string& filename,
			const PurchasableData& data);
	static bool loadPurchasables(PurchasableData& data,
			const std::string& filename);
	static bool saveSellers(const std::string& filename, const Market& market);
	static bool loadSellers(Market& market, const std::string& filename);
	static bool saveOffers(const std::string& filename, const Market& market);
	static bool loadOffers(Market& market, const std::string& filename);
	static bool saveSupplies(const std::string& filename, Market& market);
	static bool loadSupplies(Market& market, const std::string& filename);
	static bool saveRequests(const std::string& filename,
			const std::vector<BuyerRequest>& requests);
	static bool loadRequests(std::vector<BuyerRequest>& requests,
			const std::string& filename);

	static bool saveMarketOLD(const std::string& filename, const Market& market);
	static bool loadMarketOLD(Market& market, const std::string& filename);

	static void printMarket(Market& market);
	static void printPurchasables(const PurchasableData& data);
	static void printSellers(const Market& market);
	static void printCurrencies();
private:
	static void print_input_error(int line, const std::string& msg);
	static void printFileNotExists(const string& filename);
	static bool parsePrice(Price& price, std::string& currency_code,
			std::string str, int line);
	static bool addOffer(Seller* seller, const PurchasableOffer& offer);
	static bool addSupply(Seller* seller, Id purchasable_id, Quantity quantity);
	static bool addRequest(std::vector<BuyerRequest>& requests,
			Constraint<Purchasable>* cons, Quantity quantity,
			bool disjunctive, Id id, std::set<int>& was_id,
			string name, int line);
};

//class PurchasableFactory : public EntityFactory<Purchasable> {
//	virtual Purchasable createEntity();
//};

#endif /* ENTITY_IO_UTILS_H_ */

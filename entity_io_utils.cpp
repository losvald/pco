/*
 * entity_io_utils.cpp
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
 * entity_io_utils.cpp
 *
 *  Created on: Apr 14, 2010
 *      Author: losvald
 */

#include "seller.h"
#include "purchasable_data.h"
#include "entity_io_utils.h"
#include "string_utils.h"

#include <cstdio>
#include <cstdlib>
#include <vector>
#include <string>
#include <cstring>
#include <iostream>

namespace print_constants {
	static const char* MSG_TAG = "[PRINT]";
};

namespace ioconstants {
	static const char COMMENT_TAG = '#';
	static const int LINE_LENGTH = 4096;
	static const char PURCHASABLE_DECLARATION_TAG = '@';
	static const char* NAME_PROPERTY = "name";
	static const string blank = " ";
}

void EntityIOUtils::printCurrencies() {
	printf("%sCurrencies:\n", print_constants::MSG_TAG);
	printf("Base currency: %s\n", Currency::base_currency()->code().c_str());
	FORC(it, Currency::currencies()) {
		printf("%s\t%lf\t%lld\n", it->second.code().c_str(),
				it->second.relative_rate(), it->second.getPrice(1000));
	}
}

bool EntityIOUtils::loadExchangeRates(const std::string& filename) {
	static char code[4];
	double rate;
	bool first = true;

	printf("Loading currencies...\n");

	FILE *f = fopen(filename.c_str(), "r");
	if(f == NULL) {
		printFileNotExists(filename);
		return false;
	}
	while(fscanf(f, "%s%lf", code, &rate) == 2) {
		Currency currency(code, rate);
		Currency::currencies_.insert(make_pair(string(code), currency));
		if(first) {
			Currency::setBase(*Currency::getCurrency(code),
					Currency::defaultPriceUnitsPerUnitValue);
			first = false;
		}
	}
	fclose(f);
	return true;
}

void EntityIOUtils::printPurchasables(const PurchasableData& data) {
	printf("%sTest list purchasables:\n", print_constants::MSG_TAG);
	vector<const Purchasable*> v = data.getPurchasables();
	FORC(it, v) {
		const Purchasable& p = **it;
		cout << p << endl;
	}
}

bool EntityIOUtils::loadPurchasables(PurchasableData& data,
		const std::string& filename) {
	static char buff[ioconstants::LINE_LENGTH];
	static char key_buff[ioconstants::LINE_LENGTH],
	value_buff[ioconstants::LINE_LENGTH];

	printf("Loading products and services...\n");

	FILE *f = fopen(filename.c_str(), "r");
	if(f == NULL) {
		printFileNotExists(filename);
		return false;
	}
	Purchasable p;
	bool was_declaration = false;
	for( int line = 1 ; fgets(buff, ioconstants::LINE_LENGTH, f); ++line) {
		int len = strlen(buff);
		buff[len] = '\0';
		if(len == 0 || buff[0] == ioconstants::COMMENT_TAG) continue;
		if(buff[0] == ioconstants::PURCHASABLE_DECLARATION_TAG) {
			if(was_declaration) {
				data.addPurchasable(p);
			}
			p = Purchasable();
			int id;
			if(sscanf(buff+1, "%d", &id) != 1) {
				print_input_error(line, "Invalid purchasable id\n");
				return false;
			}
			p.set_id(id);
			was_declaration = true;
			continue;
		}
		if(strchr(buff, '=') != NULL) {
			sscanf(buff, "%s = %[^\n]", key_buff, value_buff);
			string key = key_buff, value = StringUtils::trim(value_buff);
			printf("(name=%s\tvalue=%s)\n", key.c_str(), value.c_str());
			if(key == ioconstants::NAME_PROPERTY)
				p.set_name(value);
			else {
				p.setProperty(key, value);
			}
		}

	}
	if(was_declaration) {
		data.addPurchasable(p);
	}

	fclose(f);
	return true;
}

bool EntityIOUtils::savePurchasables(const std::string& filename,
		const PurchasableData& data) {
	printf("Saving products and services\n");
	FILE *f = fopen(filename.c_str(), "w");
	vector<const Purchasable*> v = data.getPurchasables();
	FORC(it, v) {
		const Purchasable* p = *it;
		fprintf(f, "%c%d\n", ioconstants::PURCHASABLE_DECLARATION_TAG, p->id());
		if(!p->name().empty())
			fprintf(f, "%s = %s\n", ioconstants::NAME_PROPERTY, p->name().c_str());
		FORC(it, p->properties())
		fprintf(f, "%s = %s\n", it->first.c_str(), it->second.c_str());
	}
	fclose(f);
	return true;
}

namespace seller_ioconstants {
	static const char DECLARATION_TAG = '@';
	static const char* NAME_PROPERTY = "name";
	static const char* DEFAULT_CURRENCY_PROPERTY = "defaultCurrency";
};

namespace offer_ioconstants {
	static const char DECLARATION_TAG = '!';
	static const char* CONTENTS_DELIMITERS = ",";
	static const char* PRICE_CURRENCY_DELIMITER = " ";
	static const char* PRICE_PROPERTY = "price";
	static const char* CONTENTS_PROPERTY = "includes";
	static const char* REQUIREMENTS_PROPERTY = "requires";
	static const char* NAME_PROPERTY = "name";
	static const char* SELLER_ID_PROPERTY = "seller_id";
	static const char* WHOLESALE_PROPERTY = "wholesale_prices";
	static const char* PURCHASABLE_ID_QUANTITY_DELIMITERS = " x\t";
	static const char* WHOLESALE_DELIMITERS = "|";
	static const char* WHOLESALE_QUANTITY_RANGE_DELIMITERS = "-";
	static const char* WHOLESALE_QUANTITY_RANGE_PLUS = "+";
	static const char* WHOLESALE_QUANTITY_PRICE_DELIMITERS = ":";
}

bool EntityIOUtils::loadMarketOLD(Market& market, const std::string& filename) {

	static char buff[ioconstants::LINE_LENGTH];
	static char key_buff[ioconstants::LINE_LENGTH],
	value_buff[ioconstants::LINE_LENGTH];
	static Seller seller;
	static PurchasableOffer offer(BaseEntity::UNDEFINED_ID);

	printf("Loading sellers and their offers...\n");

	FILE *f = fopen(filename.c_str(), "r");
	if(f == NULL) {
		printFileNotExists(filename);
		return false;
	}
	bool was_declaration = false;
	bool offer_declarations_started = false;
	for(int line = 1; fgets(buff, ioconstants::LINE_LENGTH, f); ++line) {
		int len = strlen(buff);
		buff[len] = '\0';
		if(len == 0 || buff[0] == ioconstants::COMMENT_TAG) continue;
		if(buff[0] == seller_ioconstants::DECLARATION_TAG) {
			if(was_declaration) {
				if(offer_declarations_started)
					seller.addOffer(offer);
				market.registerSeller(seller);
			}
			int id;
			if(sscanf(buff+1, "%d", &id) != 1) {
				print_input_error(line, "Invalid id\n");
				return false;
			}
			seller = Seller(NULL);
			seller.set_id(id);
			printf("Declared seller #%d\n", id);
			was_declaration = true;
			offer_declarations_started = false;
		}
		else if(buff[0] == offer_ioconstants::DECLARATION_TAG) {
			if(offer_declarations_started)
				seller.addOffer(offer);
			int offer_id;
			if(sscanf(buff+1, "%d", &offer_id) != 1) {
				print_input_error(line, "Invalid offer id\n");
				return false;
			}
			offer = PurchasableOffer();
			offer.set_id(offer_id);
			offer_declarations_started = true;
//			printf("Declared offer #%d\n", offer_id);
		}
		else if(strchr(buff, '=') != NULL) {
			sscanf(buff, "%s = %[^\n]", key_buff, value_buff);
			string key = key_buff, value = StringUtils::trim(value_buff);
//			printf("(key=%s\tvalue=%s)\n", key.c_str(), value.c_str());
			if(!offer_declarations_started) {
				if(key == seller_ioconstants::NAME_PROPERTY)
					seller.set_name(value);
				else if(key == seller_ioconstants::DEFAULT_CURRENCY_PROPERTY)
					seller.set_default_currency(Currency::getCurrency(value));
			}
			else { //offer declarations
				if(key == offer_ioconstants::NAME_PROPERTY) {
					offer.set_name(value);
				}
				else if(key == offer_ioconstants::PRICE_PROPERTY) {
					vector<string> price_currency = StringUtils::split(value,
							offer_ioconstants::PRICE_CURRENCY_DELIMITER);
					//set currency
					string currency_code;
					if(price_currency.size() > 1)
						currency_code = price_currency[1];
					else {
						const Currency* currency = seller.default_currency();
						if(currency == NULL) {
							print_input_error(line, "Unknown currency, \
							forgot setting defaultCurrency on seller?");
							return false;
						}
						currency_code = currency->code();
					}
					const Currency* offer_currency = Currency
							::getCurrency(currency_code);
					if(offer_currency == NULL) {
						print_input_error(line,
								"Warning: Default currency is invalid, \
								using base..");
						offer_currency = Currency::base_currency();
					}
					offer.set_currency(*offer_currency);
					//set price
					double price_value;
					if(sscanf(price_currency[0].c_str(), "%lf",
							&price_value) != 1) {
						print_input_error(line, "Invalid price format");
						return false;
					}
					offer.set_price(offer.currency().getPrice(price_value));
				}
				else if(key == offer_ioconstants::CONTENTS_PROPERTY) {
					vector<string> tokens = StringUtils::split(value,
							offer_ioconstants::CONTENTS_DELIMITERS);

					FORC(s, tokens) {
						vector<string> id_quantity = StringUtils::split(*s,
								offer_ioconstants
								::PURCHASABLE_ID_QUANTITY_DELIMITERS);
						int purchasable_id = atoi(id_quantity[0].c_str());

						int quantity = 1;
						if(id_quantity.size() > 1) {
							quantity = atoi(id_quantity[1].c_str());
						}
						const Purchasable* purchasable = PurchasableData
								::instance().getPurchasable(purchasable_id);
						if(purchasable == NULL) {
							fprintf(stderr,
									"Warning: Purchasable with id %d not found\n",
									purchasable_id);
							continue;
						}
						offer.addPurchasable(*purchasable, quantity);
					}
				}
				else
					offer.setProperty(key, value);
			}
		}
	}
	if(was_declaration) {
		if(offer_declarations_started)
			seller.addOffer(offer);
		market.registerSeller(seller);
	}
	fclose(f);
	return true;
}

bool EntityIOUtils::saveMarketOLD(const std::string& filename, const Market& market) {
	vector<const Seller*> sellers = market.getSellers();
	FORC(it, sellers) {
		const Seller* s = *it;
		printf("%c%d\n", seller_ioconstants::DECLARATION_TAG, s->id());
		if(!s->name().empty())
			printf("%s = %s\n", seller_ioconstants::NAME_PROPERTY,
					s->name().c_str());
		printf("%s = %s\n", seller_ioconstants::DEFAULT_CURRENCY_PROPERTY,
				s->default_currency()->code().c_str());
		//print other properties
		FORC(it, s->properties()) {
			printf("%s = %s\n", it->first.c_str(), it->second.c_str());
		}
		vector<const PurchasableOffer*> offers = s->allOffers();
		printf("FOund %d offers for %s\n", offers.size(), s->name().c_str());
		FORC(o, offers) {
			if(!(*o)->name().empty())
				printf("%s = %s\n", offer_ioconstants::NAME_PROPERTY,
						(*o)->name().c_str());
			string key, value;
			map<string, string> properties = (*o)->properties();
			properties.insert(make_pair(offer_ioconstants::NAME_PROPERTY,
					(*o)->name()));
			string currency_code = (*o)->currency().code();
			double val = (*o)->currency().getValue((*o)->price());
			char price_str[25];
			sprintf(price_str, "%lf", val);
			properties.insert(make_pair(offer_ioconstants::PRICE_PROPERTY,
					string(price_str)+currency_code));

			FORC(prop, properties) {
				printf("%s = %s\n", key.c_str(), value.c_str());
			}
		}
	}
	return true;
}

void EntityIOUtils::printMarket(Market& market) {
	printf("%sPrinting market...\n", print_constants::MSG_TAG);
	std::vector<const Seller*> v = market.getSellers();
	std::cout << print_constants::MSG_TAG << " SELLERS BEGIN" << std::endl;
	FORC(it, v) {
		std::cout << **it;
	}
	std::cout << print_constants::MSG_TAG << " SELLERS END" << std::endl;
}

void EntityIOUtils::printSellers(const Market& market) {
	std::vector<const Seller*> sellers = market.getSellers();
	printf("%sSellers:", print_constants::MSG_TAG);
	FORC(it, sellers) {
		const Seller* s = *it;
		std::cout << *s << endl;
	}
}

bool EntityIOUtils::loadSellers(Market & market, const std::string & filename)
{
	static char buff[ioconstants::LINE_LENGTH];
	static char key_buff[ioconstants::LINE_LENGTH],
	value_buff[ioconstants::LINE_LENGTH];
	static Seller seller;

	printf("Loading sellers...\n");

	FILE *f = fopen(filename.c_str(), "r");
	if(f == NULL) {
		printFileNotExists(filename);
		return false;
	}
	bool pending_seller = false;
	for(int line = 1; fgets(buff, ioconstants::LINE_LENGTH, f); ++line) {
		int len = strlen(buff);
		buff[len] = '\0';
		if(len == 0 || buff[0] == ioconstants::COMMENT_TAG) continue;
		if(buff[0] == seller_ioconstants::DECLARATION_TAG) {
			if(pending_seller) {
				market.registerSeller(seller);
			}
			int id;
			sscanf(buff+1, "%d", &id);
			seller = Seller(NULL);
			seller.set_id(id);
//			printf("Declared seller #%d\n", id);
			pending_seller = true;
		}
		else if(strchr(buff, '=') != NULL) {
			sscanf(buff, "%s = %[^\n]", key_buff, value_buff);
			string key = key_buff, value = StringUtils::trim(value_buff);
			//			printf("(key=%s\tvalue=%s)\n", key.c_str(), value.c_str());
			if(key == seller_ioconstants::NAME_PROPERTY)
				seller.set_name(value);
			else if(key == seller_ioconstants::DEFAULT_CURRENCY_PROPERTY)
				seller.set_default_currency(Currency::getCurrency(value));
		}
	}
	if(pending_seller) {
		market.registerSeller(seller);
	}
	fclose(f);
	return true;
}

bool EntityIOUtils::saveSellers(const std::string & filename, const Market & market)
{
	printf("Saving sellers...\n");
	FILE *f = fopen(filename.c_str(), "w");
	if(f == NULL) {
		printFileNotExists(filename);
		return false;
	}
	vector<const Seller*> sellers = market.getSellers();
	FORC(it, sellers) {
		const Seller* s = *it;
		fprintf(f, "%c%d\n", seller_ioconstants::DECLARATION_TAG, s->id());
		if(!s->name().empty())
			fprintf(f, "%s = %s\n", seller_ioconstants::NAME_PROPERTY,
					s->name().c_str());
		printf("%s = %s\n", seller_ioconstants::DEFAULT_CURRENCY_PROPERTY,
				s->default_currency()->code().c_str());
		//print other properties
		FORC(it, s->properties())
			fprintf(f, "%s = %s\n", it->first.c_str(), it->second.c_str());
		fprintf(f, "defaultCurrency = HRK\n"); //XXX
	}
	fclose(f);
	return true;
}

bool EntityIOUtils::saveOffers(const std::string& filename,
		const Market& market) {

	printf("Saving offers...\n");

	FILE *f = fopen(filename.c_str(), "w");
	if(f == NULL) {
		printFileNotExists(filename);
		return false;
	}

	vector<const Seller*> sellers = market.getSellers();
	FORC(it, sellers) {
		const Seller* s = *it;
		vector<const PurchasableOffer*> offers = s->allOffers();
		//printf("FOund %d offers for %s\n", offers.size(), s->name().c_str());
		FORC(o, offers) {
			if((*o)->seller() == NULL) {
				fprintf(stderr, "Offer#%d has no seller!\n", (*o)->id());
				return false;
			}
			//output id
			fprintf(f, "%c%d\n", offer_ioconstants::DECLARATION_TAG,
					(*o)->id());
			//generate seller id and other properties
			string key, value;
			map<string, string> properties = (*o)->properties();
			properties.insert(make_pair(offer_ioconstants::NAME_PROPERTY,
					(*o)->name()));
			char seller_id_str[32];
			sprintf(seller_id_str, "%d", (*o)->seller()->id());
			properties.insert(make_pair(offer_ioconstants::SELLER_ID_PROPERTY,
					seller_id_str));
			string currency_code = (*o)->currency().code();
			double val = (*o)->currency().getValue((*o)->price());
			char price_str[32];
			//generate price property
			sprintf(price_str, "%lf", val);
			properties.insert(make_pair(offer_ioconstants::PRICE_PROPERTY,
					string(price_str)+" "+currency_code));
			//generate contents
			fprintf(f, "%s = ", offer_ioconstants::CONTENTS_PROPERTY);
			const set<Id>& purchasables = (*o)->purchasables();
			int cnt = purchasables.size();
			FORC(it, purchasables) {
				//if not last
				fprintf(f, "%d %c%d", *it,
						offer_ioconstants::PURCHASABLE_ID_QUANTITY_DELIMITERS[1],
						(*o)->getQuantity(*it));
				if(--cnt > 0) {
					fprintf(f, "%s", offer_ioconstants::CONTENTS_DELIMITERS);
				}
			}
			fprintf(f, "\n");
			//generate wholesale property
			string wholesale_prices;
			cnt = (*o)->wholesale_prices().size();
			FORC(it, (*o)->wholesale_prices()) {
				val = (*o)->currency().getValue(it->second);
				sprintf(price_str, "%lf", val);
				currency_code = (*o)->currency().code();
				wholesale_prices += StringUtils::to_string(it->first,
						offer_ioconstants::WHOLESALE_QUANTITY_RANGE_DELIMITERS,
						"", offer_ioconstants::WHOLESALE_QUANTITY_PRICE_DELIMITERS)
						+ ioconstants::blank
						+ string(price_str) + " " + currency_code;
				if(--cnt > 0) {
					wholesale_prices += ioconstants::blank
							+ offer_ioconstants::WHOLESALE_DELIMITERS
							+ ioconstants::blank;
				}
			}
			properties.insert(make_pair(offer_ioconstants::
					WHOLESALE_PROPERTY, wholesale_prices));
			FORC(prop, properties) {
				fprintf(f, "%s = %s\n", prop->first.c_str(), prop->second.c_str());
			}
		}
	}
	return true;
}

bool EntityIOUtils::loadOffers(Market& market, const std::string& filename) {
	static char buff[ioconstants::LINE_LENGTH];
	static char key_buff[ioconstants::LINE_LENGTH],
	value_buff[ioconstants::LINE_LENGTH];
	static PurchasableOffer offer(BaseEntity::UNDEFINED_ID);

	printf("Loading offers...\n");
	FILE *f = fopen(filename.c_str(), "r");
	if(f == NULL) {
		printFileNotExists(filename);
		return false;
	}
	bool pending_offer = false;
	int seller_id = 0;
	for(int line = 1; fgets(buff, ioconstants::LINE_LENGTH, f); ++line) {
		int len = strlen(buff);
		buff[len] = '\0';
		if(len == 0 || buff[0] == ioconstants::COMMENT_TAG) continue;
		if(buff[0] == offer_ioconstants::DECLARATION_TAG) {
			if(pending_offer) {
				if(!addOffer(market.seller(seller_id), offer))
					return false;
			}
			int offer_id;
			sscanf(buff+1, "%d", &offer_id);
			offer = PurchasableOffer();
			offer.set_id(offer_id);
			//printf("Declared offer #%d\n", offer_id);
			pending_offer = true;
		}
		else if(strchr(buff, '=') != NULL) {
			sscanf(buff, "%s = %[^\n]", key_buff, value_buff);
			string key = key_buff, value = StringUtils::trim(value_buff);
			//printf("(key=%s\tvalue=%s)\n", key.c_str(), value.c_str());
			if(key == offer_ioconstants::NAME_PROPERTY) {
				offer.set_name(value);
			}
			else if(key == offer_ioconstants::SELLER_ID_PROPERTY) {
				if(sscanf(value.c_str(), "%d", &seller_id) != 1) {
					print_input_error(line, "Invalid seller id format");
					return false;
				}
				Seller* seller = market.seller(seller_id);
				if(seller == NULL) {
					print_input_error(line, "Non-existing seller");
					return false;
				}
				offer.set_seller(seller);
			}
			else if(key == offer_ioconstants::PRICE_PROPERTY) {
				string currency_code;
				Price price;
				if(!parsePrice(price, currency_code, value, line))
					return false;
				//set currency and price
				offer.set_currency(*Currency::getCurrency(currency_code));
				offer.set_price(price);
			}
			else if(key == offer_ioconstants::WHOLESALE_PROPERTY) {
				vector<string> tokens = StringUtils::split(value,
						offer_ioconstants::WHOLESALE_DELIMITERS);
				FORC(it, tokens) {
					vector<string> qrange_price = StringUtils::split(*it,
							offer_ioconstants::
							WHOLESALE_QUANTITY_PRICE_DELIMITERS);
					if(qrange_price.size() < 2) {
						print_input_error(line, "Missing price");
						return false;
					}
					vector<string> q = StringUtils::split(qrange_price[0],
							offer_ioconstants::
							WHOLESALE_QUANTITY_RANGE_DELIMITERS);
					if(q.empty()) {
						print_input_error(line, "No quantity");
						return false;
					}
					int q1 = 1, q2 = 1;
					if(sscanf(q[0].c_str(), "%d", &q1) != 1) {
						print_input_error(line, "Bad quantity format");
						return false;
					}
					if(q.size() == 1) {
						if(qrange_price[0].find(offer_ioconstants::
								WHOLESALE_QUANTITY_RANGE_PLUS)
								!= string::npos)
							q2 = maxQuantity;
						else
							q2 = q1;
					}
					else if(sscanf(q[1].c_str(), "%d", &q2) != 1) {
						print_input_error(line, "Bad quantity format");
						return false;
					}

					string currency_code;
					Price price;
					if(!parsePrice(price, currency_code, qrange_price[1], line))
						return false;
					//set wholesale price
					offer.set_wholesale_price(
							make_pair<Quantity, Quantity>(q1, q2), price);
				}
			}
			else if(key == offer_ioconstants::CONTENTS_PROPERTY) {
				printf("Adding contents for:");
				cout << offer.name() << endl;
				vector<string> tokens = StringUtils::split(value,
						offer_ioconstants::CONTENTS_DELIMITERS);
				printf("Parsed %d tokens:\n", tokens.size());
				FORC(s, tokens)
					cout << "[" << *s << "]" << endl;
				FORC(s, tokens) {
					vector<string> id_quantity = StringUtils::split(*s,
							offer_ioconstants
							::PURCHASABLE_ID_QUANTITY_DELIMITERS);
					int purchasable_id = atoi(id_quantity[0].c_str());

					int quantity = 1;
					if(id_quantity.size() > 1) {
						quantity = atoi(id_quantity[1].c_str());
					}
					const Purchasable* purchasable = PurchasableData
							::instance().getPurchasable(purchasable_id);
					if(purchasable == NULL) {
						fprintf(stderr,
								"Warning: Purchasable with id %d not found\n",
								purchasable_id);
						continue;
					}
					printf("Adding content: %dx %d\n", quantity,
							purchasable->id());
					offer.addPurchasable(*purchasable, quantity);
				}
			}
			else
				offer.setProperty(key, value);
		}
	}
	if(pending_offer) {
		if(!addOffer(market.seller(seller_id), offer))
			return false;
	}

	fclose(f);
	return true;
}

namespace supply_ioconstants {
	static const char SUPPLY_DECLARATION_TAG = '@';
	static const char* SELLER_ID_PROPERTY = "seller_id";
	static const char* PURCHASABLE_ID_PROPERTY = "purchasable_id";
	static const char* QUANTITY_PROPERTY = "quantity";
	static const int DEFAULT_QUANTITY = 5;
}

bool EntityIOUtils::loadSupplies(Market& market, const std::string& filename) {
	static char buff[ioconstants::LINE_LENGTH];
	static char key_buff[ioconstants::LINE_LENGTH],
	value_buff[ioconstants::LINE_LENGTH];

	printf("Loading supplies...\n");

	FILE *f = fopen(filename.c_str(), "r");
	if(f == NULL) {
		printFileNotExists(filename);
		return false;
	}

	bool pending_supply = false;
	int seller_id = 0;
	int purchasable_id = 0;
	int quantity = 0;
	set<Id> was_purchasable_id;
	int line;
	for(line = 1; fgets(buff, ioconstants::LINE_LENGTH, f); ++line) {
		int len = strlen(buff);
		buff[len] = '\0';
		if(len == 0 || buff[0] == ioconstants::COMMENT_TAG) continue;
		if(buff[0] == supply_ioconstants::SUPPLY_DECLARATION_TAG) {
			if(pending_supply) {
				printf("Adding supply: seller = %d; pid = %d; q = %d\n",
						seller_id, purchasable_id, quantity);
				if(!addSupply(market.seller(seller_id), purchasable_id,
						quantity)) {
					print_input_error(line, "Non-existing seller for supply\n");
					return false;
				}
				was_purchasable_id.insert(purchasable_id);
			}
			pending_supply = true;
		}
		else if(strchr(buff, '=') != NULL) {
			sscanf(buff, "%s = %[^\n]", key_buff, value_buff);
			string key = key_buff, value = StringUtils::trim(value_buff);
			//printf("(key=%s\tvalue=%s)\n", key.c_str(), value.c_str());
			if(key == supply_ioconstants::SELLER_ID_PROPERTY) {
				if(sscanf(value.c_str(), "%d", &seller_id) != 1) {
					print_input_error(line, "Invalid seller id");
				}
			}
			else if(key == supply_ioconstants::PURCHASABLE_ID_PROPERTY) {
				if(sscanf(value.c_str(), "%d", &purchasable_id) != 1) {
					print_input_error(line, "Invalid purchasable id");
				}
			}
			else if(key == supply_ioconstants::QUANTITY_PROPERTY) {
				if(sscanf(value.c_str(), "%d", &quantity) != 1
						|| quantity < 0) {
					print_input_error(line, "Invalid quantity");
				}
			}
		}
	}
	if(pending_supply) {
		printf("Adding supply: seller = %d; pid = %d; q = %d\n",
								seller_id, purchasable_id, quantity);
		if(!addSupply(market.seller(seller_id), purchasable_id, quantity)) {
			print_input_error(line, "Non-existing seller for supply\n");
			return false;
		}
		was_purchasable_id.insert(purchasable_id);
	}

	//ensure default quantities for purchasables not specified
	vector<Seller*> sellers = market.sellers();
	FORC(s, sellers) {
		vector<const PurchasableOffer*> offers = (*s)->allOffers();
		FORC(o, offers) {
			const set<Id> pids = (*o)->purchasables();
			FORC(p, pids) {
				if((*s)->offers(*p) && was_purchasable_id.find(*p)
						== was_purchasable_id.end()) {
					(*s)->addSupply(*p, supply_ioconstants::DEFAULT_QUANTITY);
				}
			}
		}
	}

	return true;
}

bool EntityIOUtils::saveSupplies(const std::string& filename, Market& market) {
	printf("Saving supplies...\n");
	FILE* f = fopen(filename.c_str(), "w");
	if(f == NULL) {
		printFileNotExists(filename);
		return false;
	}
	vector<const Seller*> sellers = market.getSellers();
	FORC(s, sellers) {
		FORC(sup, (*s)->supplies()) {
			fprintf(f, "%c\n", supply_ioconstants::SUPPLY_DECLARATION_TAG);
			fprintf(f, "%s = %d\n", supply_ioconstants::SELLER_ID_PROPERTY,
					(*s)->id());
			fprintf(f, "%s = %d\n", supply_ioconstants::PURCHASABLE_ID_PROPERTY,
					sup->first);
			fprintf(f, "%s = %d\n", supply_ioconstants::QUANTITY_PROPERTY,
					sup->second);
		}
	}
	fclose(f);
	return true;
}

namespace requests_ioconstants {
	static const char REQUEST_DECLARATION_TAG = '@';
	static const char* NAME_PROPERTY = "name";
	static const char* QUANTITY_PROPERTY = "quantity";
	static const char* DISJUNCTIVE_PROPERTY = "disjunctive";
	static const char* MATCHES_ID_PROPERTY = "match-id";
	static const char* MATCHES_NAME_PROPERTY = "match-name";
	static const char* SUBSTRING_OF_NAME_PROPERTY = "substr-name";
}

bool EntityIOUtils::saveRequests(const std::string& filename,
			const std::vector<BuyerRequest>& requests) {
	fprintf(stderr, "Save requests not yet implemented");
	return false;
}

bool EntityIOUtils::loadRequests(std::vector<BuyerRequest>& requests,
		const std::string& filename) {
	static char buff[ioconstants::LINE_LENGTH];
	static char key_buff[ioconstants::LINE_LENGTH],
	value_buff[ioconstants::LINE_LENGTH];

	static class Constr : public Constraint<Purchasable> {
	private:
		bool b_exact_name;
		string exact_name;
		bool b_exact_id;
		Id exact_id;
		bool b_substr_name;
		string substr_name;
	public:
		Constr() {
			init();
		}
		void init() {
			b_exact_id = b_exact_name = b_substr_name = false;
		}
		void set_exact_id(Id id) {
			this->exact_id = id;
			this->b_exact_id = true;
		}
		void set_exact_name(string name) {
			this->exact_name = name;
			this->b_exact_name = true;
		}
		void set_substr_name(string s) {
			this->substr_name = s;
			this->b_substr_name = true;
		}
		bool isMet(const Purchasable& purchasable) const {
//			cout << "[Constr] " << name() << exact_id << " "
//					<< b_exact_id << b_exact_name << b_substr_name;
//			cout << " meet: " << purchasable.id() << endl;
//			cout << "Check " << purchasable.id() << " vs " << exact_id << endl;
			if(b_exact_id && purchasable.id() != exact_id)
				return false;
			if(b_exact_name && purchasable.name() != exact_name)
				return false;
			if(b_substr_name && purchasable.name()
					.find(substr_name) == string::npos)
				return false;
			return true;
		}
	} *cons;

	Id id = 0;
	set<Id> was_id;
	requests.clear();

	printf("Loading requests...\n");
	FILE *f = fopen(filename.c_str(), "r");
	if(f == NULL) {
		printFileNotExists(filename);
		return false;
	}
	bool pending_request = false;
	int quantity = 1;
	bool disjunctive = true;
	string req_name;
	int line;
	for(line = 1; fgets(buff, ioconstants::LINE_LENGTH, f); ++line) {
		int len = strlen(buff);
		buff[len] = '\0';
		if(len == 0 || buff[0] == ioconstants::COMMENT_TAG) continue;
		//TODO
		if(buff[0] == requests_ioconstants::REQUEST_DECLARATION_TAG) {
			if(pending_request) {
				if(!addRequest(requests, cons, quantity, disjunctive,
						id, was_id, req_name, line))
					return false;
			}
			if(sscanf(buff+1, "%d", &id) != 1) {
				print_input_error(line, "Missing request id\n");
				return false;
			}
			cons = new Constr;
			disjunctive = true;
			quantity = 1;
			req_name = "";
			pending_request = true;
		}
		else if(strchr(buff, '=') != NULL) {
			sscanf(buff, "%s = %[^\n]", key_buff, value_buff);
			string key = StringUtils::trim(key_buff);
			string value = StringUtils::trim(value_buff);
			//printf("(key=%s\tvalue=%s)\n", key.c_str(), value.c_str());
			if(key == requests_ioconstants::NAME_PROPERTY) {
				req_name = value;
				cons->set_name(req_name);
			}
			else if(key == requests_ioconstants::QUANTITY_PROPERTY) {
				if(sscanf(value.c_str(), "%d", &quantity) != 1) {
					print_input_error(line, "Invalid request quantity!\n");
					return false;
				}
			}
			else if(key == requests_ioconstants::MATCHES_ID_PROPERTY) {
				int id = 0;
				if(sscanf(value.c_str(), "%d", &id) != 1) {
					print_input_error(line, "Invalid match id\n");
					return false;
				}
				if(PurchasableData::instance().getPurchasable(id) == NULL) {
					print_input_error(line, "Non-existent match id\n");
					return false;
				}
				cons->set_exact_id(id);

			}
			else if(key == requests_ioconstants::MATCHES_NAME_PROPERTY) {
				cons->set_exact_name(value);
			}
			else if(key == requests_ioconstants::SUBSTRING_OF_NAME_PROPERTY) {
				cons->set_substr_name(value);
			}
			else if(key == requests_ioconstants::DISJUNCTIVE_PROPERTY) {
				int disj_val = 1;
				if(sscanf(value.c_str(), "%d", &disj_val) != 1) {
					print_input_error(line, "Invalid disjunctive value\n");
					return false;
				}
				disjunctive = disj_val;
			}
		}
	}
	if(pending_request) {
		if(!addRequest(requests, cons, quantity, disjunctive,
				id, was_id, req_name, line))
			return false;
	}

	printf("Requests loaded!\n");
	return true;
}


//private methods

void EntityIOUtils::print_input_error(int line, const std::string& msg) {
	fprintf(stderr, "Line #%d: %s\n", line, msg.c_str());
}

void EntityIOUtils::printFileNotExists(const string& filename) {
	fprintf(stderr, "File \"%s\" does not exists!\n", filename.c_str());
}

bool EntityIOUtils::parsePrice(Price& price, string& currency_code,
		string str, int line) {
	vector<string> price_currency = StringUtils::split(str,
			offer_ioconstants::PRICE_CURRENCY_DELIMITER);
	//set currency
	bool no_currency = false;
	if(price_currency.size() > 1)
		currency_code = price_currency[1];
	else {
		no_currency = true;
	}
	const Currency* currency = Currency::getCurrency(currency_code);
	if(currency == NULL) {
		print_input_error(line, "Unknown currency");
		return false;
		//					offer_currency = Currency::base_currency();
	}
	//set price
	double price_value;
	if(sscanf(price_currency[0].c_str(), "%lf", &price_value) != 1) {
		print_input_error(line, "Invalid price value");
		return false;
	}
	price = currency->getPrice(price_value);
	return true;
}

bool EntityIOUtils::addOffer(Seller* seller, const PurchasableOffer& offer) {
	if(seller == NULL) {
		fprintf(stderr, "Error adding offer#%d to seller\n", offer.id());
		return false;
	}
	seller->addOffer(offer);
	return true;
}

bool EntityIOUtils::addSupply(Seller* s, Id purchasable_id,
		Quantity quantity) {
	if(s == NULL) {
		return false;
	}
	s->addSupply(purchasable_id, quantity);
	return true;
}

bool EntityIOUtils::addRequest(std::vector<BuyerRequest>& requests,
		Constraint<Purchasable>* cons, Quantity quantity,
		bool disjunctive, Id id, set<int>& was_id,
		string name, int line) {
	BuyerRequest breq(cons, quantity, disjunctive);
	breq.set_id(id);
	if(was_id.find(id) != was_id.end()) {
		print_input_error(line, "Duplicate id\n");
		return false;
	}
	was_id.insert(id);
	breq.set_name(name);
	requests.push_back(breq);
	cout << "Added request: " << breq;
	return true;
}

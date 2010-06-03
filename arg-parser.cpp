/*
 * arg-parser.cpp
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
 * main.cpp
 *
 *  Created on: May 29, 2010
 *      Author: losvald
 */


#include <boost/program_options/parsers.hpp>
#include <boost/program_options/variables_map.hpp>


#include <string>
#include <iostream>
using namespace std;

namespace po = boost::program_options;
typedef string filename;

/*
void printVarMap( po::variables_map varMap ){
	cout << "varmap:\n";
	for( po::variables_map::iterator it = varMap.begin(); it != varMap.end(); ++it){
		cout << it->first << " " << it-> second.as<string>() << "\n";
	}
}
*/

namespace foo {

bool checkFilename(filename f){
	return f != "";
}

void parseOptions( int ac, char** av ) {
	string action="gensolve", algo;
	filename currencies, purchasables, supplies, sellers, requests, priceRanges;
	int seed=-1, numOffers, supplyMin=-1, supplyMax=-1;

	po::options_description desc("Available options");
	desc.add_options()
		("help,h", "print help message")
		("action,a"           , po::value<string>(&action), "desired program action (generate | solve | gensolve ) ")

		("currencies-file,c"  , po::value<filename>(&currencies)              , "currencies input file")
		("purchasables-file,p", po::value<filename>(&purchasables)            , "purchasables input file")
		("supplies-file,u"    , po::value<filename>(&supplies)                , "supplies input file")
		("sellers-file,s"     , po::value<filename>(&sellers)                 , "sellers input file")
		("requests-file,r"    , po::value<filename>(&requests)                , "requests input file")

		("price-ranges-file,P", po::value<filename>(&priceRanges)             , "price ranges output file if generating")
		("seed,S"               , po::value<int>(&seed)                       , "seed number")
		("num-offers,n"       , po::value<int>(&numOffers)                    , "number of offers")
		("supply-min,m"       , po::value<int>(&supplyMin)                    , "supply minimum value")
		("supply-max,M"       , po::value<int>(&supplyMax)                    , "supply maximum value")

		("algorithm,A"        , po::value<string>(&algo)                      , "algorithm to use for solving")
	;

	po::variables_map varMap;
	try{
		po::store( po::parse_command_line(ac, av, desc) , varMap);
	}
	catch( po::error e ) { cerr << e.what() << endl; exit(1); }

	po::notify(varMap);

	//printVarMap(varMap);
	if(varMap.count("help") ){
		cerr << desc << endl;
		exit(1);
	}
	if(varMap.count("action") == 0)
		cerr << "Doing default action (= " << action << ")\n";

	if(action != "generate" && action != "gensolve" && action != "solve"){
		cerr << "Unknown action " << action <<"\n";
		exit(1);
	}

	if(action == "generate" || action =="gensolve") {
		if( !checkFilename(purchasables) ){
			cerr << "Unspecified purchasables file\n";
			exit(1);
		}
		if( !checkFilename(supplies) ){
			cerr << "Unspecified supplies file\n";
			exit(1);
		}
		if( !checkFilename(priceRanges) ){
			cerr << "Unspecified price-ranges file\n";
			exit(1);
		}
		if( supplyMin == -1 || supplyMax == -1){
			cerr << "Unspecified supply min/max \n";
			exit(1);
		}
		if( supplyMin > supplyMax){
			cerr << "Invalid supply range\n";
			exit(1);
		}
		if( numOffers <= 0 ) {
			cerr << "Invalid number of offers\n";
			exit(1);
		}
		if( varMap.count("num-offers")){
			cerr << "Unspecified number of offers\n";
			exit(1);
		}

		cerr << "Generating ... \n";
		//TODO GENERATE
	}
	if(action == "gensolve"){
		if( varMap.count("algorithm")==0 ){
			cerr << "Unspecified algorithm\n";
			exit(1);
		}
		if( algo == "greedy" )
			cerr << "Running greedy\n"; //TODO
		else if( algo == "ga" )
			cerr << "Running ga\n"; // TODO
		else if( algo == "bf" )
			cerr << "Running bf\n"; // TODO
		else {
			cerr << "Invalid algorithm specified\n";
			exit(1);
		}
	}
	if(action == "solve"){
		if( !checkFilename(currencies) ){
			cerr << "Unspecified currencies file\n";
			exit(1);
		}
		if( !checkFilename(purchasables) ){
			cerr << "Unspecified purchasables file\n";
			exit(1);
		}

		if( !checkFilename(supplies) ){
			cerr << "Unspecified supplies file\n";
			exit(1);
		}

		if( !checkFilename(sellers) ){
			cerr << "Unspecified sellers file\n";
			exit(1);
		}
		if( !checkFilename(requests) ){
			cerr << "Unspecified requests file\n";
			exit(1);
		}

		if( varMap.count("algorithm")==0 ){
			cerr << "Unspecified algorithm\n";
			exit(1);
		}
		if( algo == "greedy" )
			cerr << "Running greedy\n"; //TODO
		else if( algo == "ga" )
			cerr << "Running ga\n"; // TODO
		else if( algo == "bf" )
			cerr << "Running bf\n"; // TODO
		else {
			cerr << "Invalid algorithm specified\n";
			exit(1);
		}
	}
}

}

//int main(int ac, char ** av){
//	parseOptions(ac, av);
//	return 0;
//}

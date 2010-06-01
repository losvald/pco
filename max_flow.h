/*
 * max_flow.h
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
 * max_flow.h
 *
 *  Created on: Apr 25, 2010
 *      Author: losvald
 */

#ifndef MAX_FLOW_H_
#define MAX_FLOW_H_

#include <boost/config.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/adjacency_list.hpp>

#include <iostream>
#include <string>
#include <cstring>
#include <vector>
#include <cstdio>
#include <algorithm>


struct MyEdge {
	int a, b, w;
	MyEdge(int a, int b, int w);

	friend std::ostream& operator<<(std::ostream& os, const MyEdge& e);
};


namespace boost {

template <class Graph, class CapacityMap, class ReverseEdgeMap>
	bool construct(Graph& g,
			CapacityMap capacity,
			ReverseEdgeMap reverse_edge,
			typename graph_traits<Graph>::vertex_descriptor& src,
			typename graph_traits<Graph>::vertex_descriptor& sink,
			const std::vector<MyEdge>& _edges, int _src, int _sink);

}

int find_max_flow(const std::vector<MyEdge>& _edges, int _src, int _sink,
		bool print_graph = false);


#endif /* MAX_FLOW_H_ */

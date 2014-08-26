#ifndef ADJACENCY_LIST_GRAH_H
#define ADJACENCY_LIST_GRAH_H

#include <vector>
#include <algorithm>
#include <functional>
#include <queue> 
#include <initializer_list>
#include "GraphNode.h"

namespace ex{
	
	template <class D, class W> class DirectedGraph{

	public:

		using type = D;

		DirectedGraph() = default;
		DirectedGraph(std::initializer_list<GraphNode<D, W>> list){
			for (GraphNode<D, W> &n : list){
				this->push(new GraphNode<D, W>(n));
			}
		}
		DirectedGraph(std::initializer_list<GraphNode<D, W>*> list){
			for (GraphNode<D, W> *n : list){
				this->push(new GraphNode<D, W>(*n));
			}
		}


		~DirectedGraph(){
			for (GraphNode<D, W> *n : nodes)
				delete n;
		}

		/*Pushes a node into this graph, which will have no connections initially.*/
		GraphNode<D, W> *&push(GraphNode<D, W> *node){
			auto nodePos = std::find(nodes.begin(), nodes.end(), node);
			if (nodePos == nodes.end()){
				nodes.push_back(node);
				return nodes.back();
			}
			else
				return *nodePos;

		}

		/*Erase a node from this graph.*/
		void erase(GraphNode<D, W> *node){
			auto nodePos = std::find(nodes.begin(), nodes.end(), node);
			if (nodePos != nodes.end()){
				nodes.erase(nodePos);
				delete node;
			}
		}

		/*Returns the first node that has the given 'data'.*/
		GraphNode<D, W> *getNodeByData(D data){
			auto nodePos = std::find_if(nodes.begin(), nodes.end(), [&](GraphNode<D, W> *n){
				return n->data == data;
			});
			if (nodePos != nodes.end())
				return *nodePos;
			else
				return nullptr;
		}

		void make_bidirectional(GraphNode<D, W> *a, GraphNode<D, W> *b){
			make_directional(a, b, 0);
			make_directional(b, a, 0);
		}
		void make_bidirectional(GraphNode<D, W> *a, GraphNode<D, W> *b, W weightAtoB, W weightBtoA){
			make_directional(a, b, weightAtoB);
			make_directional(b, a, weightBtoA);
		}

		/*Creates a directional weight edge in the graph. If the nodes don't already
		exist, they are created and inserted.*/
		void make_directional(GraphNode<D, W> *from, GraphNode<D, W> *to, W weight = W()){
			this->push(from)->addConnection(this->push(to), weight);
		}

		void resetVisitStatus(){
			for (GraphNode<D, W> *n : nodes){
				n->status = GraphNode<D, W>::Status::Unvisited;
			}
		}

		void bfs_left_first(GraphNode<D, W> *node, std::function<void(GraphNode<D, W>*)> func);
		void bfs_right_first(GraphNode<D, W> *node, std::function<void(GraphNode<D, W>*)> func);
		void bfs_interleaved(GraphNode<D, W> *node, std::function<void(GraphNode<D, W>*)> func, bool leftToRight = true);

		void dfs_pre_order(GraphNode<D, W> *node, std::function<void(GraphNode<D, W>*)> func){
			_dfs_pre_order(node, func);
			resetVisitStatus();
		}
		void dfs_post_order(GraphNode<D, W> *node, std::function<void(GraphNode<D, W>*)> func){
			/*In post order this is necessary because nodes arevisited last, in back-propagation
			order, so this means the node passed as paramter may be stacked twice for printing.*/
			node->status = GraphNode<D, W>::Status::ToBeVisited;
			_dfs_post_order(node, func);
			resetVisitStatus();
		}

		std::list<std::list<GraphNode<D, W>*>> getAllPaths(GraphNode<D, W> *from, GraphNode<D, W> *to);
		std::list<std::list<GraphNode<D, W>*>> getAllPathsRecursive(GraphNode<D, W> *from, GraphNode<D, W> *to){
			std::list<std::list<GraphNode<D, W>*>> allPaths;
			std::list<GraphNode<D, W>*> actualPath;
			actualPath.push_back(from);

			this->_get_all_paths_recursive(actualPath, to, allPaths);

			return allPaths;
		}

		std::list<GraphNode<D, W>*> getShortestPath(GraphNode<D, W> *from, GraphNode<D, W> *to){
		
			std::list<GraphNode<D, W>*> shortestPath;
			if (!from || !to){
				return shortestPath;
			}
			else if (from == to){
				shortestPath.push_back(from);
				return shortestPath;
			}
			
			auto allPaths = getAllPaths(from, to);

			if (allPaths.empty()){
				return shortestPath;
			}

			auto minPos = std::min_element(allPaths.begin(), allPaths.end(), []
				(std::list<GraphNode<D, W>*> &pathA, std::list<GraphNode<D, W>*> &pathB)
			{
				return pathA.size() < pathB.size();
			});

			if (minPos != allPaths.end())
				shortestPath = *minPos;
			
			return shortestPath;
		}

		std::list<std::list<Neighbor<D, W>>> getAllPathsWithWeights(GraphNode<D, W> *from, GraphNode<D, W> *to);

	private:

		void _dfs_pre_order(GraphNode<D, W> *node, std::function<void(GraphNode<D, W>*)> func);
		void _dfs_post_order(GraphNode<D, W> *node, std::function<void(GraphNode<D, W>*)> func);
		void _get_all_paths_recursive(std::list<GraphNode<D, W>*> prevPath, GraphNode<D, W> *to, std::list<std::list<GraphNode<D, W>*>> &allPaths);
		
		/*This is the master list holding all nodes of this graph.*/
		std::list<GraphNode<D, W>*> nodes;

	};

	template <class D, class W> void ex::DirectedGraph<D, W>::bfs_left_first(
		GraphNode<D, W> *node, std::function<void(GraphNode<D, W>*)> func)
	{
		std::queue <GraphNode<D, W>*> que;
		que.push(node);
		while (!que.empty()){
			node = que.front();
			func(node);
			node->status = GraphNode<D, W>::Status::Visited;
			for (Neighbor<D, W> &nei : node->neighbors){
				if (nei.node->status == GraphNode<D, W>::Status::Unvisited){
					que.push(nei.node);
					nei.node->status = GraphNode<D, W>::Status::ToBeVisited;
				}
			}
			que.pop();
		}
		this->resetVisitStatus();
	}
	template <class D, class W> void ex::DirectedGraph<D, W>::bfs_right_first(
		GraphNode<D, W> *node, std::function<void(GraphNode<D, W>*)> func)
	{
		std::queue <GraphNode<D, W>*> que;
		que.push(node);
		while (!que.empty()){
			node = que.front();
			func(node);
			node->status = GraphNode<D, W>::Status::Visited;
			for (auto it = node->neighbors.rbegin(); it != node->neighbors.rend(); ++it){
				if (it->node->status == GraphNode<D, W>::Status::Unvisited){
					que.push(it->node);
					it->node->status = GraphNode<D, W>::Status::ToBeVisited;
				}
			}
			que.pop();
		}
		this->resetVisitStatus();
	}
	template <class D, class W> void ex::DirectedGraph<D, W>::bfs_interleaved(
		GraphNode<D, W> *node, std::function<void(GraphNode<D, W>*)> func, bool leftToRight)
	{
		std::queue <GraphNode<D, W>*> que;
		que.push(node);
		while (!que.empty()){
			node = que.front();
			func(node);
			node->status = GraphNode<D, W>::Status::Visited;

			if (!node->neighbors.empty()){
				auto first = node->neighbors.begin();
				auto last = std::prev(node->neighbors.end());
				bool moveRight = leftToRight;
				/*This will reflect as an interleaved iteration. The boolean parameter indicates whether
				to start at the right or left boundary of the container..*/
				while (first != last){
					if (moveRight){
						if (first->node->status == GraphNode<D, W>::Status::Unvisited){
							que.push(first->node);
							first->node->status = GraphNode<D, W>::Status::ToBeVisited;
							moveRight = false;
						}
						++first;
					}
					else{
						if (last->node->status == GraphNode<D, W>::Status::Unvisited){
							que.push(last->node);
							last->node->status = GraphNode<D, W>::Status::ToBeVisited;
							moveRight = true;
						}
						--last;
					}
				}
				/*After the while loop ends, the midle element in the 'neighbors' list will not
				be added to the queue, so we have to add it, outside the loop.*/
				if (first->node->status == GraphNode<D, W>::Status::Unvisited){
					que.push(first->node);
					first->node->status = GraphNode<D, W>::Status::ToBeVisited;
					moveRight = false;
				}

			}
			que.pop();
		}
		this->resetVisitStatus();
	}

	template <class D, class W> void ex::DirectedGraph<D, W>::_dfs_pre_order(
		GraphNode<D, W> *node, std::function<void(GraphNode<D, W>*)> func){

		/*In theory this 'if' is not necessary, but we use it as a safety check, anyway.*/
		if (!node || node->status == GraphNode<D, W>::Status::Visited)
			return;

		func(node);
		node->status = GraphNode<D, W>::Status::Visited;

		for (Neighbor<D, W> &nei : node->neighbors){
			if (nei.node->status != GraphNode<D, W>::Status::Visited)
				_dfs_pre_order(nei.node, func);
		}
	}
	template <class D, class W> void ex::DirectedGraph<D, W>::_dfs_post_order(
		GraphNode<D, W> *node, std::function<void(GraphNode<D, W>*)> func){

		/*In theory this 'if' is not necessary, but we use it as a safety check, anyway.*/
		if (!node || node->status == GraphNode<D, W>::Status::Visited)
			return;

		for (Neighbor<D, W> &nei : node->neighbors){
			if (nei.node->status == GraphNode<D, W>::Status::Unvisited){
				nei.node->status = GraphNode<D, W>::Status::ToBeVisited;
				_dfs_post_order(nei.node, func);
			}
		}

		func(node);
		node->status = GraphNode<D, W>::Status::Visited;
	}
	
	template <class D, class W> std::list<std::list<GraphNode<D, W>*>> ex::DirectedGraph<D, W>::getAllPaths(GraphNode<D, W> *from, GraphNode<D, W> *to)
	{		
		std::list<std::list<GraphNode<D, W>*>> allPaths;
		std::list<std::list<GraphNode<D, W>*>> pathQue;
		
		pathQue.push_back(std::list<GraphNode<D, W>*>());
		pathQue.front().push_back(from);

		while (!pathQue.empty()){
			std::list<GraphNode<D, W>*> actualPath = pathQue.front();

			GraphNode<D, W> *n = actualPath.back();
			if (n == to){
				allPaths.push_back(actualPath);
			}

			for (Neighbor<D, W> &nei : n->neighbors){
				/*If true, means this node does not yet exists on the current path, so we add it.*/
				if (std::find(actualPath.begin(), actualPath.end(), nei.node) == actualPath.end()){
					std::list<GraphNode<D, W>*> tempPath(actualPath);
					tempPath.push_back(nei.node);
					
					/*This if check if this path is duplicate, which could happen, at this point, if
					a pair of have two identical connections. This is common thing to happen if the 
					graph is weighted, and two identical connections receive the same weight, for some reason.*/
					if(std::find(pathQue.begin(), pathQue.end(), tempPath) == pathQue.end())
						pathQue.push_back(tempPath);
				}
			}
			pathQue.pop_front();
		}
		return allPaths;
	}

	template <class D, class W> void ex::DirectedGraph<D, W>::_get_all_paths_recursive(
		std::list<GraphNode<D, W>*> myPath, GraphNode<D, W> *to, std::list<std::list<GraphNode<D, W>*>> &allPaths)
	{
		
		GraphNode<D, W> *thisNode = myPath.back();

		/*Fist call will come with root node inside 'myPath' already. Therefore, in 
		future calls, the 'myPath' list will already hold this currend node.*/
		if (thisNode == to){
			allPaths.push_back(myPath);
		}
		for (Neighbor<D, W> &nei : thisNode->neighbors){
			if (std::find(myPath.begin(), myPath.end(), nei.node) == myPath.end()){
				myPath.push_back(nei.node);
				
				/*This if check if this path is duplicate, which could happen, at this point, if
				a pair of have two identical connections. This is common thing to happen if the
				graph is weighted, and two identical connections receive the same weight, for some reason.*/
				if (std::find(allPaths.begin(), allPaths.end(), myPath) == allPaths.end())
					_get_all_paths_recursive(myPath, to, allPaths);

				myPath.pop_back();
			}
		}
	}

	template <class D, class W> std::list<std::list<Neighbor<D, W>>> ex::DirectedGraph<D, W>::getAllPathsWithWeights(GraphNode<D, W> *from, GraphNode<D, W> *to){
		
		std::list<std::list<Neighbor<D, W>>> allPaths;
		std::queue<std::list<Neighbor<D, W>>> pathQue;

		pathQue.push(std::list<Neighbor<D, W>>());
		pathQue.front().push_back(Neighbor<D, W>(from,0));

		while (!pathQue.empty()){
			std::list<Neighbor<D, W>> actualPath = pathQue.front();

			Neighbor<D, W> n = actualPath.back();
			if (n.node == to){
				allPaths.push_back(actualPath);
			}

			for (Neighbor<D, W> &nei : n.node->neighbors){
				/*If true, means this node does not yet exists on the current path, so we add it.*/
				if (std::find(actualPath.begin(), actualPath.end(), nei) == actualPath.end()){
					std::list<Neighbor<D, W>> tempPath(actualPath);
					tempPath.push_back(nei);
					pathQue.push(tempPath);
				}
			}
			pathQue.pop();
		}
		return allPaths;
	}

};




#endif
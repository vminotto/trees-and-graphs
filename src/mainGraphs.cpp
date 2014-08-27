#include <vector>
#include <numeric>
#include <map>
#include <iostream>
#include <opencv.hpp>
#include "DirectedGraph.h"
#include "MatrixGraph.h"


using uchar = unsigned char;
using uint = unsigned int;
using namespace std;

std::uint64_t my_coin_sum(std::vector<int> &coins, int s, int e){
	
	if (s > e)
		return 0;
	if (s == e)
		return coins[s];

	std::uint64_t begPick = coins[s] + std::min(my_coin_sum(coins, s + 1, e - 1), my_coin_sum(coins, s + 2, e));
	std::uint64_t endPick = coins[e] + std::min(my_coin_sum(coins, s + 1, e - 1), my_coin_sum(coins, s, e - 2));

	return std::max(begPick, endPick);
}

int main(){

	std::vector<int> coins = { 1,3,1};
	std::uint64_t myBestSum = my_coin_sum(coins, 0, coins.size() - 1);
	std::uint64_t opoBestSum = std::accumulate(coins.begin(), coins.end(), 0) - myBestSum;
	
	ex::MatrixGraph<double> mGraph;

	ex::DirectedGraph<char, int> gra;
	std::map<char, ex::GraphNode<char, int>*> aNodes;
	for (char c = 'A'; c < 'A' + 14; ++c)
		aNodes[c] = new ex::GraphNode<char, int>(c);

	gra.make_directional(aNodes['A'], aNodes['G'], 0);
	gra.make_directional(aNodes['A'], aNodes['H'], 0);
	gra.make_directional(aNodes['A'], aNodes['D'], 0);
	gra.make_directional(aNodes['A'], aNodes['B'], 0);
	gra.make_directional(aNodes['A'], aNodes['F'], 0);
	gra.make_directional(aNodes['B'], aNodes['A'], 0);
	gra.make_directional(aNodes['B'], aNodes['C'], 0);
	gra.make_directional(aNodes['B'], aNodes['D'], 0);
	gra.make_directional(aNodes['C'], aNodes['F'], 0);
	gra.make_directional(aNodes['C'], aNodes['F'], 1);
	gra.make_directional(aNodes['C'], aNodes['M'], 0);
	gra.make_directional(aNodes['C'], aNodes['N'], 0);
	gra.make_directional(aNodes['D'], aNodes['I'], 0);
	gra.make_directional(aNodes['E'], aNodes['I'], 0);
	gra.make_directional(aNodes['E'], aNodes['M'], 0);
	gra.make_directional(aNodes['F'], aNodes['B'], 0);
	gra.make_directional(aNodes['F'], aNodes['L'], 0);
	gra.make_directional(aNodes['G'], aNodes['A'], 0);
	gra.make_directional(aNodes['H'], aNodes['I'], 0);
	gra.make_directional(aNodes['H'], aNodes['M'], 0);
	gra.make_directional(aNodes['I'], aNodes['B'], 0);
	gra.make_directional(aNodes['I'], aNodes['E'], 0);
	gra.make_directional(aNodes['J'], aNodes['K'], 0);
	gra.make_directional(aNodes['J'], aNodes['N'], 0);
	gra.make_directional(aNodes['K'], aNodes['N'], 0);
	gra.make_directional(aNodes['L'], aNodes['J'], 0);
	gra.make_directional(aNodes['L'], aNodes['N'], 0);
	gra.make_directional(aNodes['M'], aNodes['B'], 0);
	gra.make_directional(aNodes['N'], aNodes['C'], 0);

	std::function<void(ex::GraphNode<char, int>*)> printNode = [](ex::GraphNode<char, int> *n){
		cout << n->data << " ";
	};

	std::cout << std::endl;
	gra.bfs_left_first(gra.getNodeByData('A'), printNode);
	std::cout << std::endl;
	gra.bfs_right_first(gra.getNodeByData('A'), printNode);
	std::cout << std::endl;
	gra.bfs_interleaved(gra.getNodeByData('A'), printNode);
	std::cout << std::endl;
	gra.dfs_pre_order(gra.getNodeByData('A'), printNode);
	std::cout << std::endl;
	gra.dfs_post_order(gra.getNodeByData('A'), printNode);

	char from = 'C', to = 'N';
	auto allPaths     = gra.getAllPaths(gra.getNodeByData(from), gra.getNodeByData(to));
	std::cout << endl << endl << "All paths from " << from << " to " << to << ":" << endl;
	for (auto &path : allPaths)
	{
		for (ex::GraphNode<char,int> *&n : path){
			std::cout << "'" << *n << "' ";
		}
		std::cout << std::endl;
	}

	auto allPathsRec = gra.getAllPathsRecursive(gra.getNodeByData(from), gra.getNodeByData(to));
	std::cout << endl << endl << "All paths (recursive approach) from " << from << " to " << to << ":" << endl;
	for (auto &path : allPathsRec)
	{
		for (ex::GraphNode<char, int> *&n : path){
			std::cout << "'" << *n << "' ";
		}
		std::cout << std::endl;
	}

	auto shortestPath = gra.getShortestPath(gra.getNodeByData(from), gra.getNodeByData(to));
	std::cout << endl << "Shortest path from " << from << " to " << to << ":" << endl;
	for (ex::GraphNode<char, int> *&n : shortestPath){
		std::cout << "'"<<*n << "' ";
	}
	std::cout << std::endl;

	auto allPathsWithWeights = gra.getAllPathsWithWeights(gra.getNodeByData(from), gra.getNodeByData(to));
	std::cout << endl << endl << "All paths (with weights) from " << from << " to " << to << ":" << endl;
	for (auto &path : allPathsWithWeights)
	{
		for (ex::Neighbor<char,int> &nei : path){
			std::cout << nei.weight << " '" << nei.node->data<< "' ";
		}
		std::cout << std::endl;
	}

	ex::GraphNode<char, int> *aRoot = gra.getNodeByData('A');
	ex::GraphNode<char, int> *dst = ex::find_node_dfs<char,int>(aRoot, 'E');

	cin.get();
	return 0;
}
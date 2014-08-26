#include <vector>
#include <numeric>
#include <map>
#include <iostream>

#include "DirectedGraph.h"


using uchar = unsigned char;
using uint = unsigned int;
using namespace std;

int main(){
	
	/*Fun Commentary Added.*/

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
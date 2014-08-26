#include <list>

namespace ex{

	/*Forward declaration*/
	template <class D, class W> struct Neighbor;

	/*This represents a vertex*/
	template <class D, class W> class GraphNode{

	public:

		/*ToBeVisited means the node has already been queued for the visit.*/
		enum Status{ Visited, Unvisited, ToBeVisited };

		GraphNode(D data) : data(data) {}
		~GraphNode(){
			for (GraphNode<D, W> *par : parents){
				par->removeConnection(this);
			}
			for (Neighbor<D, W> &nei : neighbors){
				this->removeConnection(nei);
			}
		}
		
		/*Visiting status of this node. Used for traversal algorithms.*/
		Status status = Status::Unvisited;

		/*Soem data this node element will hold*/
		D data;

		void addConnection(GraphNode<D, W> *node, W weight = W()){
			addConnection(Neighbor<D, W>(node, weight));
		}
		void addConnection(Neighbor<D, W> neighbor){
			/*First we test for duplicate elements. If it is not a duplicate, we add it to the list.
			Note that we consider both the node and weight for checking it. This means there may be
			two or more connections from A to B, if they have different weights.*/
			if (std::find(neighbors.begin(), neighbors.end(), neighbor) == neighbors.end())
			{
				neighbors.push_back(neighbor);
				neighbor.node->parents.push_back(this);
			}
		}

		/*Removes all neighbors composed by the node passed as parameter. In other words
		if there are three diferent neighbors containing this same 'node', but with 
		different weights all three are removed anyway.*/
		void removeConnection(GraphNode<D, W> *node){

			auto pos = std::remove_if(neighbors.begin(), neighbors.end(), [&](Neighbor<D, W> &nei){
				return nei.node == node;
			});

			for (auto it = pos; it != neighbors.end(); ++it){
				it->node->parents.remove(this);
			}
			neighbors.erase(pos, neighbors.end());
		}

		/*Removes the exact neighbor of this node, that is, we use both the
		node pointer and the weigth to find the Neighbor<D, W> and remove it.*/
		void removeConnection(const Neighbor<D, W> &neighbor){
			auto nodePos = std::find(neighbors.begin(), neighbors.end(), neighbor);
			/*If node exists as a connection*/
			if (nodePos != neighbors.end()){
				neighbor.node->parents.remove(this);
				neighbors.erase(nodePos);
			}
		}

		/*Nodes that this one is connected to. Since this actually represents
		a connect (an edge), we use the class 'Neighbor' which encapsulates
		a pointer to a 'GraphNode' as well as the weight of the edge.*/
		std::list<Neighbor<D, W>> neighbors;

		/*Nodes that connect to this one.*/
		std::list<GraphNode<D, W>*> parents;
	};

	template <class D, class W> std::ostream &operator<<(std::ostream &out, const GraphNode<D, W> *&node){
		out << node->data;
		return out;
	}
	template <class D, class W> std::ostream &operator<<(std::ostream &out, const GraphNode<D, W> &node){
		out << node.data;
		return out;
	}

	/*This tests equivalency, instead of identity.*/
	template <class D, class W> bool operator==(const GraphNode<D, W> &lhs, const GraphNode<D, W> &rhs){
		return lhs.data == rhs.data &&
			lhs.status == rhs.status &&
			lhs.parents == rhs.parents &&
			lhs.neighbors == rhs.neighbors;
	}

	/*This function may also be used to check if there exists a path between two nodes. If the return
	from the first call is a nullptr, there is not a path.*/
	template <class D, class W> GraphNode<D, W> *find_node_dfs(GraphNode<D, W> *root, D data){

		if (!root)
			return nullptr;

		root->status = GraphNode<D, W>::Status::Visited;

		/*If node is found, no need to keep searching deeper, so we return it.*/
		if (root->data == data){
			return root;
		}
		else{
			for (Neighbor<D, W> &nei : root->neighbors){
				if (nei.node->status != GraphNode<D, W>::Status::Visited){
					GraphNode<D, W> *n = find_node_dfs(nei.node, data);
					/*If we found it, we break the for, and stop searching*/
					if (n && n->data == data){
						return n;
					}
				}
			}
			/*If the above 'for' was left, this means the node was not found, so we return nullptr*/
			return nullptr;
		}
	}


	/*This represents an edge*/
	template <class D, class W> struct Neighbor{
		Neighbor<D, W>(GraphNode<D, W> *node, W weight) : node(node), weight(weight){}
		GraphNode<D, W> *node = nullptr;
		W weight;
	};
	template <class D, class W> bool operator==(const Neighbor<D, W> &lhs, const Neighbor<D, W> &rhs){
		return lhs.node == rhs.node && lhs.weight == rhs.weight;
	}
};
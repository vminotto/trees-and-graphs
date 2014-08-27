#include <opencv.hpp>
#include <cstdint>

namespace ex{

	template <class T> void expandSquarely(cv::Mat_<T> &in, size_t newSize){
		if (in.rows != in.cols) //Initial matrix is not square.
			return;
		if (in.rows > newSize)  //Matrix is larger than the requested epxansion size.
			return;

		cv::Mat_<T> tmp = in.clone();
		in = cv::Mat_<T>::zeros(newSize, newSize);			
		tmp.copyTo(in(cv::Range(0, tmp.rows), cv::Range(0, tmp.cols)));
	}

	template <class T> void shrinkSquarely(cv::Mat_<T> &in, std::list<int> &nodesToRemove){
		
		if (in.rows != in.cols) //Initial matrix is not square.
			return;

		int size = in.rows;
		nodesToRemove.remove_if([&](T &v){return v >= in.rows; });

	
		if (nodesToRemove.empty())
			return;

		int newSize = size - nodesToRemove.size();
		if (newSize == 0){
			in.create(0, 0);
			return;
		}

		cv::Mat_<std::uint8_t> mask = cv::Mat_<std::uint8_t>::ones(in.size());
		for (int i : nodesToRemove){
			mask.col(i) = 0;
			mask.row(i) = 0;
		}
	
		cv::Mat_<T> tmp = in.clone();
		in.create(newSize, newSize);
		
		for (int rMask = 0, rIn = 0; rMask < mask.rows; ++rMask){
			bool incrementRow = false;
			for (int cMask = 0, cIn = 0; cMask < mask.cols; ++cMask){
				if (mask(rMask, cMask)){
					in(rIn, cIn) = tmp(rMask, cMask);
					++cIn;
					incrementRow = true;
				}
			}
			rIn += incrementRow;
		}

	}

	/*If true, indexes in 'nodes' will be removed, if false, the remaining ones will be removed.*/
	template <class T> cv::Mat_<T> getCloneOfShrink(cv::Mat_<T> &in, std::list<int> &nodes, bool remove){
		
		std::list<int> nodesToRemove;
		if (remove)
			nodesToRemove.assign(nodes.begin(), nodes.end());
		else{
			std::iota(nodesToRemove.begin(), nodesToRemove.end(), 0);
			nodesToRemove.remove_if([&](int val){ 
				return std::find(nodes.begin(), nodes.end(), val) != nodes.end(); 
			});
		}		
		cv::Mat_<T> out = in.clone();
		shrinkSquarely(out, nodesToRemove);
		return out;
	}

	template <class W> class MatrixGraph{

	public:
		MatrixGraph(){
			
		}
		~MatrixGraph(){}

		void make_directional(int from, int to, const W &weight){
			ex::expandSquarely(matrix, std::max(from, to));
			mat(from, to) = weight;
		}
		void make_bidirectional(int indNodeA, int indNodeB, const W &symmetricWeight){
			make_directional(indNodeA, indNodeB, symmetricWeight);
			make_directional(indNodeB, indNodeA, symmetricWeight);
		}

		void print(){
			cout << mat << endl;
		}

	private:

		/*Represents the nodes and */
		cv::Mat_<W> mat;

	};

	template <class T> T getRowLowerBound(cv::Mat_<T> mat, int start, int end){
		double sum = 0;
		for (int i = start; i <= end; ++i){
			double min;
			cv::minMaxIdx(mat, min, nullptr);
			sum += min;
		}
		return static_cast<T>(sum);
	}

	/*Set all elements in the main diagonal to the maximum possible value.*/
	template <class T> void maximizeDiagonal(cv::Mat_<T> &mat){
		cv::Mat_<T> eye = cv::Mat_<T>::eye(mat.size()), invEye(mat.size());
		cv::bitwise_not(eye, invEye);
		mat = mat * invEye;
		mat = mat + (eye*std::numeric_limits<T>::max());
	}

	template <class T> void tspBranchAndBound(cv::Mat_<T> mat, int startNode){
		maximizeDiagonal(mat);
		

	}

	//to solve this, make extra struct and use iterative solution with while.
	//http ://stackoverflow.com/questions/11498035/c-implementation-of-knapsack-branch-and-bound
	template <class T> void tspBranchAndBound(cv::Mat_<T> mat, std::list<int> nextNodes){
		

		std::list<T> lowerBounds = 
		for (int n : nextNodes){
			cv::Mat_<T> subMat = ex::getCloneOfShrink(mat, false);
			T lowerBound = ex::getRowLowerBound(subMat);
		}


	}

};
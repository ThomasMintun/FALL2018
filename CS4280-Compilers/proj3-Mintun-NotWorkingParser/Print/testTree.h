#ifndef INTBINARYTREE_H
#define INTBINARYTREE_H
#include "node.h"

class IntBinaryTree{
	//private member functions
	node* insert(node *nodePtr, std::string word);

	public:
		node *root; //pointer to the root node

		void displayPreOrder(node *nodePtr, std::ofstream &outFile);
		void displayPreOrder(node *nodePtr, std::string filename);
};

// 		void displayInOrder(node *, std::string);//change to traverseInorder...
// 		void displayInOrder(node *nodePtr, std::ofstream &outFile);

// 		// void displayPostOrder(node *nodePtr, std::ofstream &outFile);
// 		// void displayPostOrder(node *nodePtr, std::string filename);

// 		node* buildTree(std::ifstream&);
// 		void insert(std::string word);
// };
#endif
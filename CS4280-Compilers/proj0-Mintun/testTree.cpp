#include "testTree.h"
#include <fstream>
#include <string>
#include <set>
#include <iostream>
using namespace std;

node* IntBinaryTree::insert(node *nodePtr, string word){
    if (nodePtr == nullptr)
        return new node(word);//declairing new node for word of unique word length
    else if (nodePtr->value > word.length()){
        nodePtr->left = insert(nodePtr->left, word);//this only has root has nothing else
        cout << "Inserted " << word;
    }
    else if (nodePtr->value < word.length()){
        nodePtr->right = insert(nodePtr->right, word);
        cout << "Inserted " << word;
    }
    else if (nodePtr->value == word.length()){
        nodePtr->set.insert(word);//using set so use dot operator. Adding to set of words of same length.
        cout << "Inserted " << word;
    }
    return nodePtr;
}

void IntBinaryTree::insert(string word){
    root = insert(root, word);//calling other insert function which will insert on itself calling root of the tree and word being passed. 
}

node* IntBinaryTree::buildTree(std::ifstream& inFile){
    string word;//helper var to hold strings

    while(inFile >> word){
        this->insert(word);//grab each word and call insert function on each word
    }
    return root; 
}

void IntBinaryTree::displayInOrder(node *nodePtr, std::ofstream &outFile){
		if (nodePtr == NULL){
			return;
		}

        displayInOrder(nodePtr->left, outFile);
        for (auto i = nodePtr->set.begin(); i != nodePtr->set.end(); i++){
        	outFile << *i << ", ";
        }
        outFile << endl;
        displayInOrder(nodePtr->right, outFile);
}

void IntBinaryTree::displayInOrder(node *nodePtr, string filename){
	filename += ".Inorder";

	std::ofstream outInorder;
    outInorder.open(filename, std::ios::out);

    displayInOrder(nodePtr, outInorder);
}

void IntBinaryTree::displayPreOrder(node *nodePtr, std::ofstream &outFile){
    if (nodePtr == NULL){
		return;
	}

    for (auto i = nodePtr->set.begin(); i != nodePtr->set.end(); i++){
        	outFile << *i << ", ";
        }
        outFile << endl;
    displayPreOrder(nodePtr->left, outFile);
    displayPreOrder(nodePtr->right, outFile);
    
}

void IntBinaryTree::displayPreOrder(node *nodePtr, string filename){
	filename += ".Preorder";

	std::ofstream outPreorder;
    outPreorder.open(filename, std::ios::out);

    displayPreOrder(nodePtr, outPreorder);
}

void IntBinaryTree::displayPostOrder(node *nodePtr, std::ofstream &outFile){
    if (nodePtr == NULL){
		return;
	}

        displayPostOrder(nodePtr->left, outFile);
        displayPostOrder(nodePtr->right, outFile);
        for (auto i = nodePtr->set.begin(); i != nodePtr->set.end(); i++){
        	outFile << *i << ", ";
        }
        outFile << endl;  
}

void IntBinaryTree::displayPostOrder(node *nodePtr, string filename){
	filename += ".Postorder";

	std::ofstream outPostorder;
    outPostorder.open(filename, std::ios::out);

    displayPostOrder(nodePtr, outPostorder);
}
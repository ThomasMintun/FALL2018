#ifndef NODE_H
#define NODE_H
#include <set>
#include <string>

struct node{
	//insert set
	std::set <std::string>set;

	int value; //The value of the node... #letters in string
	node *left; //pointer to left child
	node *right; //pointer to right child

	node(std::string word){
		this->value = word.length();//caluclate string length... node.value
		this->left = nullptr;
		this->right = nullptr;
		set.insert(word);//insert current word into the set
		}//this constructor is only called when adding a new leaf node. when adding another word to the tree not necessarily adding a new node because words could be same length and on same node.
	};

#endif
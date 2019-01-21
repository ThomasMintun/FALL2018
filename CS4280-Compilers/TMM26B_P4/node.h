#ifndef NODE_H
#define NODE_H
#include <set>
#include <string>
#include "token.h"

struct Node{
	//insert set
	std::string name;//name of the node
	int depth;//tree depth the node is at

	token_t token_1;
	token_t token_2;

	Node* child_1 = nullptr;
	Node* child_2 = nullptr;
	Node* child_3 = nullptr;
	Node* child_4 = nullptr;

	Node (){
		this->name = "";
		this->depth = 0;
		this->token_1 = token_t();
		this->token_2 = token_t();
		this->child_1 = nullptr;
		this->child_2 = nullptr;
		this->child_3 = nullptr;
		this->child_4 = nullptr;
	}

	Node(std::string name, int depth){
		this->name = name;
		this->depth = depth;
		this->token_1 = token_t();
		this->token_2 = token_t();
		this->child_1 = nullptr;
		this->child_2 = nullptr;
		this->child_3 = nullptr;
		this->child_4 = nullptr;
	}
};

#endif
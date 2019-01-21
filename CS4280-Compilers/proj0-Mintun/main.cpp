#include <iostream>
#include "tree.h"
#include <stdio.h>
#include <fstream>
#include <string>
#include <set>
using namespace std;

int main(int argc, const char * argv[]){
    IntBinaryTree tree;
    node *nodePtr = nullptr;//THIS WAS WHAT WAS CAUSING SEGFAULT

    std::ofstream outFile;
    outFile.open("out.txt", std::ios::trunc);
    std::string word = "";
    
    //node *root = NULL;
    
    if (argc > 2){
        cout << "Error:: Too many command line arguments. This program accepts ";
        cout << "a filename to read from, or just the program name P0 ";
        cout << "which then takes input from the keyboard. \n\n";
        return 0;
    }
    if (argc == 2){
        ifstream inFile;
        string fileName = argv[1];
        inFile.open(fileName + ".fs18");
        if (inFile.fail()){
            cout << "Error:: The input file did not open.\n\n";
            return 0;
        }
        tree.root = tree.buildTree(inFile);
        tree.displayInOrder(tree.root, fileName);
        tree.displayPreOrder(tree.root, fileName);
        tree.displayPostOrder(tree.root, fileName);
    }
    else {
        cout << "Keyboard option selected. Type in words to insert into the tree: \n";
        while(std::cin >> word){
            outFile << word << "\n";
            }
        outFile.close();
        if (outFile.fail()){
            cout << "Error:: ";
        }
        else{
            std::ifstream inFile;
            inFile.open("out.txt", std::ios::in);
            tree.root = tree.buildTree(inFile);
            tree.displayInOrder(tree.root, "out");
            tree.displayPreOrder(tree.root, "out");
            tree.displayPostOrder(tree.root, "out");
            inFile.close();
        }
    }
    return 0;
}
#include <iostream>
#include <fstream>
#include <string>
#include "parser.h"
using namespace std;

int main(int argc, char * argv[]){
    Node *nodePtr = nullptr;

	  std::ofstream outFile;
    outFile.open("out.txt", std::ios::trunc);
    std::string word = "";

	if (argc > 2){
		cout << "Error:: Too many command line arguments. This program accepts ";
        cout << "a filename to read from, or just the program name P0 ";
        cout << "which then takes input from the keyboard. \n\n";
        return 0;
	}
	
	if(argc == 2){//file option
      string fileName = argv[1];
      inFile.open(fileName + ".fs18");
      if (inFile.fail()){
      	cout << "Error:: The input file did not open.\n";
        return 0;
      }
      nodePtr = parser();
      inFile.close();
	}
	
	else{
		cout << "Keyboard option selected. Type in words to insert into the file: \n";
        while(std::cin >> word){
            outFile << word << "\n";
            }
        outFile.close();
        if (outFile.fail()){
            cout << "Error:: the file failed to open";
        }
        else{
          std::ifstream inFile;
          inFile.open("out.txt", std::ios::in);
          //testScanner(inFile); OLD function call
          nodePtr = parser();//call parser to build tree
          //tree.displayPreOrder(tree.root, fileName);//print tree
          inFile.close();
        }

  return 0;
  }
}//end main
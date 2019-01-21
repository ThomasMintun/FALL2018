#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include "parser.h"
#include "semantics.h"
using namespace std;

std::ofstream out_file;
std::ofstream tempFile;

int main(int argc, char* argv[]){
  Node* root = nullptr;

  tempFile.open("out.txt", std::ios::trunc);
  std::string word = "";
  //stackInit();//initiialize stack SOMETHING IN HERE CAUSING SEGFAULT
  tempArrInit();//temp var array initialization

	if (argc > 2){
		cout << "Error:: Too many command line arguments. This program accepts ";
        cout << "a filename to read from, or just the program name P0 ";
        cout << "which then takes input from the keyboard. \n\n";
        return 0;
	}
	
	if(argc == 2){//file option
      string fileName = argv[1];
      inFile.open(fileName + ".fs18");

      std::string filename = argv[1];
      filename += ".asm";
      out_file.open(filename, std::ios::trunc);

      if (inFile.fail()){
      	cout << "Error:: The input file did not open.\n";
        return 0;
      }

      root = parser();//call parser to build tree
      printTree(root);
      semanticCheck(root, 0);
      inFile.close();
      out_file.close();
	}
	
	else{//keyboard option
    std::string filename = "out.asm";
    //out_file.open(filename, std::ios::trunc);

		cout << "Keyboard option selected. Type in words to insert into the file: \n";
        while(std::cin >> word){
            tempFile << word << "\n";
            }
        tempFile.close();
        if (tempFile.fail()){
            cout << "Error:: the file failed to open";
        }
        else{
          out_file.open("out.asm", std::ios::trunc);

          root = parser();//call parser to build tree
          printTree(root);
          semanticCheck(root, 0);
          inFile.close();
        }
  return 0;
  }
}//end main
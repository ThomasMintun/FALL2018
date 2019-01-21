#include <iostream>
#include <fstream>
#include <string>
#include "testScanner.h"
using namespace std;

int main(int argc, char * argv[]){
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
	    ifstream inFile;
      string fileName = argv[1];
      inFile.open(fileName + ".fs18");
      if (inFile.fail()){
      	cout << "Error:: The input file did not open.\n";
        return 0;
      }
      testScanner(inFile);//function call
	}
	
	else{
		cout << "Keyboard option selected. Type in words to insert into the file: \n";
        while(std::cin >> word){
            outFile << word << "\n";
            }
        outFile.close();
        if (outFile.fail()){
            cout << "Error:: the file failed";
        }
        else{
          std::ifstream inFile;
          inFile.open("out.txt", std::ios::in);
          testScanner(inFile);
          inFile.close();
        }

  return 0;
  }
}//end main
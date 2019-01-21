#include "semantics.h"
#include <fstream>

std::ofstream out_file;

int main(int argc, char** argv)
{
    if(argc == 1)
    {
        out_file.open("out.asm", std::ios::trunc);
        writing_to_global_file("out.asm");
    }
    else if(argc == 2)
    {
        std::string filename = argv[1];
        filename += ".asm";
        out_file.open(filename, std::ios::trunc);
        writing_to_global_file(filename);
    }
}
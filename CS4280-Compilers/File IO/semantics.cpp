#include "semantics.h"
#include <string>

void writing_to_global_file(std::string any_var_you_want)
{
    out_file << "Hello World\n";
    out_file << "Passed argument: " << any_var_you_want << "\n";
}
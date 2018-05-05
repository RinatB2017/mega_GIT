#include "../../file_operation.h"


int main()
{
    std::cout << "Hello World!" << std::endl;
    iv::FileIO charFile;
    char *buffer = NULL;
    std::string file("../test_file_operation/test_file_operation.pro");
    size_t fileSize;
    charFile.readCharFile(file, buffer, fileSize);
    std::cout << buffer <<std::endl;
    return 0;
}


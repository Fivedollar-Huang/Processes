#include "handleFile.hpp"

std::ofstream f;

void initOutputFile()
{
    f.open("output.txt");
}

void put(int num)
{
    f << num << ' ';
}

void newline(){
    f << std::endl;
}

void closeOutputFile()
{
    f.close(); 
}

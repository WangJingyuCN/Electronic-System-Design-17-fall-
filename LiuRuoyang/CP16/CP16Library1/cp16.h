#pragma once
#include<iostream>
//#include<vector>
#include <cliext/vector>   
#include<string>
#include<fstream>

cliext::vector<float> cp16(std::string inputString);

int getOffset(std::string);
void Hex2Matrix(int *, int *, int);
#pragma once
#include<iostream>
//#include<vector>
#include <cliext/vector>   
#include<string>
#include<fstream>

int getOffset(std::string);
void Hex2Matrix(int *, int *, int);
std::string dbs_to_qbs(std::string);
cliext::vector<float> cp16(std::string inputString);
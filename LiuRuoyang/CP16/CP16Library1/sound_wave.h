#pragma once
//#include <vector>
#include <cliext/vector>   

cliext::vector<float> getSoundWave(int* dotarray, int letter_num);

void getHammingWindow(float *hamming, int N);
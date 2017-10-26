#include "sound_wave.h"
#include <math.h>
using namespace std;

const double PI = 3.1415926535;

void getHammingWindow(float *hamming, int N)
{
	for (int i = 0; i < N; ++i)
	{
		hamming[i] = 0.55836 - 0.46164*cos(2.0 * PI * i / (N - 1));
	}
}

cliext::vector<float> getSoundWave(int* dotarray, int letter_num)
{
	const int fs = 8000;
	const float freq_low = 1400;
	const float freq_interval = 19;
	const int frames = 1000;
	const int dot_height = 16;
	const int dot_width = 16;

	float hamming[frames];
	getHammingWindow(hamming, frames);

	float wave_hamming[dot_width][frames];
	for (int i = 0; i < dot_width; ++i)
	{
		float freq = freq_low + freq_interval * i;
		for (int j = 0; j < frames; ++j)
		{
			wave_hamming[i][j] = sin(2 * PI * freq * j / fs) * hamming[j];
		}
	}

	cliext::vector<float> wave_out;
	wave_out.reserve(frames * letter_num * dot_height);
	double max = 0;
	for (int i = letter_num * dot_width * dot_height - dot_width; i >=0 ; i -= dot_width)
	{
		for (int j = 0; j < frames; ++j)
		{
			float sum = 0;
			for (int k = 0; k < dot_width; ++k)
			{
				sum += dotarray[i + k] * wave_hamming[k][j];
			}
			if (max < sum)
			{
				max = sum;
			}
			wave_out.push_back(sum);
		}
	}

	for (int i = 0; i < wave_out.size(); ++i)
	{
		wave_out[i] = wave_out[i] / max;
	}

	return wave_out;
}
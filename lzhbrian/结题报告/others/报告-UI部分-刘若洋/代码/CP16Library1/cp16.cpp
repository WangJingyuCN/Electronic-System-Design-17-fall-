#define _CRT_SECURE_NO_WARNINGS

#include"cp16.h"
#include"sound_wave.h"

cliext::vector<float> cp16(std::string inputString) {
	int len1 = inputString.length();
	std::string convertedString = dbs_to_qbs(inputString);
	//std::cout << (int)(unsigned char)convertedString[0] << std::endl;
	int len = convertedString.length();
	//std::cout << len << std::endl;
	len = len / 2;
	std::ifstream fin;
	fin.open("hzk16s", std::ios::binary | std::ios::in);
	if (!fin)
	{
		throw gcnew System::Exception("Error open hzk16s");
	}
	//	std::vector<float> CSOUND;
	int * CMatrix = new int[256 * len];
	memset(CMatrix, 0, sizeof(int) * 256 * len);
	for (int i = 0; i < len; i++) {
		int offset = getOffset(convertedString.substr(i * 2, 2));
		fin.seekg(offset, std::ios::beg);
		unsigned char buf[33];
		fin.read((char *)buf, 32 * sizeof(char));

		int CHex[32];
		for (int k = 0; k<32; k++)
			CHex[k] = (int)buf[k];

		Hex2Matrix(CMatrix, CHex, i);
	}
	fin.close();

	cliext::vector<float> CSOUND = getSoundWave(CMatrix, len);

	delete[] CMatrix;
	return CSOUND;
}

/*int main()
{
	//	std::ifstream fin;
	//	fin.open("hzk16s",std::ios::binary|std::ios::in);
	//	fin.seekg(62400, std::ios::beg);
	//	unsigned char a[33];
	//	std::cout << sizeof(char) << std::endl;
	//	fin.read((char*)a, 32);
	//	std::cout << (int)a[7] << std::endl;

	//	std::string test = "a";
	//	std::cout << (int)(unsigned char)test[0] << std::endl;

	std::string a = "\xb5\xe7\xd7\xd3!";
	std::vector<float> result = cp16(a);
	std::ofstream fout("test.txt");
	for (int i = 0; i<result.size(); i++)
		fout << result[i] << std::endl;
	return 0;
}*/

std::string dbs_to_qbs(std::string inputString)
{
	std::string convertedString;
	for (int i = 0; i < inputString.length(); i++) {
		//std::cout << (int)(unsigned char)inputString[i] << std::endl;
		if ((int)(unsigned char)inputString[i] < 128 && (int)(unsigned char)inputString[i] > 32) {
			convertedString += "\xa3";
			convertedString += (unsigned char)inputString[i]+128;

		}
		else if ((int)(unsigned char)inputString[i] >= 163) {
			convertedString += inputString[i];
			convertedString += inputString[i + 1];
			i++;
		}
		else if ((int)(unsigned char)inputString[i] == 32) {
			convertedString += "\xa1\xa1";
		}
		else {
			convertedString += inputString[i];
			convertedString += inputString[i + 1];
			i++;
		}
		//std::cout << convertedString.length() << std::endl;
	}
	return convertedString;
}

int getOffset(std::string singleChar)
{
	return (94 * ((int)(unsigned char)singleChar[0] - 161) + ((int)(unsigned char)singleChar[1] - 161)) * 32;
}

void Hex2Matrix(int *CMatrix, int *CHex, int start)
{
	int keys[8] = { 128, 64, 32, 16, 8, 4, 2, 1 };
	for (int i = 0; i < 16; i++) {
		for (int j = 0; j < 2; j++) {
			for (int k = 0; k < 8; k++) {
				if (keys[k] & CHex[i * 2 + j]) {
					CMatrix[start * 256 + i * 16 + j * 8 + k] = 1;
				}
			}
		}
	}
}

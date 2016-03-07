#include <string>
#include <iostream>
#include "RandomFileGenerator.hpp"

using namespace std;

int main(int argc, char** argv)
{
	if (argc != 3)
	{
		cerr << "Missing parameter" << endl;
		exit(1);
	}
	
	//RandomFileGenerator::use_whitespace = true;
	RandomFileGenerator::use_capital_letters = true;
	RandomFileGenerator::newline_after_word = true;
    RandomFileGenerator rfg;
    const string dir_path = argv[1];
    const int file_size = atoi(argv[2]);
	rfg.generate_random_words_file(dir_path, file_size, 10, 15);	// 20 MB = 20971520 bytes, 40MB = 41943040, 160MB = 167772160, 320MB = 335544320
}

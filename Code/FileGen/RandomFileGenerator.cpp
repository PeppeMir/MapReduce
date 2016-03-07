/**
 * \file RandomFileGenerator.cpp
 * 
 * \brief implementation of the methods of the class \p RandomFileGenerator present into the file \link RandomFileGenerator.hpp
 **/

#include "RandomFileGenerator.hpp"

string RandomFileGenerator::generate_random_word()
{
    // initialize random seed
    timespec ts;
	clock_gettime(CLOCK_REALTIME, &ts);	
	srand(ts.tv_nsec % REDUNDANCY);
	
    const unsigned int wordLength = rand() % maxWordLength + minWordLength;   // value in the range [minWordLength, maxWordLength)
    char* word = new char[wordLength + 1];

    for (unsigned int i = 0; i < wordLength; i++)
		word[i] = used_char_set[rand() % used_char_set_size];

    word[wordLength] = '\0';

    return string(word);
}

void RandomFileGenerator::generate_random_words_file(const string& file_path, const int expected_file_size, const long _minWordLength, const long _maxWordLength)
{
	// check if at least a flag has been set to true
    const bool _or = RandomFileGenerator::use_capital_letters || RandomFileGenerator::use_lower_letters 
										|| RandomFileGenerator::use_numbers || RandomFileGenerator::use_others;
    if (!_or)
    {
        cerr << "RandomFileGenerator: at least a set of character to use must be set before invoking the generation of the random file" << endl;
        return;
    }
	
    if (expected_file_size <= 0 || _minWordLength <= 0 || _maxWordLength <= 0)
    {
        cerr << "RandomFileGenerator: passed zero or negative expected file size or minimum/maximum word size" << endl;
        exit(1);
    }

    // set words minimum and maximum sizes
    minWordLength = _minWordLength;
    maxWordLength = _maxWordLength;

    // initialize the char set to use in the random word generation process
    initialize_char_set();

    // actual size of the random file to generate (initially zero)
    const long long expected_file_size_byte = expected_file_size * 1024 * 1024;		// Mbyte => Byte conversion
    long long file_size_byte = 0;
	
	// generate random file name
	stringstream ss;
    ss << time(NULL);
	const string full_file_path = file_path + "file_" + ss.str() + ".txt";
	
    // open random words file and check opening correctness
    ofstream random_file(full_file_path, ios::out | ios::trunc);
    if (!random_file.is_open())
    {
        cerr << "RandomFileGenerator: unable to create/open file at \"" << file_path << "\"" << endl;
        exit(1);
    }

	cout << "RandomFileGenerator: generating random file of expected size=" << expected_file_size << " Mbytes (" << expected_file_size_byte << " bytes) and words_length in [" << _minWordLength << "," << _maxWordLength << "] ..." << endl;
	
    // until the expected file size is not reached
    while (file_size_byte < expected_file_size_byte)
    {
        // generate a random word
        string random_word = generate_random_word();
		long random_word_size = random_word.size();
		
        if (RandomFileGenerator::newline_after_word)
        {
			random_word = random_word + "\n";
			random_word_size++;
		}
		 
		if (RandomFileGenerator::whitespace_after_word)
		{
			random_word = random_word + " ";
			random_word_size++;
		}

         // write the word on the file
		random_file << random_word;

        // increase current file size counting
		file_size_byte += random_word_size;
    }

    // close the file
    random_file.close();

	long file_size = file_size_byte / 1024 / 1024;
	cout << "RandomFileGenerator: random file of about " << file_size << " Mbytes successfully created at \"" << full_file_path << "\" ..." << endl;
}

void RandomFileGenerator::initialize_char_set()
{
    if (RandomFileGenerator::use_capital_letters)
        used_char_set = used_char_set + capital_letters;

    if (RandomFileGenerator::use_lower_letters)
        used_char_set = used_char_set + lower_letters;

    if (RandomFileGenerator::use_numbers)
        used_char_set = used_char_set + numbers;

    if (RandomFileGenerator::use_others)
        used_char_set = used_char_set + others;

    if (RandomFileGenerator::use_whitespace)
        used_char_set = wspace_char + used_char_set;

    used_char_set_size = used_char_set.size();
}

// static fields initialization
bool RandomFileGenerator::use_whitespace = false;
bool RandomFileGenerator::use_numbers = false;
bool RandomFileGenerator::use_capital_letters = false;
bool RandomFileGenerator::use_lower_letters = false;
bool RandomFileGenerator::use_others = false;
bool RandomFileGenerator::newline_after_word = false;
bool RandomFileGenerator::whitespace_after_word = false;

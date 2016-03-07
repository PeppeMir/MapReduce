/**
 * \file RandomFileGenerator.hpp
 * \class RandomFileGenerator
 * 
 * \brief Class representing an entity which is able to generate a file having random content.
 * 		  The size and the content of the file can be choiced in advance using different formats.
 **/

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdlib>
#include <sys/time.h>

#define REDUNDANCY 10000

using namespace std;

class RandomFileGenerator
{
	private:
		const string wspace_char = " ";											// "whitespace" character (set) 
		const string capital_letters = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";			// "capital-case letters" character set
		const string lower_letters = "abcdefghijklmnopqrstuvwxyz";				// "lower-case letters" character set
		const string numbers = "0123456789";									// "numbers" character set
		const string others = "!@#$%^&*";										// "others" character set
		
		long minWordLength = -1;												// minimum length of each random generated word
		long maxWordLength = -1;												// maximum length of each random generated word
		long used_char_set_size = -1;											// size, in byte, of the character set used for the random generation process
		string used_char_set = "";												// the character set used for the random generation process

    public:
        static bool use_whitespace;												// flag which indicates if the "whitespace" character must be used in the random generation process
		static bool use_numbers;												// flag which indicates if the "numbers" character set must be used in the random generation process
        static bool use_capital_letters;										// flag which indicates if the "capital-case letters" character set must be used in the random generation process
        static bool use_lower_letters;											// flag which indicates if the "lower-case letters" character set must be used in the random generation process
        static bool use_others;													// flag which indicates if the "others" character set must be used in the random generation process
		static bool newline_after_word;											// flag which indicates if a newline character (i.e. '\n') must be appended to each random word before inserting it into the generated file
		static bool whitespace_after_word;										// flag which indicates if a whitespace character (i.e. ' ') must be appended to each random word before inserting it into the generated file

	public:
		/**
		 * \brief Generates a file having a size, in byte, close to the one specified as parameter, and having random words content.
		 * 		  The character set to use in the random generation process must be selected using the static boolean flags of the class.
		 * 		  The length of each word will be contained in the range formed using the minimum and maximum word length parameters.
		 * \param file_path
		 * \param expected_file_size the expected size, in Mbyte, of the random file to generate 
		 * \param _min_word_length the minimum length of the words to generate
		 * \param _max_word_length the maximum length of the words to generate
		 **/
		void generate_random_words_file(const string& file_path, const int expected_file_size, const long _min_word_length, const long _max_word_length);
		
    private:
		/**
		 * \brief Generates a single random word using the character set(s) specified for the class. The generated word will be
		 * 		  used as content of the random generated file.
		 **/
		string generate_random_word();

		/**
		 * \brief Checks the static boolean fields of the class in order to initialize the effective character set(s) to use
		 * 		  for the random file generation process.
		 **/
		void initialize_char_set();
};

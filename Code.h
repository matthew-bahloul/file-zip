#pragma once
#include <fstream>
#include <queue>
#include <sstream>
#include <unordered_map>
#include <vector>

#include "StringSplitter.h"

// operator overloads for comparing in the priority queue
struct CustomCompare
{
	bool operator()(pair<string, int>& pair_a, pair<string, int>& pair_b)
	{
		return pair_a.second < pair_b.second;
	}
};

class Code
{
private:

	// files for the compression process
	string _file_to_compress;
	string _compressed_out;
	string _compressed_codes;

	// variables for parsing the file and keeping track of word counts
	vector<string> _all_words{};
	unordered_map<string, int> _word_frequencies{};
	unordered_map<string, string> _key{};
	priority_queue< pair<string, int>, vector<pair<string, int>>, CustomCompare> _word_frequencies_queue{};

	// functions to read files
	vector<string> readFile(ifstream& some_file)
	{
		vector<string> result{};
		//is the file open?
		if (some_file.is_open() == true)
		{
			while (some_file.good() == true)
			{
				//two ways to grab data
				string data;
				//first_names >> data; //CIN style
				getline(some_file, data); //more robust method
				result.push_back(data);
			}
		}
		return result;
	}
	vector<string> readFile(ifstream&& some_file)
	{
		ifstream& local_file = some_file;

		//auto is an automatically inferred variable type
		//in our case, vector<string>
		auto result = readFile(local_file);
		local_file.close();
		return result;
	}
	vector<string> readFile(string file_name)
	{
		return readFile(ifstream{ file_name });
	}

	// turns a base10 int to base2 as a string
	string toBinary(int num)
	{
		if (num == 0)
		{
			return "0";
		}

		ostringstream _bin_num; // string to store the binary representation of the count of the word

		while (num > 0)
		{
			int temp = num % 2;
			_bin_num << temp;
			num = num / 2;
		}

		string rev_num = _bin_num.str();
		string for_num = "";

		for (int i = rev_num.length() - 1; i >= 0; i--)
		{
			for_num += rev_num[i];
		}

		return for_num;
	}

public:
	Code(string file_name)
	{
		_file_to_compress = file_name;

		makeOutputFiles();

		readAndSplitFile();

		makeFrequencyTable(_all_words);

		makePriorityQueue(_word_frequencies);

		compressFile();
	}

	// create the output file names
	void makeOutputFiles()
	{
		ostringstream compressed_file_name;
		ostringstream codes_file_name;
		// create a name for the output file
		for (char c : _file_to_compress)
		{
			if (c == '.')
			{
				break;
			}
			compressed_file_name << c;
		}
		compressed_file_name << ".compressed.txt";

		// create a name for the output file
		for (char c : _file_to_compress)
		{
			if (c == '.')
			{
				break;
			}
			codes_file_name << c;
		}
		codes_file_name << ".codes.txt";

		_compressed_out = compressed_file_name.str();
		_compressed_codes = codes_file_name.str();
	}

	// compression: reads a file and splits it based on a desired delimeter
	void readAndSplitFile()
	{
		vector<string> lines = readFile(_file_to_compress);

		for (auto line : lines)
		{
			vector<string> words = StringSplitter::split(line, " ");
			for (auto word : words)
			{
				_all_words.push_back(word);
			}
		}
	}

	// compression: creates a frequency table of the words in a file
	void makeFrequencyTable(vector<string> vector_to_map)
	{
		for (auto word : vector_to_map)
		{
			if (word != "" and  word != "\n")
			{
				_word_frequencies[word]++;
			}
		}
	}

	// compression: create a priority queue of the frequency table
	void makePriorityQueue(unordered_map<string, int> map_to_priority_queue)
	{
		for (auto group : map_to_priority_queue)
		{
			_word_frequencies_queue.push(group);
		}
	}

	// compression: writes a compress version of the file to another file
	void compressFile()
	{
		// streams to write compressed version and codes
		ofstream compressed_out_stream{ _compressed_out };
		ofstream codes_out_stream{ _compressed_codes };

		// write the codes file
		int counter = 0;

		while (!_word_frequencies_queue.empty())
		{
			codes_out_stream << _word_frequencies_queue.top().first << ": " << toBinary(counter) << endl;
			_key[_word_frequencies_queue.top().first] = toBinary(counter);
			_word_frequencies_queue.pop();
			counter++;
		}

		for (auto word : _all_words)
		{
			compressed_out_stream << _key[word] << " ";
		}
	}

	string getCompressedFileName()
	{
		return _compressed_out;
	}
	
	string getCodesFileName()
	{
		return _compressed_codes;
	}
};
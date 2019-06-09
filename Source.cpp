#include <iostream>
#include <string>
#include <sstream>

#include "Code.h"
#include "Decode.h"

using namespace std;

int main(void)
{

	// all file names
	char encode_decode = ' ';
	string file_to_code;
	string file_to_decode;
	string codes_file;

	// ask the user to encode or decode
	while (toupper(encode_decode) != 'E' && toupper(encode_decode) != 'D')
	{
		cout << "(E)ncode or (D)ecode file (decode not implemented): ";
		cin.get(encode_decode);
		cin.ignore();
	}

	// decide whether to encode or decode
	if (toupper(encode_decode) == 'E')
	{
		cout << "Enter file to compress: ";
		getline(cin, file_to_code);

		Code compress{ file_to_code };

		cout << "Encoded file written to: " << compress.getCompressedFileName() << endl;
		cout << "Codes file written to: " << compress.getCodesFileName() << endl;
	}


	return 0;
}
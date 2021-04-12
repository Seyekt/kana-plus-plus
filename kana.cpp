#include <stdio.h>
#include <iostream>
#include <io.h>
#include <sstream>

#include <string>
#include <wchar.h>
#include <vector>

#include <fstream>
#include <limits>

using namespace std;

#include "FromUTF8.h"

#define _O_U8TEXT 0x00040000 //UTF-8
#define _O_U16TEXT 0x00020000 // UTF-16

class Kana {

	wstring romaji;
	wchar_t hiragana, katakana;

	public:
		Kana (wstring x, wchar_t y, wchar_t z) {
			romaji = x;
			hiragana = y;
			katakana = z;
		}

		Kana () {
		}

		void setRomaji (wstring romaji) {
			this->romaji = romaji;
		}

		void setHiragana (wchar_t hiragana) {
			this->hiragana = hiragana;
		}

		void setKatakana (wchar_t katakana) {
			this->katakana = katakana;
		}

		wstring getRomaji(){
			return romaji;
		}

		wchar_t getHiragana(){
			return hiragana;
		}

		wstring toString() {

			return romaji + L' ' + hiragana + L' ' + katakana;
		}
		
};


vector<Kana> kanaList;

void populateList() {

	string fileInput;

	ifstream kanaUTF8("kana UTF-8.txt");
	
	vector<wstring> lines;

	for(wstring line; getline(kanaUTF8, fileInput);) {
		
		
		line = FromUTF8( fileInput.c_str() );	//convert from UTF-8 in file to UTF-16 for use with Windows CMD
		lines.push_back(line);


	}
	
	kanaUTF8.close();


	wcout << L"Kana list from file: \n";

	for (int i = 0; i < lines.size(); i++) {

		wcout << lines[i] + L'\n';

	}

	Kana insert;
	wstring tempRomaji;
	wchar_t tempHiragana;
	wchar_t tempKatakana;  

	wstring intermediate;

	for (int i = 0; i < lines.size(); i++) {

		vector<wstring> tokens;

		wstringstream check1(lines[i]);

		while(getline(check1, intermediate, L' ')) {

        	tokens.push_back(intermediate);

    	}

		tempRomaji = tokens[0];
		tempHiragana = tokens[1][0];
		tempKatakana = tokens[2][0];

		insert.setRomaji(tempRomaji);
		insert.setHiragana(tempHiragana);
		insert.setKatakana(tempKatakana);

		kanaList.push_back(insert);
	}

}

wstring convertHiraganaToRomaji(wstring hiragana){

	wstring output = L"";
	bool finished;

	bool doubleNextConsonant = false;
	

	for(int i = 0; i < hiragana.size(); i++) {

		finished = false;

		for(int j = 0; j < kanaList.size() && !finished; j++) {

			if(hiragana[i] == L'っ') {

				doubleNextConsonant = true;

			} else if(hiragana[i] == kanaList[j].getHiragana()) {

				if(doubleNextConsonant) {

					output += kanaList[j].getRomaji()[0];
					doubleNextConsonant = false;
					
				}

				output += kanaList[j].getRomaji();
				finished = true;

			}

		}
	}

	return output;
}

int main() {

	_setmode(_fileno(stdout), _O_U16TEXT);
	_setmode(_fileno(stderr), _O_U16TEXT);
	_setmode(_fileno(stdin), _O_U16TEXT);

	populateList();

	wcout << L"Kana list: \n";
	
	for (int i = 0; i < kanaList.size(); i++) {

		wcout << kanaList[i].toString();
		wcout << L'\n';

	}

	wstring userInput;

	wcin >> userInput;

	wcout << L"You entered: "<< userInput << L'\n';
	wcout << L"Romaji: " << convertHiraganaToRomaji(userInput);


	return 0;

}
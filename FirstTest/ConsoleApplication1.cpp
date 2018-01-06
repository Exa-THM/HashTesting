// "ConsoleApplication1.cpp": Definiert den Einstiegspunkt für die Konsolenanwendung.
//

#include "stdafx.h"
#include <iostream>
#include <unordered_map>
#include <string>
#include <fstream>
#include <Windows.h>

using namespace std;

unordered_map<string, int> readWordsFromLex(const string fileName) {
	unordered_map< string, int > myMap = {};
	fstream lexikon("allWordsWithB.lex");
	string word;
	while (getline(lexikon, word)) {
		myMap[word]++;
	}
	return myMap;
}

int main()
{
	LARGE_INTEGER Freq;
	LARGE_INTEGER StartTime, EndTime, ElapsedTime;
	QueryPerformanceFrequency(&Freq);
	unordered_map<string, int> map = readWordsFromLex( "allWordsWithB.txt" );
	ofstream log("test1.log");
	for( int k = 0; k<3; k++ ) {
		QueryPerformanceCounter(&StartTime);
		for (int i = 0; i < 2000000000; i++) {
			int b = map["BZZ"];
		}
		QueryPerformanceCounter(&EndTime);
		ElapsedTime.QuadPart = EndTime.QuadPart - StartTime.QuadPart;
		ElapsedTime.QuadPart *= 1000000;
		ElapsedTime.QuadPart /= Freq.QuadPart;
		log << ElapsedTime.QuadPart << "\n";
	}
}
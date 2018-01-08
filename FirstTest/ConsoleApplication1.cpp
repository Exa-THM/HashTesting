// "ConsoleApplication1.cpp": Definiert den Einstiegspunkt für die Konsolenanwendung.
//

#include "stdafx.h"
#include <iostream>
#include <unordered_map>
#include <functional>
#include <string>
#include <fstream>
#include <Windows.h>

using namespace std;

#define HASH_FN function<unsigned long(std::string)>
#define HASH_MAP unordered_map<string, unsigned long, HASH_FN>



HASH_MAP readWordsFromLex(const string fileName, HASH_FN hashFn ) {
	HASH_MAP myMap( 0, hashFn );
	fstream lexikon(fileName);
	string word;
	while (getline(lexikon, word)) {
		myMap[word] = hashFn(word);
	}
	return myMap;
}

LARGE_INTEGER measureAccessTimes(HASH_MAP map, string key, int times) {
	LARGE_INTEGER Freq;
	LARGE_INTEGER StartTime, EndTime, ElapsedTime;
	QueryPerformanceFrequency(&Freq);
	QueryPerformanceCounter(&StartTime);
	for (int i = 0; i < times; i++) {
		int b = map[key];
	}
	QueryPerformanceCounter(&EndTime);
	ElapsedTime.QuadPart = EndTime.QuadPart - StartTime.QuadPart;
	ElapsedTime.QuadPart *= 1000000;
	ElapsedTime.QuadPart /= Freq.QuadPart;
	return ElapsedTime;
}

unsigned long hashing_func(string key) {
	unsigned long hash = 0;
	for (int i = 0; i<key.size(); i++)
	{
		hash += (71 * hash + key[i]) % 5;
	}
	return hash;
}

int main()
{
	HASH_MAP map = readWordsFromLex("../../../Lexika/allWordsWithB.txt", hashing_func);
	cout << map.size();
	ofstream log("test1.log");
	for( int k = 0; k<7; k++ ) {
		log << measureAccessTimes( map, "BZZ", 2000000 ).QuadPart << "\n";
	}
}
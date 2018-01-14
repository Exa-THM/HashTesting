// "ConsoleApplication1.cpp": Definiert den Einstiegspunkt für die Konsolenanwendung.
//

#include "stdafx.h"
#include <unordered_map>
#include <functional>
#include <string>
#include <cmath>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <time.h>
#include <Windows.h>

using namespace std;
class HashEntry{
public:
	int key;
	int value;
	HashEntry(int key, int value){
		this->key = key;
		this->value = value;
	}
};
#define HASH_FN function<unsigned long(std::string)>
#define HASH_MAP unordered_map<string, unsigned long, HASH_FN>
HashEntry **table;
unsigned long badHash(string);
unsigned long lowQualityHash(string);
unsigned long exampleHash(string);
unsigned long jenkins_one_at_a_time_64_hash(string);
unsigned long jenkins_one_at_a_time_128_hash(string);
//Hier einen Prototyp für eure Hashfunktion hinzufügen.
//Ohne das wird eure Hashfunktion nicht erkannt bevor sie definiert wird - also müsstet ihr die definition über meine map legen - ich finde das wirklich hässlich
class HashEntry {
public:
	int key;
	int value;
	HashEntry(int key, int value) {
		this->key = key;
		this->value = value;
	}
};
class HashMap {
	HashEntry **table;
public:
	HashMap() {
		table = new HashEntry *[128];
		for (int i = 0; i < 128; i++) {
			table[i] = NULL;
		}
	}
	int HashFunc(int key){
		return key % 128;
	}
	void Insert(int key, int value){ //einfügen von dem key und value
		int hash = HashFunc(key);
		while (table[hash] != NULL && table[hash]->key != key){
			hash = HashFunc(hash + 1);
		}
		if (table[hash] != NULL)
			delete table[hash];

		table[hash] = new HashEntry(key, value);
	}
	int Search(int key){ // suche nach dem schlüssel
		int hash = HashFunc(key);
		while (table[hash] != NULL && table[hash]->key != key){
			hash = HashFunc(hash + 1);
		}
		if (table[hash] == NULL)
			return -1;
		else
			return table[hash]->value;
	}
	void Remove(int key){
		int hash = HashFunc(key);
		while (table[hash] != NULL){
			if (table[hash]->key == key)
				break;
			hash = HashFunc(hash + 1);
		}
		if (table[hash] == NULL){

			cout << "Kein Element konnte im Schlüssel gefunden werden " << key << endl;

			return;
		}
		else{
			delete table[hash];
		}

		cout << "Element gelöscht" << endl;
	}
	~HashMap(){ //hashmap objekt zerstören

		for (int i = 0; i < 128; i++){

			if (table[i] != NULL)

				delete table[i];

			delete[] table;
		}

	}
};

unordered_map<string, HASH_FN> hashFuncs = { { "bad hash", badHash },{ "low quality hash", lowQualityHash },{ "example hash", exampleHash },{ "Jenkins One At A Time Hash 64", jenkins_one_at_a_time_64_hash },{ "Jenkins One At A Time Hash 128", jenkins_one_at_a_time_128_hash } };
//unordered_map<string, HASH_FN> hashFuncs = { {"low quality hash", lowQualityHash} };
//Hier einen neuen Eintrag für eure Hash Funktion hinzufügen um es in die hashFuncs Map zu integrieren
//Syntax : { { hashFunktionsName, hashFunktionOhne() }, { hashFunktionsName2, hashFunktionOhne()2 } }
//Alle hash Funktionen in dieser Map werden dann später auch getested

unordered_map<string, string> lexika = { { "AllWords", "../../../Lexika/FullLex.txt"}, { "OnlyB", "../../../Lexika/allWordsWithB.txt" }, { "Links", "../../../Lexika/somelinks.txt" } };
//unordered_map<string, string> lexika = { { "OnlyB", "../../../Lexika/allWordsWithB.txt" },{ "Links", "../../../Lexika/somelinks.txt" } };
//hier könntet ihr noch zusätzliche Test Daten einfügen - die Ergebnisse sind ziemlich unterschiedlich je nachdem welche Daten man mit welchem Hash die Daten verwendet werden
//es werden alle Tests für jeden hash mit jeder Datengrundlage durchgeführt, sodass die Rechenarbeit schnell explodiert wenn ihr zu viel hinzufügt

/*
Es fühlt sich sinnvoll an hier zu beschreiben, was eigentlich getestet wird:
Wir erstellen für jede hashfunktion eine Map und füllen sie dann mit einer der Zeilen aus einer der lexika
Dann wählen wir 4 zufällige keys aus für die jeweils die Zeit gemessen wird die benötigt wird um 200000 mal auf diesen Key zuzugreifen
Danach lassen wir die keys auf grundlage ihres hashes auf n buckets verteilen und berechnen die Standardabweichung von der Durchschittsverteilung bei n buckets - das machen wir für n von 2 bis zur länge der map
Das ganze wird in einer Datei spezifisch für diese Map mit dieser Hashfunktion und diesen Probedaten ausgegeben - ihr könnt es im Ordner Logs finden
*/


unsigned long badHash(string key) { //ein schlechter hash der immer den selben Wert zurück gibt
	return 0; //er ist wirklich langsam ich würde ihn aus den tests entfernen wenn ihr ihn nicht unbedingt braucht
}

unsigned long lowQualityHash(string key) {
	switch (key.length()) {
	case 0: return 0;
	case 1: return key[0];
	case 2: return key[0] | (key[1] << 8);
	case 3: return key[0] | (key[1] << 8) | (key[2] << 16);
	default: return key[0] | (key[1] << 8) | (key[2] << 16) | (key[2] << 24);
	}
}

unsigned long exampleHash(string key) {
	unsigned long hash = 0;
	for (int i = 0; i<key.size(); i++)
	{
		hash += (71 * hash + key[i]) % 5;
	}
	return hash;
}

unsigned long jenkins_one_at_a_time_64_hash(const string key) {
	//size_t i = 0;
	size_t length = 64;
	unsigned long hash = 0;
	for (int i = 0; i<key.size(); i++) {
		hash += key[i];
		hash += hash << 10;
		hash ^= hash >> 6;
	}
	hash += hash << 3;
	hash ^= hash >> 11;
	hash += hash << 15;
	return hash;
}

unsigned long jenkins_one_at_a_time_128_hash(const string key) {
	//size_t i = 0;
	size_t length = 128;
	unsigned long hash = 0;
	for (int i = 0; i<key.size(); i++) {
		hash += key[i];
		hash += hash << 10;
		hash ^= hash >> 6;
	}
	hash += hash << 3;
	hash ^= hash >> 11;
	hash += hash << 15;
	return hash;
}

//hier die definition eurer hash funktion einfügen
long int Hashing(string key) {
	int hash = (key%128);
	while (table[hash] != NULL && table[hash]->getKey() != key)
		hash = (hash + 1) % 128;
	if (table[hash] == NULL)
		return -1;
	else
		return table[hash]->getValue();

}

HASH_MAP readWordsFromLex(const string fileName, HASH_FN hashFn ) {
	HASH_MAP myMap( 0, hashFn );
	fstream lexikon(fileName);
	string word;
	while (getline(lexikon, word)) {
		myMap[word] = hashFn(word);
	}
	return myMap;
}

int selectLargerRandom(int max) {
	static const int randomLength = (int)round(log2((double)RAND_MAX));
	int maxLength = (int)round(log2((double)max));
	int currentLength = 0;
	int ran = 0;
	while (maxLength > currentLength) {
		ran = ran << randomLength | rand();
		currentLength += randomLength;
	}
	return ran % max;
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

string measureAccessTimes2(HASH_MAP map, int differentKeys, int attempts, int accesses) {
	string output;
	for (int i = 0; i < differentKeys; i++) {
		string keyName;
		int keyIndex = selectLargerRandom( map.size() );
		int k = 0;
		for (const auto& indices : map) {
			if (k++ == keyIndex) {
				keyName = indices.first;
				cout << "accessing: " << keyName << " " << keyIndex << "\n";
				output.append("accessing: " + keyName + "\n");
				break;
			}
		}
		for( k = 0; k < attempts; k++ ) {
			output.append(to_string(measureAccessTimes(map, keyName, accesses).QuadPart));
			output.append("\n");
		}
	}
	return output;
}

string measureCollisions(HASH_MAP map, int maxModulo) {
	int i = 0;
	int mapLength = map.size();
	int* buckets = new int[maxModulo];
	unsigned long* hashes = new unsigned long[mapLength];
	string output;
	output.reserve( 10*maxModulo );
	for (const auto& indices : map) { 
		hashes[i++] = indices.second;
	}
	for ( int modulo = 2; modulo < maxModulo; modulo++ ) {
		for (i = 0; i < modulo; i++) buckets[i] = 0;
		for (i = 0; i < mapLength; i++) buckets[hashes[i] % modulo]++;
		double expectedAverage = (double)mapLength / (double)modulo;
		double strayExpectedAverage = 0;
		for (i = 0; i < modulo; i++) strayExpectedAverage += pow((double)buckets[ i ] - expectedAverage, 2);
		strayExpectedAverage /= modulo;
		output.append(to_string(sqrt(strayExpectedAverage)));
		output.append("\n");
	}
	delete[] buckets;
	delete[] hashes;
	return output;
}

void runTestsFor(HASH_FN hashing_func, string hashName, string lexikon, string lexikonName) {
	string logFolder   = "../../../Logs/" + hashName;
	string logFileName = lexikonName + "Collision.log";
	CreateDirectoryA( (LPCSTR)logFolder.c_str(), NULL );
	ofstream log(logFolder + "/" + logFileName);
	cout << "Starting the test of the hash function " << hashName << " with the word lexikon " << lexikonName << "\n";
	log << "Starting the test of the hash function " << hashName << " with the word lexikon " << lexikonName << "\n";
	cout << "Starting to load map from " << lexikonName << "\n";
	log << "Starting to load map from " << lexikonName << "\n";
	HASH_MAP map = readWordsFromLex(lexikon, hashing_func);
	cout << "Loading finished. Loaded " << map.size() << " words from file.\n";
	log << "Loading finished. Loaded " << map.size() << " words from file.\n";
	cout << "Starting to measure access time - please avoid actions that cost perfomance as that would invalidate the results.\n";
	log << "Started measuring access times\n";
	log << measureAccessTimes2(map, 4, 5, 200000);
	cout << "Finished measuring access times\n";
	log << "Finished measuring access times\n";
	cout << "Starting to calculate: " << map.size() << " Collisions\n";
	log << "Starting to calculate: " << map.size() << " Collisions\n";
	log << measureCollisions(map, map.size());
	cout << "Testing " << hashName << " with the word lexikon " << lexikonName << "has finished\nThe results can be found in Logs/" << hashName << "/" << logFileName;
}

int main(int argc, const char * argv[])
{
	srand(time(NULL));
	for (const auto& hashFn : hashFuncs) {
		for (const auto& lexikon : lexika) {
			runTestsFor(hashFn.second, hashFn.first, lexikon.second, lexikon.first);
		}
	}
}

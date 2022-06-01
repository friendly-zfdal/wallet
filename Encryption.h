#pragma once
#include <iostream>
#include <stdlib.h>
#include <map>
#include <string>
#include <fstream>
#include <process.h>

using namespace std;
class Encryption
{
	static char** alphabet;
public:
	static char finder(char a, char b, char** alphabet);
	static char de_finder(char a, char b, char** alphabet);
	static int file_encrypt(string filename, string pin);
	static string mix(string* cipher, string text);
	static string deMix(string* cipher, string text);
	//need to check returned value
	static int file_decrypt(string filename, string pin);
	static string pin_encrypt(string* pin);
	static string pin_decrypt(string* pin);
	static void alphabet_init();
};


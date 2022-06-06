#include "Encryption.h"

//126-32=94=95

char** Encryption::alphabet;

char Encryption::finder(char a, char b, char** alphabet) {
	int ver;
	int hor;
	for (int i = 0; i < 95; i++)
		if (alphabet[0][i] == a) {
			hor = i;
			break;
		}
	
	for (int i = 0; i < 95; i++)
		if (alphabet[i][0]==b){
			ver = i;
			break;
		}
	
	return alphabet[ver][hor];
}

char Encryption::de_finder(char encrypted_s, char cipher_s, char** alphabet) {
	int ver;
	int hor;
	for (int i = 0; i < 95; i++)
		if (alphabet[i][0] == cipher_s) {
			ver = i;
			break;
		}

	for (int i = 0; i < 95; i++)
		if (alphabet[ver][i] == encrypted_s) {
			hor = i;
			break;
		}

	return alphabet[0][hor];
}


string Encryption::mix(string* cipher, string text) {
	
	for (int i = 0; i < 3; i++) {
		int left_inc = (*cipher)[i]-48;
		int right_inc = (*cipher)[5 - i] - 48;
		int left_index = 0+left_inc;
		int length = text.length();
		int right_index = text.length()-right_inc;
		char temp;
		while (left_index<text.length() / 2 && right_index>text.length() / 2) {
			temp = text[left_index];
			text[left_index] = text[right_index];
			text[right_index] = temp;
			left_index += left_inc;
			right_index -= right_inc;
		}
	}
	return text;
}

string Encryption::deMix(string* cipher, string text) {
	for (int i = 2; i >= 0; i--) {
		int left_inc = (*cipher)[i] - 48;
		int right_inc = (*cipher)[5 - i] - 48;
		int left_index = 0 + left_inc;
		int right_index = text.length() - right_inc;
		char temp;
		int length = text.length();

		while (left_index<text.length() / 2 && right_index>text.length() / 2) {
			temp = text[left_index];
			text[left_index] = text[right_index];
			text[right_index] = temp;
			left_index += left_inc;
			right_index -= right_inc;
		}
	}
	return text;
}

void Encryption::alphabet_init() {
	if (alphabet == NULL) {
		alphabet = (char**)malloc(95 * sizeof(char*));
		for (int i = 0; i < 95; i++)
			alphabet[i] = (char*)malloc(95 * sizeof(char));
		int letter;
		for (int i = 0; i < 95; i++)
			for (int j = 0; j < 95; j++) {
				letter = 32 + i + j;
				if (letter > 126)
					letter = 31 + ((32 + i + j) % 126);
				alphabet[i][j] = (char)letter;
			}
		//for (int i = 0; i < 95; i++) {
		//	for (int j = 0; j < 95; j++) {
		//		cout << alphabet[i][j] << " ";
		//	}
		//	cout << "\n";
		//}
	}
	else return;
}

//there are two options
// 1 - pin is correct
// 2- pin is incorrect
//we need to check it after decryption and if it's not - then to encrypt it back with provided pin (in other case we'll get corrupted file after attempt to log in with incorrect values)
int Encryption::file_decrypt(string filename, string pin) {
	alphabet_init();
	fstream fs;
	fs.open(filename);
	string text;
	//fs >> text;
	getline(fs, text);
	fs.close();

	string output_str;
	// приходит шифрованный текст + pin
	int index_pin = 0;
	int limit_pin = pin.length();
	char output;
	int length = text.length();
	text = deMix(&pin, text);
	for (int i = 0; i < text.length(); i++) {
		output = Encryption::de_finder(text[i], pin[index_pin], alphabet);
		index_pin = (index_pin + 1) % limit_pin;
		output_str += output;
	}
	ofstream fso;
	fso.open(filename);
	fso << output_str;
	fso.close();
	
	return 0;
}

int Encryption::file_encrypt(string filename, string pin)
{
	alphabet_init();
	fstream fs;
	fs.open(filename);
	string text;
	//fs >> text;
	getline(fs, text);
	fs.close();
	string output_str;
	
	int index_pin = 0;
	int limit_pin = pin.length();
	char output;
	int length = text.length();

	for (int i = 0; i < text.length(); i++) {
		output = finder(text[i], pin[index_pin], alphabet);
		index_pin = (index_pin + 1) % limit_pin;
		output_str += output;
	}
	ofstream fso;
	fso.open(filename);
	fso << mix(&pin, output_str);
	fso.close();


//	cout << "\n" << output_str;
	return 0;
}

string Encryption::pin_encrypt(string* pin) {
	alphabet_init();
	string pid = to_string(_getpid());
	string local_pin = *pin;

	string output_str;

	int index_pid = 0;
	int limit_pid = pid.length();
	char output;
	for (int i = 0; i < local_pin.length(); i++) {
		output = finder(local_pin[i], pid[index_pid], alphabet);
		index_pid = (index_pid + 1) % limit_pid;
		output_str += output;
	}
	*pin = output_str;
	return "";
}
string Encryption::pin_decrypt(string* pin) {
	alphabet_init();
	string pid = to_string(_getpid());
	string output_str;
	string local_pin = *pin;
	int index_pid = 0;
	int limit_pid = pid.length();
	char output;

	for (int i = 0; i < local_pin.length(); i++) {
		output = Encryption::de_finder(local_pin[i], pid[index_pid], alphabet);
		index_pid = (index_pid + 1) % limit_pid;
		output_str += output;
	}
	// need to test: this one
	*pin = output_str;
	//or this
	return output_str;
}
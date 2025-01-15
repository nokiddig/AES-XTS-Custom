#include <iostream>
#include "AES_ECB.h"


using namespace std;


class Preprocess{
private:
	string message;
public:
	Preprocess(){}

	Preprocess(string message){
		this->message = message;
	}

	void addPadding(){

	}
	void removePadding(){

	}
	vector<string> splitBlock(){

	}

};

class AES_XTS{
private:
    AES_ECB aes_key1;
    AES_ECB aes_key2;
public:
	AES_XTS(){};

	void calculate_tweak0(){}

	void generate_tweak(){}

	void encode(){}

	void decode(){}

};


int main(){
	string key, message;
	freopen("input.txt", "r", stdin);
	//	cin >> Key >> message;
	key = "0f1571c947d9e8590cb7add6af7f6798";
	message = "0123456789abcdeffedcba9876543210";
	// CIPHER = "ff0b844a0853bf7c6934ab4364148fb9";

    AES_ECB a;
    a.setKey(key);
    string encode = a.encode(message);
    string decode = a.decode(encode);
	cout << encode << endl;
	cout << decode;
	// Math math;
	// cout << math.xorStr("123456", "000000");
} 
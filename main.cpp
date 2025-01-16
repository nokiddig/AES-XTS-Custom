#include <iostream>
#include "AES_ECB.h"


using namespace std;


class Preprocess{
private:
	string message;
	Math math;
public:
	Preprocess(){}

	Preprocess(string message){
		this->message = message;
	}

	string toHexString(int num){
		stringstream ss;
		ss << uppercase << setw(2) << setfill('0') << hex << (int)num;
		return ss.str();
	}

	char hexToAscii(const std::string& hex) {
		// Chuyển chuỗi hex thành số nguyên
		int asciiValue;
		std::stringstream ss;
		ss << std::hex << hex;
		ss >> asciiValue;

		// Chuyển giá trị số thành ký tự ASCII
		return static_cast<char>(asciiValue);
	}

	string pkcs5_pad(const string& input, int blockSize=16) {
		string res = "";
		for (unsigned char c : input) {
			res += toHexString(c);
		}

		// Thêm padding PKCS#5
		int paddingSize = blockSize - (input.size() % blockSize);
		for (int i = 0; i < paddingSize; i++) {
			res += toHexString(paddingSize);
		}

		return res;
	}

	// Bỏ padding PKCS#5
	string pkcs5_unpad(string padded_data, int blockSize=16) {
		int padSize = math.hexToDec(padded_data.back());
		padSize += blockSize * (padSize==0);
		padded_data.erase(padded_data.size()-padSize*2, padSize*2);
		string res = "";
		for (int i=0; i+2<=padded_data.size(); i+=2) {
			res += hexToAscii(padded_data.substr(i, 2));
		}
		return res;
	}

	// vector<string> splitBlock(){

	// }

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

    // AES_ECB a;
    // a.setKey(key);
    // string encode = a.encode(message);
    // string decode = a.decode(encode);
	// cout << encode << endl;
	// cout << decode;
	// Math math;
	// cout << math.xorStr("123456", "000000");

	Preprocess pre;
	string tmp = pre.pkcs5_pad("This is a test");
	cout << tmp << endl;
	cout << pre.pkcs5_unpad(tmp);
} 
#include <iostream>
#include <iomanip>
#include "AES_ECB.h"
#include "gen_tweak.h"
using namespace std;

#define BLOCK_SIZE 16 // 128 bits = 16 bytes
const int MAX_BLOCK = 35;

class AES_XTS{
private:
    AES_ECB aes_key1;
    AES_ECB aes_key2;
	string key1, key2;
	
	Math math;
	string tweak = "123456789ABCDEF123456789ABCDEF12"; //hex
	string T[MAX_BLOCK];
	const string a  = "00000000000000000000000000000002";
	string aj[MAX_BLOCK] = {"00000000000000000000000000000001"};
public:
	AES_XTS(){};

	AES_XTS(string k1, string k2){
		aes_key1.setKey(k1);
		aes_key2.setKey(k2);
		this->key1 = k1;
		this->key2 = k2;
		this->generateTweak();
	};

	void generateTweak(){
		T[0] = aes_key2.encode(tweak);
		for (int i=1; i<MAX_BLOCK; i++) {
			// a[j] = a[j-1] * a
			aj[i] = multiplyHexStrings(aj[i-1], a);
			T[i] = multiplyHexStrings(T[i-1], aj[i]);
		}
	}

	vector<string> encodeMess(const vector<string>& plain){
		vector<string> result;
		int size = plain.size();
		string cipherI;
		for (int i=0; i<size-1; i++) {
			cipherI = encodeBlock(plain[i], T[i]);
			result.push_back(cipherI);
		}

		string lastBlock = plain.back();
		if (lastBlock.size() == 32){
			cipherI = encodeBlock(lastBlock, T[size-1]);
			result.push_back(cipherI);
		}
		else { 
			// thieu, can padding bang block truoc 
			int padSize = 32-lastBlock.size();
			string padStr = result.back().substr(32 - padSize);
			string lastCipher = result.back().substr(0, 32 - padSize);

			result.back() = encodeBlock(lastBlock + padStr, T[size-1]);
			result.push_back(lastCipher);
		}
		return result;
	}

	string encodeBlock(string plainI, string Ti){
		string res = math.xorStr(plainI, Ti);
		res = aes_key1.encode(res);
		res = math.xorStr(res, Ti);
		return res;
	}

	vector<string> decodeMess(vector<string> cypher){
		vector<string> result;
		int size = cypher.size();
		string plainI;
		for (int i=0; i<size-1; i++) {
			plainI = decodeBlock(cypher[i], T[i]);
			result.push_back(plainI);
		}

		string &lastBlock = cypher.back();
		if (lastBlock.size() == 32){
			plainI = decodeBlock(lastBlock, T[size-1]);
			result.push_back(plainI);
		}
		else { 
			// thieu, can padding bang block truoc 
			int padSize = 32-lastBlock.size();
			string padStr = result.back().substr(32 - padSize);
			string lastPlain = result.back().substr(0, 32 - padSize);

			result.back() = encodeBlock(lastBlock + padStr, T[size-1]);
			result.push_back(lastPlain);
		}
		return result;
	}

	string decodeBlock(string cipherI, string Ti){
		string res = math.xorStr(cipherI, Ti);
		res = aes_key1.decode(res);
		res = math.xorStr(res, Ti);
		return res;
	}
};


int main(){
	string key1Hex, key2Hex, message, cipherECB;
	key1Hex = 		"0f1571c947d9e8590cb7add6af7f6798";
	key2Hex = 		"81932804fdaea2347342836120132318";
	message = 		"0123456789abcdeffedcba98765432100123456789abcdeffedcba9876543210";
	cipherECB = 	"ff0b844a0853bf7c6934ab4364148fb9";

	//xts - dưới 16 byte -> áp dụng padding, trên 16 byte -> ko dùng padding
	Preprocess pre;
	auto x = pre.splitBlock(pre.toHexString("Toi yeu viet nam"));
	printVector(x);

	AES_XTS aes_xts(key1Hex, key2Hex);
	auto res = aes_xts.encodeMess(x);
	vector<string> cpXTS = {"19803A6ACF802DF18A342B2C124DD3DA"};
	printVector(res);
	printVector(aes_xts.decodeMess(cpXTS));
	return 0;
} 
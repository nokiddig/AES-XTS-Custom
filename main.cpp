#include <iostream>
#include "AES_ECB.h"

using namespace std;
class AES_XTS{
private:
    AES_ECB aes_key1;
    AES_ECB aes_key2;
public:
}


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
} 
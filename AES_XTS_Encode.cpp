// ref: https://viblo.asia/p/tim-hieu-thuat-toan-ma-hoa-khoa-doi-xung-aes-gAm5yxOqldb
#include <bits/stdc++.h>
#include "MyMath.h"
using namespace std;
/*
    NOTE:
    AES: advanced encryption standard
    Size khoi(mini plaintext): 128-b
    Size khoa(key): 128/192/256-b
*/


class AES{
private:
	Math math;
	int KEY_LENGTH = 0;
    int BLOCK_SIZE = 4;
    string plainText = "", KEY, plainTextCopy;
    string w[50];
    vector<string> vtPair[4];
    int nRound = 0;
    const string S_BOX[16][16] = {
        {"63", "7C", "77", "7B", "F2", "6B", "6F", "C5", "30", "01", "67", "2B", "FE", "D7", "AB", "76"},
        {"CA", "82", "C9", "7D", "FA", "59", "47", "F0", "AD", "D4", "A2", "AF", "9C", "A4", "72", "C0"},
        {"B7", "FD", "93", "26", "36", "3F", "F7", "CC", "34", "A5", "E5", "F1", "71", "D8", "31", "15"},
        {"04", "C7", "23", "C3", "18", "96", "05", "9A", "07", "12", "80", "E2", "EB", "27", "B2", "75"},
        {"09", "83", "2C", "1A", "1B", "6E", "5A", "A0", "52", "3B", "D6", "B3", "29", "E3", "2F", "84"},
        {"53", "D1", "00", "ED", "20", "FC", "B1", "5B", "6A", "CB", "BE", "39", "4A", "4C", "58", "CF"},
        {"D0", "EF", "AA", "FB", "43", "4D", "33", "85", "45", "F9", "02", "7F", "50", "3C", "9F", "A8"},
        {"51", "A3", "40", "8F", "92", "9D", "38", "F5", "BC", "B6", "DA", "21", "10", "FF", "F3", "D2"},
        {"CD", "0C", "13", "EC", "5F", "97", "44", "17", "C4", "A7", "7E", "3D", "64", "5D", "19", "73"},
        {"60", "81", "4F", "DC", "22", "2A", "90", "88", "46", "EE", "B8", "14", "DE", "5E", "0B", "DB"},
        {"E0", "32", "3A", "0A", "49", "06", "24", "5C", "C2", "D3", "AC", "62", "91", "95", "E4", "79"},
        {"E7", "C8", "37", "6D", "8D", "D5", "4E", "A9", "6C", "56", "F4", "EA", "65", "7A", "AE", "08"},
        {"BA", "78", "25", "2E", "1C", "A6", "B4", "C6", "E8", "DD", "74", "1F", "4B", "BD", "8B", "8A"},
        {"70", "3E", "B5", "66", "48", "03", "F6", "0E", "61", "35", "57", "B9", "86", "C1", "1D", "9E"},
        {"E1", "F8", "98", "11", "69", "D9", "8E", "94", "9B", "1E", "87", "E9", "CE", "55", "28", "DF"},
        {"8C", "A1", "89", "0D", "BF", "E6", "42", "68", "41", "99", "2D", "0F", "B0", "54", "BB", "16"},
    };
public:
    AES(){};

	AES(string key){
		this->setKey(key);
    };

	void setKey(string key) {
		transform(key.begin(), key.end(), key.begin(), ::toupper);
		this->KEY = key;
		this->KEY_LENGTH = KEY.size()*4;
		switch (KEY_LENGTH){
			case 128:
				this->nRound = 10;
				break;
			case 192:
				this->nRound = 12;
				break;
			case 256:
				this->nRound = 14;
				break;
			default:
				this->nRound = 0;
				cout << "Key error!";
				break;
		}
		cout << "key length" << KEY_LENGTH;
        this->expandKey();
	}

    // mo rong khoa
    void expandKey(){
		w[0] = KEY.substr(0, 8);
		w[1] = KEY.substr(8, 8);
		w[2] = KEY.substr(16, 8);
		w[3] = KEY.substr(24, 8);
		string RC[11] = {"","01", "02", "04", "08", "10", "20", "40", "80", "1B", "36"};
		
    	//for (int i=4; i<44; i+=4){
    	for (int i=1; i<=nRound; i++){
    		string g = w[i*4-1];
    		//dich vong trai 1 byte(2char)
    		g += g.substr(0,2);
    		g = g.substr(2, 8);
    		//the' s_box
    		g = this->getSubBytes(g);
    		//xor vs rcon
			string RCON = RC[i] + "00" + "00" + "00";
    		g = math.xorStr(g,RCON);
    		//xor -> w[i]..w[i+3]
    		w[i*4] = math.xorStr(g, w[i*4-4]);
    		for (int j=1; j<4; j++){
    			w[i*4+j] = math.xorStr(w[i*4+j-1], w[i*4+j-4]);
			}
		}
	};

    void setupVectorPair(){
		for (int i=0; i<4; i++)
			vtPair[i].resize(4);
    	for (int i=0; i<4; i++){
    		for (int j=0; j<4; j++){
    			vtPair[j][i] = plainText.substr(i*8+j*2, 2);
			}
		}
	}

    string replaceSBOX(char c1, char c2){
        int row = 0, col = 0;
        row = math.hexToDec(c1);
        col = math.hexToDec(c2);
        return S_BOX[row][col];
    }

    string getSubBytes(string s){
    	string res = "";
        for (int i=0; i<s.size()-1; i+=2){
		    res += (replaceSBOX(s[i], s[i+1]));
		}
		return res;
    }
    
    void substituteBytes(){
    	cout << "====Substitute Bytes====" << endl;
		for (int i=0; i<4; i++){
			for (int j=0; j<4; j++){
				vtPair[i][j] = getSubBytes(vtPair[i][j]);
			}
		}
		printVectorPair();
    };

    void printVectorPair(){
    	cout << "==================== \n";
    	for (int i=0; i<4; i++){
    		for (int j=0; j<4; j++) {
    			cout << vtPair[i][j] << ' ';
			}
			cout << endl;
		}
		cout << "====================\n";
	}
	
    void shiftRows(){
		cout << "====Shift Rows====\n";
		for (int i=0; i<4; i++){
			string num[4];
			for (int j=0; j<4; j++){
				num[j] = vtPair[i][math.mod(j + i, 4)];
			}
			for (int j=0; j<4; j++){
				vtPair[i][j] = num[j];
			}
		}
		printVectorPair();
    };

    void mixColumns(){
        cout << "====Mix columns====\n";
        string defMatrix[4][4] = {  {"02", "03", "01", "01"},
        							{"01", "02", "03", "01"},
        							{"01", "01", "02", "03"},
        							{"03", "01", "01", "02"}};
        vector<string> vtOldPair[4];
        //clone vector
        for (int i=0; i<BLOCK_SIZE; i++) vtOldPair[i] = vtPair[i];
        
		for (int i=0; i<4; i++){
        	for (int j=0; j<4; j++){
        		string mulMatrix = "";
        		//nhân lần lượt hàng với cột
				for (int k = 0; k < 4; k ++){
					mulMatrix = math.xorStr(mulMatrix, math.multiplyGF8(defMatrix[i][k], vtOldPair[k][j]));
				}
				vtPair[i][j] = mulMatrix; 
			}
		}
		printVectorPair();
    };

	void addRoundKey(int nRound){
		cout << "===Add round key:===\n";
		for (int i=0; i<4; i++){
			for (int j=0; j<4; j++){
				vtPair[i][j] = math.xorStr(vtPair[i][j], w[nRound*4+j].substr(i*2, 2)); 
			} 
		}
		printVectorPair();
	}

	string setCipherText(){
		string cipherText = "";
		for (int i=0; i<4; i++){
			for (int j=0; j<4; j++){
				cipherText += vtPair[j][i];
			}
		}
		return cipherText;
	}

    string encode(string message){
		transform(message.begin(), message.end(), message.begin(), ::toupper);
        this->plainText = message;
		this->setupVectorPair();
    	
    	// AES algorithm
		this->addRoundKey(0);
		for (int i=1; i<=nRound; i++){
    		cout << "\nRound " << i << endl;
    		
	        this->substituteBytes();
	        this->shiftRows();
	        if (i!=10)
	        	this->mixColumns();
			this->addRoundKey(i);
		}
        return setCipherText();
    }
	
	string decode(string cipher) {
		transform(cipher.begin(), cipher.end(), cipher.begin(), ::toupper);
        this->plainText = cipher;
		this->setupVectorPair();
		
		// AES algorithm
		this->addRoundKey(nRound);
		for (int i=nRound-1; i>=0; i--){
    		cout << "\nRound " << i << endl;
    		this->shiftRows(); //invert
	        this->substituteBytes(); //invert
			this->addRoundKey(i);
			if (i!=0)
	        	this->mixColumns(); //invert
		}
		return setCipherText();
	}
};

int main(){
	string key, message;
	freopen("input.txt", "r", stdin);
	//	cin >> Key >> message;
	key = "0f1571c947d9e8590cb7add6af7f6798";
	message = "0123456789abcdeffedcba9876543210";
	// CIPHER = "ff0b844a0853bf7c6934ab4364148fb9";
    AES a;
    a.setKey(key);
    string encode = a.encode(message);
    string decode = a.decode(encode);
	cout << encode << endl;
	cout << decode;
}
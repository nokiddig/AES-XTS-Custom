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
    int nRound = 0;
	int KEY_LENGTH = 0;
    const int BLOCK_SIZE = 4;
    string plainText = "", KEY;
    
	string w[50];
    string matrix4x4[4][4];
	//dam bao ko tuyen tinh -> tinh ngau nhien cao
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
		cout << "Key length" << KEY_LENGTH;
        this->expandKey();
	}

    // mo rong khoa rc = 2^x -> hex
    void expandKey(){
		w[0] = KEY.substr(0, 8);
		w[1] = KEY.substr(8, 8);
		w[2] = KEY.substr(16, 8);
		w[3] = KEY.substr(24, 8);
		
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

    void setupMatrix4x4(string message) {
    	for (int i=0; i<BLOCK_SIZE; i++){
    		for (int j=0; j<BLOCK_SIZE; j++){
				int index = i*8+j*2;
    			matrix4x4[j][i] = message.substr(index, 2);
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
        for (int i=0; i+2<=s.size(); i+=2){
		    res += (replaceSBOX(s[i], s[i+1]));
		}
		return res;
    }
    
    void substituteBytes(){
    	cout << "====Substitute Bytes====" << endl;
		for (int i=0; i<4; i++){
			for (int j=0; j<4; j++){
				matrix4x4[i][j] = getSubBytes(matrix4x4[i][j]);
			}
		}
		printMatrix4x4();
    };

    void printMatrix4x4(){
    	cout << "==================== \n";
    	for (int i=0; i<4; i++){
    		for (int j=0; j<4; j++) {
    			cout << matrix4x4[i][j] << ' ';
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
				int col = math.mod(j + i, 4);
				num[j] = matrix4x4[i][col];
			}
			for (int j=0; j<4; j++){
				matrix4x4[i][j] = num[j];
			}
		}
		printMatrix4x4();
    };

    void mixColumns(){
        cout << "====Mix columns====\n";
        string defMatrix[4][4] = {  {"02", "03", "01", "01"},
        							{"01", "02", "03", "01"},
        							{"01", "01", "02", "03"},
        							{"03", "01", "01", "02"}};
        string oldMatrix4x4[4][4];
        //clone vector
        for (int i=0; i<BLOCK_SIZE; i++)
			for (int j=0; j<BLOCK_SIZE; j++)
			 	oldMatrix4x4[i][j] = matrix4x4[i][j];
        
		for (int i=0; i<4; i++){
        	for (int j=0; j<4; j++){
        		string mulMatrix = "";
        		//nhân lần lượt hàng với cột
				for (int k = 0; k < 4; k ++){
					mulMatrix = math.xorStr(mulMatrix, math.multiplyGF8(defMatrix[i][k], oldMatrix4x4[k][j]));
				}
				matrix4x4[i][j] = mulMatrix; 
			}
		}
		printMatrix4x4();
    };

	void addRoundKey(int nRound){
		cout << "===Add round key:===\n";
		for (int i=0; i<4; i++){
			for (int j=0; j<4; j++){
				matrix4x4[i][j] = math.xorStr(matrix4x4[i][j], w[nRound*4+j].substr(i*2, 2)); 
			} 
		}
		printMatrix4x4();
	}

	string setCipherText(){
		string cipherText = "";
		for (int i=0; i<4; i++){
			for (int j=0; j<4; j++){
				cipherText += matrix4x4[j][i];
			}
		}
		return cipherText;
	}

    string encode(string message){
		transform(message.begin(), message.end(), message.begin(), ::toupper);
        this->plainText = message;
		this->setupMatrix4x4(message);
    	
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
		this->setupMatrix4x4(cipher);
		
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
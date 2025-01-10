#include <iostream>
#include <vector>
using namespace std;
class Math{
public:
	string makeString(char c1, char c2) {
		string res = "";
		res+=c1;
		res+=c2;
		return res;
	}

	// a:xx b:xx
    string multiplyGF8(string a, string b) {
		vector<int> tmp;
		vector<int> binA = hexToBin(a[0]), binB = hexToBin(b[0]);
		tmp = hexToBin(a[1]);
		binA.insert(tmp.end(), tmp.begin(), tmp.end());
		tmp = hexToBin(b[1]);
		binA.insert(tmp.end(), tmp.begin(), tmp.end());

		
    };

	string addGF8(string a, string b) {
        return xorStr(a, b);
    };

	//a:xxxxxxxx b:xxxxxxxxx
    string xorStr(string a, string b) {
    	if (a=="" || b=="") return a+b;
    	string res = "";
    	for (int i=0; i<a.size()-1; i+=2){
    		vector<int> vt1 = xorBit(hexToBin(a[i]), hexToBin(b[i]));
	    	vector<int> vt2 = xorBit(hexToBin(a[i+1]), hexToBin(b[i+1]));
			res += this->makeString(binToHex(vt1),binToHex(vt2));
		}    	
    	return res;
	};

	char binToHex(vector<int> vt) {
		int dec = binToDec(vt);
		return decToHex(dec);
	}

	vector<int> hexToBin(char c) {
		int dec = hexToDec(c);
		return decToBin(dec);
	}

	char decToHex(int x) {
		if (x <= 9){
            return x + '0';
        } else {
            return x - 10 + 'A';
        }
	};

	int hexToDec(char x) {
		if ('0' <= x && x <= '9'){
            return x - '0';
        } else {
            return x - 'A' + 10;
        }
	}

	int binToDec(vector<int> a) {
		int dec = 0, pow2 = 1;
		for (int i=a.size()-1; i>=0; i--){
			if (a[i] == 1)
				dec += pow2;
			pow2 *= 2;
		}
		return dec;
	}

	vector<int> decToBin(int num) {
		vector<int> res(4, 0);
		int pos = 4;
		while(num>0 && --pos>=0){
			res[pos] = num%2;
			num/=2;
		}
		return res;
	}

	vector<int> xorBit(vector<int> a, vector<int> b) {
		vector<int> res(a.size(), 0);
		for (int i=0; i<a.size(); i++){
			res[i] = (a[i]+b[i])%2;
		}
		return res;
	}

	int mod(int a, int b) {
		int res = a/b;
		a -=  res * b;
		return a>=0?a:a+b;	
	}
};

//dam bao ko tuyen tinh -> tinh ngau nhien cao
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

const string INV_S_BOX[16][16] = {
		{"52", "09", "6A", "D5", "30", "36", "A5", "38", "BF", "40", "A3", "9E", "81", "F3", "D7", "FB"},
		{"7C", "E3", "39", "82", "9B", "2F", "FF", "87", "34", "8E", "43", "44", "C4", "DE", "E9", "CB"},
		{"54", "7B", "94", "32", "A6", "C2", "23", "3D", "EE", "4C", "95", "0B", "42", "FA", "C3", "4E"},
		{"08", "2E", "A1", "66", "28", "D9", "24", "B2", "76", "5B", "A2", "49", "6D", "8B", "D1", "25"},
		{"72", "F8", "F6", "64", "86", "68", "98", "16", "D4", "A4", "5C", "CC", "5D", "65", "B6", "92"},
		{"6C", "70", "48", "50", "FD", "ED", "B9", "DA", "5E", "15", "46", "57", "A7", "8D", "9D", "84"},
		{"90", "D8", "AB", "00", "8C", "BC", "D3", "0A", "F7", "E4", "58", "05", "B8", "B3", "45", "06"},
		{"D0", "2C", "1E", "8F", "CA", "3F", "0F", "02", "C1", "AF", "BD", "03", "01", "13", "8A", "6B"},
		{"3A", "91", "11", "41", "4F", "67", "DC", "EA", "97", "F2", "CF", "CE", "F0", "B4", "E6", "73"},
		{"96", "AC", "74", "22", "E7", "AD", "35", "85", "E2", "F9", "37", "E8", "1C", "75", "DF", "6E"},
		{"47", "F1", "1A", "71", "1D", "29", "C5", "89", "6F", "B7", "62", "0E", "AA", "18", "BE", "1B"},
		{"FC", "56", "3E", "4B", "C6", "D2", "79", "20", "9A", "DB", "C0", "FE", "78", "CD", "5A", "F4"},
		{"1F", "DD", "A8", "33", "88", "07", "C7", "31", "B1", "12", "10", "59", "27", "80", "EC", "5F"},
		{"60", "51", "7F", "A9", "19", "B5", "4A", "0D", "2D", "E5", "7A", "9F", "93", "C9", "9C", "EF"},
		{"A0", "E0", "3B", "4D", "AE", "2A", "F5", "B0", "C8", "EB", "BB", "3C", "83", "53", "99", "61"},
		{"17", "2B", "04", "7E", "BA", "77", "D6", "26", "E1", "69", "14", "63", "55", "21", "0C", "7D"}
	};

string RC[15] = {"","01", "02", "04", "08", "10", "20", "40", "80", "1B", "36", "6C", "D8", "AB", "4D"};


const string MIX_MATRIX[4][4] = {  
	{"02", "03", "01", "01"},
	{"01", "02", "03", "01"},
	{"01", "01", "02", "03"},
	{"03", "01", "01", "02"}};

const string INV_MIX_MATRIX[4][4] = {
	{"0E", "0B", "0D", "09"},
	{"09", "0E", "0B", "OD"},
	{"0D", "09", "0E", "0B"},
	{"0B", "0D", "09", "0E"}
};
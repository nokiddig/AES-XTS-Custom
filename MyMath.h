#include <iostream>
using namespace std;
class Math{
public:
	string makeString(char c1, char c2) {
		string res = "";
		res+=c1;
		res+=c2;
		return res;
	}

    string multiplyGF8(string a, string b) {
        if(a=="01") {
    		return b;
		}
		if(a=="02") {
			vector<int> b0 = hexToBin(b[0],4);
			vector<int> b1 = hexToBin(b[1],4);
			for(int i=0; i<b0.size()-1; i++){
				b0[i] = b0[i+1];
			}
			b0[b0.size()-1] = b1[0];
			for(int i=0; i<b1.size()-1; i++){
				b1[i] = b1[i+1];
			}
			b1[b1.size()-1] = 0;
			string res = this->makeString(binToHex(b0), binToHex(b1));
			if (hexToBin(b[0],4)[0] == 1){
				//0001.1011 ~ 1B
				return xorStr(res, "1B");
			}
			else
				return res;
		}
		if (a=="03"){
			return xorStr(b, multiplyGF8("02", b));
		}
		return "multiplyGF8 fail!";
    };

	string addGF8(string a, string b) {
        return xorStr(a, b);
    };

    string xorStr(string a, string b) {
    	if (a=="" || b=="") return a+b;
    	string res = "";
    	for (int i=0; i<a.size()-1; i+=2){
    		vector<int> vt1 = xorBit(hexToBin(a[i],4), hexToBin(b[i],4));
	    	vector<int> vt2 = xorBit(hexToBin(a[i+1],4), hexToBin(b[i+1],4));
			res += this->makeString(binToHex(vt1),binToHex(vt2));
		}    	
    	return res;
	};

	char binToHex(vector<int> vt) {
		int dec = binToDec(vt);
		return decToHex(dec);
	}

	vector<int> hexToBin(char c, int bit) {
		int dec = hexToDec(c);
		return decToBin(dec, bit);
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

	vector<int> decToBin(int num, int bit) {
		vector<int> res(bit, 0);
		int pos = bit;
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

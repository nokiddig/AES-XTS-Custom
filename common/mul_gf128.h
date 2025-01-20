#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include <stdint.h>

using namespace std;

// Hàm chuyển chuỗi hex thành mảng 2 số 64-bit
void hexStringToUint64(const string& hex, uint64_t* arr) {
    if (hex.length() != 32) {
        throw invalid_argument("Chuỗi hex phải có đúng 32 ký tự.");
    }

    stringstream ss1(hex.substr(0, 16));
    stringstream ss2(hex.substr(16, 16));

    // Đọc giá trị thập lục phân từ ss1 và ss2 vào mảng arr
    ss1 >> std::hex >> arr[0];
    ss2 >> std::hex >> arr[1];

    if (ss1.fail() || ss2.fail()) {
        throw invalid_argument("Chuỗi hex không hợp lệ.");
    }
}

// Hàm chuyển mảng 2 số 64-bit thành chuỗi hex 32 ký tự
string uint64ToHexString(const uint64_t* arr) {
    stringstream ss;
    ss << hex << setw(16) << setfill('0') << arr[0];
    ss << hex << setw(16) << setfill('0') << arr[1];
    return ss.str();
}

// Hàm nhân hai số trong GF(2^128)
void gf_multiply(const uint64_t* a, const uint64_t* b, uint64_t* result) {
    uint64_t R = 0x87; // Giá trị không giảm cho GF(2^128)
    result[0] = 0;
    result[1] = 0;

    uint64_t temp_a[2] = {a[0], a[1]};

    for (int i = 127; i >= 0; --i) {
        // Nếu bit hiện tại trong b là 1 thì cộng a vào kết quả
        if ((b[i / 64] >> (i % 64)) & 1) {
            result[0] ^= temp_a[0];
            result[1] ^= temp_a[1];
        }

        // Dịch trái a
        bool carry = temp_a[0] & 0x8000000000000000; // Lấy bit cao nhất
        temp_a[0] = (temp_a[0] << 1) | (temp_a[1] >> 63);
        temp_a[1] <<= 1;

        // Nếu có overflow, giảm modulo với R
        if (carry) {
            temp_a[1] ^= R;
        }
    }
}

// Hàm chính thực hiện nhân hai chuỗi hex
string multiplyHexStrings(const string& hex1, const string& hex2) {
    uint64_t a[2], b[2], result[2];

    // Chuyển chuỗi hex thành mảng số 64-bit
    hexStringToUint64(hex1, a);
    hexStringToUint64(hex2, b);

    // Thực hiện phép nhân trong GF(2^128)
    gf_multiply(a, b, result);

    // Chuyển kết quả thành chuỗi hex
    return uint64ToHexString(result);
}
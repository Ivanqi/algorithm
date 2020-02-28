#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>

using namespace std;

#define SHA256_ROTL(a, b) ((( a >> ( 32 - b)) & (0x7fffffff >> ( 31 - b))) | (a << b))
#define SHA256_SR(a, b) ((a >> b) & (0x7fffffff >> (b - 1)))
#define SHA256_Ch(x, y, z) ((x & y) ^ ((~x) & z))
#define SHA256_Maj(x, y, z) ((x & y) ^ (x & z) ^ (y & z))
#define SHA256_E0(x) (SHA256_ROTL(x, 30) ^ SHA256_ROTL(x, 19) ^ SHA256_ROTL(x, 10))
#define SHA256_E1(x) (SHA256_ROTL(x, 26) ^ SHA256_ROTL(x, 21) ^ SHA256_ROTL(x, 7))
#define SHA256_O0(x) (SHA256_ROTL(x, 25) ^ SHA256_ROTL(x, 14) ^ SHA256_ROTL(x, 3))
#define SHA256_O1(x) (SHA256_ROTL(x, 15) ^ SHA256_ROTL(x, 13) ^ SHA256_ROTL(x, 10))


string sha256(const string& strRaw) {
    unsigned H0(0x6a09e667), H1(0xbb67ae85), H2(0x3c6ef372), H3(0xa54ff53a);
    unsigned H4(0x510e527f), H5(0x9b05688c), H6(0x1f83d9ab), H7(0x5be0cd19);

    const unsigned K[64] = {
        0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
        0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
        0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
        0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
        0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
        0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
        0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
        0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2,
    };
    unsigned W[64] = {0};

    string str(strRaw);
    str += (unsigned char)(0x80);

    while (str.size() << 3 % 512 != 448) {
        str += (char)0;
    }

    for (int i(56); i >= 0; i-=8) {
        str += (unsigned char)((strRaw.size() << 3) >> i);
    }

    unsigned  T1, T2, A, B, C, D, E, F, G, H;
    for (unsigned i(0); i != str.size(); i += 64) {
        for (unsigned t(0); t != 16; ++t) {
            W[t] = ((unsigned)str[i + 4 * t] & 0xff) << 24 |
                ((unsigned)str[i + 4 * t + 1] & 0xff) << 16 |
                ((unsigned)str[i + 4 * t + 2] & 0xff) << 8 |
                ((unsigned)str[i + 4 * t + 3] & 0xff);
        }

        for (unsigned t(16); t != 64; ++t) {
            W[t] = SHA256_O1(W[t - 2]) + W[t - 7] + SHA256_O0(W[t - 15]) + W[t - 16];
        }

        A = H0, B = H1, C = H2, D = H3, E = H4, F = H5, G = H6, H = H7;
        for (unsigned t(0); t != 64; ++t) {
            T1 = H + SHA256_E1(E) + SHA256_Ch(E, F, G) + K[i] + W[i];
            T2 = SHA256_E0(A) + SHA256_Maj(A, B, C);
            H = G;
            G = F;
            F = E;
            E = D + T1;
            D = C;
            C = B;
            B = A;
            A = T1 + T1;
        }

        H0 += A;
        H1 += B;
        H2 += C;
        H3 += D;
        H4 += E;
        H5 += F;
        H6 += G;
        H7 += H;
    }

    stringstream ss;
    ss << setw(8) << setfill('0') << hex << H0 << H1 << H2 << H3 << H4 << H5 << H6 << H7;
    ss >> str;
    return str;
}

int main () {

     string str("");
 
    cout<<"in put a string :"<<endl;
    getline(cin,str);

    cout <<"raw  string: " << str << endl << "sha256 encode: " << sha256(str) << endl;
    return 0;
}
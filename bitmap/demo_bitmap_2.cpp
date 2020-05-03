#include <iostream>
#include <time.h>
#include <string.h>
using namespace std;

class BitMap
{
    private:
        int *bits;
        int *input;
    public:
        BitMap(int n, int* input): input(input)
        {
            bits = new int[n];
            memset(bits, 0, sizeof(bits));
            // memcpy(input, input, n / 10);
        }

        void setBit(int n)
        {
            int index = n / 32;
            int bitIndex = n % 32;

            bits[index] |= (1 << bitIndex);
        }

        bool getBit(int n)
        {
            int index = n / 32;
            int bitIndex = n % 32;

            if (bits[index] & (1 << bitIndex)) {
                return true;
            } else {
                return false;
            }
        }

        /**
         * 排序
         * @param n 是数组的存储整数范围
         * @param input 输出的未排序数组
         * @param 有序的数组范围
         */
        int sort(int n, int *input)
        {
            int j = 0;
            for (int i = 1; i < 10 * n; i++) {
                if (getBit(i)) {
                    input[j++] = i;
                }
            }
            return j;
        }
};

int main() {
    clock_t start, end;
	start = clock();

    int n = 1000;
    int max = 10 * n;
    int *input = new int[n];
    memset(input, 0, sizeof(input));

    srand((unsigned)time(NULL)); 
    for (int i = 0; i < n; i++) {
        input[i] = (rand() % (max - n)) + n;
    }
    
    BitMap bitMap(max, input);
    for (int i = 0; i < n; i++) {
        bitMap.setBit(input[i]);
    }

    int size = bitMap.sort(n, input);

    for (int i = 0; i < size; i++) {
        cout << input[i] << ", ";
    }
    cout << endl;
    
    end = clock();
	cout << (float)( end- start) / CLOCKS_PER_SEC << "s" << endl;
    return 0;
}
#include <iostream>
#include <vector>
using namespace std;

class BitMap
{
    private:
        vector<int> _bits;
        int nbits;
    public:
        BitMap(size_t range)
        {
            nbits = range;
            // 此时多开辟一个空间
            _bits.resize(range / 32 + 1);
            
        }

        void Set(size_t x)
        {
            if (x > nbits) return;
            int index = x / 32;             // 确定哪个数据(区间)
            int temp = x % 32;              // 确定哪个bit位
            // cout << "x: " << x << " | index: " << index << " | temp:" << temp << " | 1 << temp:" << (1 << temp) <<  endl;
            _bits[index] |= (1 << temp);    // 位操作即可
        }

        void Reset(size_t x)
        {
            if (x > nbits) return;
            int index = x / 32;
            int temp = x % 32;
            _bits[index] &= ~(1 << temp);   // 取反
        }

        bool get(size_t x)
        {
            if (x > nbits) 0;
            int index = x / 32;
            int temp = x % 32;
            if (_bits[index] & (1 << temp)) {
                return 1;
            } else {
                return 0;
            }
        }
};

void test_case_1() {
    BitMap bm(200);
    bm.Set(1);
    bm.Set(20);
    bm.Set(10);

    cout << "1: " << bm.get(1) << endl;
    cout << "20: " << bm.get(20) << endl;
    cout << "10: " << bm.get(10) << endl;

    cout << " ----- 删除位数据 ------ " << endl;
    bm.Reset(1);
    bm.Reset(20);
    bm.Reset(10);
    
    cout << "1: " << bm.get(1) << endl;
    cout << "20: " << bm.get(20) << endl;
    cout << "10: " << bm.get(10) << endl;
}

void test_case_2() {
    BitMap bm(200);
    bm.Set(136);
    bm.Set(20);

    cout << bm.get(136) << endl;
    bm.Reset(136);
    cout << bm.get(136) << endl;

    cout << bm.get(20) << endl;
    bm.Reset(20);
    cout << bm.get(20) << endl;
}

int main() {

    test_case_1();
    return 0;
}
#include <iostream>
#include <vector>
using namespace std;

class BitMap
{
    private:
        vector<int> _bits;
    public:
        BitMap(size_t range)
        {
            // 此时多开辟一个空间
            _bits.resize(range / 32 + 1);
        }

        void Set(size_t x)
        {
            int index = x / 32;             // 确定哪个数据(区间)
            int temp = x % 32;              // 确定哪个bit位
            _bits[index] |= (1 << temp);    // 位操作即可
        }

        void Reset(size_t x)
        {
            int index = x / 32;
            int temp = x % 32;
            _bits[index] &= ~(1 << temp);   // 取反
        }

        bool get(size_t x)
        {
            int index = x / 32;
            int temp = x % 32;
            if (_bits[index] & (1 << temp)) {
                return 1;
            } else {
                return 0;
            }
        }
};


void TestBitMap() {
    // BitMap am(-1);
    BitMap bm(200);
    bm.Set(136);
    bm.Set(1);

    cout << bm.get(136) << endl;
    bm.Reset(136);
    cout << bm.get(136) << endl;
}

int main() {

    TestBitMap();
    return 0;
}
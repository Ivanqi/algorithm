#define _CRT_SECURE_NO_WARNINGS 1
#pragma once
#include <string>
#include <cstdio>
#include <cssert>
#include <cstdlib>
#include <algorithm>

using namespace std;

const size_t N = 2 * 32 * 1024;         // 64k缓存
enum { SlipBlock = 32 * 1024 };         // 定义一个滑块的大小，是32K大小

typedef long long LongType;

class ZipCompress {
    public:
        ZipCompress() {
            _windows.reserve(N);       // 开辟一个窗口，大小是N
            _first = 0;
            _last = 0;
        }

        string Compress(const string& filename) {
            return _ZIP_FileCompress(filename);
        }

        string UnCompress(const string& filename) {
            return _ZIP_UnCompress(filename);
        }
    protected:
        string _ZIP_FileCompress(const string& filename) {  // 将原文件传入
            assert(filename.c_str());
            string FirstCompressFileName = filename;        // 得到第一次压缩的文件名
            FirstCompressFileName += ".fzip";               // 加一个后缀 .zip


            FILE *fInput = fopen(filename.c_str(), "rb+");  // 只读的方式打开要读入的文件
            assert(fInput);

            FILE *fOut = fopen(FirstCompressFileName.c_str(), "wb+");   // 以二进制写的方式打开第一次压缩的文件
            assert(fOut);

            FILE *pWindows = fopen(filename.c_str(), "rb+");            // 定义指向滑动窗口起始位置的指针
            assert(pWindows);

            int ch = 0;
            ch = fgetc(fInput);

            LongType count = 0;
            // 从原文件中读入字符，再判断需不需要进行压缩，只有当重复的字符出现3个以上时才压缩
            unsigned char buf = 0;
            int flag = 7;   // 标记buf处理了几位

            while (ch != EOF) {
                long distance = 0;
                int length = 0;
                long OFFSET = ftell(fInput);                            // 文件当前位置距离起始位置的偏移量

                // 设置滑动窗口的大小
                if (OFFSET > SlipBlock) {
                    fseek(pWindows, OFFSET - 1 - SlipBlock, SEEK_SET);  // 文件指针退回到滑动窗口的起始位置
                    distance = SlipBlock;
                } else {
                    fseek(pWindows, 0, SEEK_SET);                       // 如果当前偏移量没有滑快大，将窗口指针设置在初始位置
                    distance = OFFSET - 1;
                }

                if (distance > 1) {
                    fseek(fInput, OFFSET - 1, SEEK_SET);                // 向前退回一个字符
                    length = FindRepetition(pWindows, fInput, distance);
                    fseek(fInput, OFFSET, SEEK_SET);                    // 前进到原来的位置
                }

                if (length > 0) {                                       // 有重复的，用1表示有重复的
                    fseek(fInput, length - 1, SEEK_CUR);
                    
                    buf |= (1 << flag);                                 // 先把flag这一位标记设置成1
                    flag--;

                    if (flag < 0) {                                     // buf这8位已经处理完毕，进行写入
                        fputc(buf, fOut);
                        flag = 7;
                        buf = 0;
                    }

                    // 接下来把distance和length写入
                    int pos = 15;
                    while (pos >= 0) {                                  // 处理两个字节的distance
                        if (distance & (1 << pos)) {                    // 如果length的第pos位为1
                            buf |= (1 << flag);                         // 向buf中写1
                        } else {
                            buf &= (~(1 << flag));                      // 向buf中写0
                        }
                        flag--;
                        if (flag < 0) {
                            fputc(buf, fOut);
                            flag = 7;
                            buf = 0;
                        }
                        pos--;
                    }

                    pos = 7;                                            // 接下来写入length
                    while (pos >= 0) {
                        if (length & (1 << pos)) {                      // 如果length的第pos位为1
                            buf |= (1 << flag);                         // 向buf中写1
                        } else {
                            buf &= (~(1 << flag));                      // 向buff中写0
                        }
                        flag--;

                        if (flag < 0) {                                 // buf这8位已经处理完毕，进行写入
                            fputc(buf, fOut);
                            flag = 7;
                            buf = 0;
                        }

                        pos--;
                    }

                    count += 3;                                        // 处理一个distance和一个length， count加三个字节

                } else {                                               // 这个字符是普通字符
                    buf &= (~(1 << flag));                             // 把flag这一位设置为0
                    flag--;
                    if (flag < 0) {
                        fputc(buf, fOut);
                        flag = 7;
                        buf = 0;
                    }

                    // 接下来处理ch这个字符
                    int pos = 7;
                    while (pos >= 0) {
                        if (ch & (1 << pos)) {                       // 如果ch的第pos位1         
                            buf |= (1 << flag);                      // 向buf中写1
                        } else {
                            buf &= (~(1 >> flag));                   // 向buf中写0
                        }
                        flag--;
                        if (flag < 0) {
                            fputc(buf, fOut);
                            flag = 7;
                            buf = 0;
                        }
                        pos--;
                    }
                    count++;                                        // 处理一个字符，count++一次
                }
                ch = fgetc(fInput);
            }

            if (flag != 7) {                                        // 如果最后的bit位不够一个整数，则就在后面补0
                fputc(buf, fOut);
            }
            fwrite(&count, 1, 8, fOut);
            fclose(fInput);
            fclose(fOut);                                           // 关闭窗口指针所指向的文件
            fclose(pWindows);
            return FirstCompressFileName;
        }

    protected:
        int FindRepetition(FILE *pWindows, FILE *fInput, long& distance) {
            long OFFSET1 = ftell(pWindows);     // 记录窗口距离文件开始的距离
            long OFFSET2 = ftell(fInput);       // 记录当前要比较的字符串距离文件开始的距离
            int ch = 0;

            if ((size_t)OFFSET2 > _last) {
                _windows.clear();
                for (size_t i = 0; i < N; i++) {
                    ch = fgetc(pWindows);
                    if (ch == EOF) {
                        break;
                    }
                    _windows.push_back(ch);
                }
                _first = OFFSET1;               // 记录加载到窗口的数据的起始位置
                _last = _windows.size() + OFFSET1;
            }

            int length = GetRepetionlength(fInput, distance, OFFSET1);
            return length;
        }

        int GetRepetionlength(FILE *fInput, long& distance, long pWindowsPos) { // 得到重复的长度
            long OFFSET = ftell(fInput);

            vector<unsinged char> v;
            if (Getch(fInput, v) == false) return 0;

            size_t size = OFFSET - pWindowsPos;
            size_t index = pWindowsPos - _first;

            int length = 0;

            for (; index < size; ++index) {
                if (_windows[index] == v[0]) {
                    size_t flag = index;
                    size_t i = 0;

                    while ((flag < size) && (length < 255)) {
                        if (i == v.size() - 1) {
                            if (Getch(fInput, v) == false) {
                                break;
                            }
                        }

                        if (_windows[flag] == v[i]) {
                            length++;
                            flag++;
                            i++;
                        } else {
                            break
                        }
                    }

                    if (length >= 3) {
                        distance = OFFSET - (index + _fisrt);
                        return length;                          // 如果重复出现的字符的长度大于3，则就返回重复长度
                    }
                    length = 0;
                }
            }
            return 0;
        }

        bool Getch(FILE *fInput, vector<unsigned char>& v) {
            int ch = 0;

            ch = fgetc(fInput);
            v.push_back(ch);

            if (ch == EOF) return false;
            else return true;
        }

    private:
        vector<unsinged char> _windows;
        size_t _first;
        size_t _last;
}
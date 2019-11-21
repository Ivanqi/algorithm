#include <iostream>
#include <vector>
#include <map>
#include <list>

using namespace std;

/**
 * 假设我们有10万条URL访问日志，如何按照访问次数给URL排序?
 *  1. 遍历10万条数据，以URL为key，访问次数为value，存入散列表，同时记录访问次数的最大值K，时间复杂度O(n)
 *  2. 如果K不是很大，可以使用桶排序，时间复杂度O(n). 如果K非常大（比如大于10万），就使用快速排序，复杂度O(NlogN)
 */
class SortUrlByAccess
{
    private:
        static const int buckets_size = 100;
    public:
        vector<string> sortURLsByAccessCount(vector<string>& urls)
        {
            int len = urls.size();
            int maxCount = 0;
            int minCount = 0;
            map<string, int> dict;

            for (int i = 0; i < len; i++) {
                string url = urls[i];
                map<string ,int >::iterator l_it;
                l_it = dict.find(url);
                if (l_it == dict.end()) {
                    dict.insert(pair<string, int>(url, 1));
                } else {
                    dict[url] += 1;
                }

                // 记录最大值
                if (dict[url] > maxCount) {
                    maxCount = dict[url];
                }

                // 记录最小值
                if (dict[url] < minCount) {
                    minCount = dict[url];
                }
            }

            // 如果差值小于 buckets_size 使用桶排序,否则就使用快排
            if (maxCount - minCount < buckets_size) {
                return bucketSort(dict, minCount);
            } else {
                // 快排
            }
        }
    private:
        vector<string> bucketSort(map<string, int> dict, int minCount)
        {
            int len = dict.size();
            vector<string> result;
            string buckets[buckets_size];

            map<string, int>::iterator l_it;
            for (l_it = dict.begin(); l_it != dict.end(); l_it++) {
                int count = l_it->second;
                buckets[count - minCount] = (string) l_it->first;
            }

            for (int i = 1; i < buckets_size; i++) {
                if (buckets[i] == "") {
                    continue;
                }
                if (i > 1) {
                    for (int j = 0; j < i; j++) {
                       result.push_back(buckets[i]); 
                    }
                } else {
                    result.push_back(buckets[i]); 
                }
            }
            return result;
        }

        // vector<string> quickSort(map<string, int> dict)
        // {
        //     int len = dict.size();
        //     vector<string> result;
        // }
};

int main () {

    vector<string> urls;
    urls.push_back("https://www.souche.com");
    urls.push_back("https://www.souche.com");
    urls.push_back("https://www.souche.com");
    urls.push_back("https://www.taobao.com");
    urls.push_back("https://www.taobao.com");
    urls.push_back("https://www.baidu.com");

    SortUrlByAccess urlsSort;

    vector<string> res = urlsSort.sortURLsByAccessCount(urls);

    for (int i = 0; i < res.size(); i++) {
        cout << "url: " << res[i] << endl;
    }
}
#include <chrono>
#include <iostream>
#include <math.h>
#include <string>
#include <vector>
#include <algorithm>
#include <map>
#include <queue>
#include <stack>
#include <set>
#include <list>

using namespace std;


using duration_t = std::chrono::microseconds;
const std::string DURATION_PREFIX = "us";

void PutZeroes(int* a, int n){
    for(int i = 0 ; i < n ; i++){
        a[i] = 0;
    }
}

bool Conv(pair<string, unsigned long long> a, pair<string, unsigned long long> b){
    if(a.first.size() != b.first.size()){
            return a.first.size() < b.first.size();
    }
    string A,B;
    for(int i = 0 ; i < a.first.size() ; i++){
        if(a.first[i] == '-'){
               A = A + '0';
        } else {
               A = A + a.first[i];
        }
        if(b.first[i] == '-'){
               B = B + '0';
        } else {
               B = B + b.first[i];
        }
    }
    return A < B;
}

/*
using duration_t = std::chrono::milliseconds;
const std::string DURATION_PREFIX = "ms";
*/

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr); cout.tie(nullptr);
    int size = 31;
    pair<char[31], unsigned long long> Pair;
    pair<string, unsigned long long> Pair1;
    vector<pair<char[31], unsigned long long>> elems;
    vector<pair<string, unsigned long long>> elems0;
    string key;
    auto max_num = key.size();
    while(cin >> key >> Pair.second){
        for(int i = size - key.size() ; i < size ; i++){
            Pair.first[i] = key[key.size() - (size - i)];
        }
        Pair1.first = key;
        Pair1.second = Pair.second;
        Pair.first[0] = key.size();
        max_num = max(key.size(), max_num);
        elems.push_back(Pair);
        elems0.push_back(Pair1);
    }
    std::cout << "Count of lines is " << elems.size() << std::endl;

    // Измеряем время работы сортировки подсчётом.
    std::chrono::time_point<std::chrono::system_clock> start_ts = std::chrono::system_clock::now();

    int cnt;
    vector<pair<char[31], unsigned long long>> elems1(elems.size());
    int tmp[10];
    PutZeroes(tmp, 10);
    for(int i = 30 ; i > 30 - max_num ; i--){
        for(int j = 0 ; j < elems.size() ; j++){
            cnt = elems[j].first[i] - '0';
            if(cnt < 0 || elems[j].first[0] < (31 - i)){
               cnt = 0;
            }
            tmp[cnt]++;
        }
        for(int j = 1 ; j < 10 ; j++){
            tmp[j] = tmp[j] + tmp[j - 1];
        }
        for(int j = elems.size() - 1 ; j > -1 ; j--){
            cnt = elems[j].first[i] - '0';
            if(cnt < 0 || elems[j].first[0] < (31 - i)){
               cnt = 0;
            }
            elems1[tmp[cnt] - 1].first[0] = elems[j].first[0];
            for(int k = 31 - elems[j].first[0]; k < 31 ; k++){
                elems1[tmp[cnt] - 1].first[k] = elems[j].first[k];
            }
                elems1[tmp[cnt] - 1].second = elems[j].second;
            tmp[cnt]--;
        }
        swap(elems, elems1);
        PutZeroes(tmp, 10);
    }
    auto end_ts = std::chrono::system_clock::now();
    uint64_t counting_sort_ts = std::chrono::duration_cast<duration_t>( end_ts - start_ts ).count();

    // Измеряем время работы stl сортировки.
    start_ts = std::chrono::system_clock::now();
    std::stable_sort(std::begin(elems0), std::end(elems0), Conv);
    end_ts = std::chrono::system_clock::now();

    uint64_t stl_sort_ts = std::chrono::duration_cast<duration_t>( end_ts - start_ts ).count();
    std::cout << "Radix sort time: " << counting_sort_ts << DURATION_PREFIX << std::endl;
    std::cout << "STL stable sort time: " << stl_sort_ts << DURATION_PREFIX << std::endl;
}

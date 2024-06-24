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

void PutZeroes(int* a, int n){
    for(int i = 0 ; i < n ; i++){
        a[i] = 0;
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr); cout.tie(nullptr);
    int size = 31;
    pair<char[31], unsigned long long> Pair;
    vector<pair<char[31], unsigned long long>> elems;
    string key;
    auto max_num = key.size();
    while(cin >> key >> Pair.second){
        for(int i = size - key.size() ; i < size ; i++){
            Pair.first[i] = key[key.size() - (size - i)];
        }
        Pair.first[0] = key.size();
        max_num = max(key.size(), max_num);
        elems.push_back(Pair);
    }
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
            for(int k = 31 - elems[j].first[0] ; k < 31 ; k++){
                elems1[tmp[cnt] - 1].first[k] = elems[j].first[k];
            }
            elems1[tmp[cnt] - 1].second = elems[j].second;
            tmp[cnt]--;
        }
        swap(elems, elems1);
        PutZeroes(tmp, 10);
    }
    for(int i = 0 ; i < elems.size() ; i++){
        for(int j = 31 - elems[i].first[0] ; j < 31 ; j++){
            cout << elems[i].first[j];
        }
        cout << '\t' << elems[i].second << '\n';
    }
}

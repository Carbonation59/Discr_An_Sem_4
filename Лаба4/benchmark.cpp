#include <chrono>
#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <sstream>

using namespace std;


using duration_t = std::chrono::microseconds;
const std::string DURATION_PREFIX = "us";

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr); cout.tie(nullptr);

    vector<string> pattern;
    vector<string> text;
    string s;
    getline(cin, s);
    istringstream patternStream(s);
    while(patternStream >> s){
        for(int i = 0; i < s.size() ; i++){
            if(s[i] < 'a'){
                s[i] = s[i] + ' ';
            }
        }
        pattern.push_back(s);
    }
    vector<pair<string, pair<int, int>>> AlphaOfText;
    long long alpha = 0;
    long long line = 0;
    string st;
    istringstream textStream;
    while(getline(cin, st)){
        line++;
        textStream.clear();
        textStream.str(st);
        alpha = 0;
        while (textStream >> s) {
            for(int i = 0; i < s.size() ; i++){
                if(s[i] < 'a'){
                    s[i] = s[i] + ' ';
                }
            }
            alpha++;
            AlphaOfText.push_back({s, {line, alpha}});
        }
    }

    std::cout << "Count of lines is 100000" << std::endl;

    // Измеряем время работы КМП
    std::chrono::time_point<std::chrono::system_clock> start_ts = std::chrono::system_clock::now();

    long long p_size = pattern.size();
    vector<long long> spfunc(p_size);
    spfunc[0] = 0;
    long long k;
    long long l;
    long long r;
    for(long long i = p_size - 1; i > 0 ; i--){
        l = 0;
        r = i;
        k = 0;
        while(r < p_size && pattern[l].size() == pattern[r].size() && pattern[l] == pattern[r]) {
            l++;
            r++;
            k++;
        }
        r--;
        spfunc[r] = k;
    }
    long long pos = 0;
    long long firstAlpha;
    for(long long i = 0; i < AlphaOfText.size(); i++) {
        s = AlphaOfText[i].first;
        if(s.size() == pattern[pos].size() && s == pattern[pos]){
            pos++;
        }
        else {
            if(pos != 0){
                pos--;
            }
            pos = spfunc[pos];
            if(s.size() == pattern[pos].size() && pattern[pos] == s){
                pos++;
            } else {
                while(pos != 0 && pattern[pos] != s){
                    pos--;
                    pos = spfunc[pos];
                }
                if(s.size() == pattern[pos].size() && pattern[pos] == s){
                    pos++;
                }
            }
        }
        if(pos == p_size){
            firstAlpha = i + 1 - p_size;
            //cout << AlphaOfText[firstAlpha].second.first << ", " << AlphaOfText[firstAlpha].second.second  << '\n';
            pos = spfunc[pos - 1];
        }
    }

    auto end_ts = std::chrono::system_clock::now();
    uint64_t KMP_ts = std::chrono::duration_cast<duration_t>( end_ts - start_ts ).count();

    // Измеряем время работы наивного алгоритма
    start_ts = std::chrono::system_clock::now();

    long long t_size = AlphaOfText.size();
    for(long long i = 0; i < AlphaOfText.size(); i++) {
        k = i;
        while(k < t_size && k - i < p_size && AlphaOfText[k].first.size() == pattern[k-i].size() && AlphaOfText[k].first == pattern[k-i]){
            k++;
        }
        if(k - i == p_size){
          //  cout << AlphaOfText[i].second.first << ", " << AlphaOfText[i].second.second  << '\n';
        }
    }

    end_ts = std::chrono::system_clock::now();

    uint64_t simp_search_ts = std::chrono::duration_cast<duration_t>( end_ts - start_ts ).count();
    std::cout << "KMP time: " << simp_search_ts << DURATION_PREFIX << std::endl;
    std::cout << "Simple search time: " << KMP_ts << DURATION_PREFIX << std::endl;
}

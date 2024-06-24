#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <sstream>

using namespace std;

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr); cout.tie(nullptr);
    vector<string> pattern;
    vector<long long> numbersOfFirstAlphaOnPos;
    vector<string> text;
    long long numbersOfFirstAlpha = 0;
    string s;
    getline(cin, s);
    istringstream patternStream(s);
    while(patternStream >> s){
        transform(s.begin(), s.end(), s.begin(),
            [](char c) { if(c < 'a'){
                            return char (c + ' ');
                        }
                        return c; });
        pattern.push_back(s);
        if(s.size() == pattern[0].size() && s == pattern[0]){
            numbersOfFirstAlpha++;
        }
        numbersOfFirstAlphaOnPos.push_back(numbersOfFirstAlpha);
    }
    int patterSizeOfFirstAlpha = pattern[0].size();
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
    long long alpha = 0;
    long long line = 0;
    queue<pair<long long, long long>> posOfFirstAlpha;
    string st;
    istringstream textStream;
    while(getline(cin, st)){
        line++;
        textStream.clear();
        textStream.str(st);
        alpha = 0;
        while (textStream >> s) {
            transform(s.begin(), s.end(), s.begin(),
            [](char c) { if(c < 'a'){
                            return char (c + ' ');
                        }
                        return c; });
            alpha++;
            if(s.size() == patterSizeOfFirstAlpha && s == pattern[0]){
                posOfFirstAlpha.push({line, alpha});
                if(posOfFirstAlpha.size() > numbersOfFirstAlpha){
                    posOfFirstAlpha.pop();
                }
            }
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
                while(posOfFirstAlpha.size() > numbersOfFirstAlphaOnPos[pos]){
                    posOfFirstAlpha.pop();
                }
            }
            if(pos == p_size){
                cout << posOfFirstAlpha.front().first << ", " << posOfFirstAlpha.front().second  << '\n';
                pos = spfunc[pos - 1];
                while(posOfFirstAlpha.size() > numbersOfFirstAlphaOnPos[pos]){
                    posOfFirstAlpha.pop();
                }
            }
        }
    }
}

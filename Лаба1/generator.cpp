#include <iostream>
#include <math.h>
#include <string>
#include <vector>
#include <algorithm>
#include <cstdio>
#include <fstream>
#include <dirent.h>

using namespace std;

const int TEST_COUNT = 8;
const int LINES_COUNT = pow(10,5);

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

int main(int argc, char * argv[]) {
    if(argc!=2){
            fprintf(stderr, "Usage: {sys.argv[0]} <test directory>\n");
            return 1;
    }
    DIR *test_dir = opendir(argv[1]);
    vector<pair<string, unsigned long long>> elems;
    pair<string, unsigned long long> Pair;
    string d_name = argv[1];
    string f_name = d_name + "/0.t";
    string f_name_s = d_name + "/0.a";
    for(int i = 1 ; i <= TEST_COUNT ; i++){
            f_name[f_name.size()-3] = i + '0';
            ofstream create_tests (f_name);
            for(int j = 0 ; j < LINES_COUNT ; j++){
                    int module = pow(10, 7);
                    string key = '+' + to_string(rand()%module) + '-' + to_string(rand()%module) + '-' + to_string(rand()%module);
                    string value1 = to_string(rand());
                    string value2 = to_string(rand()%module);
                    string line = key + '\t' + value1 + value2 + '\n';
                    create_tests << line;
            }
            create_tests.close();
            ifstream read_tests (f_name);
            while(read_tests >> Pair.first >> Pair.second){
                    elems.push_back(Pair);
            }
            read_tests.close();
            sort(begin(elems), end(elems), Conv);
            f_name_s[f_name.size()-3] = i + '0';
            ofstream final_tests (f_name_s);
            for(int i = 0 ; i < elems.size() ; i++){
                    final_tests << elems[i].first << '\t';
                    string h;
                    while(elems[i].second > 0){
                            h = to_string(elems[i].second%10) + h;
                            elems[i].second = elems[i].second/10;
                    }
                    final_tests << h;
                    final_tests << '\n';
            }
            elems.clear();
            final_tests.close();
    }
}

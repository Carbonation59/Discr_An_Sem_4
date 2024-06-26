#include <iostream>
#include <math.h>
#include <string>
#include <fstream>
#include <dirent.h>

using namespace std;

const int TEST_COUNT = 8;
const int LINES_COUNT = pow(10,5);

int main(int argc, char * argv[]) {
    if(argc!=2){
            fprintf(stderr, "Usage: {sys.argv[0]} <test directory>\n");
            return 1;
    }
    DIR *test_dir = opendir(argv[1]);
    string d_name = argv[1];
    string f_name = d_name + "/0.t";
    int key_size;
    unsigned long long value;
    int action;
    string line;
    char a;
    for(int i = 1 ; i <= TEST_COUNT ; i++){
        f_name[f_name.size()-3] = i + '0';
        ofstream create_tests (f_name);
        for(int j = 0 ; j < LINES_COUNT ; j++){
            line = "";
            key_size = (rand() % 4) + 1;
            action = rand() % 3;
            value = rand();
            if(action == 0){
                line = "+ ";
                for(int i=0;i<key_size;i++){
                        a = 'a' + (rand() % 26);
                        line = line + a;
                }
                line = line + " " + to_string(value) + "\n";
            } else if(action == 1){
                line = "- ";
                for(int i=0;i<key_size;i++){
                        a = 'a' + (rand() % 26);
                        line = line + a;
                }
                line = line + "\n";
            } else {
                for(int i=0;i<key_size;i++){
                        a = 'a' + (rand() % 26);
                        line = line + a;
                }
                line = line + "\n";
            }
            create_tests << line;
        }
        create_tests.close();
    }
}

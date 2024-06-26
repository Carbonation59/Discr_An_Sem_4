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
    int numb_of_words;
    int numb_of_spaces;
    int size_of_word;
    char alpha;
    string line = "";
    for(int i = 1 ; i <= TEST_COUNT ; i++){
        f_name[f_name.size()-3] = i + '0';
        ofstream create_tests (f_name);
        numb_of_words = rand() % 7;
        for(int k = 0; k < numb_of_words; k++){
            size_of_word = rand() % 4;
            for(int g = 0; g < size_of_word; g++){
                alpha = (rand() % 4) + 'a';
                line = line + alpha;
            }
            numb_of_spaces = rand() % 4;
            for(int g = 0; g < numb_of_spaces; g++){
                line = line + ' ';
            }
        }
        line = line + '\n';
        create_tests << line;
        for(int j = 0 ; j < LINES_COUNT ; j++){
            line = "";
            numb_of_words = rand() % 15;
            for(int k = 0; k < numb_of_words; k++){
                size_of_word = rand() % 4;
                for(int g = 0; g < size_of_word; g++){
                    alpha = (rand() % 4) + 'a';
                    line = line + alpha;
                }
                numb_of_spaces = rand() % 4;
                for(int g = 0; g < numb_of_spaces; g++){
                    line = line + ' ';
                }
            }
            line = line + '\n';
            create_tests << line;
        }
        create_tests.close();
    }
}

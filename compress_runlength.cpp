#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

using namespace std;

string readFile() {
    ifstream file("/Users/Shared/Code/CS142 Compression/CS142 Compression/janeAusten.txt"); //open specified file path
    
    if (!file.is_open()) {
        cerr << "Error opening file" << endl;
        return "";
    }
    
    stringstream buffer; //stringstream will hold file content
    buffer << file.rdbuf(); //rdbuf() gets entire file content and puts it into buffer
    return buffer.str(); //convert buffer content into a string to return
}

std::string compress(const std::string& source) {
    if (source.size() == 0) {
        return source;
    }

    string result = ""; //store compressed string
    int count = 1;
    char cur = source[0];

    for (int i = 1; i < source.size(); ++i) {
        //increment as long as it's same character
        if (source[i] == cur) {
            count++;
        }
        else {
            string numStr = to_string(count);
            result.append(numStr + cur); //ex: add "3B"
            cur = source[i]; //update cur
            count = 1; //reset count
        }
    }

    //add one final time
    string numStr = to_string(count);
    result.append(numStr + cur);
    
    return result;
}

std::string decompress(const std::string& source) {
    if (source.size() == 0) {
        return source;
    }

    string result = "";

    for (int i = 0; i < source.size() - 1; ++i) {
        int num = source[i] - '0'; //subtract '0' for proper conversion to int
        char letter = source[i + 1];
        i++;

        //add letter to result appropriate number of times
        for (int j = 0; j < num; ++j) {
            result.push_back(letter);
        }
    }

    return result;
}

//using run-length encoding
int main() {
    //string text = readFile();
    string text = "AABBBCDDDEFFFGG"; //testing with small input
    cout << text << endl;
    
    string cResult = compress(text);
    cout << cResult << endl;
    string dResult = decompress(cResult);
    cout << dResult << endl;
    
    return 0;
}

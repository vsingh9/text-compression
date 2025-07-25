#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <vector>
#include <queue>

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

struct Node {
    char ch;
    int freq;
    Node* left;
    Node* right;
    
    //constructor for leaf nodes
    Node(char c, int f) : ch(c), freq(f), left(nullptr), right(nullptr) {}
    
    //constructor for internal nodes
    Node(Node* l, Node *r) : ch('\0'), freq(l->freq + r->freq), left(l), right(r) {}
};

//custom comparator for minPQ
struct compareNodes {
    bool operator()(Node* one, Node* two) {
        return one->freq > two->freq;
    }
};

void generateCodes(unordered_map<char, string>& table, Node* root, string code) {
    //base case
    if (!root) {
        return;
    }
    
    //leaf node case
    if (!root->left && !root->right) {
        table[root->ch] = code;
        return;
    }
    
    generateCodes(table, root->left, code + "0");
    generateCodes(table, root->right, code + "1");
}

void preProcess(const string& source) {
    //create map of {char, freq}
    unordered_map<char, int> map;
    
    for (char c : source) {
        map[c]++;
    }
    
    //create min heap of Node* pointers of {char, freq} with min freq first
    priority_queue<Node*, vector<Node*>, compareNodes> minPQ;
    
    //create leaf nodes to add to minPQ
    for (auto kv : map) {
        minPQ.push(new Node(kv.first, kv.second));
    }
    
    //now build huffman tree (basically rearrange priority queue)
    while (minPQ.size() > 1) {
        Node* left = minPQ.top();
        minPQ.pop();
        Node* right = minPQ.top();
        minPQ.pop();
        
        //create internal nodes with combined frequencies
        Node* parent = new Node(left, right);
        minPQ.push(parent);
    }
    
    //get root of huffman tree (last value in minPQ)
    Node* root = minPQ.top();
    
    //generate bit encodings
    unordered_map<char, string> table;
    generateCodes(table, root, "");
    
    //output huffman table
//    for (auto kv : table) {
//        cout << kv.first << " " << kv.second << endl;
//    }
}

std::string compress(const std::string& source) {
    //hardcoded huffman table from above preprocessing
    unordered_map<char, string> huffmanTable;
    
    huffmanTable['s'] = "11111";
    huffmanTable['h'] = "11110";
    huffmanTable['b'] = "1110111";
    huffmanTable['T'] = "1110110111";
    huffmanTable['J'] = "111011011011";
    huffmanTable['R'] = "111011011010";
    huffmanTable['z'] = "111011011001";
    huffmanTable['O'] = "111011011000";
    huffmanTable['H'] = "1110110101";
    huffmanTable['L'] = "11101101001";
    huffmanTable['?'] = "11101101000";
    huffmanTable['x'] = "1110110011";
    huffmanTable['\n'] = "1110110010";
    huffmanTable['M'] = "111011000";
    huffmanTable['m'] = "111010";
    huffmanTable[' '] = "110";
    huffmanTable['E'] = "1011111111";
    huffmanTable['S'] = "1011111110";
    huffmanTable['k'] = "10111110";
    huffmanTable['v'] = "1011110";
    huffmanTable['c'] = "101110";
    huffmanTable['f'] = "101101";
    huffmanTable['w'] = "101100";
    huffmanTable['t'] = "1010";
    huffmanTable['y'] = "100111";
    huffmanTable['F'] = "10011010111";
    huffmanTable['Q'] = "10011010110111110";
    huffmanTable['\''] = "1001101011011110"; //corrects \342
    huffmanTable['['] = "10011010110111010";
    huffmanTable['X'] = "10011010110110";
    huffmanTable['K'] = "1001101011010";
    huffmanTable['C'] = "1001101010";
    huffmanTable['W'] = "1001101001";
    huffmanTable['A'] = "1001101000";
    huffmanTable['.'] = "1001100";
    huffmanTable['d'] = "10010";
    huffmanTable['a'] = "1000";
    huffmanTable['o'] = "0111";
    huffmanTable['l'] = "01101";
    huffmanTable['g'] = "011001";
    huffmanTable['I'] = "01100011";
    huffmanTable['D'] = "01100010111";
    huffmanTable['N'] = "01100010101";
    huffmanTable['/'] = "01100010100111110101";
    huffmanTable['9'] = "01100010100111111";
    //huffmanTable['–'] = "01100010100111110011"; //corrects \302
    huffmanTable['#'] = "0110001010011111001010";
    huffmanTable['6'] = "01100010100111011";
    //huffmanTable['™'] = "0110001010011111001001"; //corrects \230
    huffmanTable['%'] = "0110001010011111001000";
    huffmanTable['Z'] = "0110001010011111000";
    huffmanTable['8'] = "0110001010011100";
    //huffmanTable['¢'] = "0110001010011011"; //corrects \242
    huffmanTable['5'] = "0110001010011010";
    huffmanTable['$'] = "011000101001100101111";
    huffmanTable['Y'] = "01100010110";
    //huffmanTable['»'] = "011000101001100101101"; //corrects \273
    huffmanTable[','] = "000111";
    //huffmanTable['ï'] = "011000101001100101100"; //corrects \357
    huffmanTable['3'] = "0110001010011000";
    huffmanTable['P'] = "01100001010";
    huffmanTable['!'] = "0110000110";
    huffmanTable['('] = "0110000101111";
    huffmanTable['V'] = "0110000101110";
    //huffmanTable['€'] = "01100010100111010"; //corrects \200
    huffmanTable['&'] = "01100010100110011";
    huffmanTable['U'] = "0110000101101";
    huffmanTable['4'] = "0110000101100110";
    huffmanTable['"'] = "10011011";
    // huffmanTable['…'] = "0110000101100111"; //corrects \204
    huffmanTable['*'] = "011000010110010";
    huffmanTable['0'] = "0110001010011110";
    huffmanTable['_'] = "0110000111";
    huffmanTable['1'] = "01100001011000";
    huffmanTable[';'] = "101111110";
    //huffmanTable['½'] = "0110001010011111011"; //corrects \235
    huffmanTable['r'] = "0000";
    huffmanTable['-'] = "01100000";
    huffmanTable['2'] = "1001101011011100";
    //huffmanTable['“'] = "011000101001100101110"; //corrects \224
    huffmanTable['i'] = "0100";
    huffmanTable[']'] = "10011010110111011";
    //huffmanTable['ÿ'] = "0110001010011111001011"; //corrects \277
    huffmanTable['e'] = "001";
    huffmanTable[':'] = "100110101100";
    //huffmanTable['¼'] = "011000101001100100"; //corrects \234
    huffmanTable['q'] = "0110001001";
    huffmanTable['B'] = "0110001000";
    huffmanTable['p'] = "000110";
    huffmanTable['j'] = "0110000100";
    huffmanTable['G'] = "011000101000";
    //huffmanTable['₧'] = "01100010100111110100"; //corrects \240
    huffmanTable['u'] = "00010";
    huffmanTable[')'] = "0110001010010";
    huffmanTable['7'] = "10011010110111111";
    //huffmanTable['¹'] = "0110001010011001010"; //corrects \231
    huffmanTable['n'] = "0101";
    
    string encoding = "";
    unsigned char buffer = 0; //buffer to accumulate bits
    int bitCount = 0; //current number of bits in buffer
    
    for (char c : source) {
        string bits = huffmanTable[c];
        
        //skip if no huffman code for this char (disregarded weird symbols)
        if (bits.size() == 0) {
            continue;
        }
        
        for (char bit : bits) {
            //1) bit shift left to make space for new bit
            //2) convert bit ('1' or '0') to actual integer (1 or 0) using ASCII
            //3) take the OR result of this
            buffer = (buffer << 1) | (bit - '0');
            bitCount++;
            
            //if we have filled a byte (8 bits), store result in encoding
            if (bitCount == 8) {
                encoding.push_back(buffer);
                buffer = 0; //empty buffer
                bitCount = 0; //reset bitCount
            }
        }
    }
    
    //take care of remaining bits by padding last byte with zeros
    if (bitCount > 0) {
        buffer = buffer << (8 - bitCount);
        encoding.push_back(buffer);
    }
    
    return encoding;
}

std::string decompress(const std::string& source) {
    //same huffman table as above but keys and values are flipped
    unordered_map<string, char> huffmanTable;
    
    huffmanTable["11111"] = 's';
    huffmanTable["11110"] = 'h';
    huffmanTable["1110111"] = 'b';
    huffmanTable["1110110111"] = 'T';
    huffmanTable["111011011011"] = 'J';
    huffmanTable["111011011010"] = 'R';
    huffmanTable["111011011001"] = 'z';
    huffmanTable["111011011000"] = 'O';
    huffmanTable["1110110101"] = 'H';
    huffmanTable["11101101001"] = 'L';
    huffmanTable["11101101000"] = '?';
    huffmanTable["1110110011"] = 'x';
    huffmanTable["1110110010"] = '\n';
    huffmanTable["111011000"] = 'M';
    huffmanTable["111010"] = 'm';
    huffmanTable["110"] = ' ';
    huffmanTable["1011111111"] = 'E';
    huffmanTable["1011111110"] = 'S';
    huffmanTable["10111110"] = 'k';
    huffmanTable["1011110"] = 'v';
    huffmanTable["101110"] = 'c';
    huffmanTable["101101"] = 'f';
    huffmanTable["101100"] = 'w';
    huffmanTable["1010"] = 't';
    huffmanTable["100111"] = 'y';
    huffmanTable["10011010111"] = 'F';
    huffmanTable["10011010110111110"] = 'Q';
    huffmanTable["1001101011011110"] = '\''; //corrects \342
    huffmanTable["10011010110111010"] = '[';
    huffmanTable["10011010110110"] = 'X';
    huffmanTable["1001101011010"] = 'K';
    huffmanTable["1001101010"] = 'C';
    huffmanTable["1001101001"] = 'W';
    huffmanTable["1001101000"] = 'A';
    huffmanTable["1001100"] = '.';
    huffmanTable["10010"] = 'd';
    huffmanTable["1000"] = 'a';
    huffmanTable["0111"] = 'o';
    huffmanTable["01101"] = 'l';
    huffmanTable["011001"] = 'g';
    huffmanTable["01100011"] = 'I';
    huffmanTable["01100010111"] = 'D';
    huffmanTable["01100010101"] = 'N';
    huffmanTable["01100010100111110101"] = '/';
    huffmanTable["01100010100111111"] = '9';
    huffmanTable["0110001010011111001010"] = '#';
    huffmanTable["01100010100111011"] = '6';
    huffmanTable["0110001010011111001000"] = '%';
    huffmanTable["0110001010011111000"] = 'Z';
    huffmanTable["0110001010011100"] = '8';
    huffmanTable["0110001010011011"] = '\xA2'; // '¢'
    huffmanTable["0110001010011010"] = '5';
    huffmanTable["011000101001100101111"] = '$';
    huffmanTable["01100010110"] = 'Y';
    huffmanTable["011000101001100101101"] = '\xBB'; // '»'
    huffmanTable["000111"] = ',';
    huffmanTable["0110001010011000"] = '3';
    huffmanTable["01100001010"] = 'P';
    huffmanTable["0110000110"] = '!';
    huffmanTable["0110000101111"] = '(';
    huffmanTable["0110000101110"] = 'V';
    huffmanTable["01100010100110011"] = '&';
    huffmanTable["0110000101101"] = 'U';
    huffmanTable["0110000101100110"] = '4';
    huffmanTable["10011011"] = '"';
    huffmanTable["011000010110010"] = '*';
    huffmanTable["0110001010011110"] = '0';
    huffmanTable["0110000111"] = '_';
    huffmanTable["01100001011000"] = '1';
    huffmanTable["101111110"] = ';';
    huffmanTable["0000"] = 'r';
    huffmanTable["01100000"] = '-';
    huffmanTable["1001101011011100"] = '2';
    huffmanTable["0100"] = 'i';
    huffmanTable["10011010110111011"] = ']';
    huffmanTable["001"] = 'e';
    huffmanTable["100110101100"] = ':';
    huffmanTable["0110001001"] = 'q';
    huffmanTable["0110001000"] = 'B';
    huffmanTable["000110"] = 'p';
    huffmanTable["0110000100"] = 'j';
    huffmanTable["011000101000"] = 'G';
    huffmanTable["00010"] = 'u';
    huffmanTable["0110001010010"] = ')';
    huffmanTable["10011010110111111"] = '7';
    huffmanTable["0101"] = 'n';
    
    string decoding = "";
    string curBits = "";
    
    for (unsigned char byte : source) {
        for (int i = 7; i >= 0; --i) {
            //1) bit shift right by i positions from leftmost (i = 7) to rightmost (i = 0)
            //2) do AND with 1 to get the least signifcant bit (1 or 0)
            //3) use ternary operator to append '1' or '0' accordingly
            curBits += ((byte >> i) & 1) ? '1' : '0';
            
            //check if curBits matches a huffman code in the table
            if (huffmanTable.find(curBits) != huffmanTable.end()) {
                decoding += huffmanTable[curBits]; //add character to decoding
                curBits.clear(); //reset for next character
            }
        }
    }
    
    //remove extra 'r' at end
    if (decoding[decoding.size() - 1] == 'r') {
        decoding.pop_back();
    }
    
    return decoding;
};

//using huffman encoding
int main() {
    string text = readFile();
    //string text = "appeared. 'I have the happiness to tell you,' said he, 'that, if no"; //testing with small input
    //cout << text << endl;
    preProcess(text);
    
    string cResult = compress(text);
    cout << cResult << endl;
    string dResult = decompress(cResult);
    cout << dResult << endl;
    
    return 0;
}

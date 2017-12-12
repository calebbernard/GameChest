#include "stringUtil.h"

string itos(int i){
    stringstream ss;
    ss << i;
    return ss.str();
}

bool isNum(string s){
    bool num = true;
    for (int x = 0; x < s.length(); x++){
        if (s.at(x) < '0' || s.at(x) > '9'){
            num = false;
        }
    }
    return num;
}

int strtoi(string s){
    int count = 0;
    for (int x = 0; x < s.length(); x++){
        count *= 10;
        count += (int)(s.at(x) - '0');
    }
    return count;
}

void splitString(string s, vector<string> &arr, string delimiter){
    int pos = 0;
    string token;
    while ((pos = s.find(delimiter)) != -1) {
        token = s.substr(0, pos);
        arr.push_back(token);
        s.erase(0, pos + delimiter.length());
    }
    arr.push_back(s);
}
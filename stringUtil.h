#pragma once

#include <string>
#include <sstream>
#include <vector>
#include <iostream>
using namespace std;

string itos(int i);

bool isNum(string s);

int strtoi(string s);

void splitString(string s, vector<string> &arr, string delimiter);

string conditionalString(string a, string b, bool condition);

#include "text_parser.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <set>
#include <unordered_map>

using namespace std;

namespace text_parser {

unordered_map<string, int> CountNGrams(const string& input_unclean, int n) {
  string input = "";
  for (auto it: input_unclean) {
    if (!isalpha(it)) continue;
    input.push_back(toupper(it));
  }
  unordered_map<string, int> result;
  size_t size = input.size() - n + 1;
  for (size_t i = 0; i < size; i++) {
    string str = input.substr(i, n);
    if (find_if(str.begin(), str.end(),
                [](char c) { return !isalpha(c); }) == str.end()) {
      // Ako string sadrzi samo slova.
      transform(str.begin(), str.end(), str.begin(), ::toupper);
      result[str]++;
    }
  }
  return result;
}

string ReadFile(const string& file_name) {
  ifstream ifs(file_name.c_str());
  return string((istreambuf_iterator<char>(ifs)),
                (istreambuf_iterator<char>()));
}

}  // text_parser

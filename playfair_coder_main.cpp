#include "playfair_coder.h"

#include <cassert>
#include <cstdlib>
#include <ctime>
#include <iostream>

using namespace std;

//testovi
void RunTestCase(int seed, int size) {
  PlayfairCoder coder;
  string key = coder.GenerateKey(seed);
  string text = "";
  for (int i = 0; i < size; i++) {
    text.push_back('A' + rand() % 26);
  }

  string encoded_text = coder.Encode(key, text);
  string decoded_text = coder.Decode(key, encoded_text);

  assert(decoded_text.size() == text.size());
  for (int i = 0; i < text.size(); i++) {
    if (decoded_text[i] == 'X') {
      assert(text[i] == 'X' || text[i] == 'Z');
      continue; // X = Z
    }
    if (text[i] != decoded_text[i]) {
      cout << key << endl << decoded_text << endl << encoded_text << endl << text << endl;
    }
    assert(text[i] == decoded_text[i]);
  }
}

void RunTests() {
  for (int i = 0; i < 1000; i++) {
    RunTestCase(rand(), 100);
  }
  for (int i = 0; i < 1000; i++) {
    RunTestCase(rand(), 99);
  }
}
// testovi

void ProcessFile(const string& base_file_name) {
  PlayfairCoder coder(base_file_name + ".txt");
  string key = coder.GenerateKey(rand());
  coder.WriteTo(base_file_name + "_coded.txt", coder.Encode(key));
}

int main() {
  srand(time(NULL));
  RunTests();
  ProcessFile("input/input_small");
  ProcessFile("input/input_medium");
  ProcessFile("input/input_large");
  return 0;
}

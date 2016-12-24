// Klasa koja generise kljuc i sifruje zadati tekst playfair metodom.
// U kljucu su slova x i z u jednom polju (ova cinjenice se NECE koristiti u
// kriptoanalizi)

#include "playfair_coder.h"
#include "text_parser.h"

#include <algorithm>
#include <cassert>
#include <cctype>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iostream>

using namespace std;

PlayfairCoder::PlayfairCoder() {}

PlayfairCoder::PlayfairCoder(const string& input_file) {
  string read_text = text_parser::ReadFile(input_file);
  text_ = "";
  for (auto it : read_text) {
    if (isalpha(it)) {
      text_.push_back(toupper(it));
    }
  }
}

string PlayfairCoder::GenerateKey(int seed) {
  srand(seed);
  int p[25];  // niz permutacija azbuke X = Z;
  for (int i = 0; i < 25; i++) {
    p[i] = i;
  }
  random_shuffle(p, p + 25);
  string key = "";
  for (int i = 0; i < 25; i++) {
    key.push_back(p[i] + 'A');
  }
  return key;
}

string PlayfairCoder::Encode(const string& key) { return Encode(key, text_); }

string PlayfairCoder::Encode(const string& key, const string& text) {
  int char_to_pos[26];  // mapa pozicije karaktera kljuca
  assert(key.size() == 25);
  for (size_t i = 0; i < key.size(); i++) {
    char_to_pos[key[i] - 'A'] = i;
  }
  char_to_pos['Z' - 'A'] = char_to_pos['X' - 'A'];
  string result = "";
  for (int i = 0; i < text.size(); i += 2) {
    // Neparan broj karaktera u tekstu, sifrujemo samo jedan karakter.
    if (i + 1 == text.size()) {
      PushString(EncodeBigram(char_to_pos[text[i] - 'A'], -1, key), &result);
    } else {
      PushString(EncodeBigram(char_to_pos[text[i] - 'A'],
                              char_to_pos[text[i + 1] - 'A'], key),
                 &result);
    }
  }
  return result;
}

string PlayfairCoder::Decode(const string& key, const string& text) {
  int char_to_pos[26];  // mapa pozicije karaktera kljuca
  assert(key.size() == 25);
  for (size_t i = 0; i < key.size(); i++) {
    char_to_pos[key[i] - 'A'] = i;
  }
  char_to_pos['Z' - 'A'] = char_to_pos['X' - 'A'];

  string result = "";
  for (int i = 0; i < text.size(); i += 2) {
    // Neparan broj karaktera u tekstu, sifrujemo samo jedan karakter.
    if (i + 1 == text.size()) {
      PushString(DecodeBigram(char_to_pos[text[i] - 'A'], -1, key), &result);
    } else {
      PushString(DecodeBigram(char_to_pos[text[i] - 'A'],
                              char_to_pos[text[i + 1] - 'A'], key),
                 &result);
    }
  }
  return result;
}

void PlayfairCoder::WriteTo(const string& output_file,
                            const string& encoded_text) {
  ofstream file(output_file.c_str());
  file << encoded_text << "\n";
  file.close();
}

string PlayfairCoder::text() { return text_; }

// privatne metode
string PlayfairCoder::EncodeBigram(int pos_a, int pos_b, const string& key) {
  if (pos_b == -1) {
    pos_a++;
    if (pos_a % 5 == 0) pos_a -= 5;
    return string(1, key[pos_a]);
  } else if (pos_a / 5 == pos_b / 5) { // isti red
    pos_a++;
    pos_b++;
    if (pos_a % 5 == 0) pos_a -= 5;
    if (pos_b % 5 == 0) pos_b -= 5; 
  } else if (pos_a % 5 == pos_b % 5) { // ista kolona
    pos_a += 5;
    pos_b += 5;
    if (pos_a >= 25) pos_a -= 25;
    if (pos_b >= 25) pos_b -= 25;
  } else {
    int tmp_pos_a = (pos_a / 5) * 5 + pos_b % 5;
    int tmp_pos_b = (pos_b / 5) * 5 + pos_a % 5;
    pos_a = tmp_pos_a;
    pos_b = tmp_pos_b;
  }
  return string(1, key[pos_a]) + string(1, key[pos_b]);
}

string PlayfairCoder::DecodeBigram(int pos_a, int pos_b, const string& key) {
  if (pos_b == -1) {
    pos_a--;
    if (pos_a % 5 == 4 || pos_a < 0) pos_a += 5;
    return string(1, key[pos_a]);
  } else if (pos_a / 5 == pos_b / 5) { // isti red
    pos_a--;
    pos_b--;
    if (pos_a % 5 == 4 || pos_a < 0) pos_a += 5;
    if (pos_b % 5 == 4 || pos_b < 0) pos_b += 5; 
  } else if (pos_a % 5 == pos_b % 5) { // ista kolona
    pos_a -= 5;
    pos_b -= 5;
    if (pos_a < 0) pos_a += 25;
    if (pos_b < 0) pos_b += 25;
  } else {
    int tmp_pos_a = (pos_a / 5) * 5 + pos_b % 5;
    int tmp_pos_b = (pos_b / 5) * 5 + pos_a % 5;
    pos_a = tmp_pos_a;
    pos_b = tmp_pos_b;
  }
  return string(1, key[pos_a]) + string(1, key[pos_b]);
}


void PlayfairCoder::PushString(const string& in, string* out) {
  for (auto it : in) {
    out->push_back(it);
  }
}

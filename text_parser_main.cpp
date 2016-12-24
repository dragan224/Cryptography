// Ucitava ceo rat it mir i analizira frekvencije
// pojavljivanja mono-grama, bi-grama, tri-grama i quad-grama.

#include "text_parser.h"

#include <fstream>
#include <iostream>
#include <unordered_map>

using namespace std;

using text_parser::CountNGrams;
using text_parser::ReadFile;

void WriteToFile(const string& file_name, unordered_map<string, int> content) {
  ofstream out(file_name);
  for (auto it = content.begin(); it != content.end(); it++) {
    out << it->first << " " << it->second << "\n";
  }
  out.close();
}

int main() { 
  string war_and_peace = ReadFile("war_and_peace.txt");
  WriteToFile("statistics/monograms.txt", CountNGrams(war_and_peace, 1));
  WriteToFile("statistics/bigrams.txt", CountNGrams(war_and_peace, 2));
  WriteToFile("statistics/trigrams.txt", CountNGrams(war_and_peace, 3));
  WriteToFile("statistics/quadgrams.txt", CountNGrams(war_and_peace, 4));
  return 0; 
}

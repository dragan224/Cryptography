// Metode za parsiranje teksta i statisticke analize.

#ifndef TEXT_PARSER_H
#define TEXT_PARSER_H

#include <fstream>
#include <iostream>
#include <set>
#include <unordered_map>

using namespace std;

namespace text_parser {

// Prebrojava sve n-grame iz zadatog fajlova. Sva slova se pretvaraju u velika
// i ignorisu se brojevi i specijalni karakteri.
// Primer "Danas je lep dan." n = 2
// bigrami: DA, AN, NA, AS, JE, LE, EP, DA, AN
// Vraca: (DA, 2), (AN, 2), (NA, 1), (AS, 1) itd.
unordered_map<string, int> CountNGrams(const string& input_unclean, int n);

// Cita sadrzaj fajla i vraca ga kao string.
string ReadFile(const string& file_name);

}  // namespace text_parser

#endif  // TEXT_PARSER_H

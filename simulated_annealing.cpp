// Probijanje playfair-ove sifre pomocu simuliranog kaljenja
// https://en.wikipedia.org/wiki/Simulated_annealing

#include "playfair_coder.h"
#include "text_parser.h"

#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <thread>
#include <unordered_map>

const string kNonExistentCode = "ne";
const double kAnnealingTemperature = 20;
const double kAnnealingTemperatureDecrease = 0.2;
const int kAnnealingCount = 900;  // broj koraka u JEDNOJ rundi annealinga.

using namespace std;

// Racunamo skor za tekst prebrojavanjem n-grama.
// n moze biti 1 (monogrami) , 2 (bigrami), 3 (trigrami) ili 4 (quadgrami).
double ScoreText(const string& encoded_text, int n,
                 const unordered_map<string, double>& map, const string& key) {
  PlayfairCoder coder;
  string text = coder.Decode(key, encoded_text);
  double score = 0;
  for (int i = 0; i < text.size() - n + 1; i++) {
    string gram = text.substr(i, n);
    auto it = map.find(gram);
    if (it != map.end()) {
      score += it->second;
    } else {
      it = map.find(kNonExistentCode);
      score += it->second;
    }
  }
  return score;
}

// Ucitava statistiku iz fajla i racuna logaritamsku skalu.
void InitMap(unordered_map<string, double>* map, const string& file_name) {
  ifstream ifs(file_name.c_str());
  string str;
  int frequency;
  while (ifs >> str >> frequency) {
    map->insert(make_pair(str, frequency));
  }
  int total = 0;
  for (auto it : *map) {
    total += it.second;
  }
  // logaritamska skala...
  for (auto it = map->begin(); it != map->end(); it++) {
    it->second = log10(it->second / static_cast<double>(total));
  }

  // nesto sto nikad nece da se desi na ulazi... tj nepostojeci ngram
  map->insert(
      make_pair(kNonExistentCode, log10(0.01 / static_cast<double>(total))));
}

double RunAnnealingStep(string* best_key, const string& encoded_text, int n,
                        const unordered_map<string, double> map) {
  double temp = kAnnealingTemperature;
  double best_score = ScoreText(encoded_text, n, map, *best_key);

  string curr_key = *best_key;
  double curr_score = best_score;

  while (temp > 0) {
    int count = kAnnealingCount;
    while (count--) {
      // modifikacija trenutnog kljuca
      string new_key = curr_key;
      swap(new_key[rand() % 25], new_key[rand() % 25]);
      double new_score = ScoreText(encoded_text, n, map, new_key);
      double df = new_score - curr_score;
      if (df >= 0 || exp(df / temp) > 1.0 * rand() / RAND_MAX) {
        curr_score = new_score;
        curr_key = new_key;
      }
      if (curr_score > best_score) {
        best_score = curr_score;
        *best_key = curr_key;
      }
    }
    temp -= kAnnealingTemperatureDecrease;
  }
  return best_score;
}

void RunAnnealing(int n, const string& input_file,
                  const unordered_map<string, double> map, int rounds,
                  const string& output_file) {
  ofstream file(output_file.c_str());
  file << "Pokrecemo annealing sa " << n << "-gram analizom"
       << "\n";

  string encoded_text = text_parser::ReadFile(input_file);
  while (!isalpha(encoded_text.back())) {  // \n
    encoded_text.erase(encoded_text.size() - 1, 1);
  }
  string best_key = "ABCDEFGHIJKLMNOPQRSTUVWXY";
  double best_score = ScoreText(encoded_text, n, map, best_key);

  while (rounds--) {
    cout << rounds << " " << output_file
         << endl;  // da znamo status izvrsavanja
    double score = RunAnnealingStep(&best_key, encoded_text, n, map);
    if (score > best_score) {
      file << "Trenutni skor: " << score << "\n";
      PlayfairCoder coder;
      file << coder.Decode(best_key, encoded_text) << "\n\n";
      best_score = score;
    }
  }
  file.close();
}

int main() {
  srand(time(NULL));
  unordered_map<string, double> monogram_map;
  unordered_map<string, double> bigram_map;
  unordered_map<string, double> trigram_map;
  unordered_map<string, double> quadgram_map;

  InitMap(&monogram_map, "statistics/monograms.txt");
  InitMap(&bigram_map, "statistics/bigrams.txt");
  InitMap(&trigram_map, "statistics/trigrams.txt");
  InitMap(&quadgram_map, "statistics/quadgrams.txt");

  // Dekoridarnje input_small.txt
  int small_annealing_rounds = 500;
  thread monogram_thread_small(RunAnnealing, 1, "input/input_small_coded.txt",
                               monogram_map, small_annealing_rounds,
                               "output/input_small_decoded_monograms.txt");
  thread bigram_thread_small(RunAnnealing, 2, "input/input_small_coded.txt",
                             bigram_map, small_annealing_rounds,
                             "output/input_small_decoded_bigrams.txt");
  thread trigram_thread_small(RunAnnealing, 3, "input/input_small_coded.txt",
                              trigram_map, small_annealing_rounds,
                              "output/input_small_decoded_trigrams.txt");
  thread quadgram_thread_small(RunAnnealing, 4, "input/input_small_coded.txt",
                               quadgram_map, small_annealing_rounds,
                               "output/input_small_decoded_quadgrams.txt");

  // Dekodiranje input_medium.txt
  int medium_annealing_rounds = 250;
  thread monogram_thread_medium(RunAnnealing, 1, "input/input_medium_coded.txt",
                                monogram_map, medium_annealing_rounds,
                                "output/input_medium_decoded_monograms.txt");
  thread bigram_thread_medium(RunAnnealing, 2, "input/input_medium_coded.txt",
                              bigram_map, medium_annealing_rounds,
                              "output/input_medium_decoded_bigrams.txt");
  thread trigram_thread_medium(RunAnnealing, 3, "input/input_medium_coded.txt",
                               trigram_map, medium_annealing_rounds,
                               "output/input_medium_decoded_trigrams.txt");
  thread quadgram_thread_medium(RunAnnealing, 4, "input/input_medium_coded.txt",
                                quadgram_map, medium_annealing_rounds,
                                "output/input_medium_decoded_quadgrams.txt");

  // Dekoridarnje input_large.txt
  int large_annealing_rounds = 100;
  thread monogram_thread_large(RunAnnealing, 1, "input/input_large_coded.txt",
                               monogram_map, large_annealing_rounds,
                               "output/input_large_decoded_monograms.txt");
  thread bigram_thread_large(RunAnnealing, 2, "input/input_large_coded.txt",
                             bigram_map, large_annealing_rounds,
                             "output/input_large_decoded_bigrams.txt");
  thread trigram_thread_large(RunAnnealing, 3, "input/input_large_coded.txt",
                              trigram_map, large_annealing_rounds,
                              "output/input_large_decoded_trigrams.txt");
  thread quadgram_thread_large(RunAnnealing, 4, "input/input_large_coded.txt",
                               quadgram_map, large_annealing_rounds,
                               "output/input_large_decoded_quadgrams.txt");

  monogram_thread_small.join();
  bigram_thread_small.join();
  trigram_thread_small.join();
  quadgram_thread_small.join();
  monogram_thread_medium.join();
  bigram_thread_medium.join();
  trigram_thread_medium.join();
  quadgram_thread_medium.join();
  monogram_thread_large.join();
  bigram_thread_large.join();
  trigram_thread_large.join();
  quadgram_thread_large.join();

  return 0;
}

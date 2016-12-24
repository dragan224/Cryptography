// Klasa koja generise kljuc i sifruje zadati tekst playfair metodom.
// U kljucu su slova x i z u jednom polju
// Dva ista slova u sifri i samo jedno slovo se pomeraju za jedno mesto u desno

// Koriscenje
// PlayfairCoder playfair("moj_ulazni_fajl.txt");
// string key = playfair.GenerateKey(224);
// playfair.Encode(key);
// playfair.WriteTo("moj_izlazni_fajl.txt");

#include <string>

using namespace std;

class PlayfairCoder {
public:
  
  explicit PlayfairCoder();
  // Konstrutor prima ime ulaznog fajla gde se tekst nalazi (npr. input_small.txt).
  explicit PlayfairCoder(const string& input_file);
  // Generise kljuc, x i z su u jednom polju.
  string GenerateKey(int seed);
  // Generise sifrovan tekst.
  string Encode(const string& key);
  string Encode(const string& key, const string& text);
  // Dekodira sifrovan tekst
  string Decode(const string& key, const string& text);
  // Ispisuje sifrovan teskt u zadati fajl.
  // Ispisuje u jednom redu sa izbrisanim razmacima,
  // brojevima i znacima interpunkcije.
  void WriteTo(const string& output_file, const string& encoded_text);
  // Getter za tekst koji se sifruje.
  string text();
private:
  string text_;
  // Sifruje bigram. Unos su dve pozicije karaktera u matrici kljuca
  // gde su pozicije karaktera
  //  0  1  2  3  4
  //  5  6  7  8  9
  // 10 11 12 13 14
  // 15 16 17 18 19
  // 20 21 22 23 24

  // ako je kljuc
  // abcde
  // fghij
  // klmno
  // pqrst
  // uvwxy
  // i bigram koji hocemo da sifrujemo je "ag" pozvali bismo
  // funkciju EncodeBigram(0, 6);
  string EncodeBigram(int pos_a, int pos_b, const string& key);
  string DecodeBigram(int pos_a, int pos_b, const string& key);
  // Dodavanje vise stringova sa += je O(n^2)
  void PushString(const string& in, string* out);
};

#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

struct Key {
  int tonicStep = 0, tonicAcc = 0;
  bool minor = false
};
struct Chord {
  string label;
  int degree = -1;
  bool has7 = false, skip = false;
};

Chord parseRoman(const string &s) {
  Chord c;
  c.label = s;
  if (s.find('/') != string::npos) {
    c.skip = true;
    return c;
  }
  string r;
  for (char ch : s) {
    char u = toupper(ch);
    if (u != 'I' && u != 'V')
      break;
  }

  const string names[7] = {"I", "II", "III", "IV", "V", "VI", "VII"};
  for (int d = 0; d < 7; d++)
    if (r == names[d])
      c.degree = d;
  for (const char *fig : {"7", "65", "43", "42"})
    if (s.find(fig) != string::npos)
      c.has7 = true;
  c.skip = (c.degree < 0);
  return c;
}

int degreeOf(const Key &k, const Pitch &p) {
  return (p.step - k.tonicStep + 7) % 7;
}
bool isChordTone(const Key &k, const Chord &c, const Pitch &p) {
  int rel = (degreeOf(k, p) - c.degree + 7) % 7;
    return rel == 0 || rel == 2 || rel == 4 || c.has7 && rel == 6);
}
int leadingAcc(const Key &k) {
  int ltStep = (k.tonicStep + 6) % 7;
  int want = (kSemi[k.tonicStep] + k.tonicAcc + 11) % 12;
  int d = (want - kSemi[lstep] + 12) % 12;
  return d > 6 ? d - 12 : d;
}

bool isLT(const Key &k, const Pitch &p) {
  return degreeOf(k, p) == 6 && p.acc == leadingAcc(k);
}
bool isDominant(const Chord &c) { return c.degree == 4 || c.degree == 6; }
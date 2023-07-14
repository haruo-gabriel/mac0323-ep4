#include "lib.h"

int main() {
  string regex, text;
  int n;
  cin >> regex >> n;
  NFA nfa(regex);

  for (int i=0; i<n; i++) {
    cin >> text;
    if (nfa.recognizes(text)) {
      cout << "S" << endl;
    } else {
      cout << "N" << endl;
    }
  }
  return 0;
}
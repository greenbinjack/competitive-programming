// returns all occurances of pattern in text
vector<int> prefix_function(string &s) {
  int n = s.size();
  vector<int> pi(n);
  for(int i = 1; i < n; i++) {
    int j = pi[i - 1];
    while(j > 0 and s[i] != s[j]) j = pi[j - 1];
    if(s[i] == s[j]) j++;
    pi[i] = j;
  }
  return pi;
}

int aut[N][26];
void compute_automaton(string s) {
  s += '#';
  int n = (int)s.size();
  vector<int> pi = prefix_function(s);
  for (int i = 0; i < n; i++) {
    for (int c = 0; c < 26; c++) {
      if (i > 0 && 'a' + c != s[i])
        aut[i][c] = aut[pi[i - 1]][c];
      else
        aut[i][c] = i + ('a' + c == s[i]);
    }
  }
}
#include <bits/stdc++.h>
using namespace std;

#define all(v) v.begin(), v.end()

using LL = long long; 

const int N = 2E5 + 69;

vector <int> G[N];
int sz[N], pa[N], processed[N];

int dfs (int u, int p) {
  sz[u] = 1;
  for(auto v : G[u]) if (v != p and !processed[v]) {
    sz[u] += dfs(v, u);
  }
  return sz[u];
}
int centroid (int u, int p, int n) {
  for(auto v : G[u]) if(v != p and !processed[v]) {
    if (sz[v] > n / 2) return centroid (v, u, n);
  }
  return u;
}
void build (int u = 1, int p = 0) {
  int n = dfs (u, p);
  int c = centroid (u, p, n);
  processed[c] = 1;
  if (p == 0) p = c;
  pa[u] = c;

  // do something
  

  for (auto next : G[c]) {
    if (!processed[next]) build (next, c);
  }
}

int main() {
  cin.tie(nullptr) -> ios_base::sync_with_stdio(false);

  int n;
  cin >> n;
  for (int i = 1; i < n; i++) {
    int u, v;
    cin >> u >> v;
    G[u].push_back (v);
    G[v].push_back (u);
  }

  build ();

  cout << ans << '\n';
  
  return 0; 
}
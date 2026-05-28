int a[N], ans[N], sz[N], heavy[N];
vector<int> adj[N];
ordered_set < pair <int, int> > os;

void preDfs (int u = 1, int p = -1) { 
  sz[u] = 1;
  for (auto &v : adj[u]) if (v ^ p) {
    preDfs(v, u);
    sz[u] += sz[v];
    if (sz[v] > sz[heavy[u]]) {
      heavy[u] = v;
    }
  }
}
void update(int u, int d) {
  if (d == +1) {
    os.insert ({a[u], u});
  } else {
    os.erase ({a[u], u});
  }
}
void collect(int u, int p, int d) {
  update(u, d);
  for (auto &v : adj[u]) if (v ^ p) {
    collect(v, u, d);
  }
}
void dfs(int u = 1, int p = -1, bool keep_in_the_bucket = 0) { 
  // First process the Light children, then erase them
  // Then go to the Heavy child, but don't erase them 
  for (auto &v : adj[u]) if (v ^ p and v ^ heavy[u]) {
    dfs(v, u, false); 
  }
  if (heavy[u]) {
    dfs(heavy[u], u, true);
  }

  update(u, +1);
  for (auto &v : adj[u]) if (v ^ p and v ^ heavy[u]) {
    collect(v, u, +1);  
  }
  ans[u] = os.size () - os.order_of_key ({a[u] + 1, -1});
  if (not keep_in_the_bucket) collect(u, p, -1);
}
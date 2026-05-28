const int N = 2E5 + 69;

int n;

/**
 *
 * Lazy Segment Tree
 * 
 */

using T = long long;  // Segment tree value type
using U = long long;  // Lazy value type

T Tree[4 * N];
U Lazy[4 * N];
T I = 0;  // Identity;

T merge (T a, T b) { 
  return a + b; 
}

void push(int id, int le, int ri) {
  if (Lazy[id] == 0) return;
  Tree[id] = (ri - le + 1) * Lazy[id];
  if (le != ri) {
    Lazy[2 * id] += Lazy[id];
    Lazy[2 * id + 1] += Lazy[id];
  }
  Lazy[id] = 0;  // Reset Lazy value
}

void build(int id, int le, int ri, const vector<T>& arr) {
  if (le == ri) {
    Tree[id] = arr[le];
    return;
  }
  int mid = (le + ri) / 2;
  build(2 * id, le, mid, arr);
  build(2 * id + 1, mid + 1, ri, arr);
  Tree[id] = merge(Tree[2 * id], Tree[2 * id + 1]);
}

void update(int id, int le, int ri, int l, int r, U val) {
  push(id, le, ri);
  if (r < le || l > ri) return;  // No overlap
  if (l <= le && ri <= r) {
    Lazy[id] += val;
    push(id, le, ri);
    return;
  }
  int mid = (le + ri) / 2;
  update(2 * id, le, mid, l, r, val);
  update(2 * id + 1, mid + 1, ri, l, r, val);
  Tree[id] = merge(Tree[2 * id], Tree[2 * id + 1]);
}

T query(int id, int le, int ri, int l, int r) {
  push(id, le, ri);
  if (r < le || l > ri) return I;           /// No overlap
  if (l <= le && ri <= r) return Tree[id];  /// Total overlap
  int mid = (le + ri) / 2;
  return merge(query(2 * id, le, mid, l, r), query(2 * id + 1, mid + 1, ri, l, r));
}
void build (const vector<T>& arr) { build (1, 0, n - 1, arr); }
void update (int l, int r, U val) { update (1, 0, n - 1, l, r, val); }
T query (int l, int r) { return query (1, 0, n - 1, l, r); }

/**
 *
 * Heavy Light Decomposition
 * 
 */

vector <int> adj[N];
int cur_pos;
bool WEIGHT_IN_EDGE = false;
int Parent[N], Depth[N], Heavy[N], Head[N], Pos[N]; 
int Start[N], End[N]; vector <int> Euler; /// ONLY NEEDED FOR SUBTREE QUERIES

void reset (bool WIE = false) {
  cur_pos = 0;
  WEIGHT_IN_EDGE = WIE;
  fill (Heavy, Heavy + n, -1);
  // build (vector <T> (n, I));
}

int dfs (int v = 0) {
  int size = 1, max_c_size = 0;
  for (int c : adj[v]) {
    if (c != Parent[v]) {
      Parent[c] = v;
      Depth[c] = Depth[v] + 1;
      int c_size = dfs(c);
      size += c_size;
      if (c_size > max_c_size) {
        max_c_size = c_size;
        Heavy[v] = c;
      }
    }
  }
  return size;
}

void decompose (int v = 0, int h = 0) {
  Head[v] = h;
  Pos[v] = cur_pos++;
  Euler.push_back(v);
  Start[v] = (int)Euler.size() - 1;

  if (Heavy[v] != -1) decompose(Heavy[v], h);

  for (int c : adj[v]) {
    if (c != Parent[v] && c != Heavy[v]) decompose(c, c);
  }

  End[v] = Euler.size() - 1;
}

void update_path (int a, int b, LL val) {
  while (Head[a] != Head[b]) {
    if (Depth[Head[a]] < Depth[Head[b]]) swap(a, b);
    update(Pos[Head[a]], Pos[a], val);
    a = Parent[Head[a]];
  }
  if (Depth[a] > Depth[b]) swap(a, b);
  update(Pos[a] + WEIGHT_IN_EDGE, Pos[b], val);
}

// LL query_path (int a, int b) {
//   LL res = 0;
//   while (Head[a] != Head[b]) {
//     if (Depth[Head[a]] < Depth[Head[b]]) swap(a, b);
//     res = merge (res, query(Pos[Head[a]], Pos[a]));
//     a = Parent[Head[a]];
//   }
//   if (Depth[a] > Depth[b]) swap(a, b);
//   res = merge (res, query(Pos[a] + WEIGHT_IN_EDGE, Pos[b]));
//   return res;
// }

vector <T> query_up(int u, int anc) {
  vector <T> res;
  while (Head[u] != Head[anc]) {
    res.emplace_back (query(Pos[Head[u]], Pos[u]));
    u = Parent[Head[u]];
  }
  res.emplace_back (query(Pos[anc] + 1, Pos[u]));
  return res;
}

LL query_path(int u, int v) {
  int w = LCA:: lca(u, v);

  auto left  = query_up(u, w); 
  auto right = query_up(v, w); 

  node res = I;
  for (auto e : left) {
    res = merge (e, res);
  }
  if (not WEIGHT_IN_EDGE) {
    res = merge(query(Pos[w], Pos[w]), res);
  }
  swap (res.AP, res.PA);

  auto res2 = I;
  for (auto e : right) {
    res2 = merge (e, res2); 
  }

  res = merge (res, res2);

  return max ({res.PP, res.PA, res.AP, res.AA});
}

void update_subtree (int v, LL val) { update(Start[v], End[v], val); }

LL query_subtree (int v) { return query(Start[v], End[v]); }

void HLD_initialize (vector<LL> &weight) {
  reset (true);
  Parent[0] = -1;
  dfs(); decompose();
  for (int i = 0; i < n; i++) update(Pos[i], Pos[i], weight[i]);
}

vector < pair <int, LL> > adj_weight[N]; 
void early_dfs (vector<LL> &weight, int u = 0, int p = -1) {
  for (auto [v, w] : adj_weight[u]) if (v ^ p) {
    weight[v] = w;
    early_dfs (weight, v, u);
  }
}
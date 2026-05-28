class HLD {
  vector<int> parent, depth, heavy, head, pos, euler, start, end;
  int n, cur_pos;
  LazySegmentTree segTree;

  int dfs(int v, const vector<vector<int>> &adj) {
    int size = 1, max_c_size = 0;
    for (int c : adj[v]) {
      if (c != parent[v]) {
        parent[c] = v;
        depth[c] = depth[v] + 1;
        int c_size = dfs(c, adj);
        size += c_size;
        if (c_size > max_c_size) {
          max_c_size = c_size;
          heavy[v] = c;
        }
      }
    }
    return size;
  }

  void decompose(int v, int h, const vector<vector<int>> &adj) {
    head[v] = h;
    pos[v] = cur_pos++;
    euler.push_back(v);
    start[v] = euler.size() - 1;

    if (heavy[v] != -1) decompose(heavy[v], h, adj);

    for (int c : adj[v]) {
      if (c != parent[v] && c != heavy[v]) decompose(c, c, adj);
    }

    end[v] = euler.size() - 1;
  }

public:
  HLD(int n, const vector<vector<int>> &adj, vector<LL> &v) 
    : n(n), parent(n), depth(n), heavy(n, -1), head(n), pos(n), start(n), end(n), cur_pos(0), segTree(v, 0) {
    parent[0] = -1;
    depth[0] = 0;
    dfs(0, adj);
    decompose(0, 0, adj);
    for (int i = 0; i < n; i++) segTree.update(pos[i], pos[i], v[i]);
  }

  void update_path (int a, int b, LL val) {
    while (head[a] != head[b]) {
      if (depth[head[a]] < depth[head[b]]) swap(a, b);
      segTree.update(pos[head[a]], pos[a], val);
      a = parent[head[a]];
    }
    if (depth[a] > depth[b]) swap(a, b);
    segTree.update(pos[a], pos[b], val);
  }

  LL query_path (int a, int b) {
    LL res = 0;
    while (head[a] != head[b]) {
      if (depth[head[a]] < depth[head[b]]) swap(a, b);
      res = max (res, segTree.query(pos[head[a]], pos[a]));
      a = parent[head[a]];
    }
    if (depth[a] > depth[b]) swap(a, b);
    res = max (res, segTree.query(pos[a], pos[b]));
    return res;
  }

  void update_subtree (int v, LL val) { segTree.update(start[v], end[v], val); }

  LL query_subtree (int v) { return segTree.query(start[v], end[v]); }
};
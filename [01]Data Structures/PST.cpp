template <typename T>
class PST {
  struct Node {
    int l = 0, r = 0;
    T val = T();
  };
 
  int N;
  vector<Node> tree;
 
  ///////////////////////////////////////////////////////////////////////////
 
  T merge (T a, T b) {
    return a + b;
  }
  T apply (T Old, T New) {
    return Old + New;
  }
 
  ///////////////////////////////////////////////////////////////////////////
 
  int timer = 0;
 
  int new_node() {
    tree.push_back(Node());
    return ++timer;
  }
 
  int build(int l, int r) {
    int id = new_node();
    if (l == r) return id;
 
    int m = (l + r) >> 1;
    tree[id].l = build(l, m);
    tree[id].r = build(m + 1, r);
 
    tree[id].val = merge(tree[tree[id].l].val, tree[tree[id].r].val);
    return id;
  }
 
  int update(int id, int l, int r, int pos, T v) {
    int nid = new_node();
    tree[nid] = tree[id];
 
    if (l == r) {
      tree[nid].val = apply(tree[id].val, v);
      return nid;
    }
 
    int m = (l + r) >> 1;
    if (pos <= m)
      tree[nid].l = update(tree[id].l, l, m, pos, v);
    else
      tree[nid].r = update(tree[id].r, m + 1, r, pos, v);
 
    tree[nid].val = merge(tree[tree[nid].l].val, tree[tree[nid].r].val);
 
    return nid;
  }
 
  T query(int id, int l, int r, int ql, int qr) {
    if (qr < l || r < ql) return T();
    if (ql <= l && r <= qr) return tree[id].val;
 
    int m = (l + r) >> 1;
 
    return merge(query(tree[id].l, l, m, ql, qr), query(tree[id].r, m + 1, r, ql, qr));
  }
 
  T query(int u, int v, int l, int r, int ql, int qr) {
    if (qr < l || r < ql) return T();
    if (ql <= l && r <= qr) return tree[v].val - tree[u].val;
 
    int m = (l + r) >> 1;
 
    return merge(
      query(tree[u].l, tree[v].l, l, m, ql, qr),
      query(tree[u].r, tree[v].r, m + 1, r, ql, qr)
    );
  }
 
  int first_diff(int a, int b, int l, int r) {
    if (tree[a].val == tree[b].val) return -1;
    if (l == r) return l;
 
    int m = (l + r) >> 1;
 
    if (tree[tree[a].l].val != tree[tree[b].l].val)
      return first_diff(tree[a].l, tree[b].l, l, m);
 
    return first_diff(tree[a].r, tree[b].r, m + 1, r);
  }
 
public:
  PST(int _N, int U) {
    N = _N;
    int LG = 33 - __builtin_clz(N);
    tree.reserve((N + U) * LG);
 
    tree.push_back(Node()); // dummy node (0 index)
  }
 
  // build empty tree
  int build() {
    return build(0, N - 1);
  }
 
  // point update (persistent)
  int update(int root, int pos, T val) {
    return update(root, 0, N - 1, pos, val);
  }
 
  // range query
  T query(int root, int l, int r) {
    return query(root, 0, N - 1, l, r);
  }
 
  T query(int left_root, int right_root, int l, int r) {
    return query(left_root, right_root, 0, N - 1, l, r);
  }
 
  // first index where two versions differ
  int first_diff(int root_a, int root_b) {
    return first_diff(root_a, root_b, 0, N - 1);
  }
};


// SUM PST
PersistentSegTree<long long> pst(
    n,
    [](LL a, LL b){ return a + b; },
    [](LL old, LL v){ return v; } // assign
);

// Freequency PST
PersistentSegTree<int> pst(
    n,
    [](int a, int b){ return a + b; },
    [](int old, int v){ return old + v; }
);

// XOR parity
PersistentSegTree<int> pst(
    n,
    [](int a, int b){ return a ^ b; },
    [](int old, int v){ return old ^ v; }
);

// class PST {
// #define lc(u) L[u]
// #define rc(u) R[u]

//   int *L, *R;
//   long long *val;

//   int N, tim = 0;

//   inline long long merge(long long a, long long b) {
//     return a + b;
//   }

//   int build(int l, int r) {
//     int id = ++tim;

//     L[id] = R[id] = 0;
//     val[id] = 0;

//     if (l == r) return id;

//     int m = (l + r) >> 1;
//     lc(id) = build(l, m);
//     rc(id) = build(m + 1, r);

//     val[id] = merge(val[lc(id)], val[rc(id)]);
//     return id;
//   }

//   int update(int id, int l, int r, int pos, int x) {
//     int nid = ++tim;

//     L[nid] = L[id];
//     R[nid] = R[id];
//     val[nid] = val[id];

//     if (l == r) {
//       val[nid] = x;
//       return nid;
//     }

//     int m = (l + r) >> 1;

//     if (pos <= m)
//       lc(nid) = update(lc(id), l, m, pos, x);
//     else
//       rc(nid) = update(rc(id), m + 1, r, pos, x);

//     val[nid] = merge(val[lc(nid)], val[rc(nid)]);
//     return nid;
//   }

//   long long query(int id, int l, int r, int ql, int qr) {
//     if (qr < l || r < ql) return 0;
//     if (ql <= l && r <= qr) return val[id];

//     int m = (l + r) >> 1;

//     return merge(
//       query(lc(id), l, m, ql, qr),
//       query(rc(id), m + 1, r, ql, qr)
//     );
//   }

// public:
//   PST(int N, int Q) {
//     this->N = N;

//     int LG = 33 - __builtin_clz(N);
//     int MAXNODE = N * 4 + Q * LG;

//     L = new int[MAXNODE];
//     R = new int[MAXNODE];
//     val = new long long[MAXNODE];
//   }

//   int build(int n) {
//     return build(0, n - 1);
//   }

//   int update(int root, int pos, int x) {
//     return update(root, 0, N - 1, pos, x);
//   }

//   long long query(int root, int l, int r) {
//     return query(root, 0, N - 1, l, r);
//   }
// };

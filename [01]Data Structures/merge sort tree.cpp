class MergeSortTree {
private:
  int L, R;
  vector < vector <int> > Tree;
public:
  MergeSortTree (int n) : L (0), R (n - 1), Tree (n << 2) {}
  void build (int curLeft, int curRight, vector <int> &v, int pos = 1) {
    if (curLeft == curRight) {
      Tree[pos].push_back (v[curLeft]);
      return;
    }
    int mid = (curLeft + curRight) >> 1, lft = pos << 1, ryt = pos << 1 | 1;
    build (curLeft, mid, v, lft);
    build (mid + 1, curRight, v, ryt);
    merge (all (Tree[lft]), all (Tree[ryt]), back_inserter (Tree[pos]));
  }
  // count number of values less than or equal k in [l, r] range
  int query(int curLeft, int curRight, int l, int r, int k, int pos = 1) {
    if (r < curLeft or l > curRight or l > r) return 0;
    if (curLeft >= l and curRight <= r) return upper_bound (all (Tree[pos]), k) - Tree[pos].begin();
    int mid = (curLeft + curRight) >> 1, lft = pos << 1, ryt = pos << 1 | 1;
    return query(curLeft, mid, l, r, k, lft) + query(mid + 1, curRight, l, r, k, ryt);
  }
  // count if [mn, mx] is in [l, r] range or if there is any segment x belongs to [l, r] in [mn, mx]
  int count (int curLeft, int curRight, int l, int r, int mn, int mx, int pos = 1) {
    if (r < curLeft or l > curRight or l > r) return 0;
    if (curLeft >= l and curRight <= r) return lower_bound (all (Tree[pos]), mx) - lower_bound (all (Tree[pos]), mn);
    int mid = (curLeft + curRight) >> 1, lft = pos << 1, ryt = pos << 1 | 1;
    return count(curLeft, mid, l, r, mn, mx, lft) + count(mid + 1, curRight, l, r, mn, mx, ryt);
  }
  void build (vector <int> &v) { build(L, R, v); }
  int query (int ql, int qr, int k) { return query(L, R, ql, qr, k); }
  int count (int ql, int qr, int mn, int mx) { return count(L, R, ql, qr, mn, mx); }
};
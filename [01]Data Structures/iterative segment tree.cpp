using T = long long;

T Tree[2 * N];

T combine(T l, T r) {
  T res;
  res.sum = l.sum + r.sum;
  return res;
}

void build(vector<T> &a) {
  for (int i = 0; i < n; i++) {
    Tree[i + n] = a[i];
  }
  for (int i = n - 1; i > 0; i--) {
    Tree[i] = combine(Tree[i << 1], Tree[i << 1 | 1]);
  }
}

void update(int p, T v) {
  Tree[p + n] = v;
  for (p += n; p >>= 1;) {
    Tree[p] = combine(Tree[p << 1], Tree[p << 1 | 1]);
  }
}

T query(int l, int r) { 
  ++r;
  T resl, resr;
  bool f1 = 1, f2 = 1;
  for (l += n, r += n; l < r; l >>= 1, r >>= 1) {
    if (l & 1) resl = f1 ? Tree[l++] : combine(resl, Tree[l++]), f1 = 0;
    if (r & 1) resr = f2 ? Tree[--r] : combine(Tree[--r], resr), f2 = 0;
  }
  if (f2) return resl;
  if (f1) return resr;
  return combine(resl, resr);
}
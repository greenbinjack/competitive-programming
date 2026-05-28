using T = long long;  // Segment Tree value type
using U = long long;  // Lazy value type

const int N = 1E5 + 69;

T Tree[4 * N];
U Lazy[4 * N];
T I = 0;  // Identity;

T merge(T a, T b) { return max(a, b); }

void push(int id, int le, int ri) {
  if (Lazy[id] == 0) return;
  Tree[id] += (ri - le + 1) * Lazy[id];
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
  if (r < le || l > ri) return I;           // No overlap
  if (l <= le && ri <= r) return Tree[id];  // Total overlap
  int mid = (le + ri) / 2;
  return merge(query(2 * id, le, mid, l, r), query(2 * id + 1, mid + 1, ri, l, r));
}

// Find the left-most index where the segment exceeds value `val` in range [l, r]
int findL(int id, int le, int ri, int l, int r, T val) {
  push(id, le, ri);
  if (r < le || l > ri) return r + 1;
  if (l <= le && ri <= r && Tree[id] > val) return r + 1;
  if (le == ri) return le;

  int mid = (le + ri) / 2;
  int left = findL(2 * id, le, mid, l, r, val);
  if (left <= r) return left;
  return findL(2 * id + 1, mid + 1, ri, l, r, val);
}
void build (const vector<T>& arr) { build (1, 0, n - 1, arr); }
void update (int l, int r, U val) { update (1, 0, n - 1, l, r, val); }
T query (int l, int r) { return query (1, 0, n - 1, l, r); }
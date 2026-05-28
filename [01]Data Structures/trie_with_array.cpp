const int B = 30;
const int MAX_NODES = (32 * 100000) + 69;
int nxt[MAX_NODES][2], sz[MAX_NODES], node_count, root;

int new_node () {
  ++node_count;
  nxt[node_count][0] = nxt[node_count][1] = mxID[node_count] = -1;
  return node_count;
}

void insert(int val) {
  int cur = root;
  sz[cur]++;
  for (int bit = B - 1; bit >= 0; bit--) {
    int b = (val >> bit) & 1;
    if (nxt[cur][b] == -1) {
      nxt[cur][b] = ++node_count;
    }
    cur = nxt[cur][b];
    sz[cur]++;
  }
}

LL query(LL x) {
  int cur = root, ans = 0;
  for (int bit = B - 1; bit >= 0; bit--) {
    if (cur == -1) break;
    int xb = (x >> bit) & 1;
    if (nxt[cur][xb ^ 1] != -1 and sz[cur]) {
      ans |= (1LL << bit);
      cur = nxt[cur][xb ^ 1];
    } else {
      cur = nxt[cur][xb];
    }
  }
  return ans;
}

void erase(LL x) {
  int cur = root;
  sz[cur]--;
  for (int bit = B - 1; bit >= 0; bit--) {
    int xb = (x >> bit) & 1;
    cur = nxt[cur][xb];
    sz[cur]--;
  }
}
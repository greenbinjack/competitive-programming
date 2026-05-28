struct Trie {
  static const int B = 30;
  struct node {
    node* nxt[2];
    int sz;
    node() {
      nxt[0] = nxt[1] = NULL;
      sz = 0;
    }
  }* root;

  Trie() { root = new node(); }
  ~Trie () { del (root); }

  void insert(int val) {
    node* cur = root;
    cur->sz++;
    for (int i = B - 1; i >= 0; i--) {
      int b = (val >> i) & 1;
      if (!cur->nxt[b]) cur->nxt[b] = new node();
      cur = cur->nxt[b];
      cur->sz++;
    }
  }

  void erase(int val) {
    node* cur = root;
    cur->sz--;
    for (int i = B - 1; i >= 0; i--) {
      int b = (val >> i) & 1;
      cur = cur->nxt[b];
      cur->sz--;
    }
  }

  void del(node* cur) {
    for(int i = 0; i < 2; i++) {
      if(cur->nxt[i]) del(cur->nxt[i]);
    }
    delete(cur);
  }

  int get_max(int x) {
    node* cur = root;
    int ans = 0;
    for (int i = B - 1; i >= 0; i--) {
      if (cur == NULL) return ans;
      int b = (x >> i) & 1;
      if (cur->nxt[1 ^ b] && cur->nxt[1 ^ b]->sz > 0) {
        ans |= (1 << i);
        cur = cur->nxt[1 ^ b];
      } else {
        cur = cur->nxt[b];
      }
    }
    return ans;
  }

  /**
   * HOW TO FIND THE MIN XOR FROM ALL PAIRS IN A ARRAY
   * SORT THE ARRAY
   * mn_xor = min (array[i] ^ arr[i + 1]) for 0 <= i < n
   */

  int f (node* cur) {
    if (cur == NULL) return 0;
    return cur->sz;
  }
  // NUMBER OF w such that w ^ x <= mn_xor
  int query (int x, int mn_xor) {
    node* cur = root;
    int ans = 0, num = 0;
    for (int i = B - 1; i >= 0; i--) {
      if (cur == NULL) return ans;
      int b = (x >> i) & 1;
      if (num + (1 << i) <= mn_xor) {
        ans += f (cur->nxt[b]);
        num |= (1 << i);
        cur = cur->nxt[1 ^ b];
      } else {
        cur = cur->nxt[b];
      }
    }
    if (cur) ans += cur->sz;
    return ans;
  }

  // number of val such that (val ^ x < k)
  int less_query (int x, int k) {
    node* cur = root;
    int ans = 0;
    for (int bit = B - 1; bit >= 0; bit--) {
      if (cur == NULL) break;
      int xb = (x >> bit) & 1;
      int kb = (k >> bit) & 1;
      if (kb) {
        if (cur->nxt[xb]) ans += cur->nxt[xb]->sz;
        cur = cur->nxt[xb ^ 1];
      } else {
        cur = cur->nxt[xb];
      }
    }
    return ans;
  }

};
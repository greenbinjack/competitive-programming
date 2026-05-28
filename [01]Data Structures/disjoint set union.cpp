struct DSU{
  vector <int> par,size;
  DSU(int n) : par(n), size(n) {
    fill(size.begin(), size.end(), 1);
    iota(par.begin(), par.end(), 0);
  }
  int Anc(int node){
    if(par[node] == node) return node;
    return par[node] = Anc(par[node]); 
  }
  bool Same(int x, int y){
    return Anc(x) == Anc(y);
  }
  void Union(int x, int y){
    x = Anc(x), y = Anc(y);
    if(x == y) return;
    if(size[x] < size[y]) swap(x, y);
    size[x] += size[y], par[y] = x;
  }
  int Size(int node){
    return size[Anc(node)];
  }
};
struct Rollback_DSU {
  int n;
  vector <int> par, sz;
  vector <pair <int, int>> op;
  Rollback_DSU(int n) : par(n), sz(n, 1) {
    iota(par.begin(), par.end(), 0);
    op.reserve(n);
  }
  int Anc(int node) {
    for(; node != par[node]; node = par[node]);
    return node;
  }
  bool Unite(int x, int y) {
    x = Anc(x), y = Anc (y);
    if (x == y) return false;
    if(sz[x] < sz[y]) swap(x, y);
    op.emplace_back(x, y);
    par[y] = x;
    sz[x] += sz[y];
    return true;
  }
  void Undo(size_t t) {
    for(; op.size() > t; op.pop_back()) {
      par[op.back().second] = op.back().second;
      sz[op.back().first] -= sz[op.back().second];
    }
  }
};

struct DSU{
    vector<int> p, cnt, sum;
    
    DSU(int n) : p(n + 1), cnt(n + 1, 1), sum(n + 1) {
        iota(p.begin(), p.end(), 0);
        iota(sum.begin(), sum.end(), 0);
    }

    int find(int x) {
        return p[x] == x ? x : p[x] = find(p[x]);
    }

    bool same(int x, int y) {
        return find(x) == find(y);
    }

    bool merge(int x, int y) {
        x = find(x), y = find(y);
        if (x == y) return false;
        p[y] = x;
        cnt[x] += cnt[y];
        sum[x] += sum[y];
        return true;
    }

    void erase(int x) {
        int fx = find(x);
        cnt[fx]--;
        sum[fx] -= x;
        p[x] = x;
    }

    bool move(int x, int y) {
        int fx = find(x), fy = find(y);
        if (fx == fy) return false;
        p[x] = fy;
        cnt[fx]--; cnt[fy]++;
        sum[fx] -= x; sum[fy] += x;
        return true;
    }
};

#define lg(x) (31 - __builtin_clz(x))
using LL = long long;

template <typename VT>
class sparseTable {
private:
  using DT = typename VT::DT;
  int N, K;
  vector<vector<DT>> tr;
  vector<int> log2;

public:
  sparseTable(const vector<DT> &v) {
    N = v.size();
    K = lg(N);
    tr.assign(N, vector<DT>(K + 1));
    log2.assign(N + 1, 0);
    for(int i = 0; i < N; i++)
      tr[i][0] = v[i];
    for(int j = 1; j <= K; j++) {
      for(int i = 0; i + (1 << j) <= N; i++)
        tr[i][j] = VT::merge(tr[i][j - 1], tr[i + (1 << (j - 1))][j - 1]);
    }
    for(int i = 2; i <= N; i++)
      log2[i] = log2[i >> 1] + 1;
  }

  DT query(int L, int R) {
    int len = R - L + 1;
    int j = log2[len];
    return VT::merge(tr[L][j], tr[R - (1 << j) + 1][j]);
  }

  DT query_non_idempotent(int L, int R) {
    DT ans = 0;
    for(int j = K; j >= 0; j--) {
      if((1 << j) <= R - L + 1) {
        ans = VT::merge(ans, tr[L][j]);
        L += (1 << j);
      }
    }
    return ans;
  }
};

struct summation {
  using DT = LL;
  static DT merge(const DT &a, const DT &b) {
    return a + b;
  }
};

struct minimum {
  using DT = LL;
  static DT merge(const DT &a, const DT &b) {
    return min(a, b);
  }
};

struct maximum {
  using DT = LL;
  static DT merge(const DT &a, const DT &b) {
    return max(a, b);
  }
};

struct xorsum {
  using DT = LL;
  static DT merge(const DT &a, const DT &b) {
    return a ^ b;
  }
};

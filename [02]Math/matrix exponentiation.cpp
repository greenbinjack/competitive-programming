template <typename DT>
struct Matrix {
  vector<vector<DT>> mat;
  Matrix(vector<vector<DT>>& mat) : mat(mat) {}
  Matrix(int n) : mat(n, vector<DT>(n)) {
    for (int i = 0; i < n; i++) {
      mat[i][i] = 1;
    }
  }
  Matrix(int n, int m) : mat(n, vector<DT>(m)) {}
};

template <typename DT>
Matrix<DT> multiply(const Matrix<DT>& a, const Matrix<DT>& b, LL MOD) {
  auto& matA = a.mat;
  auto& matB = b.mat;
  assert(matA[0].size() == matB.size());  
  int n = matA.size(), m = matB[0].size(), s = matA[0].size();

  Matrix<DT> result(n, m);
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < m; j++) {
      DT temp = 0;
      for (int k = 0; k < s; k++) {
        temp = (temp + 1LL * matA[i][k] * matB[k][j] % MOD);
        if (temp >= MOD) temp -= MOD;
      }
      result.mat[i][j] = temp;
    }
  }
  return result;
}

Matrix<LL> pow(Matrix<LL> a, LL p, LL MOD) {
  int n = a.mat.size();
  Matrix<LL> ret(n);
  while (p) {
    if (p & 1) ret = multiply(ret, a, MOD); 
    a = multiply(a, a, MOD); 
    p >>= 1;
  }
  return ret;
}
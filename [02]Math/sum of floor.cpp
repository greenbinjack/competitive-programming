// Computes sum_{i=0}^{n-1} floor((a*i + b) / m)
LL floor_sum(LL n, LL m, LL a, LL b) {
  LL ans = 0;

  // ---- Handle negative a and b using floor division ----
  if (a < 0) {
    LL a_div = (a - (m - 1)) / m;  // floor(a / m)
    ans += a_div * n * (n - 1) / 2;
    a -= a_div * m;
  }
  if (b < 0) {
    LL b_div = (b - (m - 1)) / m;  // floor(b / m)
    ans += b_div * n;
    b -= b_div * m;
  }

  // ---- Now 0 <= a,b < m ----
  while (true) {
    if (a >= m) {
      ans += (n - 1) * n / 2 * (a / m);
      a %= m;
    }
    if (b >= m) {
      ans += n * (b / m);
      b %= m;
    }

    // Max y such that a*i + b < m*y
    LL y_max = (a * n + b) / m;
    if (y_max == 0) break;

    LL x_max = y_max * m - b;
    ans += (n - (x_max + a - 1) / a) * y_max;

    n = y_max;
    b = (a - x_max % a) % a;
    swap(a, m);
  }

  return ans;
}
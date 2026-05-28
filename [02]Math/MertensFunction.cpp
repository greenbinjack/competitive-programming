// ---------- Compute M(x) = sum_{i=1..x} mu(i) ----------
map <LL, LL> dp; 
int mob[N];
LL Mertens (LL x) {
  if (x < N) return mob[x];
  if (dp.count (x)) return dp[x];

  // Du Jiao style recursion: group equal floor(x / i) values
  LL res = 1, l = 2; // M(1) = 1 and recursion base (we subtract contributions)
  while (l <= x) {
    LL t = x / l;
    LL r = x / t;              // maximal r with floor(x / r) == t
    res -= (r - l + 1) * Mertens (t);
    l = r + 1;
  }

  return dp[x] = res;
}

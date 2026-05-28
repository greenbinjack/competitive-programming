template <LL mod> struct modint {
  LL val;
  modint(LL val = 0): val(val >= 0 ? val % mod : val % mod + mod) {}
  modint operator + (modint rhs) const { return modint((val + rhs.val)); }
  modint operator - (modint rhs) const { return modint((val - rhs.val)); }
  modint operator * (modint rhs) const { return modint((val * rhs.val)); }
  modint operator / (modint rhs) const { return modint( binpow(rhs, mod - 2) * val);}
  void operator += (modint rhs) { *this = *this + rhs; }
  void operator -= (modint rhs) { *this = *this - rhs; }
  void operator *= (modint rhs) { *this = *this * rhs; }
  void operator /= (modint rhs) { *this = *this / rhs; }
  friend modint binpow (modint val, LL p) {
    modint ans = 1;
    for (; p > 0; p >>= 1, val *= val) if (p & 1) ans = ans * val;
    return ans;
  }
  friend ostream& operator << (ostream& o, modint &a) {
    o << a.val; return o;
  }
  friend istream& operator >> (istream& o, modint &a) {
    o >> a.val; return o;
  }
  friend LL abs(modint a) {
    return abs(a.val);
  }
};

using mint = modint<MOD>;
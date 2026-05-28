namespace FIBONACCI {

std::random_device rseed;
std::mt19937 rng(rseed());
std::uniform_int_distribution<LL> dist(1, 1E12);

LL random(LL a, LL b) {
  return dist(rng) % abs(b - a + 1) + min(a, b);
}

LL gcd(LL a, LL b) {
  while (b) {
    a %= b;
    swap(a, b);
  }
  return a;
}

LL lcm(LL a, LL b) {
  return a / gcd(a, b) * b;
}

LL binPower(LL base, LL e, LL mod) {
  LL result = 1;
  base %= mod;
  while (e) {
    if (e & 1)
      result = (__int128)result * base % mod;
    base = (__int128)base * base % mod;
    e >>= 1;
  }
  return result;
}

bool checkComposite(LL n, LL a, LL d, int s) {
  LL x = binPower(a, d, n);
  if (x == 1 || x == n - 1)
    return false;
  for (int r = 1; r < s; r++) {
    x = (__int128)x * x % n;
    if (x == n - 1)
      return false;
  }
  return true;
}

bool isPrime(LL n) {
  if (n < 2)
    return false;
  int r = 0;
  LL d = n - 1;
  while (!(d & 1))
    d >>= 1, r++;
  for (int a : {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37}) {
    if (n == a)
      return true;
    if (checkComposite(n, a, d, r))
      return false;
  }
  return true;
}

LL brent(LL n) {
  if (n % 2 == 0)
    return 2;
  LL y = random(1, n - 1), c = random(1, n - 1), mm = random(1, n - 1);
  LL g = 1, r = 1, q = 1, x, ys;
  while (g == 1) {
    x = y;
    for (int i = 0; i < r; i++)
      y = ((__int128)y * y % n + c) % n;
    LL k = 0;
    while (k < r && g == 1) {
      ys = y;
      for (int i = 0; i < min(mm, r - k); i++) {
        y = ((__int128)y * y % n + c) % n;
        q = (__int128)q * abs(x - y) % n;
      }
      g = gcd(q, n);
      k += mm;
    }
    r <<= 1;
  }
  if (g == n) {
    while (true) {
      ys = ((__int128)ys * ys % n + c) % n;
      g = gcd(abs(x - ys), n);
      if (g > 1)
        break;
    }
  }
  return g;
}

vector<pair<LL, int>> factorize(LL n) {
  vector<pair<LL, int>> ret;
  vector<LL> v(1, n);
  while (!v.empty()) {
    vector<LL> temp;
    for (auto x : v) {
      if (isPrime(x)) {
        if (n % x)
          continue;
        int cnt = 0;
        while (n % x == 0) {
          n /= x;
          cnt++;
        }
        ret.emplace_back(x, cnt);
      } else {
        LL y = x;
        while (y == x)
          y = brent(x);
        temp.push_back(y);
        temp.push_back(x / y);
      }
    }
    swap(v, temp);
  }
  return ret;
}

// Returns {F(n), F(n-1)} modulo mod
pair<LL, LL> _fib(LL n, LL mod) {
  if (n == 0)
    return {0, 1 % mod};
  auto p = _fib(n / 2, mod);
  LL a = p.first, b = p.second;
  if (b >= mod)
    b %= mod;
  LL c = (__int128)a * (2 * b - a + mod) % mod;
  LL d = ((__int128)b * b + (__int128)a * a) % mod;
  if (n & 1)
    return {d % mod, (c + d) % mod};
  return {c % mod, d % mod};
}

LL period(LL n) {
  if (n <= 5)
    return vector<LL>{0, 1, 3, 8, 6, 20}[n];
  LL psi;
  LL md5 = n % 5;
  if (md5 == 1 || md5 == 4)
    psi = n - 1;
  else
    psi = 2 * n + 2;
  auto factors = factorize(psi);
  for (auto fact : factors) {
    for (int j = 0; j < fact.second; j++) {
      auto p = _fib(psi / fact.first, n);
      if (p.first != 0 || p.second != 1)
        break;
      psi /= fact.first;
    }
  }
  return psi;
}

// Return Pisano period of n
LL getPisanoPeriod(LL n) {
  LL pis = 1;
  auto ret = factorize(n);
  for (auto i : ret) {
    LL base = period(i.first);
    for (int j = 1; j < i.second; j++)
      base *= i.first;
    pis = lcm(pis, base);
  }
  return pis;
}

// Fibonacci number F(x) mod mod in O(log x)
LL fib(LL x, LL mod) {
  return _fib(x, mod).first;
}

// Fibonacci number F(s) mod mod where s is a big number (up to 10^1,000,000)
LL fib(string s, LL mod) {
  LL x = 0;
  LL pisMod = getPisanoPeriod(mod);
  for (auto c : s) {
    x = (10 * x + (c - '0')) % pisMod;
  }
  return fib(x, mod);
}

/**
 * if n is a fibonacci number then 5n^2 + 4 or 5n^2 - 4 is square number [proof from fibo and luca number]
 * summation of Fib[i] in [1, n] = Fib[n + 2] - 1 
 * summation of Fib[i * 2] in [1, n] = Fib[2 * n + 1] - 1
 * summation of Fib[i * 2 + 1] in [0, n - 1] = Fib[2 * n]
 * Fib[2 * n] / Fib[2 * n - 1] or all proofs come from the following matrix
 *      [1, 1] = [Fib[n + 1], Fib[n]    ]
 *      [1, 0] = [Fib[n]    , Fib[n - 1]]
 * summation of Fib[i]^2 in [1, n] = Fib[n] * Fib[n + 1], [proof = think of squares in a rectangle]
 */

}  // namespace fibonacci

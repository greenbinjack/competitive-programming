// Finds prime numbers between a and b, using basic primes up to sqrt(b)
// a must be greater than 1.
int seg_sieve(LL a, LL b) {
	LL ant = a; a = max(a, 3LL);
	vector<bool> pmap(b - a + 1);
	for (int i = 0; i < prime.size (); ++i) {
		LL p = primes[i]; if (p * p > b) break;
		LL j = (a + p - 1) / p;
		for (LL v=(j == 1) ? p+p : j*p; v<=b; v+=p) pmap[v - a] = true;
	}
	vector<LL> ans;
	if (ant == 2) ans.push_back(2);
	int start = a % 2 ? 0 : 1;
	for (int i = start, I = b - a + 1; i < I; i \+= 2)
		if (pmap[i] == false) ans.push_back(a + i);
	return ans.size ();
}

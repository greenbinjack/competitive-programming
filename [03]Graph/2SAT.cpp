struct two_sat {
  int n;
  vector<vector<int>> g, gr; 
  vector<int> comp, topological_order, answer; 
  vector<bool> vis;
  two_sat() {}
  two_sat(int _n) { init(_n); }
  void init(int _n) {
    n = _n;
    g.assign(2 * n, vector<int>());
    gr.assign(2 * n, vector<int>());
    comp.resize(2 * n);
    vis.resize(2 * n);
    answer.resize(2 * n);
  }
  void add_edge(int u, int v) {
    g[u].push_back(v);
    gr[v].push_back(u);
  }
  void add_clause_or(int i, bool f, int j, bool g) {
    add_edge(i + (f ? n : 0), j + (g ? 0 : n));
    add_edge(j + (g ? n : 0), i + (f ? 0 : n));
  }
  void add_clause_xor(int i, bool f, int j, bool g) {
    add_clause_or(i, f, j, g);
    add_clause_or(i, !f, j, !g);
  }
  void add_clause_xnor(int i, bool f, int j, bool g) {
    add_clause_or(i, f, j, !g);
    add_clause_or(i, !f, j, g);
  }
  void add_clause_and(int i, bool f, int j, bool g) {
    add_clause_xor(i, !f, j, g);
  }
  void dfs(int u) {
    vis[u] = true;
    for (const auto &v : g[u])
      if (!vis[v]) dfs(v);
    topological_order.push_back(u);
  }
  void scc(int u, int id) {
    vis[u] = true;
    comp[u] = id;
    for (const auto &v : gr[u])
      if (!vis[v]) scc(v, id);
  }
  bool satisfiable() {
    fill(vis.begin(), vis.end(), false);
    for (int i = 0; i < 2 * n; i++)
      if (!vis[i]) dfs(i);
    fill(vis.begin(), vis.end(), false);
    reverse(topological_order.begin(), topological_order.end());
    int id = 0;
    for (const auto &v : topological_order)
      if (!vis[v]) scc(v, id++);
    for (int i = 0; i < n; i++) {
      if (comp[i] == comp[i + n]) return false;
      answer[i] = (comp[i] > comp[i + n] ? 1 : 0);
    }
    return true;
  }
};


/*

==================== 🧠 2-SAT CHEAT SHEET ====================

VARIABLE MODEL:
------------------------------------------------------------
For each variable x_i:

i       = x_i is TRUE
i + n   = x_i is FALSE

------------------------------------------------------------
CORE FUNCTION:

add_clause_or(i, f, j, g);

Means:
(x_i = f) OR (x_j = g)

------------------------------------------------------------
BASIC CLAUSES:

x_i OR x_j                  -> add_clause_or(i, 1, j, 1)
x_i OR !x_j                 -> add_clause_or(i, 1, j, 0)
!x_i OR x_j                 -> add_clause_or(i, 0, j, 1)
!x_i OR !x_j                -> add_clause_or(i, 0, j, 0)

------------------------------------------------------------
COMMON CONDITION -> CONVERSION:

If A then B                 -> !A OR B
                            -> add_clause_or(A, 0, B, 1)

A implies B                 -> same as above

At least one(A, B)          -> A OR B
                            -> add_clause_or(A, 1, B, 1)

At most one(A, B)           -> !A OR !B
                            -> add_clause_or(A, 0, B, 0)

Exactly one(A, B)           -> XOR
                            -> add_clause_xor(A, 1, B, 1)

A == B                      -> (A → B) AND (B → A)
                            -> add_clause_or(A, 0, B, 1)
                            -> add_clause_or(B, 0, A, 1)

A != B                      -> XOR
                            -> add_clause_xor(A, 1, B, 1)

NAND(A, B)                  -> !(A AND B)
                            -> !A OR !B
                            -> add_clause_or(A, 0, B, 0)

NOR(A, B)                   -> !(A OR B)
                            -> (!A) AND (!B)
                            -> force both false
                            -> add_clause_or(A, 0, A, 0)
                            -> add_clause_or(B, 0, B, 0)

------------------------------------------------------------
FORCING VALUES:

Force x_i = TRUE            -> add_clause_or(i, 1, i, 1)
Force x_i = FALSE           -> add_clause_or(i, 0, i, 0)

------------------------------------------------------------
HELPERS:

XOR (exactly one true):
add_clause_xor(i, f, j, g);

AND (both must hold):
add_clause_and(i, f, j, g);

------------------------------------------------------------
FORBIDDING COMBINATIONS:

Forbidden (x_i = T AND x_j = T)
-> !x_i OR !x_j
-> add_clause_or(i, 0, j, 0)

Forbidden (x_i = T AND x_j = F)
-> !x_i OR x_j
-> add_clause_or(i, 0, j, 1)

Forbidden (x_i = F AND x_j = T)
-> x_i OR !x_j
-> add_clause_or(i, 1, j, 0)

Forbidden (x_i = F AND x_j = F)
-> x_i OR x_j
-> add_clause_or(i, 1, j, 1)

------------------------------------------------------------
PATTERN THINKING:

"Bad combination"
-> forbid using OR clause

"If A then B"
-> !A OR B

"Exactly one"
-> XOR

"At most one"
-> !A OR !B

------------------------------------------------------------
EXAMPLE:

x1 and x2 cannot both be true:

!x1 OR !x2

add_clause_or(1, 0, 2, 0);

------------------------------------------------------------
USAGE:

two_sat sat(n);

// add constraints
sat.add_clause_or(0, 1, 1, 0);

if (!sat.satisfiable()) {
    cout << "NO\n";
} else {
    cout << "YES\n";
    for (int i = 0; i < n; i++) {
        cout << sat.answer[i] << " ";
    }
}

------------------------------------------------------------
COMMON MISTAKES:

- mixing meaning of true/false
- off-by-one indexing
- forgetting variables are 0-based
- wrong implication (ALWAYS: !A OR B)

============================================================

*/
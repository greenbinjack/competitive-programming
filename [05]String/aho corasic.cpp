struct AhoCorasick {
  // --- CONSTRAINTS & CONFIGURATION ---
  const int ALPHABET = 26;
  int num_nodes;  // Total number of states in the automaton
  int num_words;  // Total number of dictionary words inserted

  // --- AUTOMATON ARRAYS ---
  // next[u][c] = The state you reach from state 'u' by typing character 'c'.
  // After build(), this becomes a full Trie Graph (no missing edges).
  vector<vector<int>> next;

  // link[u] = The standard Failure Link (Backup Plan).
  // Points to the longest proper suffix of 'u' that exists in the Trie.
  vector<int> link;

  // out_link[u] = The Dictionary / Exit Link.
  // Points to the longest proper suffix of 'u' that is an ACTUAL completed
  // word. This allows us to find overlapping words in O(1) time without
  // traversing all failure links.
  vector<int> out_link;

  // out[u] = A list of Word IDs that exactly end at state 'u'.
  vector<vector<int>> out;

  // end_in_pattern[u] = True (1) if state 'u' contains ANY dictionary word.
  // Useful for Matrix Exponentiation or avoiding "forbidden" states.
  vector<int> end_in_pattern;

  // --- CONSTRUCTOR ---
  AhoCorasick() : num_nodes(0), num_words(0) {
    create_node();  // Create the Root (State 0)
  }

  // --- METHODS ---

  // Helper to map characters to array indices.
  // Modify this (e.g., return c - 'a') if you change ALPHABET to 26.
  inline int get(char c) { return c - 'a'; }

  // Dynamically allocates memory for a new state.
  int create_node() {
    next.emplace_back(ALPHABET, 0);
    link.emplace_back(0);
    out_link.emplace_back(0);
    out.emplace_back(vector<int>());
    end_in_pattern.emplace_back(0);
    return num_nodes++;
  }

  // Inserts a string into the Trie blueprint. Returns the Word ID.
  int add_pattern(const string& s) {
    int u = 0;
    for (char c : s) {
      int char_idx = get(c);
      if (next[u][char_idx] == 0) {
        next[u][char_idx] = create_node();
      }
      u = next[u][char_idx];
    }
    out[u].push_back(num_words);
    end_in_pattern[u] = 1;
    return num_words++;
  }

  // Wires the Failure Links and builds the O(1) Trie Graph.
  // ALWAYS call this after inserting all your patterns, but BEFORE searching!
  void build() {
    queue<int> q;

    // Initialize depth-1 nodes
    for (int c = 0; c < ALPHABET; ++c) {
      if (next[0][c] != 0) {
        link[next[0][c]] = 0;  // Backup plan for 1-letter prefix is the root
        q.push(next[0][c]);
      }
    }

    // BFS for depth-2 and deeper
    while (!q.empty()) {
      int u = q.front();
      q.pop();

      for (int c = 0; c < ALPHABET; ++c) {
        int v = next[u][c];

        if (v != 0) {
          // CASE A: The child edge actually exists in the Trie.

          // 1. Assign Standard Failure Link
          // Child's backup = Parent's backup's path for this character
          link[v] = next[link[u]][c];

          // 2. Assign Dictionary Exit Link (out_link)
          // If my backup is a completed word, point to it.
          // Otherwise, copy my backup's out_link.
          if (!out[link[v]].empty()) {
            out_link[v] = link[v];
          } else {
            out_link[v] = out_link[link[v]];
          }

          // 3. Contagious Forbidden Status
          // If my out_link points to a forbidden word, I am also forbidden.
          end_in_pattern[v] |= end_in_pattern[out_link[v]];

          q.push(v);
        } else {
          // CASE B: The child edge DOES NOT exist. (Trie Graph Optimization)
          // Draw a permanent shortcut stealing the path from our backup plan.
          next[u][c] = next[link[u]][c];
        }
      }
    }
  }
};

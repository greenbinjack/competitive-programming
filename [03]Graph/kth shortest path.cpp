int n, m, k; cin >> n >> m >> k;
vector<pair<int, int>> g[n + 1];
for(int i = 0; i < m; i++){
    int u, v, w; cin >> u >> v >> w;
    g[u].push_back({v, w});
    g[v].push_back({u, w});
}
priority_queue<pair<LL, LL>, vector<pair<LL, LL>>, greater<pair<LL, LL>>> pq;
pq.push({0, 1});
vector<vector<LL>> dis(n + 1, vector<LL> (k + 1, INF));
vector<int> vis(n + 1);
while(not pq.empty()){
  auto [c, u] = pq.top(); pq.pop();
  if(vis[u] >= k) continue;
  vis[u]++;
  dis[u][vis[u]] = c;
  for(auto [v, w]: g[u]){
    pq.push({c + w, v});
  }
}
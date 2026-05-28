template <typename DT>
class point {
public:
  DT x, y;
  point() = default;
  point(DT x, DT y): x(x), y(y) {}
  bool operator < (point rhs) const { return x < rhs.x || (x == rhs.x && y < rhs.y); }
  bool operator == (const point &rhs) const { return x == rhs.x && y == rhs.y; }
  point operator - (const point &rhs) const { return point(x - rhs.x, y - rhs.y); }
  DT operator & (const point &rhs) const { return x * rhs.y - y * rhs.x; } // cross product
  friend DT orientation (const point &a, const point &b, const point &c) { return (b - a) & (c - a); }
};
 
template <typename DT> 
using polygon = vector<point<DT>>;
 
template <typename DT>
polygon<DT> ConvexHull(polygon<DT> &PT) {
  sort(PT.begin(), PT.end());
  int m = 0, n = PT.size();
  polygon<DT> hull(n + n + 2);
  for (int i = 0; i < n; i++) {
    for (; m > 1 && orientation (hull[m - 2], hull[m - 1], PT[i]) < 0; m--);
    hull[m++] = PT[i];
  }
  for (int i = n - 2, k = m; i >= 0; i--) {
    for (; m > k && orientation (hull[m - 2], hull[m - 1], PT[i]) < 0; m--);
    hull[m++] = PT[i];
  }
  if (n > 1) m--;
  hull.resize(m);
  return hull;
}
// Andrew’s monotone chain algorithm
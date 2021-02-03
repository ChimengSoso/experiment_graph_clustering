
int bsearch(int val, const std::vector<int>& a, int l, int r) {
  return lower_bound(a.begin() + l, a.begin() + r + 1, val) - a.begin();
}

int merge_base(const std::vector<int>& a, const std::vector<int>& b) {
  int c = 0, i = 0, j = 0;
  int n = (int) a.size();
  int m = (int) b.size();
  while (i < n && j < m) {
    if (a[i] < b[j]) ++i;
    else if (a[i] > b[j]) ++j;
    else ++c, ++i, ++j;
  }
  return c;
}    


int intersect(const std::vector<int>& a, const std::vector<int>& b, 
              int la, int ra, int lb, int rb) {
  if (la > ra || lb > rb) return 0;

  int mid_b = (lb + rb + 1) >> 1, c = 0;
  int val = b[mid_b];
  int mid_a = bsearch(val, a, la, ra);

  if (mid_b - lb > mid_a - la) {
    c = c + intersect(b, a, lb, mid_b - 1, la, mid_a - 1);
  } else {
    c = c + intersect(a, b, la, mid_a - 1, lb, mid_b - 1);
  }

  if (mid_a <= ra && val == a[mid_a]) {
    c = c + 1;
  } else if (val < a[mid_a]) mid_a = mid_a - 1;

  if (ra - mid_a > rb - mid_b) {
    c = c + intersect(a, b, mid_a + 1, ra, mid_b + 1, rb);
  } else {
    c = c + intersect(b, a, mid_b + 1, rb, mid_a + 1, ra);
  }

  return c;
}

int fast_intersect(const std::vector<int>& a, const std::vector<int>& b) {
  int n = (int) a.size();
  int m = (int) b.size();
  if (n > m) {
    return intersect(a, b, 0, n - 1, 0, m - 1);
  } else {
    return intersect(b, a, 0, m - 1, 0, n - 1);
  }
}

// trim from start (in place)
static inline void ltrim(std::string &s) {
  s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
    return !std::isspace(ch);
  }));
}

// trim from end (in place)
static inline void rtrim(std::string &s) {
  s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
    return !std::isspace(ch);
  }).base(), s.end());
}

// trim from both ends (in place)
static inline void trim(std::string &s) {
  ltrim(s);
  rtrim(s);
}

namespace data_struct{
  class DSU {
    public: 
      DSU() {}
      ~DSU() {
        delete parent;
      }
      void assign(int n) {
        parent = new int[n + 1];
        std::iota(parent, parent + n + 1, 0);
        all = n + 1;
      }
      void union_node(int u, int v) {
        if (find_set(u) != find_set(v)) {
          parent[u] = v;
        }
      }
      int find_set(int u) {
        if (parent[u] == u) return u;
        return parent[u] = find_set(parent[u]);
      }
    private:
      int *parent;
      int all;
  };
}
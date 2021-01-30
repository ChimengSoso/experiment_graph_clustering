#include <bits/stdc++.h>
#define DEBUG if (0)
using namespace std;

int merge_base(const vector<int>& a, const vector<int>& b) {
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

int bsearch(int val, const vector<int>& a, int l, int r) {
  return lower_bound(a.begin() + l, a.begin() + r + 1, val) - a.begin();
}

int intersect(const vector<int>& a, const vector<int>& b, int la, int ra, int lb, int rb) {
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

int fast_intersect(const vector<int>& a, const vector<int>& b) {
  int n = (int) a.size();
  int m = (int) b.size();
  if (n > m) {
    return intersect(a, b, 0, n - 1, 0, m - 1);
  } else {
    return intersect(b, a, 0, m - 1, 0, n - 1);
  }
}

vector<int>* adj;
pair<int, int>* edge;
int* visit, *d;
map<pair<int, int>, double> sigma;

int main() {
  int num_V = 12;
  int num_E = 18;
  
  // allocate external memory
  adj = new vector<int>[num_V + 1];
  edge = new pair<int, int>[num_E + 1];
  visit = new int[num_V + 1];
  d = new int[num_V + 1];

  // read graph data from file
  string line;
  ifstream in("input.txt");
  
  int idx_edge = 0;
  if (in.is_open()) {
    int u, v;
    while ( getline(in, line)) {
      istringstream iss(line);
      iss >> u >> v;
      adj[u].push_back(v);
      adj[v].push_back(u);
      edge[idx_edge++] = make_pair(u, v);
    }
    in.close();
  } else {
    cout << "Unable to open file" << "\n";
  }

  // sort adj list of each node
  for (int i = 0; i <= num_V; ++i) {
    adj[i].push_back(i);
    sort(adj[i].begin(), adj[i].end());
    d[i] = (int) adj[i].size();
    DEBUG {
      printf("node %d:", i);
      for (auto elm : adj[i]) {
        printf(" %d", elm);
      }
      printf("\n");
    }
  }

  // Check error dataset
  if (num_E != idx_edge) {
    printf("Error number of Edge");
    assert(false);
  }

  // Start Time for algorithm 1
  clock_t t = clock();

  // Calculating sigma cost
  for (int i = 0; i < num_E; ++i) {
    int u = edge[i].first, v = edge[i].second;
    if (d[u] <= 2 || d[v] <= 2) {
      sigma[{u, v}] = min(d[u], d[v]) / sqrt(d[u] * d[v]);
    } else {
      if (d[u] < d[v]) swap(u, v);
      double alpha = 1.0 * d[u] / d[v];
      if (pow(2., 1 + alpha) / alpha <= d[v]) {
        sigma[{u, v}] = merge_base(adj[u], adj[v]) / sqrt(d[u] * d[v]);
      } else {
        sigma[{u, v}] = fast_intersect(adj[v], adj[u]) / sqrt(d[u] * d[v]);
      }
    }
    sigma[{v, u}] = sigma[{u, v}];
  }

  t = clock() - t;
  cout << "Time for algorithm 1: " << 1.00 * t / CLOCKS_PER_SEC << "\n";

  for (int i = 0; i < num_E; ++i) {
    int u = edge[i].first, v = edge[i].second;
    printf("node %d, %d: %.2f\n", u, v, sigma[{u, v}]);
  }
  return 0;

}








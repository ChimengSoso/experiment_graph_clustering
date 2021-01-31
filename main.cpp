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
bool* visit;
int *d;
map<pair<int, int>, double> sigma;
int* N_eps;

const double mu = 4, epsilon = 0.7;

void cluster(int s, set<int>& C) {
  queue<int> q;
  q.push(s);

  while (q.size()) {
    int u = q.front(); q.pop();
    if (!visit[u] && N_eps[u] >= mu) {
      visit[u] = true;
      C.insert(u);
      for (int v: adj[u]) {
        if (sigma[{u, v}] >= epsilon) {
          q.push(v);

          // Union node that may core or non-core
          C.insert(v);
        }
      }
    }
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

int main() {

  string data_set = "roadNet-TX.txt";
  // string data_set = "roadNet-PA.txt";
  // string data_set = "roadNet-CA.txt";
  // const string data_set = "input.txt";
  // const string data_set = "RO_edges.csv";
  // const string data_set = "HU_edges.csv";
  // const string data_set = "HR_edges.csv";
  // const string data_set = "soc-Epinions1.txt";
  // const string data_set = "facebook_combined.txt";
  // const string data_set = "twitter_combined.txt"; // very many of nodes
  // const string data_set = "twitter-2010.txt"; // very super ultimate so big graph
  
  // add path
  data_set = "dataset/" + data_set;

  // twitter graph
  // int num_V = 41652230;
  // int num_E = 1468364884;
  // ifstream in("twitter-2010.txt");
  
  int num_V = 0, num_E = 0;

  // read graph data from file
  string line;
  ifstream scan_data(data_set);
  if (scan_data.is_open()) {
    // get size of file
    scan_data.seekg(0, scan_data.end);
    long long file_size = scan_data.tellg();
    scan_data.seekg(0, scan_data.beg);
    
    char show_size[20];
    if (file_size < 1e3) sprintf(show_size, "%lld byte(s)", file_size);
    else if (file_size < 1e6) sprintf(show_size, "%.2f KB", 1.* file_size / (1LL << 10));
    else if (file_size < 1e9) sprintf(show_size, "%.2f MB", 1. * file_size / (1LL << 20));
    else if (file_size < 1e12) sprintf(show_size, "%.2f GB", 1. * file_size / (1LL << 30));
    else sprintf(show_size, "%.2f TB", file_size / (1LL << 40));

    printf("[Status] size of dataset: %s\n", show_size);
    printf("[Preparing] load dataset...");
    fflush(stdout);

    int u, v;
    long long load_step = 0;
    while (getline(scan_data, line)) {
      replace(line.begin(), line.end(), ',', ' ');
      trim(line);
      if (line.size() == 0) continue;
      if (! ('0' <= line[0] && line[0] <= '9')) continue;
      if (! ('0' <= line.back() && line.back() <= '9')) continue;
      istringstream iss(line);
      iss >> u >> v;
      num_V = max({num_V, u, v});
      ++num_E;
      DEBUG {
        printf("See: u = %d, v = %d, line size = %d\n", u, v, line.size());
      }

      // calculate loading file status
      long long loadnow = scan_data.tellg();
      if (load_step >= 50) {
        printf("\r[Status] Loading %.4f%%    ", 100. * loadnow / file_size);
        fflush(stdout);
        load_step = 0;
      } else {
        ++load_step;
      }
    }
    scan_data.close();
  } else {  
    cout << "[Fail] to scan datset \"" << data_set << "\"\n";
    return 0;
  }
  printf("\r[Status] Loaded: 100%%         \n");
  printf("[Complete] scan dataset from \"%s\"\n", data_set.c_str());
  printf("[Info] number of Nodes: %d\n", num_V + 1);
  printf("[Info] number of Edges: %d\n\n", num_E);
  printf("[Report] calculating time for algorithm 1 ...");
  fflush(stdout);

  ifstream in(data_set);

  // allocate external memory
  adj = new vector<int>[num_V + 1];
  edge = new pair<int, int>[num_E + 1];
  visit = new bool[num_V + 1]();
  d = new int[num_V + 1];
  N_eps = new int[num_V + 1];

  
  int idx_edge = 0;
  if (in.is_open()) {
    int u, v;
    while (getline(in, line)) {
      replace(line.begin(), line.end(), ',', ' ');
      trim(line);
      if (line.size() == 0) continue;
      if (! ('0' <= line[0] && line[0] <= '9')) continue;
      if (! ('0' <= line.back() && line.back() <= '9')) continue;

      istringstream iss(line);
      iss >> u >> v;
      adj[u].push_back(v);
      adj[v].push_back(u);
      adj[u].shrink_to_fit();
      adj[v].shrink_to_fit();
      edge[idx_edge++] = make_pair(u, v);
    }
    in.close();
  } else {
    cout << "Unable to open file" << "\n";
    return 0;
  }

  // sort adj list of each node
  for (int i = 0; i <= num_V; ++i) {
    adj[i].push_back(i);
    
    sort(adj[i].begin(), adj[i].end());
    adj[i].resize(unique(adj[i].begin(), adj[i].end()) - adj[i].begin());
    adj[i].shrink_to_fit();

    d[i] = (int) adj[i].size();
    DEBUG {
      printf("node %d:", i);
      for (auto elm : adj[i]) {
        printf(" %d", elm);
      }
      printf("\n");
    }
  }

  // Start Time for algorithm 1 (Chi algorithm)
  clock_t t = clock();

  // Calculating sigma cost
  for (int i = 0; i < num_E; ++i) {
    int u = edge[i].first, v = edge[i].second;
    if (d[u] <= 2 || d[v] <= 2) {
      sigma[{u, v}] = min(d[u], d[v]) / sqrt(d[u] * d[v]);
    } else {
      if (d[u] < d[v]) swap(u, v);
      double alpha = 1.0 * d[u] / d[v];
      // if (pow(2., 1 + alpha) / alpha <= d[v]) {
      if (1 + alpha <= log2(alpha) + log2(d[v])) {
        sigma[{u, v}] = merge_base(adj[u], adj[v]) / sqrt(d[u] * d[v]);
      } else {
        sigma[{u, v}] = fast_intersect(adj[v], adj[u]) / sqrt(d[u] * d[v]);
      }
    }
    sigma[{v, u}] = sigma[{u, v}];
  }


  // Memoization N_eps
  for (int u = 0; u <= num_V; ++u) {
    int count = 0;
    for (int v: adj[u]) {
      if (u == v || sigma[{u, v}] >= epsilon)
        ++count;
    }
    N_eps[u] = count;
  }

  // Make cluster
  set<set<int>> Cluster;
  for (int u = 0; u <= num_V; ++u) {
    set<int> C;
    if (!visit[u]) {
      cluster(u, C);
      if (C.size() > 1)
        Cluster.insert(C);
    }
  }

  // Finish Time for algorithm 1
  t = clock() - t;
  printf("\r[Report] time for algorithm 1: %.6f second(s)\n", 1.00 * t / CLOCKS_PER_SEC);;

  DEBUG {
  
    // Show N_eps
    for (int i = 0; i <= num_V; ++i) {
      printf("N_eps for node %d: %d\n", i, N_eps[i]);
    }

    // show Cluster
    int clus = 0;
    for (auto C: Cluster) {
      printf("Cluster %d:", ++clus);
      for (int node: C) {
        printf(" %d", node);
      }
      printf("\n");
    }
  }
  
  printf("=================================\n");
  printf("[Report] calculating time for algorithm 2 ...");
  fflush(stdout);

  // Reset everything
  for (int i = 0; i <= num_V; ++i) {
    visit[i] = 0;
    N_eps[i] = 0;
  }
  sigma.clear();

  // Start Time for algorithm 2 (SCAN algorithm)
  t = clock();

  // Calculating sigma cost
  for (int i = 0; i < num_E; ++i) {
    int u = edge[i].first, v = edge[i].second;
    sigma[{u, v}] = merge_base(adj[u], adj[v]) / sqrt(d[u] * d[v]);
    sigma[{v, u}] = sigma[{u, v}];
  }

  // Memoization N_eps
  for (int u = 0; u <= num_V; ++u) {
    int count = 0;
    for (int v: adj[u]) {
      if (u == v || sigma[{u, v}] >= epsilon)
        ++count;
    }
    N_eps[u] = count;
  }

  // Make cluster
   Cluster = set<set<int>>();
  for (int u = 0; u <= num_V; ++u) {
    set<int> C;
    if (!visit[u]) {
      cluster(u, C);
      if (C.size() > 1)
        Cluster.insert(C);
    }
  }

  // Finish Time for algorithm 1
  t = clock() - t;
  printf("\r[Report] time for algorithm 2: %.6f second(s)\n", 1.00 * t / CLOCKS_PER_SEC);;

  printf("\n\n...END\n");
  scanf(" ");
  return 0;

}

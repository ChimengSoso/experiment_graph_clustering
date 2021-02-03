#include <algorithm>
#include <vector>
#include <queue>
#include <set>
#include <map>
#include <cmath>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdio>
#include <numeric>

#include "utility.hpp"
#define DEBUG if (0)

const double mu = 4, epsilon = 0.7;

std::vector<int>* adj;
std::pair<int, int>* edge;
bool* visit;
int *d;
std::map<std::pair<int, int>, double> sigma;
int* N_eps;
int* sd;
int* ed;
data_struct::DSU dsu;

// Use for SCAN and Chi algorithm
void cluster(int s, std::set<int>& C) {
  std::queue<int> q;
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

// Use for pSCAN
void CheckCore(int u) {
  if (ed[u] >= mu && sd[u] < mu) {
    ed[u] = d[u];
    sd[u] = 0;
    for (int v: adj[u]) {
      // Compute sigma(u, v)
      sigma[{u, v}] = merge_base(adj[u], adj[v]) / sqrt(d[u] * d[v]);

      if (sigma[{u, v}]>= epsilon) sd[u] = sd[u] + 1;
      else ed[u] = ed[u] - 1;

      if (!visit[v]) {
        if (sigma[{u, v}] >= epsilon) sd[v] = sd[v] + 1; 
        else ed[v] = ed[v] - 1;
      }

      if (ed[u] < mu || sd[u] >= mu) break;
    }
  }
  visit[u] = 1;
}

// Use for pSCAN
void ClusterCore(int u) {
  std::vector<int> Nplam_u;
  for (int v: adj[u]) {
    if (sigma[{u, v}] != 0) {
      // sigma(u, v) is computed
      Nplam_u.push_back(v);
    }
  }

  for (int v: Nplam_u) {
    if (sd[v] >= mu && sigma[{u, v}] >= epsilon) {
      dsu.union_node(u, v);
    }
  }

  // finding N[u] - N'[u]
  std::vector<int> N_remain;
  int i = 0, j = 0, n = (int) adj[u].size(), m = (int) Nplam_u.size();
  while (i < n && j < m) {
    if (adj[u][i] == Nplam_u[j]) ++j;
    else N_remain.push_back(adj[u][i]);
    ++i;
  } 
  while (i < n) N_remain.push_back(adj[u][i++]);
  
  for (int v: N_remain) {
    if (dsu.find_set(u) != dsu.find_set(v) && ed[v] >= mu) {
      // computed sigma(u, v)
      sigma[{u, v}] = merge_base(adj[u], adj[v]) / sqrt(d[u] * d[v]);
      if (!visit[v]) {
        if (sigma[{u, v}] >= epsilon) sd[v] = sd[v] + 1;
        else ed[v] = ed[v] - 1;
      }
      if (sd[v] >= mu && sigma[{u, v}] >= epsilon) {
        dsu.union_node(u, v);
      }
    }    
  }
}

int main() {

  std::string data_set = "input.txt";
  // std::string data_set = "com-youtube.ungraph.txt";  // Success
  // std::string data_set = "com-amazon.ungraph.txt";
  // std::string data_set = "Email-Enron.txt";
  // std::string data_set = "CA-HepTh.txt";
  // std::string data_set = "CA-GrQc.txt";
  // std::string data_set = "CA-CondMat.txt";
  // std::string data_set = "CA-HepPh.txt";
  // std::string data_set = "CA-AstroPh.txt";
  // std::string data_set = "oregon1_010526.txt";
  // std::string data_set = "oregon1_010519.txt"; // Success
  // std::string data_set = "oregon1_010505.txt";
  // std::string data_set = "oregon1_010512.txt";
  // std::string data_set = "oregon1_010428.txt"; // Success
  // std::string data_set = "oregon1_010421.txt"; // Success
  // std::string data_set = "oregon2_010414.txt"; // Success
  // std::string data_set = "oregon2_010407.txt";
  // std::string data_set = "oregon1_010331.txt"; // Success
  // std::string data_set = "as20000102.txt"; // Success
  // std::string data_set = "as-skitter.txt"; // Success
  // std::string data_set = "Brightkite_edges.txt";
  // std::string data_set = "Gowalla_edges.txt"; // Success
  // std::string data_set = "musae_chameleon_edges.csv"; 
  // std::string data_set = "musae_crocodile_edges.csv"; // Success 
  // std::string data_set = "musae_squirrel_edges.csv";  // Almost Success
  // std::string data_set = "roadNet-TX.txt"; 
  // std::string data_set = "roadNet-PA.txt";
  // std::string data_set = "roadNet-CA.txt";
  // std::string data_set = "RO_edges.csv";
  // std::string data_set = "HU_edges.csv";
  // std::string data_set = "HR_edges.csv";
  // std::string data_set = "soc-Epinions1.txt";
  // std::string data_set = "facebook_combined.txt";
  // std::string data_set = "twitter_combined.txt"; // very many of nodes
  // std::string data_set = "twitter-2010.txt"; // very super ultimate so big graph

  // add path
  data_set = "dataset/" + data_set;

  // twitter graph
  // int num_V = 41652230;
  // int num_E = 1468364884;
  // ifstream in("twitter-2010.txt");
  
  int num_V = 0, num_E = 0;

  // read graph data from file
  std::string line;
  std::ifstream scan_data(data_set);
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
    while (std::getline(scan_data, line)) {
      std::replace(line.begin(), line.end(), ',', ' ');
      trim(line);
      if (line.size() == 0) continue;
      if (! ('0' <= line[0] && line[0] <= '9')) continue;
      if (! ('0' <= line.back() && line.back() <= '9')) continue;
      std::istringstream iss(line);
      iss >> u >> v;
      num_V = std::max({num_V, u, v});
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
    std::cout << "[Fail] to scan datset \"" << data_set << "\"\n";
    return 0;
  }
  printf("\r[Status] Loaded: 100%%         \n");
  printf("[Complete] scan dataset from \"%s\"\n", data_set.c_str());
  printf("[Info] number of Nodes: %d\n", num_V + 1);
  printf("[Info] number of Edges: %d\n\n", num_E);
  printf("[Report] calculating time for algorithm Chi ...");
  fflush(stdout);

  std::ifstream in(data_set);

  // allocate external memory
  adj = new std::vector<int>[num_V + 1];
  edge = new std::pair<int, int>[num_E + 1];
  visit = new bool[num_V + 1]();
  d = new int[num_V + 1];
  N_eps = new int[num_V + 1];

  int idx_edge = 0;
  if (in.is_open()) {
    int u, v;
    while (std::getline(in, line)) {
      std::replace(line.begin(), line.end(), ',', ' ');
      trim(line);
      if (line.size() == 0) continue;
      if (! ('0' <= line[0] && line[0] <= '9')) continue;
      if (! ('0' <= line.back() && line.back() <= '9')) continue;

      std::istringstream iss(line);
      iss >> u >> v;
      adj[u].push_back(v);
      adj[v].push_back(u);
      adj[u].shrink_to_fit();
      adj[v].shrink_to_fit();
      edge[idx_edge++] = std::make_pair(u, v);
    }
    in.close();
  } else {
    std::cout << "Unable to open file" << "\n";
    return 0;
  }

  // sort adj list of each node
  for (int i = 0; i <= num_V; ++i) {
    adj[i].push_back(i);
    
    std::sort(adj[i].begin(), adj[i].end());
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
      sigma[{u, v}] = std::min(d[u], d[v]) / sqrt(d[u] * d[v]);
    } else {
      if (d[u] < d[v]) std::swap(u, v);
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
  std::set<std::set<int>> Cluster;
  for (int u = 0; u <= num_V; ++u) {
    std::set<int> C;
    if (!visit[u]) {
      cluster(u, C);
      if (C.size() > 1)
        Cluster.insert(C);
    }
  }

  // Finish Time for algorithm 1 (Chi algorithm)
  t = clock() - t;
  printf("\r[Report] time for algorithm Chi: %.6f second(s)\n", 1.00 * t / CLOCKS_PER_SEC);;

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
  
  printf("=================================================\n");
  printf("[Report] calculating time for algorithm SCAN ...");
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
  Cluster = std::set<std::set<int>>();
  for (int u = 0; u <= num_V; ++u) {
    std::set<int> C;
    if (!visit[u]) {
      cluster(u, C);
      if (C.size() > 1)
        Cluster.insert(C);
    }
  }

  // Finish Time for algorithm 2
  t = clock() - t;

  printf("\r[Report] time for algorithm pSCAN: %.6f second(s)\n", 1.00 * t / CLOCKS_PER_SEC);;
  printf("=================================================\n");
  printf("[Report] calculating time for algorithm pSCAN ...");

   // Reset everything
  for (int i = 0; i <= num_V; ++i) {
    visit[i] = 0;
    N_eps[i] = 0;
  }
  sigma.clear();

  // Start Time for algorithm 3 (pSCAN algorithm)
  t = clock();
  
  // Do pscan algorithm
  dsu.assign(num_V + 1);
  sd = new int[num_V + 1];
  ed = new int[num_V + 1];
  for (int u = 0; u <= num_V; ++u) {
    sd[u] = 0;
    ed[u] = d[u];
  }
  
  struct edu {
    int u;
    int ed;
    bool operator<(const edu& rsh) const {
      // Non-increasing order
      return ed < rsh.ed; 
    }
  };

  std::priority_queue<edu> q;
  while (q.size()) {
    int u = q.top().u; q.pop();
    if (visit[u]) continue;
    visit[u] = 1;
    
    CheckCore(u);
    if (sd[u] >= mu) {
      ClusterCore(u);
    }
  }
  
  // Finish Time for algorithm 3
  t = clock() - t;
  printf("\r[Report] time for algorithm pSCAN: %.6f second(s)\n", 1.00 * t / CLOCKS_PER_SEC);;


  printf("\n\n...END\n");
  scanf(" ");
  
  for (int i = 0; i <= num_V; ++i)
    adj[i].clear();
  delete adj, edge, visit, d, N_eps, sd, ed;
  return 0;
}

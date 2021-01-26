#include <bits/stdc++.h>
using namespace std;

vector<int> bugA;
vector<pair<int, int>> bugB;
int DEBUG = 0;

int merge_base(int *a, int *b, int n, int m) {
  int c = 0, i = 0, j = 0;
  while (i < n && j < m) {
    if (a[i] < b[j]) ++i;
    else if (a[i] > b[j]) ++j;
    else {
      if (DEBUG) {
        bugA.push_back(a[i]);
      }
      ++c, ++i, ++j;
    }
  }
  return c;
}    

int bsearch(int val, int *a, int l, int r) {
  return lower_bound(a + l, a + r + 1, val) - a;
}

int intersect(int *a, int *b, int la, int ra, int lb, int rb) {
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
    if (DEBUG) {
      bugB.push_back({val, mid_a});
    }
  } else if (val < a[mid_a]) mid_a = mid_a - 1;

  if (ra - mid_a > rb - mid_b) {
    c = c + intersect(a, b, mid_a + 1, ra, mid_b + 1, rb);
  } else {
    c = c + intersect(b, a, mid_b + 1, rb, mid_a + 1, ra);
  }

  return c;
}

int fast_intersect(int *a, int *b, int n, int m) {
  if (n > m) {
    return intersect(a, b, 0, n - 1, 0, m - 1);
  } else {
    return intersect(b, a, 0, m - 1, 0, n - 1);
  }
}

mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());
unsigned int randInt(int l, int r) {
  return uniform_int_distribution<unsigned int>(l, r)(rng);
};

int test_correctness(int id_test, bool show = false) {
  int n = (int) randInt(1, 50);
  int m = (int) randInt(1, 50);
  int *a = new int[n];
  int *b = new int[m];
  for (int i = 0; i < n; ++i) {
    a[i] = randInt(1, 100);
  }
  for (int i = 0; i < m; ++i) {
    b[i] = randInt(1, 100);
  }
  sort(a, a + n);
  sort(b, b + m);
  
  if (show) { printf("old size array a: %d\n", n);
    printf("Array a: ");
    for (int i = 0; i < n; ++i) {
      printf("%d ", a[i]);
    }
    printf("\n");

    printf("old size array b: %d\n", m);
    printf("Array b: ");
    for (int i = 0; i < m; ++i) {
      printf("%d ", b[i]);
    }
    printf("\n");

    printf("\n\nAfter uniqueness\n\n");
  }
  n = unique(a, a + n) - a;
  m = unique(b, b + m) - b;
  if (show) {
    printf("new size array a: %d\n", n);
    printf("Array a: ");
    for (int i = 0; i < n; ++i) {
      printf("%d ", a[i]);
    }
    printf("\n");

    printf("new size array b: %d\n", m);
    printf("Array b: ");
    for (int i = 0; i < m; ++i) {
      printf("%d ", b[i]);
    }
    printf("\n");
  }
  int merge_cnt = merge_base(a, b, n, m);
  if (show) printf("\nmerge count: %d\n", merge_cnt);
  int fast_cnt = fast_intersect(a, b, n, m);
  if (show) printf("fast intersection count: %d\n", fast_cnt);
  if (merge_cnt != fast_cnt) {
    printf("id test: %d\n", id_test);
    printf("size array a: %d\n", n);
    printf("Array a:");
    for (int i = 0; i < n; ++i) {
      printf(" %d%c", a[i], i + 1 < n ? ',' :' ');
    }
    printf("\n");

    printf("size array b: %d\n", m);
    printf("Array b:");
    for (int i = 0; i < m; ++i) {
      printf(" %d%c", b[i], i + 1 < m ? ',' : ' ');
    }
    printf("\n");
    printf("merge_cnt = %d\n", merge_cnt);
    printf("fast intersect = %d\n", fast_cnt);
    DEBUG = 1;
    printf("Calculated AGAIN!!! [%d ----- %d]\n",
       merge_base(a, b, n, m), fast_intersect(a, b, n, m));
    printf("algor A: "); for (auto e: bugA) printf("%d ", e); printf("\n");
    printf("algor B: "); for (auto e: bugB) printf("val = %d, idx = %d\n", e.first, e.second); printf("\n");
    assert(false);
  }
  delete a;
  delete b;
}

int main() {
  for (int testing = 1; testing <= 1000000; ++testing) {
    test_correctness(testing);
  }
  printf("end testing");
  return 0;
}
#include <bits/stdc++.h>
using namespace std;

int main() {
  int a[] = {1, 2, 3, 5, 6, 7, 10};
  int b[] = {0, 4, 5, 7, 9, 11, 12};

  int n = sizeof(a) / sizeof(a[0]);
  int m = sizeof(b) / sizeof(b[0]);

  int merge_cnt = merge_base(a, b, n, m);
  int fast_cnt = fast_intersect(a, b, n, m);

  printf("merge = %d\n", merge_cnt);
  printf("fast = %d\n", fast_cnt);
  return 0;
}
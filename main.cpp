#include <bits/stdc++.h>
using namespace std;

int merge_base(int a[], int b[], int n, int m) {
  int c = 0, i = 0, j = 0;
  while (i < n && j < m) {
    if (a[i] < b[j]) ++i;
    else if (a[i] > b[j]) ++j;
    else ++c, ++i, ++j;
  }
  return c;
}    

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
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

  if (val == a[mid_a]) {
    c = c + 1;
  } else if (val < a[mid_a]) mid_a = mid_a - 1;

  if (ra - mid_a > rb - mid_b) {
    c = c + intersect(a, b, mid_a + 1, ra, mid_b + 1, rb);
  } else {
    c = c + intersect(b, a, mid_b + 1, rb, mid_a + 1, ra);
  }
  return c;
}

int fast_intersect(int a[], int b[], int n, int m) {
  if (n > m) {
    return intersect(a, b, 0, n - 1, 0, m - 1);
  } else if (m > n){
    return intersect(b, a, 0, m - 1, 0, n - 1);
  } else { // the case that not suitable for fast_intersect
    printf("m and n is equal.");
    assert(false);
  }
}

int a[] = {3, 4, 5};
int b[] = {1, 2, 5, 8, 9};

int main() {
  int n = sizeof(a) / sizeof(a[0]);
  int m = sizeof(b) / sizeof(b[0]);

  int merge_cnt = merge_base(a, b, n, m);
  printf("merge = %d\n", merge_cnt);

  if (n != m) {
    int fast_cnt = fast_intersect(a, b, n, m);
    printf("fast = %d\n", fast_cnt);
  } else {
    printf("fast intersection is stopped. n = %d, m = %d\n", n, m);
  }
  return 0;
}
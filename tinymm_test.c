#include <stdio.h>
#include "tinymm.h"

void set(char* p, int len) {
  for (int i = 0; i < len; i++) {
    p[i] = i % 251;
  }
}
int test(char* p, int len) {
  for (int i = 0; i < len; i++) {
    if (p[i] != (char)(i % 251)) {
      return 0;
    }
  }
  return 1;
}

#define SIZE (1 * 1024 * 1024)
char buf[SIZE];

int main() {
  tinymm_init(buf, SIZE, 100);
  const int N = 100;
  const int M = 100;
  void* pp[M];
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < M; j++) {
      int len = 1024 << ((1 + j) % 3);
      void* p = tinymm_malloc(len);
      if (!p) {
        return 2;
      }
      set(p, len);
      pp[j] = p;
    }
    for (int j = 0; j < M; j++) {
      int len = 1024 << ((1 + j) % 3);
      void* p = pp[j];
      if (!test(p, len)) {
        return 1;
      }
      tinymm_free(p);
    }
    //printf("%d\n", i);
  }

  return 0;
}

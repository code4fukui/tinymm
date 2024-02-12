#define BUF_SIZE (1000 * 1024)
#define MAX_BLOCK 100

void* tinymm_memtop;
int tinymm_pbuf;
int tinymm_memlen;

typedef struct {
  int size;
  char* p;
} tinymm_buf;

tinymm_buf tinymm_bufs[MAX_BLOCK];
int tinymm_nblocks;
int tinymm_maxblocks;

void tinymm_init(void* memtop, int memlen, int entrylen) {
  tinymm_memtop = memtop;
  tinymm_memlen = memlen;
  tinymm_pbuf = 0;
  tinymm_nblocks = 0;
  tinymm_maxblocks = entrylen;
  for (int i = 0; i < tinymm_maxblocks; i++) {
    tinymm_bufs[i].size = 0;
    tinymm_bufs[i].p = 0;
  }
}

void* tinymm_malloc(unsigned long len) {
  if (len == 0) {
    return 0;
  }
  for (int i = 0; i < tinymm_nblocks; i++) {
    if (-tinymm_bufs[i].size == len) {
      tinymm_bufs[i].size = -tinymm_bufs[i].size;
      return tinymm_bufs[i].p;
    }
  }
  if (tinymm_nblocks == tinymm_maxblocks) {
    return 0;
  }
  if (tinymm_pbuf + len >= tinymm_memlen) {
    return 0;
  }
  tinymm_bufs[tinymm_nblocks].size = len;
  char* res = tinymm_memtop + tinymm_pbuf;
  tinymm_bufs[tinymm_nblocks].p = res;
  tinymm_nblocks++;
  tinymm_pbuf += len;
  return res;
}

void tinymm_free(void* p) {
  for (int i = 0; i < tinymm_nblocks; i++) {
    if (tinymm_bufs[i].p == p) {
      tinymm_bufs[i].size = -tinymm_bufs[i].size;
      return;
    }
  }
}

# tinymm

a tiny memory management library

## api

- void tinymm_init(void* memtop, int memlen, int entrylen);
- void* tinymm_malloc(unsigned long len);
- void tinymm_free(void* p);

## test

```sh
make
```
→ 0 is OK

## usage

```sh
# update .gitmodules
git submodule add https://github.com/code4fukui/tinymm
git commit -a
```

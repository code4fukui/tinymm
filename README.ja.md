# tinymm

依存関係のない、非常に軽量なメモリ管理ライブラリです。

`tinymm` は、事前に提供されたメモリバッファ上で動作するシンプルなメモリアロケータを提供します。新規のメモリ割り当てにはバンプポインタを使用し、解放されたブロックは、完全に同じサイズの後続のリクエストに対して再利用できます。

## 特徴

*   **シンプルなバンプアロケーション:** 新規のメモリ要求は、メインバッファ内のポインタを進めることによって処理されます。
*   **ブロックの再利用:** `tinymm_free` はブロックを利用可能としてマークします。新規の要求がそのサイズと完全に一致する場合、`tinymm_malloc` はこのブロックを再利用できます。
*   **固定メタデータ:** 静的配列がすべての割り当て済みおよび解放済みブロックを追跡し、個別の割り当ての総数を制限します。
*   **デフラグメンテーションなし:** このライブラリは、隣接する解放済みブロックを結合しません。

これにより、組み込みシステムや、予測可能で反復的なメモリ割り当てパターンを持つアプリケーションに適しています。

## API

公開APIは `[tinymm.h](tinymm.h)` で定義されています。

```c
void tinymm_init(void* memtop, int memlen, int entrylen);
void* tinymm_malloc(unsigned long len);
void tinymm_free(void* p);
```

*   `tinymm_init(void* memtop, int memlen, int entrylen)`
    ユーザーが提供するバッファ (`memtop`)、その総サイズ (`memlen`)、および追跡する割り当てエントリの最大数 (`entrylen`) を使用して、メモリマネージャを初期化します。

*   `void* tinymm_malloc(unsigned long len)`
    `len` バイトのブロックを割り当てます。最初に、完全に同じサイズの以前に解放されたブロックの再利用を試みます。利用可能なものがない場合は、メインバッファから新しいブロックを割り当てます。エントリ数の上限に達した場合、またはメモリが不足している場合は `NULL` を返します。

*   `void tinymm_free(void* p)`
    メモリブロックを解放済みとしてマークし、将来の同じサイズの `tinymm_malloc` 要求で利用できるようにします。メモリはメインのバッファプールやOSには返却されません。

## 使い方

### Gitサブモジュールとして追加

```sh
git submodule add https://github.com/code4fukui/tinymm
git commit -m "Add tinymm submodule"
```

### 例

```c
#include "tinymm.h"
#include <stdio.h>

// Define the memory pool and entry limit
#define MEM_SIZE (1024 * 1024)
#define MAX_ENTRIES 100
char memory_pool[MEM_SIZE];

int main() {
  // Initialize the allocator
  tinymm_init(memory_pool, MEM_SIZE, MAX_ENTRIES);

  // Allocate memory
  char* block1 = tinymm_malloc(1024);
  if (block1) {
    printf("Allocated 1024 bytes at %p\n", block1);
    // Use the memory...
    block1[0] = 'a';
  }

  // Free the block
  tinymm_free(block1);
  printf("Freed block.\n");

  // This allocation will reuse the memory from block1
  char* block2 = tinymm_malloc(1024);
  if (block2) {
    printf("Re-allocated 1024 bytes at %p\n", block2);
  }

  return 0;
}
```

## ビルドとテスト

付属のテストスイートをコンパイルして実行するには、Makeを使用します。

```sh
make
```

このコマンドは `tinymm_test.c` をコンパイルし、生成された実行ファイルを実行して、終了コードを出力します。テストでは、100個のメモリブロックの割り当てと解放を100サイクル実行し、プロセス全体を通してデータの整合性を検証します。

→ 終了コードが `0` の場合、すべてのテストに合格したことを示します。

## ライセンス

このプロジェクトは MIT License の下でライセンスされています。

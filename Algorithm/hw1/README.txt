# HW1 -- 最大區間和

## 非遞迴實作

1. 不斷累加數值 `acc`
2. 如果 `acc` 小於 0 ，則 `acc` 歸零
3. 如果 `acc` 出現更大的數值時，記錄下來 (`max_acc`)

## 遞迴實作

基本上跟投影片上的解法一樣

## 編譯方式

有 `Makefile`，UNIX 環境下 `make` 即可，`recursive.c`, `non-recursive.c` 為 symbolic link，
Windows 環境下可能無法正常處理 tar

> vim: ft=markdown

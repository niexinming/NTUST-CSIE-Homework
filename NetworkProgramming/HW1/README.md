# Server / Client file transport

- Environtment: Linux

## Build Requirement

- gcc compiler v5.0+
- GNU Make

## Build Instruction

- make

## Run sample

```
./server/server 9999 file1 file2 &
./client/client 127.0.0.1 9999 1 output-file
```

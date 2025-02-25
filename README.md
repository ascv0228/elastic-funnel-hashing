# OptOpenHash in cpp

## Compile Method

```bash
g++ -c src/elastic_hashing.cpp -o elastic_hashing.o -std=c++17
g++ -c src/funnel_hashing.cpp -o funnel_hashing.o -std=c++17
g++ -c test.cpp -o test.o -std=c++17

g++ -o test test.cpp optopenhash/elastic_hashing.cpp optopenhash/funnel_hashing.cpp -std=c++17
./test.exe
```

CMake:

```bash
mkdir build && cd build && cmake .. && cmake --build .. && ./bin/test
```

# References

> **Optimal Bounds for Open Addressing Without Reordering**
> Martín Farach‐Colton, Andrew Krapivin, William Kuszmaul
> [Link](https://arxiv.org/pdf/2501.02305)

- [optopenhash](https://github.com/sternma/optopenhash)
- [mason276752](https://gist.github.com/mason276752/001b5c5eab686ef85b61ef04bcc46551)
- [elastic-funnel-hashing](https://github.com/ascv0228/elastic-funnel-hashing)


# HINT

对于 Elastic Hash 的 string - string 类型，仅通过 `capacity = 1000`, `delta=0.1` 的场景，而当仅修改 `capcity = 100` 时则会随机出现错误……


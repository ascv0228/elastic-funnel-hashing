# optopenhash_cpp

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

# 参考: 

- [optopenhash](https://github.com/sternma/optopenhash)
- [mason276752](https://gist.github.com/mason276752/001b5c5eab686ef85b61ef04bcc46551)


# HINT: 不知道為甚麼 elastic_hashing 的測試一直錯

对于 Elastic Hash 的 string - string 类型，仅通过 `capacity = 1000`, `delta=0.1` 的场景，而当仅修改 `capcity = 100` 时则会随机出现错误……


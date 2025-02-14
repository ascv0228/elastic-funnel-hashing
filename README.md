# optopenhash_cpp

```
g++ -c optopenhash/elastic_hashing.cpp -o elastic_hashing.o -std=c++17
g++ -c optopenhash/funnel_hashing.cpp -o funnel_hashing.o -std=c++17
g++ -c test.cpp -o test.o -std=c++17

g++ -o test test.cpp optopenhash/elastic_hashing.cpp optopenhash/funnel_hashing.cpp -std=c++17
./test.exe
```

=====================
參考: 
[optopenhash](https://github.com/sternma/optopenhash)
[mason276752](https://gist.github.com/mason276752/001b5c5eab686ef85b61ef04bcc46551)


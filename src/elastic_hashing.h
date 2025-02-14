#ifndef ELASTIC_HASHING_H
#define ELASTIC_HASHING_H

#include <string>
#include <vector>
#include <optional>

class ElasticHashTable {
private:
    struct Entry {
        std::string key;
        int value;
        bool occupied = false;
    };
    int capacity;
    double delta;
    int max_inserts;
    int num_inserts;
    std::vector<std::vector<Entry>> levels;
    std::vector<int> salts;
    std::vector<int> occupancies;
    int c = 4;

    int _hash(const std::string &key, int level) const;
    int _quad_probe(const std::string &key, int level, int j, int table_size) const;

public:
    ElasticHashTable(int capacity, double delta = 0.1);
    bool insert(const std::string &key, const int &value);
    std::optional<int> search(const std::string &key);
    bool contains(const std::string &key);
    int size() const;
};

#endif // ELASTIC_HASHING_H
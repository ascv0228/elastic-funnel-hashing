#ifndef ELASTIC_HASHING_H
#define ELASTIC_HASHING_H

#include <string>
#include <vector>
#include <optional>

template <typename KT, typename VT>
class ElasticHashTable {
private:
    struct Entry {
        KT key;
        VT value;
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

    int _hash(const KT &key, int level) const;
    int _quad_probe(const KT &key, int level, int j, int table_size) const;

public:
    ElasticHashTable(int capacity, double delta = 0.1);
    bool insert(const KT &key, const VT &value);
    std::optional<VT> search(const KT &key);
    bool contains(const KT &key);
    int size() const;
    void print();
};

#endif // ELASTIC_HASHING_H
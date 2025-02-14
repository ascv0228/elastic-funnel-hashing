#ifndef FUNNEL_HASHING_H
#define FUNNEL_HASHING_H

#include <string>
#include <vector>
#include <optional>

class FunnelHashTable {
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
    int alpha, beta;
    int special_size, primary_size;
    std::vector<std::vector<Entry>> levels;
    std::vector<int> level_bucket_counts;
    std::vector<int> level_salts;
    std::vector<Entry> special_array;
    int special_salt;
    int special_occupancy;

    int _hash(const std::string &key, int salt) const;
    int _hash_level(const std::string &key, int level_index) const;
    int _hash_special(const std::string &key) const;

public:
    FunnelHashTable(int capacity, double delta = 0.1);
    bool insert(const std::string &key, const int &value);
    std::optional<int> search(const std::string &key);
    bool contains(const std::string &key);
    int size() const;
};

#endif // FUNNEL_HASHING_H

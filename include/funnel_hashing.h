#ifndef FUNNEL_HASHING_H
#define FUNNEL_HASHING_H

#include <string>
#include <vector>
#include <optional>

template <typename KT, typename VT>
class FunnelHashTable {
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
    int alpha, beta;
    int special_size, primary_size;
    std::vector<std::vector<Entry>> levels;
    std::vector<int> level_bucket_counts;
    std::vector<int> level_salts;
    std::vector<Entry> special_array;
    int special_salt;
    int special_occupancy;

    int _hash(const KT &key, int salt) const;
    int _hash_level(const KT &key, int level_index) const;
    int _hash_special(const KT &key) const;

public:

    class Proxy {
        FunnelHashTable& table;
        KT key;  // 按值存储键，避免悬挂引用

    public:
        Proxy(FunnelHashTable& t, const KT &k) : table(t), key(k) {}

        Proxy& operator=(const VT& value) {
            table.insert(key, value);
            return *this;
        }

        operator VT() const {
            auto entry = table.search(key);
            if (!entry) throw std::out_of_range("Key not found");
            return entry.value();
        }
    };

    // 重载operator[]返回代理对象
    Proxy operator[](const KT& key) {
        return Proxy(*this, key);
    }

    Proxy operator[](KT&& key) {
        return Proxy(*this, std::move(key));
    }

    FunnelHashTable(int capacity, double delta = 0.1);
    bool insert(const KT &key, const VT &value);
    std::optional<VT> search(const KT &key);
    bool contains(const KT &key);
    int size() const;
    void print();
};

#endif // FUNNEL_HASHING_H

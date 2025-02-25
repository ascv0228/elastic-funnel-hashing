#ifndef ELASTIC_HASHING_H
#define ELASTIC_HASHING_H

#include <string>
#include <vector>
#include <optional>

template <class KT, class VT>
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

    class Proxy {
        ElasticHashTable& table;
        KT key;  // 按值存储键，避免悬挂引用
    public:
        Proxy(ElasticHashTable& t, const KT &k) : table(t), key(k) {}
    
        Proxy& operator=(const VT& value) {
            table.insert(key, value);
            return *this;
        }
    
        // template<typename T>
        // Proxy& operator=(T&& value) {
        //     try {
        //         VT converted_value = VT(std::forward<T>(value));  // 显式构造
        //         return operator=(converted_value);  // 调用标准赋值操作
        //     } catch (const std::exception& e) {
        //         throw std::runtime_error("Failed to assign value: " + std::string(e.what()));
        //     }
        // }
    
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


    ElasticHashTable(int capacity, double delta = 0.1);
    bool insert(const KT &key, const VT &value);
    std::optional<VT> search(const KT &key);
    bool contains(const KT &key);
    int size() const;
    void print();
};

#endif // ELASTIC_HASHING_H
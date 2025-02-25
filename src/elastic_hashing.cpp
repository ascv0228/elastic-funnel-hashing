#include <iostream>
#include <vector>
#include <cmath>
#include <random>
#include <stdexcept>
#include <limits>
#include <optional>
#include <tuple>
#include <type_traits>
#include "elastic_hashing.h"


// 重载operator[]返回代理对象


// 对于字面量类型（如int）需要右值重载


template <class KT, class VT>
int ElasticHashTable<KT, VT>::_hash(const KT &key, int level) const {
    std::hash<KT> hasher;
    return (hasher(key) ^ salts[level]) & 0x7FFFFFFF;
}

/**
 * @return The index for the j-th quadratic probe.
 */
template <class KT, class VT>
int ElasticHashTable<KT, VT>::_quad_probe(const KT &key, int level, int j, int table_size) const {
    return (_hash(key, level) + j * j) % table_size;
}

template <class KT, class VT>
ElasticHashTable<KT, VT>::ElasticHashTable(int capacity, double delta) {
    if (capacity <= 0) 
        throw std::invalid_argument("Capacity must be positive.");
    if (!(0 < delta && delta < 1)) 
        throw std::invalid_argument("Delta must be between 0 and 1.");
    this->capacity = capacity;
    this->delta = delta;
    num_inserts = 0;
    max_inserts = capacity - static_cast<int>(delta * capacity);
    
    int num_levels = std::max(1, static_cast<int>(std::floor(std::log2(capacity))));
    
    std::vector<int> sizes;
    int remaining = capacity;
    for (int i = 0; i < num_levels - 1; ++i) {
        int size = std::max(1, remaining / (1 << (num_levels - i)));
        sizes.push_back(size);
        remaining -= size;
    }
    sizes.push_back(remaining);
    
    for (int s : sizes) {
        std::vector<Entry> tmp_level(s);
        levels.emplace_back(tmp_level);
    }
    occupancies.resize(num_levels, 0);
    
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(0, std::numeric_limits<int>::max());
    
    for (int i = 0; i < num_levels; ++i) {
        salts.push_back(dist(gen));
    }
}

// template <class KT, class VT>
// VT & ElasticHashTable<KT, VT>::operator[](const KT &key) {
//     // __setitem__(key, value)
//     auto ret = search(key);
//     if (ret.has_value()) {
//         return ret.value();
//     }

//     insert(key, VT{});
//     // return (*this)[key];
//     // return operator[](key);
//     ret = search(key);  // 再次查找，确保插入成功
    
//     if (ret.has_value()) {
//         std::cout << "ret value: " << ret.value() << std::endl;
//         insert(key, ret.value());
//         return ret.value();
//     }

//     throw std::runtime_error("Failed to insert key");
// }



template <class KT, class VT>
bool ElasticHashTable<KT, VT>::insert(const KT &key, const VT &value) {
    if (this->num_inserts >= this->max_inserts) 
        throw std::runtime_error("Hash table is full.");
    
    for (size_t i = 0; i < levels.size(); ++i) {
        auto &level = levels[i];
        int size = level.size();
        int occ = occupancies[i];
        double free = size - occ;
        double load = size > 0 ? free / static_cast<double>(size) : 0;
        
        int probe_limit = std::max( 
            1, 
            static_cast<int>(
                c * 
                std::min(
                    // load > 0 ? std::pow(std::log2(1 / load), 2) : 0,
                    load > 0 ? std::log2(1 / load) : 0, 
                    std::log2(1 / delta)
                )
            )
        ); // f(\epsilon) = c \cdot \min(\log^{2} \epsilon^{-1}, \log \delta^{-1})

        if (i < levels.size() - 1) {
            auto& next_level = levels[i + 1];
            int next_size = next_level.size();
            int next_occ = occupancies[i + 1];
            double next_free = next_size - next_occ;
            double load_next = next_size > 0 ? next_free / static_cast<double>(next_size) : 0;
            
            double threshold = 0.25;

            if (load > (delta / 2) && load_next > threshold) {
                for (int j = 0; j < probe_limit; ++j) {
                    int idx = _quad_probe(key, i, j, size);
                    if (!level[idx].occupied) {
                        level[idx] = {key, value, true};
                        ++occupancies[i];
                        ++num_inserts;
                        return true;
                    }
                }

                // for (int j = 0; j < size; ++j) {
                //     int idx = _quad_probe(key, i, j, size);
                //     if (!level[idx].occupied) {
                //         level[idx] = {key, value, true};
                //         ++occupancies[i];
                //         ++num_inserts;
                //         return true;
                //     }
                // }

            } else if (load <= (delta / 2)) {
                continue;
            } else if (load_next <= threshold) {
                for (int j = 0; j < size; ++j) {
                    int idx = _quad_probe(key, i, j, size);
                    if (!level[idx].occupied) {
                        level[idx] = {key, value, true};
                        ++occupancies[i];
                        ++num_inserts;
                        return true;
                    }
                }
            }
        } else {
            for (int j = 0; j < size; ++j) {
                int idx = _quad_probe(key, i, j, size);
                if (!level[idx].occupied) {
                    level[idx] = {key, value, true};
                    ++occupancies[i];
                    ++num_inserts;
                    return true;
                }
            }
        }
    }
    std::cout << key << " " << value << " " << "Failed" << "\n";
    throw std::runtime_error("Insertion failed in all levels; hash table is full.");
}

// template <class KT, class VT>
// const VT & ElasticHashTable<KT, VT>::operator[](const KT &key) const {
//     auto ret = search(key);

//     if (!ret.has_value()) {
//         throw std::out_of_range("Key not found.");
//     }
//     return ret.value();
// }

// std::optional<int> ElasticHashTable::get(const std::string &key, std::optional<int> default_val) {
//     return search(key).value_or(default_val);
// }

template <class KT, class VT>
std::optional<VT> ElasticHashTable<KT, VT>::search(const KT &key) {
    for (size_t i = 0; i < levels.size(); ++i) {
        auto &level = levels[i];
        int size = level.size();
        
        for (int j = 0; j < size; ++j) {
            int idx = _quad_probe(key, i, j, size);
            if (!level[idx].occupied) {
                break;
            }
            if (level[idx].key == key) {
                return level[idx].value;
            }
        }
    }
    return std::nullopt;
}

template <class KT, class VT>
bool ElasticHashTable<KT, VT>::contains(const KT &key) {
    return search(key).has_value();
}

template <class KT, class VT>
int ElasticHashTable<KT, VT>::size() const {
    return this->num_inserts;
}

template <class KT, class VT>
void ElasticHashTable<KT, VT>::print() {
    for (size_t i = 0; i < levels.size(); ++i) {
        std::cout << "Level " << i << ":\n";
        for (size_t j = 0; j < levels[i].size(); ++j) {
            if (levels[i][j].occupied) {
                std::cout << "  " << levels[i][j].key << " -> " << levels[i][j].value << "\n";
            }
        }
    }
}

template class ElasticHashTable<std::string, int>;
template class ElasticHashTable<std::string, std::string>;
#include <iostream>
#include <vector>
#include <cmath>
#include <random>
#include <stdexcept>
#include <limits>
#include <optional>
#include "elastic_hashing.h"


int ElasticHashTable::_hash(const std::string &key, int level) const {
    std::hash<std::string> hasher;
    return (hasher(key) ^ salts[level]) & 0x7FFFFFFF;
}

int ElasticHashTable::_quad_probe(const std::string &key, int level, int j, int table_size) const {
    return (_hash(key, level) + j * j) % table_size;
}

ElasticHashTable::ElasticHashTable(int capacity, double delta) {
    if (capacity <= 0) throw std::invalid_argument("Capacity must be positive.");
    if (!(0 < delta && delta < 1)) throw std::invalid_argument("Delta must be between 0 and 1.");
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
    
    for (int s : sizes) levels.emplace_back(s);
    occupancies.resize(num_levels, 0);
    
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(0, std::numeric_limits<int>::max());
    
    for (int i = 0; i < num_levels; ++i) {
        salts.push_back(dist(gen));
    }
}
bool ElasticHashTable::insert(const std::string &key, const int &value) {
    if (num_inserts >= max_inserts) throw std::runtime_error("Hash table is full.");
    
    for (size_t i = 0; i < levels.size(); ++i) {
        auto &level = levels[i];
        int size = level.size();
        int occ = occupancies[i];
        double free = size - occ;
        double load = free / static_cast<double>(size);
        int probe_limit = std::max( 1, static_cast<int>(c * std::min(
            load > 0 ? std::log2(1 / load) : 0, 
            std::log2(1 / delta)))
        );
        if (i < levels.size() - 1) {
            auto& next_level = levels[i + 1];
            int next_occ = occupancies[i + 1];
            int next_free = next_level.size() - next_occ;
            double load_next = next_level.size() > 0 ? next_free / static_cast<double>(next_level.size()) : 0;
            double threshold = 0.25;

            if (load > (delta / 2) && load_next < threshold) {
                for (int j = 0; j < probe_limit; ++j) {
                    int idx = _quad_probe(key, i, j, size);
                    if (!level[idx].occupied) {
                        level[idx] = {key, value, true};
                        ++occupancies[i];
                        ++num_inserts;
                        return true;
                    }
                }
            } else if (load <= (delta / 2)) {
                continue;
            } else if (load_next <= threshold) {
                for (int j = 0; j < probe_limit; ++j) {
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
            for (int j = 0; j < probe_limit; ++j) {
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
    return false;
}

std::optional<int> ElasticHashTable::search(const std::string &key) {
    for (size_t i = 0; i < levels.size(); ++i) {
        auto &level = levels[i];
        int size = level.size();
        
        for (int j = 0; j < size; ++j) {
            int idx = _quad_probe(key, i, j, size);
            if (!level[idx].occupied) break;
            if (level[idx].key == key) return level[idx].value;
        }
    }
    return std::nullopt;
}

bool ElasticHashTable::contains(const std::string &key) {
    return search(key).has_value();
}

int ElasticHashTable::size() const {
    return num_inserts;
}

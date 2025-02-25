#include <iostream>
#include <string>
#include <cassert>
#include <chrono>
#include "elastic_hashing.h"  // Header file for ElasticHashTable
#include "funnel_hashing.h"   // Header file for FunnelHashTable

void test_elastic_hash_table() {
    int capacity = 1000;
    double delta = 0.1;
    ElasticHashTable<std::string, int> et_si(capacity, delta);
    ElasticHashTable<std::string, std::string> et_ss(capacity, delta);
    int n_insert = capacity - static_cast<int>(delta * capacity);
    
    for (int i = 0; i < n_insert; ++i) {
        if (i % 2) {
            et_si.insert("key" + std::to_string(i), i);
            et_ss["key" + std::to_string(i)] = "value" + std::to_string(i);
        } else {
            et_si["key" + std::to_string(i)] = i;
            et_ss.insert("key" + std::to_string(i), "value" + std::to_string(i));
        }
    }
    
    for (int i = 0; i < n_insert; ++i) {        
        auto res_si = i % 2 ? et_si.search("key" + std::to_string(i)) : et_si["key" + std::to_string(i)];
        auto res_ss = i % 2 ? et_ss["key" + std::to_string(i)] : et_ss.search("key" + std::to_string(i));
        
        if (res_si.value() != i) {
            std::cout << "Expected: value \"" + std::to_string(i) << "\", but got: " << res_si.value() << std::endl;
        }

        if (res_ss.value() != "value" + std::to_string(i)) {
            std::cout << "Expected: value \"" << "value" + std::to_string(i) << "\", but got: " << res_ss.value() << std::endl;
        }
        
        assert(res_si && res_si.value() == i);
        assert(res_ss && res_ss.value() == "value" + std::to_string(i));
    }

    assert(! et_si.search("nonexistent").has_value());
    assert(! et_ss.search("nonexistent").has_value());
    
    // std::cout << "ElasticHashTable tests passed!\n";
    // std::cout << "ElasticHashTable tests passed in " << duration.count() << " seconds!" << std::endl;
}

void test_funnel_hash_table() {
    int capacity = 1000;
    double delta = 0.1;
    FunnelHashTable<std::string, int> ft_si(capacity, delta);
    FunnelHashTable<std::string, std::string> ft_ss(capacity, delta);
    int n_insert = capacity - static_cast<int>(delta * capacity);
    
    for (int i = 0; i < n_insert; ++i) {
        if (i % 2) {
            ft_si.insert("key" + std::to_string(i), i);
            ft_ss["key" + std::to_string(i)] = "value" + std::to_string(i);
        } else {
            ft_si["key" + std::to_string(i)] = i;
            ft_ss.insert("key" + std::to_string(i), "value" + std::to_string(i));
        }
    }
    
    for (int i = 0; i < n_insert; ++i) {
        auto res_si = i % 2 ? ft_si.search("key" + std::to_string(i)) : ft_si["key" + std::to_string(i)];
        auto res_ss = i % 2 ? ft_ss["key" + std::to_string(i)] : ft_ss.search("key" + std::to_string(i));
        
        if (res_si.value() != i) {
            std::cout << "Expected: value \"" + std::to_string(i) << "\", but got: " << res_si.value() << std::endl;
        }

        if (res_ss.value() != "value" + std::to_string(i)) {
            std::cout << "Expected: value \"" << "value" + std::to_string(i) << "\", but got: " << res_ss.value() << std::endl;
        }
        
        assert(res_si && res_si.value() == i);
        assert(res_ss && res_ss.value() == "value" + std::to_string(i));
        
        
    }

    assert(! ft_si.search("nonexistent").has_value());
    assert(! ft_ss.search("nonexistent").has_value());

    // std::cout << "FunnelHashTable tests passed!\n";
    // std::cout << "FunnelHashTable tests passed in " << duration.count() << " seconds!" << std::endl;
}

int main() {
    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < 1000; i++) {
        test_funnel_hash_table();
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;
    std::cout << "Funnel Hash: " << duration.count() << " seconds" << std::endl;

    start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < 1000; i++) {
        test_elastic_hash_table();
    }
    end = std::chrono::high_resolution_clock::now();
    duration = end - start;
    std::cout << "Elastic Hash: " << duration.count() << " seconds" << std::endl;
    // test_funnel_hash_table();
    // test_elastic_hash_table();
    std::cout << "Done" << std::endl;
    return 0;
}

#include <iostream>
#include <string>
#include <cassert>
#include "elastic_hashing.h"  // Header file for ElasticHashTable
#include "funnel_hashing.h"   // Header file for FunnelHashTable

void test_elastic_hash_table() {
    int capacity = 1000;
    double delta = 0.1;
    ElasticHashTable<std::string, int> et_si(capacity, delta);
    ElasticHashTable<std::string, std::string> et_ss(capacity, delta);
    int n_insert = capacity - static_cast<int>(delta * capacity);
    
    for (int i = 0; i < n_insert; ++i) {
        et_si.insert("key" + std::to_string(i), i);
        et_ss.insert("key" + std::to_string(i), "value" + std::to_string(i));
    }

    // et_ss.print();
    
    for (int i = 0; i < n_insert; ++i) {        
        auto res_si = et_si.search("key" + std::to_string(i));
        auto res_ss = et_ss.search("key" + std::to_string(i));
        
        if (*res_si != i) {
            std::cout << "Expected: value \"" + std::to_string(i) << "\", but got: " << res_si.value() << std::endl;
        }

        if (*res_ss != "value" + std::to_string(i)) {
            std::cout << "Expected: value \"" << "value" + std::to_string(i) << "\", but got: " << res_ss.value() << std::endl;
        }
        
        assert(res_si && *res_si == i);
        assert(res_ss && *res_ss == "value" + std::to_string(i));
    }

    assert(! et_si.search("nonexistent").has_value());
    assert(! et_ss.search("nonexistent").has_value());
    
    std::cout << "ElasticHashTable tests passed!" << std::endl;
}

void test_funnel_hash_table() {
    int capacity = 1000;
    double delta = 0.1;
    FunnelHashTable<std::string, int> ft_si(capacity, delta);
    FunnelHashTable<std::string, std::string> ft_ss(capacity, delta);
    int n_insert = capacity - static_cast<int>(delta * capacity);
    
    for (int i = 0; i < n_insert; ++i) {
        ft_si.insert("key" + std::to_string(i), i);
        ft_ss.insert("key" + std::to_string(i), "value" + std::to_string(i));
    }
    
    for (int i = 0; i < n_insert; ++i) {
        assert(ft_si.search("key" + std::to_string(i)) == i);
        assert(ft_ss.search("key" + std::to_string(i)).value() == "value" + std::to_string(i));
    }

    assert(! ft_si.search("nonexistent").has_value());
    assert(! ft_ss.search("nonexistent").has_value());
    
    std::cout << "FunnelHashTable tests passed!" << std::endl;
}

int main() {
    test_funnel_hash_table();
    test_elastic_hash_table();
    std::cout << "Done" << std::endl;
    return 0;
}

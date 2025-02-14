#include <iostream>
#include <cassert>
#include "optopenhash/elastic_hashing.h"  // Assuming header file for ElasticHashTable
#include "optopenhash/funnel_hashing.h"   // Assuming header file for FunnelHashTable

void test_elastic_hash_table() {
    int capacity = 1000;
    double delta = 0.1;
    ElasticHashTable et(capacity, delta);
    int n_insert = capacity - static_cast<int>(delta * capacity);
    
    for (int i = 0; i < n_insert; ++i) {
        et.insert("key" + std::to_string(i), i);
    }
    
    for (int i = 0; i < n_insert; ++i) {        
        auto result = et.search("key" + std::to_string(i));
        
        if (*result != i) {
            std::cout << "Expected: value" + std::to_string(i) << ", but got: " << *result << std::endl;
        }
        
        assert(result && *result == i);
    }
    
    assert(! et.search("nonexistent").has_value());
    
    std::cout << "ElasticHashTable tests passed!" << std::endl;
}

void test_funnel_hash_table() {
    int capacity = 1000;
    double delta = 0.1;
    FunnelHashTable ft(capacity, delta);
    int n_insert = capacity - static_cast<int>(delta * capacity);
    
    for (int i = 0; i < n_insert; ++i) {
        ft.insert("key" + std::to_string(i), i);
    }
    
    for (int i = 0; i < n_insert; ++i) {
        assert(ft.search("key" + std::to_string(i)) == i);
    }

    assert(! ft.search("nonexistent").has_value());
    
    std::cout << "FunnelHashTable tests passed!" << std::endl;
}

int main() {
    test_elastic_hash_table();
    
    test_funnel_hash_table();
    return 0;
}

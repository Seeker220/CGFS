#ifndef HASHMAP_HPP
#define HASHMAP_HPP

#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

inline unsigned int djb2_hash(const std::string &str) {
    // http://www.cse.yorku.ca/~oz/hash.html
    unsigned long hash = 5381;
    for (char c: str) {
        hash = ((hash << 5) + hash) + c; // hash * 33 + c
    }
    return hash;
}

template<typename K>
struct CustomHasher {
    // std::hash replacement for string and int keys.
    unsigned int operator()(const K &key) const {
        if constexpr (std::is_same_v<K, int>) {
            return key;
        } else if constexpr (std::is_same_v<K, std::string>) {
            return djb2_hash(key);
        } else {
            throw std::invalid_argument("Custom Hash only supports int and std::string keys. Try using std::hash.");
        }
    }
};

template<typename K, typename V>
struct Bucket {
    // Node element in Hashmap Array
    K key;
    V value;
    Bucket *next;

    Bucket(K k, V v) {
        key = k;
        value = v;
        next = nullptr;
    }
};

template<typename K, typename V, typename Hasher = CustomHasher<K> >
class HashMap {
    // Implementation of Hash Map
private:
    std::vector<Bucket<K, V> *> table; // stores buckets
    int current_size;
    const float threshold = 0.75; // threshold for doubling
    Hasher hasher;

    int hash(const K &key) const {
        // gets index from key
        return (hasher(key) % table.size() + table.size()) % table.size();
    }

    void extend() {
        // doubling trick
        int oldc = table.size();
        std::vector<Bucket<K, V> *> oldt = move(table);
        table.assign(oldc * 2, nullptr);
        current_size = 0;
        for (int i = 0; i < oldc; i++) {
            Bucket<K, V> *current = oldt[i];
            while (current != nullptr) {
                insert(current->key, current->value);
                Bucket<K, V> *todel = current;
                current = current->next;
                delete todel;
            }
        }
    }

public:
    HashMap(int capacity = 16) {
        // constructor
        table.assign(capacity, nullptr);
        current_size = 0;
    }

    void clear() {
        // deletes the hashmap
        for (int i = 0; i < table.size(); i++) {
            Bucket<K, V> *current = table[i];
            while (current != nullptr) {
                Bucket<K, V> *todel = current;
                current = current->next;
                delete todel;
            }
            table[i] = nullptr;
        }
        current_size = 0;
    }

    ~HashMap() {
        // destructor
        clear();
    }

    bool count(const K &key) const {
        // checks if key is present
        Bucket<K, V> *current = table[hash(key)];
        while (current != nullptr) {
            if (current->key == key) {
                return true;
            }
            current = current->next;
        }
        return false;
    }

    void insert(const K &key, const V &value) {
        // updates/ inserts value into key
        if (current_size >= (table.size()) * threshold) {
            extend();
        }
        int idx = hash(key);
        Bucket<K, V> *current = table[idx];
        while (current != nullptr) {
            // update
            if (current->key == key) {
                current->value = value;
                return;
            }
            current = current->next;
        }
        Bucket<K, V> *newbucket = new Bucket<K, V>(key, value);
        newbucket->next = table[idx];
        table[idx] = newbucket; // insert
        current_size++;
    }

    const V &get(const K &key) const {
        // returns value of key if found
        Bucket<K, V> *current = table[hash(key)];
        while (current != nullptr) {
            if (current->key == key) {
                return current->value;
            }
            current = current->next;
        }
        throw std::out_of_range("Error: Key not found in HashMap.");
    }

    void remove(const K &key) {
        // removes key:value from hashmap if present
        int idx = hash(key);
        Bucket<K, V> *current = table[idx];
        if ((current != nullptr) && (current->key == key)) {
            table[idx] = table[idx]->next;
            delete current;
            current_size--;
            return;
        }
        while ((current != nullptr) && (current->next != nullptr)) {
            if (current->next->key == key) {
                Bucket<K, V> *todel = current->next;
                current->next = current->next->next;
                delete todel;
                current_size--;
                return;
            }
            current = current->next;
        }
    }

    void print() const {
        // prints all key:value pairs
        for (int i = 0; i < table.size(); i++) {
            if (table[i] != nullptr) {
                Bucket<K, V> *current = table[i];
                while (current != nullptr) {
                    std::cout << current->key << " : " << current->value << "\n";
                    current = current->next;
                }
            }
        }
    }

    V &operator[](const K &key) {
        // update or insert key, access value by key, if key not present, it creates a key with default value.
        Bucket<K, V> *current = table[hash(key)];
        while (current != nullptr) {
            if (current->key == key) {
                return current->value;
            }
            current = current->next;
        }
        insert(key, V());
        return table[hash(key)]->value;
    }
};

#endif

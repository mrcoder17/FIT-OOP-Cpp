#include <iostream>
#include <cstring>
#include <list>
typedef std::string Key;

struct Value {
    unsigned age;
    unsigned weight;
    bool operator == (const Value& v){
        return age == v.age && weight == v.weight;
    }
};

struct Chain {
    Chain *next;
    Value val;
    Key key;
    Chain(Chain *next_, const Value& val_, const Key& key_){
        next = next_;
        val = val_;
        key = key_;
    }
};


class HashTable
{
private:
    Chain **arr;
    size_t _size;
    int calc_hash(const Key& k) const {
        int hash = 0;
        for (int i = 0; i < k.size(); ++i) {
            hash += k[i];
        }
        return hash % 977;
    };

public:
    HashTable(){
        _size = 0;
        arr = nullptr;
    };

    ~HashTable(){
        for (size_t i = 0; i < _size; ++i)
            if (arr[i])
                delete arr[i];
        delete[] arr;
    };

    HashTable(const HashTable& b){
        _size = b._size;
        arr = new Chain *[_size];
        for (size_t i = 0; i < _size; ++i) {
            arr[i] = nullptr;
        }
        for (size_t  i = 0; i < _size; ++i) {
            Chain *cur_b = b.arr[i];
            while (cur_b != nullptr) {
                insert(cur_b->key, cur_b->val);
                cur_b = cur_b->next;
            }
        }
    };
    
    void swap(HashTable& b) {
        std::swap(arr, b.arr);
        std::swap(_size, b._size);
    };

    void clear() {
        for (int i = 0; i < _size; ++i) {
            while (arr[i] != nullptr) {
                Chain *cur = arr[i]->next;
                delete arr[i];
                arr[i] = cur;
            }
            arr[i] = nullptr;
        }
        delete[] arr;
        _size = 0;
    };
    
    bool erase(const Key& k){
        int hash = calc_hash(k);
        Chain *cur = arr[hash];
        Chain *prev = nullptr;

        while (cur != nullptr) {
            if (cur->key == k) {
                if (prev == nullptr)
                    arr[hash] = cur->next;
                else
                    prev->next = cur->next;
                delete cur;
                return true;
            }
            prev = cur;
            cur = cur->next;
        }

        return false;
    };
    
    bool insert(const Key& k, const Value& v){
        int hash = calc_hash(k);
        if (hash >= _size){
            size_t size = _size;
            _size = hash * 2 + 1;
            Chain ** arr1 = new Chain *[_size];
            for (size_t i = 0; i < size; i++){
                arr1[i] = arr[i];
            }
            for (size_t  i = size; i < _size; i++) {
                arr1[i] = nullptr;
            }
            delete[] arr;
            arr = arr1;
        }
        if (arr[hash] == nullptr){
            arr[hash] = new Chain (nullptr, v, k);
            return true;
        }
        else{
            Chain *cur = arr [hash];
            Chain *prev = nullptr;
            while (cur != nullptr){
                if (cur->key == k)
                    return false;
                prev = cur;
                cur = cur->next;
            }
            cur = new Chain(nullptr, v, k);
            prev->next = cur;
            return true;
        }
    };

    bool contains(const Key &k) const {
        int hash = calc_hash(k);
        Chain *cur = arr[hash];
        while (cur != nullptr) {
            if (cur->key == k) {
                return true;
            }
            cur = cur->next;
        }
        return false;
    };

    Value& at(const Key& k){
        int hash = calc_hash(k);
        Chain *cur = arr[hash];
        while (cur != nullptr) {
            if (cur->key == k)
                return cur->val;
            cur = cur->next;
        }
        throw std::invalid_argument("error");
    };

    const Value& at(const Key& k) const{
        int hash = calc_hash(k);
        const Chain *cur = arr[hash];
        while (cur != nullptr) {
            if (cur->key == k)
                return cur->val;
            cur = cur->next;
        }
        throw std::invalid_argument("error");
    };

    size_t size() const { return _size; }
    bool empty() const{
        return _size == 0;
    };


    HashTable &operator=(const HashTable &b){
        clear();
        _size = b._size;
        arr = new Chain *[_size];
        for (size_t i = 0; i < _size; ++i) {
            arr[i] = nullptr;
        }
        for (size_t  i = 0; i < _size; ++i) {
            Chain *cur_b = b.arr[i];
            while (cur_b != nullptr) {
                insert(cur_b->key, cur_b->val);
                cur_b = cur_b->next;
            }
        }
        return *this;
    };

    Value& operator[](const Key& k){
        int hash = calc_hash(k);
        if (!contains(k))
            insert(k, {0, 0});
        Chain *cur = arr[hash];
        while (cur != nullptr) {
            if (cur->key == k)
                return cur->val;
            cur = cur->next;
        }
    }

    friend bool operator==(const HashTable& a, const HashTable& b){
        if(a._size != b._size)
            return false;

        for (int i = 0; i < a._size; ++i) {
            Chain *cur_a = a.arr[i];
            while (cur_a != nullptr){
                Chain *cur_b = b.arr[i];
                while  (cur_b != nullptr){
                    if ((cur_a->val == cur_b->val) && (cur_a->key == cur_b->key)) {
                        break;
                    }
                    cur_b = cur_b->next;
                }
                if (cur_b == nullptr) {
                    return false;
                }
                cur_a = cur_a->next;
            }
        }

        return true;
    }
    friend bool operator!=(const HashTable &a, const HashTable &b) {
        return !(a==b);
    };
};




int main() {
    return 0;
}
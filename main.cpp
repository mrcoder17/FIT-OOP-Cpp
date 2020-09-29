#include <iostream>
#include <gtest/gtest.h>

typedef std::string Key;
using testing::Eq;

struct Value {
    unsigned age;
    unsigned weight;
    bool operator == (const Value& v) const{
        return age == v.age && weight == v.weight;
    }
};


struct Node {
    Node *next;
    Value val;
    Key key;
    Node(Node *next_, const Value& val_, const Key& key_){
        next = next_;
        val = val_;
        key = key_;
    }
};


class HashTable
{
private:
    Node **arr;
    size_t _size;
    static int calc_hash(const Key& k) {
        int hash = 0;
        for (char i : k) {
            hash += i;
        }
        return hash % 977;
    };

public:
    HashTable(){
        _size = 0;
        arr = nullptr;
    }

    ~HashTable(){
        for (size_t i = 0; i < _size; ++i)
            if (arr[i])
                delete arr[i];
        delete[] arr;
    }

    HashTable(const HashTable& b){
        _size = b._size;
        arr = new Node *[_size];
        for (size_t i = 0; i < _size; ++i) {
            arr[i] = nullptr;
        }
        for (size_t  i = 0; i < _size; ++i) {
            Node *cur2 = b.arr[i];
            while (cur2 != nullptr) {
                insert(cur2->key, cur2->val);
                cur2 = cur2->next;
            }
        }
    }

    void swap(HashTable& b) {
        std::swap(arr, b.arr);
        std::swap(_size, b._size);
    }

    void clear() {
        for (int i = 0; i < _size; ++i) {
            while (arr[i] != nullptr) {
                Node *cur = arr[i]->next;
                delete arr[i];
                arr[i] = cur;
            }
            arr[i] = nullptr;
        }
        delete[] arr;
        _size = 0;
    }

    bool erase(const Key& k){
        int hash = calc_hash(k);
        Node *cur = arr[hash];
        Node *prev = nullptr;

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
    }

    bool insert(const Key& k, const Value& v){
        int hash = calc_hash(k);
        if (hash >= _size){
            size_t size = _size;
            _size = hash * 2 + 1;
            Node ** arr1 = new Node *[_size];
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
            arr[hash] = new Node (nullptr, v, k);
            return true;
        }
        else{
            Node *cur = arr [hash];
            Node *prev = nullptr;
            while (cur != nullptr){
                if (cur->key == k)
                    return false;
                prev = cur;
                cur = cur->next;
            }
            cur = new Node(nullptr, v, k);
            prev->next = cur;
            return true;
        }
    }

    bool contains(const Key &k) const {
        int hash = calc_hash(k);
        Node *cur = arr[hash];
        while (cur != nullptr) {
            if (cur->key == k) {
                return true;
            }
            cur = cur->next;
        }
        return false;
    }

    Value& at(const Key& k){
        int hash = calc_hash(k);
        Node *cur = arr[hash];
        while (cur != nullptr) {
            if (cur->key == k)
                return cur->val;
            cur = cur->next;
        }
        throw std::invalid_argument("error");
    }

    const Value& at(const Key& k) const{
        int hash = calc_hash(k);
        const Node *cur = arr[hash];
        while (cur != nullptr) {
            if (cur->key == k)
                return cur->val;
            cur = cur->next;
        }
        throw std::invalid_argument("error");
    }

    size_t size() const { return _size; }

    bool empty() const{ return _size == 0;}


    HashTable &operator=(const HashTable &b){
        clear();
        _size = b._size;
        arr = new Node *[_size];
        for (size_t i = 0; i < _size; ++i) {
            arr[i] = nullptr;
        }
        for (size_t  i = 0; i < _size; ++i) {
            Node *cur2 = b.arr[i];
            while (cur2 != nullptr) {
                insert(cur2->key, cur2->val);
                cur2 = cur2->next;
            }
        }
        return *this;
    }

    Value& operator[](const Key& k){
        int hash = calc_hash(k);
        if (!contains(k))
            insert(k, {0, 0});
        Node *cur = arr[hash];
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
            Node *cur1 = a.arr[i];
            while (cur1 != nullptr){
                Node *cur2 = b.arr[i];
                while  (cur2 != nullptr){
                    if ((cur1->val == cur2->val) && (cur1->key == cur2->key)) {
                        break;
                    }
                    cur2 = cur2->next;
                }
                if (cur2 == nullptr) {
                    return false;
                }
                cur1 = cur1->next;
            }
        }

        return true;
    }
    friend bool operator!=(const HashTable &a, const HashTable &b) {
        return !(a==b);
    };
};

namespace {
    class HTTest : public testing::Test {
    };
}

TEST_F (HTTest, test1){
    HashTable a;
    a.insert("Key1", {22, 90});
    a.clear();
    ASSERT_EQ(true, a.empty());
}

TEST_F(HTTest, test2) {
    HashTable a;
    a.insert ("one", {1, 2});
    Value c = {1, 2};
    EXPECT_EQ(c, a.at ("one"));
}

TEST_F(HTTest, test3){
    HashTable a, b;
    a.insert ("one_a", {1, 2});
    b.insert ("one_b", {3, 4});
    a.swap(b);
    EXPECT_EQ( a.at("one_b").age==3, b.at ("one_a").age==1);
    EXPECT_EQ( a.at("one_b").weight==4, b.at ("one_a").weight==2);
}

TEST_F(HTTest, test4){
    HashTable a;
    a.insert ("one", {1, 2});
    EXPECT_EQ(true, a.contains("one"));
}

TEST_F(HTTest, test5){
    HashTable a;
    a.insert ("", {});
    EXPECT_EQ(false, a.erase("key"));

}

TEST_F(HTTest, test6){
    HashTable a,b;
    a.insert ("one", {1, 2});
    a.insert ("omf", {3, 4});
    a.erase("one");
    EXPECT_EQ(true, a.at("omf").age == 3);
    EXPECT_EQ(true, a.at("omf").weight == 4);
}

TEST_F (HTTest, test7){
    HashTable a;
    a.insert("Key1", {22, 90});
    a.clear();
    ASSERT_EQ(false, a.contains("Key1"));
}

// 94% lines covered

int main() {
    testing::InitGoogleTest();
    RUN_ALL_TESTS();
    return 0;
}

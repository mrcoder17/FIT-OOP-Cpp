#include <iostream>
#include <ostream>

class Array{
private:
    int size;
    int *pointer;

public:
    ~Array(){
        delete [] pointer;
    }

    void inputArray(){
        for (int i = 0; i < size; i++)
            std:: cin >> pointer[i];
    }

    void outputArray(){
        for (int i = 0; i < size; ++i)
            std:: cout << pointer[i];
    }

    int operator[](int index){
        return pointer[index];
    }

    Array() {
        size = 10;
        pointer = new int[size];
        for (int i = 0; i < size; i++)
            pointer[i] = 0;
    }

    Array(const Array &ArrayNew) : size(ArrayNew.size){
        pointer = new int [size];
        for (int i = 0; i < size; i++)
            pointer[i] = ArrayNew.pointer[i];
    }

    const Array &operator= (const Array &need){
        if (&need != this){
            if (size != need.size){
                delete [] pointer;
                size = need.size;
                pointer = new int [size];
            }
            for (int i = 0; i < size; i++)
                pointer[i] = need.pointer[i];
        }
        return *this;
    }
};

int main(){
}
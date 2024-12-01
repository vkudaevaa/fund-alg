#include <iostream>
#include <initializer_list>
#include <stdexcept>
#include <algorithm>
#include <vector>
#include <compare>
#include <cmath>



class vector{
    double* data;
    size_t capacity;
    size_t size;

    void reallocate(size_t num){
        double* new_data = new double[num];
        std::copy(data, data + size, new_data);
        delete[] data;
        data = new_data;
        capacity = num;
    }

public:
    vector(size_t size_v, double value) : size(size_v), capacity(size_v), data(new double[size_v]){
        for (size_t i = 0; i < size_v; ++i){
            data[i] = value;
        }
    }

    vector(size_t size_v) : size(size_v), capacity(size_v), data(new double[size_v]){
        for (size_t i = 0; i < size_v; ++i){
            data[i] = 0.0;
        }
    }

    vector(std::vector<double>::iterator start, std::vector<double>::iterator end){
        size = end - start;
        capacity = size;
        data = new double[size];
        std::copy(start, end, data);
    }

    vector(std::initializer_list<double> init) : size(init.size()), capacity(init.size()), data(new double[init.size()]) {
        size_t index = 0;
        for(double n: init){
            data[index++] = n;
        }
    }

    ~vector(){
        delete[] data;
    }

    double& at(size_t index) const {
        if (index >= size){
            throw std::out_of_range("Index out of range");
        }
        return data[index];
    }

    double& front() const {
        if (size == 0){
            throw std::out_of_range("Index out of range");
        }
        return data[0];
    }

    double& back() const {
        if (empty()){
            throw std::out_of_range("Index out of range");
        }
        return data[size - 1];
    }

    double* get_data() const {return data;}

    bool empty() const { return size == 0;}

    size_t get_size() const {return size;}



    void reserve(size_t num){
        if (capacity < num){
            reallocate(num);
        }
    }

    size_t get_capacity() const {return capacity;}

    void shrink_to_fit(){
        if (capacity > size){
            reallocate(size);
        }
    }

    void clear(){
        for (size_t i = 0; i < size; ++i){
            data[i] = 0.0;
        }
        size = 0;
    }

    void insert(size_t index, double elem){
        if (index > size) throw std::out_of_range("Index out of range");
        if (size == capacity) reserve(capacity == 0 ? 1 : capacity * 2);
        for (size_t i = size; i > index; --i) {
            data[i] = data[i - 1];
        }
        data[index] = elem;
        ++size;
    }

    void erase(size_t index){
        if (index > size) throw std::out_of_range("Index out of range");
        for (size_t i = index; i < size - 1; ++i) {
            data[i] = data[i + 1];
        }
        --size;
    }

    void  push_back(double elem){
        if (size == capacity) reserve(capacity == 0 ? 1 : capacity * 2);
        data[size++] = elem;
    }

    void pop_back(){
        if (empty()) throw std::out_of_range("Index out of range");
        --size;
    }

    void resize(size_t new_size, double elem){
        if (new_size > capacity) reserve(new_size);
        if (new_size > size){
            for (size_t i = size; i < capacity; ++i){
                data[i] = elem;
            }
        }
        size = new_size;
    }

    bool operator==(const vector &other) const {
        if (size != other.size) return false;
        return std::equal(data, data + size, other.data);
    }

    std::partial_ordering operator<=>(const vector& other) const {

        size_t min_size = std::min(size, other.size);
        for (size_t i = 0; i < min_size; ++i) {

            if (std::isnan(data[i]) || std::isnan(other.data[i])) {
                return std::partial_ordering::unordered;
            }
            if (data[i] < other.data[i]) {
                return std::partial_ordering::less;
            }
            if (data[i] > other.data[i]) {
                return std::partial_ordering::greater;
            }
        }

        if (size < other.size) {
            return std::partial_ordering::less;
        }
        if (size > other.size) {
            return std::partial_ordering::greater;
        }
        return std::partial_ordering::equivalent;
    }

    class Iterator{
        double* ptr;

    public:
        Iterator(double* p) : ptr(p) {}

        double& operator*() { return *ptr; }

        Iterator& operator++() {
            ++ptr;
            return *this;
        }

        Iterator operator++(int) {
            Iterator temp = *this;
            ++(*this);
            return temp;
        }

        Iterator operator+(size_t i) const {
            return Iterator(ptr + i);
        }

        Iterator operator-(size_t i) const {
            return Iterator(ptr - i);
        }

        Iterator& operator--() {
            --ptr;
            return *this;
        }

        Iterator operator--(int) {
            Iterator temp = *this;
            --(*this);
            return temp;
        }

        Iterator& operator+=(size_t i) {
            *this = *this + i;
            return *this;
        }

        Iterator& operator-=(size_t i) {
            *this = *this - i;
            return *this;
        }

        bool operator==(const Iterator& other) const {
            return ptr == other.ptr;
        }
        bool operator!=(const Iterator& other) const {
            return ptr != other.ptr;
        }

        double& operator[] (size_t i) const {
            return *(ptr + i);
        }

        double* operator->() {return ptr;}

    };

    Iterator begin() {return Iterator(data);}
    Iterator end() { return Iterator(data + size);}

};


int main(){
    try{

        vector v1 = {1.1, 2.2, 3.3, 4.4};
        v1.push_back(5.5);
        v1.push_back(6.6);
        v1.push_back(7.7);
        v1.push_back(8.8);

        std::cout << "v1 = { ";
        for (double n: v1) {
            std::cout << n << " ";
        }
        std::cout << "}" << std::endl;

        v1.erase(5);
        v1.erase(0);

        std::cout << "v1 = { ";
        for (double n: v1) {
            std::cout << n << " ";
        }
        std::cout << "}" << std::endl;

        v1.insert(4, 0.0);
        v1.insert(3, 0.0);

        std::cout << "v1 = { ";
        for (double n: v1) {
            std::cout << n << " ";
        }
        std::cout << "}" << std::endl;

        vector v2 = {1.9, 2.8, 3.7, 4.6, 5.5, 6.4};
        v2.pop_back();
        v2.push_back(0.0);
        v2.push_back(0.0);

        std::cout << "v2 = { ";
        for (vector::Iterator it = v2.begin(); it != v2.end(); ++it) {
            std::cout << *it << " ";
        }
        std::cout << "}" << std::endl;

        std::cout << "begin + 3 : " << *(v2.begin() + 3) << std::endl;
        std::cout << "begin[3]  : " << v2.begin()[3] << std::endl;
        std::cout << "end [-3]  : " << v2.end()[-3] << std::endl;

        vector v3 = {1, 2, 3, 4, 5};
        std::cout << "v3 = { ";
        for (double n: v3) {
            std::cout << n << " ";
        }
        std::cout << "}" << std::endl;

        vector v4 = {1, 2, 3, 4, 5};
        std::cout << "v4 = { ";
        for (double n: v4) {
            std::cout << n << " ";
        }
        std::cout << "}" << std::endl;

        std::cout << "v3 == v4: " << (v3 == v4) << std::endl;
        std::cout << "v1 == v2: " << (v1 == v2) << std::endl;
        std::cout << "v1 < v2 : " << (v1 < v2) << std::endl;
        std::cout << "v2 > v3 : " << (v2 > v3) << std::endl;

    }catch(const std::out_of_range &err){
        std::cout << "Error: " << err.what() << std::endl;
    }
}
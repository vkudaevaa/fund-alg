#include <iostream>
#include <bitset>

class binary_int {
private:
    int value;
public:
    binary_int() {value = 0;}
    binary_int(int a) {value = a;}
    int get_val() {return value;}

    binary_int operator+(const binary_int& other) const;
    binary_int& operator++();
    binary_int operator++(int);
    binary_int& operator--();
    binary_int operator--(int);
    binary_int operator-() const;
    binary_int& operator+=(const binary_int& other);
    binary_int operator-(const binary_int& other) const;
    binary_int& operator-=(const binary_int& other);
    binary_int operator<<(int shift) const;
    binary_int operator>>(int shift) const;
    binary_int& operator<<=(int shift);
    binary_int& operator>>=(int shift);
    binary_int operator*(const binary_int& other) const;
    binary_int& operator*=(const binary_int& other);
    friend std::ostream& operator<<(std::ostream& os, const binary_int& obj);
    std::pair<binary_int, binary_int> split() const;
};

int add(int a, int b) {
    while (b != 0) {
        int carry = a & b; 
        a = a ^ b; 
        b = carry << 1;
    }
    return a;
}

int increment(int n) {
    int mask = 1;
    while (n & mask) {
        n = n ^ mask; 
        mask <<= 1;   
    }
    n = n ^ mask;
    return n;
}

int decrement(int n){
    return add(n, add(~1, 1));
}

int mult(int a, int b){
    int res = 0;
    while (b != 0) {        
        if (b & 1) {        
            res = add(res, a);    
        }
        a <<= 1;            
        b >>= 1;            
    }
    return res;
}

binary_int binary_int::operator+(const binary_int& other) const{
    return binary_int(add(value, other.value));
}

binary_int& binary_int::operator++(){
    value = increment(value);
    return *this;
}

binary_int binary_int::operator++(int){
    binary_int temp = *this;
    value = increment(value);
    return temp;
}

binary_int& binary_int::operator--(){
    value = decrement(value);
    return *this;
}

binary_int binary_int::operator--(int){
    binary_int temp = *this;
    value = decrement(value);
    return temp;
}

binary_int binary_int::operator-() const{
    return binary_int(~value + 1);
}

binary_int& binary_int::operator+=(const binary_int& other){
    value = add(value, other.value);
    return *this;
}

binary_int binary_int::operator-(const binary_int& other) const{
    return binary_int(add(value, -other.value));
}

binary_int& binary_int::operator-=(const binary_int& other){
    value = add(value, -other.value);
    return *this;
}

binary_int binary_int::operator<<(int shift) const{
    return binary_int(value<<shift);
}

binary_int binary_int::operator>>(int shift) const{
    return binary_int(value>>shift);
}

binary_int& binary_int::operator<<=(int shift){
    value = value<<shift;
    return *this;
}

binary_int& binary_int::operator>>=(int shift){
    value = value>>shift;
    return *this;
}

binary_int binary_int::operator*(const binary_int& other) const{
    return binary_int(mult(value, other.value));
}

binary_int& binary_int::operator*=(const binary_int& other){
    value = mult(value, other.value);
    return *this;
}

std::ostream& operator<<(std::ostream& os, const binary_int& ob) {
    os << std::bitset<32>(ob.value); 
    return os;
}

std::pair<binary_int, binary_int> binary_int::split() const {
    int high = value & 0xFFFF0000;  
    int low = value & 0x0000FFFF;  
    return {binary_int(high), binary_int(low)};
}

int main(){

    binary_int a(5), b(3);

    std::cout << "a = " << a << ", b = " << b << "\n";

    binary_int c = a + b;
    std::cout << "a + b = " << c << "\n";

    a += b;
    std::cout << "a += b: " << a << "\n";

    binary_int d = a * b;
    std::cout << "a * b = " << d << "\n";
    a *= b;
    std::cout << "a *= b: " << a << "\n";

    binary_int e = a - b;
    std::cout << "a - b = " << e << "\n";
    a -= b;
    std::cout << "a -= b: " << a << "\n";

    std::cout << "a = " << a << "\n";
    std::cout << "-a = " << -a << "\n";
    std::cout << "++a : " << ++a << "\n";
    std::cout << "a++: " << a++ << "\n";
    std::cout << "--a : " << --a << "\n";
    std::cout << "a--: " << a-- << "\n";

    std::cout << "b = " << b << "\n";
    c = b << 2;
    std::cout << "b << 2 = " << c << "\n";
    c = b >> 1;
    std::cout << "b >> 1 = " << c << "\n";

    std::cout << "a = " << a << "\n";
    a <<= 2;
    std::cout << "a <<=2: " << a << "\n";
    a >>= 1;
    std::cout << "a >>= 1: " << a << "\n";

    binary_int w(999999999);
    std::cout << "w = " << w << "\n";
    std::pair <binary_int, binary_int> p = w.split();
    std::cout << "high part = " << p.first << ", low part = " << p.second << "\n";
    return 0;
}
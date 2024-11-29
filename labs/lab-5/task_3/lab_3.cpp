#include <iostream>
#include <bitset>
#define SIZE 33

class logical_values_array{
    unsigned int value;
public:
    logical_values_array(unsigned int num = 0): value(num){}
    unsigned int get_value() const{return value;}
    void set_value(unsigned int new_num){value = new_num;}
    static bool equals(const logical_values_array &a, const logical_values_array &b){return a.value == b.value;}
    bool get_bit(unsigned int pos) const;
    logical_values_array inversion() const;
    logical_values_array conjunction(const logical_values_array &other) const;
    logical_values_array disjunction(const logical_values_array &other) const;
    logical_values_array implication(const logical_values_array &other) const;
    logical_values_array equivalence(const logical_values_array &other) const;
    logical_values_array xor_mod_two(const logical_values_array &other) const;
    logical_values_array pierce_arrow(const logical_values_array &other) const;
    logical_values_array sheffer_stroke(const logical_values_array &other) const;
    logical_values_array coimplication(const logical_values_array &other) const;
    void valueToString(char* buf) const;

};

logical_values_array logical_values_array::inversion() const {
    return logical_values_array(~value);
}

logical_values_array logical_values_array::conjunction(const logical_values_array &other) const{
    unsigned int new_val = value & other.value;
    return logical_values_array(new_val);
}

logical_values_array logical_values_array::disjunction(const logical_values_array &other) const{
    unsigned int new_val = value | other.value;
    return logical_values_array(new_val);
}

logical_values_array logical_values_array::implication(const logical_values_array &other) const{
    unsigned int new_val = ~value | other.value;
    return logical_values_array(new_val);
}

logical_values_array logical_values_array::equivalence(const logical_values_array &other) const{
    unsigned int new_val = (value & other.value) | (~value & ~other.value);
    return logical_values_array(new_val);
}

logical_values_array logical_values_array::xor_mod_two(const logical_values_array &other) const{
    unsigned int new_val = value ^ other.value;
    return logical_values_array(new_val);
}

logical_values_array logical_values_array::pierce_arrow(const logical_values_array &other) const{
    unsigned int new_val = ~(value | other.value);
    return logical_values_array(new_val);
}

logical_values_array logical_values_array::sheffer_stroke(const logical_values_array &other) const{
    unsigned int new_val = ~(value & other.value);
    return logical_values_array(new_val);
}

logical_values_array logical_values_array::coimplication(const logical_values_array &other) const{
    unsigned int new_val = value & ~other.value;
    return logical_values_array(new_val);
}

bool logical_values_array::get_bit(unsigned int pos) const{
    if (pos < 0 || pos > 31){
        throw std::out_of_range("Index out of range");
    }
    std::bitset<32> val(value);
    return val.test(pos);
}

void logical_values_array::valueToString(char* buf) const{
    for (size_t i = 0; i < SIZE - 1; ++i) {
        buf[SIZE - 2 - i] = (value & (1u << i)) ? '1' : '0';
    }
    buf[SIZE - 1] = '\0';
}

int main() {
    
    logical_values_array a(5); 
    logical_values_array b(3); 

    std::cout << "a = " << a.get_value() << ", b = " << b.get_value() << std::endl;

    logical_values_array c = a.conjunction(b); 
    std::cout << "Conjunction: " << c.get_value() << std::endl;

    logical_values_array d = a.disjunction(b); 
    std::cout << "Disjunction: " << d.get_value() << std::endl;

    logical_values_array e = a.inversion(); 
    std::cout << "Inversion : " << e.get_value() << std::endl;

    logical_values_array f = a.implication(b); 
    std::cout << "Implication : " << f.get_value() << std::endl;

    logical_values_array g = a.equivalence(b); 
    std::cout << "Equivalence : " << g.get_value() << std::endl;

    logical_values_array h = a.xor_mod_two(b); 
    std::cout << "Xor mod 2 : " << h.get_value() << std::endl;

    logical_values_array j = a.pierce_arrow(b); 
    std::cout << "Pierce arrow : " << j.get_value() << std::endl;

    logical_values_array k = a.sheffer_stroke(b); 
    std::cout << "Sheffer stroke : " << k.get_value() << std::endl;

    logical_values_array l = a.coimplication(b); 
    std::cout << "Coimplication : " << l.get_value() << std::endl;

    char buffer[SIZE]; 
    a.valueToString(buffer);
    std::cout << "Binary representation of a: " << buffer << std::endl;

    try {
        std::cout << "Bit at position 1 in a: " << a.get_bit(1) << std::endl;
    } catch (const std::exception& ex) {
        std::cerr << "Error: " << ex.what() << std::endl;
    }

    if (logical_values_array::equals(a, b)) {
        std::cout << "a is equal to b" << std::endl;
    } else {
        std::cout << "a is not equal to b" << std::endl;
    }

}
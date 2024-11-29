#include <iostream>
#include <cmath>
#include <stdexcept>

class Complex {
private:
    double re;
    double im;
public:
    Complex() {re = 0.0; im = 0.0;}
    Complex(double a, double b) {re = a; im = b;}
    double get_re() {return re;}
    double get_im() {return im;}
    Complex add(const Complex& other) const;
    Complex sub(const Complex& other) const;
    Complex mult(const Complex& other) const;
    Complex div(const Complex& other) const;
    Complex conjugate() const;
    double module() const;
    double argument() const;

};

Complex Complex::add(const Complex& other) const {
    return Complex(re + other.re, im + other.im);
}

Complex Complex::sub(const Complex& other) const {
    return Complex(re - other.re, im - other.im);
}

Complex Complex::mult(const Complex& other) const {
    return Complex(re * other.re - im * other.im, im * other.re + re * other.im);
}

Complex Complex::conjugate() const {
    return Complex(re, - im);
}

Complex Complex::div(const Complex& other) const {
    Complex num = this->mult(other.conjugate());
    double denom = other.re * other.re + other.im * other.im;
    if (denom == 0){
        throw std::logic_error("Division by zero");
    }
    return Complex(num.re / denom, num.im / denom);
}

double Complex::module() const {
    return sqrt(re * re + im * im);
}

double Complex::argument() const {
    return atan2(im, re);
}

int main(){
    Complex one(1.0, 3.0);
    Complex two(2.0, 1.0);
    std::cout << "complex number 1 = " << one.get_re() << " + (" << one.get_im() << "i)" << "\n";
    std::cout << "complex number 2 = " << two.get_re() << " + (" << two.get_im() << "i)" << "\n";

    Complex res = one.add(two);
    std::cout << "addition(+) = " << res.get_re() << " + (" << res.get_im() << "i)" << "\n";
    res = one.sub(two);
    std::cout << "subtraction(-) = " << res.get_re() << " + (" << res.get_im() << "i)" << "\n";
    res = one.mult(two);
    std::cout << "multiplication(*) = " << res.get_re() << " + (" << res.get_im() << "i)" << "\n";
    
    try{
        res = one.div(two);
        std::cout << "division(/) = " << res.get_re() << " + (" << res.get_im() << "i)" << "\n";
    }catch (const std::logic_error& err){
        std::cout << "Error: " << err.what() << "\n";
    }
    
    std::cout << "number 1 module = " << one.module() << "\n";
    std::cout << "number 1 argument = " << one.argument() << "\n";
}
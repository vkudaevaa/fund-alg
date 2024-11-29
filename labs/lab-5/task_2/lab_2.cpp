#include <iostream>
#include <string>
#include <vector>
#include <cstddef>
#include <fstream>
#include <stdexcept>


class encoder {
    std::vector<std::byte> key;
    void arrayInitializer(std::vector<std::byte> &s);
    void mutator(std::vector<std::byte> &s, std::vector<std::byte> &key_array, size_t new_size);
public:
    encoder(const std::vector<std::byte> &k);
    void encode(const std::string &input, const std::string &output);
    
};

encoder::encoder(const std::vector<std::byte> &k): key(k) {}

void encoder::arrayInitializer(std::vector<std::byte> &s){
    for (size_t i = 0; i < 256; ++i){
        s[i] = static_cast<std::byte>(i);
    }

    size_t j = 0;
    for (size_t i = 0; i < 256; ++i) {
        j = (j + static_cast<size_t>(s[i]) + static_cast<size_t>(key[i % key.size()])) % 256;
        std::swap(s[i], s[j]);
    }
}

void encoder::mutator(std::vector<std::byte> &s, std::vector<std::byte> &key_array, size_t new_size){
    size_t i = 0, j = 0;
    key_array.resize(new_size);

    for (size_t k = 0; k < new_size; ++k) {
        i = (i + 1) % 256;
        j = (j + static_cast<size_t>(s[i])) % 256;
        std::swap(s[i], s[j]);
        key_array[k] = s[(static_cast<size_t>(s[i]) + static_cast<size_t>(s[j])) % 256];
    }
}

void encoder::encode(const std::string &input, const std::string &output){
    std::ifstream inputfile(input, std::ios::binary);
    if (!inputfile.is_open()) {
        throw std::runtime_error("The input file could not be opened");
    }

    std::ofstream outputfile(output, std::ios::binary);
    if (!outputfile.is_open()) {
        inputfile.close();
        throw std::runtime_error("The output file could not be opened");
    }

    char current;
    std::vector<std::byte> input_data;
    while (inputfile.get(current)) {
        input_data.push_back(static_cast<std::byte>(current));
    }

    std::vector<std::byte> s(256);
    arrayInitializer(s);
    std::vector<std::byte> key_array(input_data.size());
    mutator(s, key_array, input_data.size());

    std::vector<std::byte> output_data(input_data.size());
    for (size_t i = 0; i < input_data.size(); ++i){
        output_data[i] = input_data[i] ^ key_array[i];
    }

    for (const std::byte& current : output_data) {
        outputfile << static_cast<char>(current);
    }
    inputfile.close();
    outputfile.close();
}

int main(){
    try{
        std::vector<std::byte> key = {std::byte{1}, std::byte{2}, std::byte{3}, std::byte{4}, std::byte{5}};
        encoder en(key);
        en.encode("input.txt", "output-1.bin");
        std::cout << "The file has been successfully encrypted" << std::endl;

        en.encode("output-1.bin", "output-2.txt");
        std::cout << "The file has been successfully decrypted" << std::endl;
    }catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}
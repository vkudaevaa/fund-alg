#include <iostream>
#include <string>
#include <vector>


class Product{
protected:
    std::string name;
    unsigned int id;
    double weight;
    double price;
    unsigned int shelf_life;
public:
    Product(const std::string &n, unsigned int i, double w, double p, unsigned int s): name(n), id(i), weight(w), price(p), shelf_life(s) {}
    Product(const Product &other){
        name = other.name;
        id = other.id;
        weight = other.weight;
        price = other.price;
        shelf_life = other.shelf_life;
    }
    ~Product() {}
   
    Product& operator=(const Product &other){
        if (this == &other){
            return *this;
        }
        name = other.name;
        id = other.id;
        weight = other.weight;
        price = other.price;
        shelf_life = other.shelf_life;
        return *this;
    }
    unsigned int get_id() const{
        return id;
    }
    unsigned int get_shelf_life(){
        return shelf_life;
    }

    virtual double calculateStorageFee(){
        return weight * 0.1;
    }

    virtual void displayInfo(){
        std::cout << "Name: " << name << " ID: " << id << " Weight: " << weight << " kg" << 
        " Price: " << price << " rub" << " Shelf life: " << shelf_life << " days" << std::endl;
    }
};


class PerishableProduct: public Product{
    unsigned int expirationDate;
public:
    PerishableProduct(const std::string &n, unsigned int i, double w, double p, unsigned int s, unsigned int ex):
        Product(n, i, w, p, s), expirationDate(ex) {}
    unsigned int get_expirationDate() const{
        return expirationDate;
    }
    double calculateStorageFee() override {
        if (expirationDate - shelf_life < 5){
            return weight * 0.3;
        }else{
            return weight * 0.1;
        }
    }

};


class ElectronicProduct: public Product{
    unsigned int warrantyPeriod;
    unsigned int powerRating;
public:

    ElectronicProduct(const std::string &n, unsigned int i, double w, double p, unsigned int s, unsigned int warranty, unsigned int power):
        Product(n, i, w, p, s), warrantyPeriod(warranty), powerRating(power) {}

    void displayInfo(){
        std::cout << "Name: " << name << " ID: " << id << " Weight: " << weight << " kg" << " Price: " << price << " rub" << 
        " Power rating: " << powerRating << " watts" << " Shelf life: " << shelf_life << " days" << 
        "Warranty period: " << warrantyPeriod << " days" << std::endl;
    }
};


class BuildingMaterial: public Product{
    bool flammability;
public:
    BuildingMaterial(const std::string &n, unsigned int i, double w, double p, unsigned int s, bool f):
        Product(n, i, w, p, s), flammability(f) {}

    double calculateStorageFee(){
        if (flammability){
            return weight * 0.5;
        }else{
            return weight * 0.1;
        }
    }
};


class Warehouse{
    std::vector<Product*> all_products;
public:
    ~Warehouse(){
        for (size_t i = 0; i < all_products.size(); ++i){
            delete all_products[i];
        }
    }

    Warehouse& operator+=(Product* product) {
        all_products.push_back(product);
        return *this;
    }

     Warehouse& operator-=(unsigned int identifier) {
        for(size_t i = 0; i < all_products.size(); ++i){
            if (all_products[i]->get_id() == identifier){
                delete all_products[i];
                all_products.erase(all_products.begin() + i);
                break;
            }

        }
        return *this;
    }

    Product* operator[](unsigned int identifier) const{
        for(size_t i = 0; i < all_products.size(); ++i){
            if (all_products[i]->get_id() == identifier){
                return all_products[i];
            }
        }
        return nullptr;
    }

    friend std::ostream& operator<<(std::ostream& os, const Warehouse& warehouse) {
        os << "Current status of the warehouse:" << std::endl;
        for(size_t i = 0; i < warehouse.all_products.size(); ++i){
            warehouse.all_products[i]->displayInfo();
        }
        return os;
    }

    void add(Product* product) {
        *this += product;
    }

    void remove(unsigned int identifier) {
        *this -= identifier; 
    }

    double totalCost() const{
        double sum = 0.0;
        for (size_t i = 0; i < all_products.size(); ++i){
            sum += all_products[i]->calculateStorageFee();
        }
        return sum;
    }

    std::vector<PerishableProduct*> getExpiringProducts(int days) const {
        std::vector<PerishableProduct*> expiringProducts;
        for (size_t i = 0; i < all_products.size(); ++i) {
            PerishableProduct* perishable = dynamic_cast<PerishableProduct*>(all_products[i]);
            if (perishable) {
                if (perishable->get_expirationDate() - perishable->get_shelf_life() <= days) { 
                    expiringProducts.push_back(perishable);
                }
            }
        }
        return expiringProducts;
    }

    void displayInventory() const {
        std::cout << "Current status of the warehouse by category:\n";

        std::cout << "Perishable Products:\n";
        for (size_t i = 0; i < all_products.size(); ++i) {
            if (dynamic_cast<PerishableProduct*> (all_products[i])) {
                all_products[i]->displayInfo(); 
            }
        }

        std::cout << "\nElectronic Products:\n";
        for (size_t i = 0; i < all_products.size(); ++i) {
            if (dynamic_cast<ElectronicProduct*>(all_products[i])) {
                all_products[i]->displayInfo();
            }
        }

        std::cout << "\nBuilding Materials:\n";
        for (size_t i = 0; i < all_products.size(); ++i) {
            if (dynamic_cast<BuildingMaterial*>(all_products[i])) {
                all_products[i]->displayInfo();
            }
        }
    }

    std::vector<Product*> searchByCategory(const std::string& category) const{
        std::vector<Product*> result;
        for (size_t i = 0; i < all_products.size(); ++i) {
            if (category == "PerishableProduct" && dynamic_cast<PerishableProduct*>(all_products[i])) {
                result.push_back(all_products[i]);
            } else if (category == "ElectronicProduct" && dynamic_cast<ElectronicProduct*>(all_products[i])) {
                result.push_back(all_products[i]);
            } else if (category == "BuildingMaterial" && dynamic_cast<BuildingMaterial*>(all_products[i])) {
                result.push_back(all_products[i]);
            }
        }
        return result;
    }
};


int main(){
    Warehouse warehouse;
    warehouse += new PerishableProduct("Milk", 1, 1.0, 78.0, 1, 7); 
    warehouse += new PerishableProduct("Cheese", 2, 0.3, 250.0, 4, 8);  
    warehouse += new ElectronicProduct("Microwave oven", 3, 11.2, 18000.0, 32, 365, 600);
    warehouse += new BuildingMaterial("Bricks", 4, 1000, 22000.0, 72, false);
    warehouse += new BuildingMaterial("Wood", 5, 500, 15000.0, 200, true); 
    warehouse += new PerishableProduct("Oil", 6, 1.0, 89.0, 92, 93);

    std::cout << warehouse;
    warehouse -= 4;
    std::cout << "\nCurrent status of the warehouse after removing Bricks (ID 4):\n";
    std::cout << warehouse;

    std::cout << "\nFinding all Perishable Products:\n";
    std::vector<Product*> perishableProducts = warehouse.searchByCategory("PerishableProduct");
    for (size_t i = 0; i < perishableProducts.size(); ++i) {
        perishableProducts[i]->displayInfo();
    }

    std::cout << "\nA list of products that expire within 5 days:\n";
    std::vector<PerishableProduct*> expiringProducts = warehouse.getExpiringProducts(5);
    for (size_t i = 0; i < expiringProducts.size(); ++i) {
        expiringProducts[i]->displayInfo();
    }

    double totalSum = warehouse.totalCost();
    std::cout << "\nThe total cost of storing all goods in the warehouse: " << totalSum << " rub" << std::endl;

    int idToFind = 3; 
    Product* product = warehouse[idToFind];
    if (product) {
        std::cout << "\nProduct with ID " << idToFind << ":\n";
        product->displayInfo();
    } else {
        std::cout << "\nProduct with ID " << idToFind << " not found on the warehouse\n";
    }

}
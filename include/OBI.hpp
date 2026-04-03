#ifndef PRODUCT_HPP
#define PRODUCT_HPP

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <iomanip>

namespace ProductDB {

    class Product {
    private:
        int id;
        std::string name;
        double price;

    public:
        Product();
        Product(int id, const std::string& name, double price);
        Product(const Product& other);
        
        ~Product();
        
        int getId() const;
        std::string getName() const;
        double getPrice() const;
        
        void setId(int id);
        void setName(const std::string& name);
        void setPrice(double price);
        
        friend std::ostream& operator«(std::ostream& os, const Product& product);
        friend std::istream& operator»(std::istream& is, Product& product);
        
        std::string operator[](int index) const;
        
        Product& operator=(const Product& other);
        
        bool operator==(const Product& other) const;
    };

    class Database {
    private:
        std::vector<Product> products;
        std::string filename;
        int nextId;
        
        void updateNextId();
        
    public:
        Database();
        explicit Database(const std::string& filename);
        ~Database();
        
        void displayAll() const;
        void addProduct(const std::string& name, double price);
        bool deleteProduct(int id);
        bool editProduct(int id, const std::string& newName, double newPrice);
        
        std::vector<Product> searchById(int id) const;
        std::vector<Product> searchByName(const std::string& name) const;
        std::vector<Product> searchByPrice(double minPrice, double maxPrice) const;
        std::vector<Product> searchByKeyword(const std::string& keyword) const;
        
        bool loadFromFile();
        bool saveToFile() const;
        bool saveToFile(const std::string& customFilename) const;
        
        int getSize() const;
        bool isEmpty() const;
        void clear();
        
        Product& operator[](int index);
        const Product& operator[](int index) const;
        
        friend std::ostream& operator«(std::ostream& os, const Database& db);
    };
    
    void clearScreen();
    void waitForEnter();
    bool isValidPrice(double price);
    

#endif // PRODUCT_HPP

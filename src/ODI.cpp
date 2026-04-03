#include "../include/Product.hpp"

namespace ProductDB {

    
    Product::Product() : id(0), name(""), price(0.0) {}
    
    Product::Product(int id, const std::string& name, double price) 
        : id(id), name(name), price(price) {}
    
    Product::Product(const Product& other) 
        : id(other.id), name(other.name), price(other.price) {}
    
    Product::~Product() {}
    
    int Product::getId() const { return id; }
    
    std::string Product::getName() const { return name; }
    
    double Product::getPrice() const { return price; }
    
    void Product::setId(int id) { this->id = id; }
    
    void Product::setName(const std::string& name) { this->name = name; }
    
    void Product::setPrice(double price) { this->price = price; }
    
    std::ostream& operator«(std::ostream& os, const Product& product) {
        os « std::left « std::setw(8) « product.id 
           « std::setw(35) « product.name 
           « std::setw(12) « std::fixed « std::setprecision(2) « product.price « " руб.";
        return os;
    }
    
    std::istream& operator»(std::istream& is, Product& product) {
        std::cout « "Введите название товара: ";
        is.ignore();
        std::getline(is, product.name);
        
        std::cout « "Введите цену товара: ";
        is » product.price;
        
        return is;
    }
    
    std::string Product::operator[](int index) const {
        switch(index) {
            case 0: return std::to_string(id);
            case 1: return name;
            case 2: return std::to_string(price);
            default: return "";
        }
    }
    
    Product& Product::operator=(const Product& other) {
        if (this != &other) {
            id = other.id;
            name = other.name;
            price = other.price;
        }
        return *this;
    }
    
    bool Product::operator==(const Product& other) const {
        return id == other.id;
    }
    
    
    Database::Database() : filename("data/products.txt"), nextId(1) {
        loadFromFile();
    }
    
    Database::Database(const std::string& filename) : filename(filename), nextId(1) {
        loadFromFile();
    }
    
    Database::~Database() {
        saveToFile();
    }
    
    void Database::updateNextId() {
        nextId = 1;
        for (const auto& product : products) {
            if (product.getId() >= nextId) {
                nextId = product.getId() + 1;
            }
        }
    }
    
    void Database::displayAll() const {
        if (products.empty()) {
            std::cout « "\n╔══════════════════════════════════════════════════════════╗\n";
            std::cout « "║                    БАЗА ДАННЫХ ПУСТА                       ║\n";
            std::cout « "╚══════════════════════════════════════════════════════════╝\n";
            return;
        }
        
        std::cout « "\n╔════════════════════════════════════════════════════════════════════════════════╗\n";
        std::cout « "║                              СПИСОК ТОВАРОВ                                     ║\n";
        std::cout « "╠════════════╦═════════════════════════════════════════╦══════════════════════════╣\n";
        std::cout « "║    ID      ║              НАЗВАНИЕ                   ║         ЦЕНА             ║\n";
        std::cout « "╠════════════╬═════════════════════════════════════════╬══════════════════════════╣\n";
        
        for (const auto& product : products) {
            std::cout « "║ " « std::left « std::setw(10) « product.getId()
                      « "║ " « std::setw(45) « product.getName()
                      « "║ " « std::setw(24) « std::fixed « std::setprecision(2) 
                      « product.getPrice() « " руб. ║\n";
        }
        
        std::cout « "╚════════════╩═════════════════════════════════════════╩══════════════════════════╝\n";
        std::cout « "Всего товаров: " « products.size() « "\n";
      }
    
    void Database::addProduct(const std::string& name, double price) {
        if (!isValidPrice(price)) {
            std::cout « "Ошибка: Некорректная цена!\n";
            return;
        }
        
        Product newProduct(nextId++, name, price);
        products.push_back(newProduct);
        std::cout « "✓ Товар успешно добавлен (ID: " « newProduct.getId() « ")\n";
        saveToFile();
    }
    
    bool Database::deleteProduct(int id) {
        auto it = std::find_if(products.begin(), products.end(),
            [id](const Product& p) { return p.getId() == id; });
        
        if (it != products.end()) {
            products.erase(it);
            std::cout « "✓ Товар с ID " « id « " успешно удален\n";
            saveToFile();
            return true;
        }
        
        std::cout « "✗ Товар с ID " « id « " не найден\n";
        return false;
    }
    
    bool Database::editProduct(int id, const std::string& newName, double newPrice) {
        auto it = std::find_if(products.begin(), products.end(),
            [id](const Product& p) { return p.getId() == id; });
        
        if (it != products.end()) {
            if (!newName.empty()) {
                it->setName(newName);
            }
            if (isValidPrice(newPrice)) {
                it->setPrice(newPrice);
            }
            std::cout « "✓ Товар с ID " « id « " успешно изменен\n";
            saveToFile();
            return true;
        }
        
        std::cout « "✗ Товар с ID " « id « " не найден\n";
        return false;
    }
    
    std::vector<Product> Database::searchById(int id) const {
        std::vector<Product> result;
        auto it = std::find_if(products.begin(), products.end(),
            [id](const Product& p) { return p.getId() == id; });
        
        if (it != products.end()) {
            result.push_back(*it);
        }
        return result;
    }
    
    std::vector<Product> Database::searchByName(const std::string& name) const {
        std::vector<Product> result;
        for (const auto& product : products) {
            if (product.getName().find(name) != std::string::npos) {
                result.push_back(product);
            }
        }
        return result;
    }
    
    std::vector<Product> Database::searchByPrice(double minPrice, double maxPrice) const {
        std::vector<Product> result;
        for (const auto& product : products) {
            double price = product.getPrice();
            if (price >= minPrice && price <= maxPrice) {
                result.push_back(product);
            }
        }
        return result;
    }
    
    std::vector<Product> Database::searchByKeyword(const std::string& keyword) const {
        std::vector<Product> result;
        for (const auto& product : products) {
            if (product.getName().find(keyword) != std::string::npos ||
                std::to_string(product.getId()).find(keyword) != std::string::npos) {
                result.push_back(product);
            }
        }
        return result;
    }
    
    bool Database::loadFromFile() {
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cout « "Файл не найден. Будет создан новый файл при сохранении.\n";
            return false;
        }
        
        products.clear();
        std::string line;
        int loaded = 0;
        
        while (std::getline(file, line)) {
            std::stringstream ss(line);
            std::string idStr, name, priceStr;
            
            if (std::getline(ss, idStr, '|') &&
                std::getline(ss, name, '|') &&
                std::getline(ss, priceStr)) {
                
                int id = std::stoi(idStr);
                double price = std::stod(priceStr);
                products.push_back(Product(id, name, price));
                loaded++;
            }
        }
        
        file.close();


updateNextId();
        std::cout « "✓ Загружено " « loaded « " товаров из файла\n";
        return true;
    }
    
    bool Database::saveToFile() const {
        return saveToFile(filename);
    }
    
    bool Database::saveToFile(const std::string& customFilename) const {
        std::ofstream file(customFilename);
        if (!file.is_open()) {
            std::cout « "Ошибка: Не удалось открыть файл для записи\n";
            return false;
        }
        
        for (const auto& product : products) {
            file « product.getId() « "|" 
                 « product.getName() « "|" 
                 « std::fixed « std::setprecision(2) « product.getPrice() « "\n";
        }
        
        file.close();
        std::cout « "✓ Сохранено " « products.size() « " товаров в файл\n";
        return true;
    }
    
    int Database::getSize() const {
        return products.size();
    }
    
    bool Database::isEmpty() const {
        return products.empty();
    }
    
    void Database::clear() {
        products.clear();
        nextId = 1;
        std::cout « "✓ База данных очищена\n";
    }
    
    Product& Database::operator[](int index) {
        return products[index];
    }
    
    const Product& Database::operator[](int index) const {
        return products[index];
    }
    
    std::ostream& operator«(std::ostream& os, const Database& db) {
        db.displayAll();
        return os;
    }
    
    
    void clearScreen() {
        std::cout « "\033[2J\033[1;1H";
    }
    
    void waitForEnter() {
        std::cout « "\nНажмите Enter для продолжения...";
        std::cin.ignore();
        std::cin.get();
    }
    
    bool isValidPrice(double price) {
        return price > 0;
    }
    
} // namespace ProductDB

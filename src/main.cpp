#include "../include/OBI.hpp"
#include <limits>

using namespace ProductDB;

void printMenu() {
    std::cout « "\n╔══════════════════════════════════════════════════════════════╗\n";
    std::cout « "║                    МАГАЗИН СТРОЙМАТЕРИАЛОВ                    ║\n";
    std::cout « "╠══════════════════════════════════════════════════════════════╣\n";
    std::cout « "║  1. Показать все товары                                      ║\n";
    std::cout « "║  2. Добавить новый товар                                     ║\n";
    std::cout « "║  3. Удалить товар                                            ║\n";
    std::cout « "║  4. Редактировать товар                                      ║\n";
    std::cout « "║  5. Поиск по ID                                              ║\n";
    std::cout « "║  6. Поиск по названию                                        ║\n";
    std::cout « "║  7. Поиск по диапазону цен                                   ║\n";
    std::cout « "║  8. Поиск по ключевому слову                                 ║\n";
    std::cout « "║  9. Сохранить в файл                                         ║\n";
    std::cout « "║ 10. Загрузить из файла                                       ║\n";
    std::cout « "║  0. Выход                                                    ║\n";
    std::cout « "╚══════════════════════════════════════════════════════════════╝\n";
    std::cout « "Ваш выбор: ";
}

void displaySearchResults(const std::vector<Product>& results) {
    if (results.empty()) {
        std::cout « "\n❌ Товары не найдены\n";
        return;
    }
    
    std::cout « "\n╔════════════════════════════════════════════════════════════════════════════════╗\n";
    std::cout « "║                              РЕЗУЛЬТАТЫ ПОИСКА                                  ║\n";
    std::cout « "╠════════════╦═════════════════════════════════════════╦══════════════════════════╣\n";
    std::cout « "║    ID      ║              НАЗВАНИЕ                   ║         ЦЕНА             ║\n";
    std::cout « "╠════════════╬═════════════════════════════════════════╬══════════════════════════╣\n";
    
    for (const auto& product : results) {
        std::cout « "║ " « std::left « std::setw(10) « product.getId()
                  « "║ " « std::setw(45) « product.getName()
                  « "║ " « std::setw(24) « std::fixed « std::setprecision(2) 
                  « product.getPrice() « " руб. ║\n";
    }
    
    std::cout « "╚════════════╩═════════════════════════════════════════╩══════════════════════════╝\n";
    std::cout « "Найдено товаров: " « results.size() « "\n";
}

int main() {
    Database db;
    int choice;
    
    std::cout « "\n=== ДОБРО ПОЖАЛОВАТЬ В СИСТЕМУ УПРАВЛЕНИЯ ТОВАРАМИ ===\n";
    
    do {
        printMenu();
        std::cin » choice;
        
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout « "Ошибка: Введите число!\n";
            continue;
        }
        
        switch (choice) {
            case 0:
                std::cout « "\nСохранение данных и выход из программы...\n";
                db.saveToFile();
                std::cout « "До свидания!\n";
                break;
                
            case 1:
                db.displayAll();
                break;
                
            case 2: {
                std::string name;
                double price;
                
                std::cout « "Введите название товара: ";
                std::cin.ignore();
                std::getline(std::cin, name);
                
                std::cout « "Введите цену товара: ";
                std::cin » price;
                
                db.addProduct(name, price);
                break;
            }
            
            case 3: {
                int id;
                db.displayAll();
                std::cout « "Введите ID товара для удаления: ";
                std::c


in » id;
                db.deleteProduct(id);
                break;
            }
            
            case 4: {
                int id;
                std::string name;
                double price;
                
                db.displayAll();
                std::cout « "Введите ID товара для редактирования: ";
                std::cin » id;
                
                std::cout « "Введите новое название (оставьте пустым для пропуска): ";
                std::cin.ignore();
                std::getline(std::cin, name);
                
                std::cout « "Введите новую цену (0 для пропуска): ";
                std::cin » price;
                
                if (name.empty()) {
                    name = "";
                }
                if (price == 0) {
                    price = -1;
                }
                
                db.editProduct(id, name, price);
                break;
            }
            
            case 5: {
                int id;
                std::cout « "Введите ID для поиска: ";
                std::cin » id;
                displaySearchResults(db.searchById(id));
                break;
            }
            
            case 6: {
                std::string name;
                std::cout « "Введите название для поиска: ";
                std::cin.ignore();
                std::getline(std::cin, name);
                displaySearchResults(db.searchByName(name));
                break;
            }
            
            case 7: {
                double minPrice, maxPrice;
                std::cout « "Введите минимальную цену: ";
                std::cin » minPrice;
                std::cout « "Введите максимальную цену: ";
                std::cin » maxPrice;
                displaySearchResults(db.searchByPrice(minPrice, maxPrice));
                break;
            }
            
            case 8: {
                std::string keyword;
                std::cout « "Введите ключевое слово для поиска: ";
                std::cin.ignore();
                std::getline(std::cin, keyword);
                displaySearchResults(db.searchByKeyword(keyword));
                break;
            }
            
            case 9:
                db.saveToFile();
                break;
                
            case 10:
                db.loadFromFile();
                break;
                
            default:
                std::cout « "Неверный выбор! Пожалуйста, выберите пункт от 0 до 10.\n";
                break;
        }
        
    } while (choice != 0);
    
    return 0;
}

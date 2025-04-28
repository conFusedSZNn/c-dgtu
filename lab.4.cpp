#include <iostream>
#include <memory>
#include <vector>
#include <string>

class Inventory {
private:
    std::vector<std::unique_ptr<std::string>> items;

public:
    // Добавление предмета в инвентарь
    void addItem(const std::string& item) {
        items.push_back(std::make_unique<std::string>(item));
    }

    // Отображение содержимого инвентаря
    void displayInventory() const {
        if (items.empty()) {
            std::cout << "Inventory is empty.\n";
            return;
        }

        std::cout << "Inventory contents:\n";
        for (size_t i = 0; i < items.size(); ++i) {
            std::cout << i + 1 << ". " << *items[i] << "\n";
        }
    }

    // Получение количества предметов
    size_t getItemCount() const {
        return items.size();
    }
};

int main() {
    // Создание инвентаря с использованием умных указателей
    Inventory playerInventory;

    // Добавление предметов
    playerInventory.addItem("Steel Sword");
    playerInventory.addItem("Health Potion");
    playerInventory.addItem("Magic Amulet");
    playerInventory.addItem("Leather Armor");

    // Отображение инвентаря
    playerInventory.displayInventory();

    // Проверка количества предметов
    std::cout << "\nTotal items: " << playerInventory.getItemCount() << "\n";

    // Инвентарь будет автоматически очищен при выходе из области видимости
    return 0;
}

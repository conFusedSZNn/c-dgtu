#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <stdexcept>

// Класс Character (персонаж)
class Character {
private:
    std::string name;
    int health;
    int level;

public:
    Character(const std::string& n, int h, int lvl)
        : name(n), health(h), level(lvl) {}

    // Метод для вывода информации о персонаже
    void displayInfo() const {
        std::cout << "Name: " << name << ", Health: " << health 
                  << ", Level: " << level << std::endl;
    }

    // Методы для доступа к полям (для сериализации)
    std::string getName() const { return name; }
    int getHealth() const { return health; }
    int getLevel() const { return level; }
};

// Шаблонный класс GameManager для управления игровыми объектами
template <typename T>
class GameManager {
private:
    std::vector<T> entities;

public:
    void addEntity(const T& entity) {
        entities.push_back(entity);
    }

    void displayAll() const {
        for (const auto& entity : entities) {
            entity.displayInfo();
        }
    }

    // Получить все сущности (для сохранения/загрузки)
    const std::vector<T>& getEntities() const {
        return entities;
    }

    // Очистить менеджер (перед загрузкой новых данных)
    void clear() {
        entities.clear();
    }
};

// Функция для сохранения данных в файл
void saveToFile(const GameManager<Character>& manager, const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open file for writing.");
    }

    for (const auto& character : manager.getEntities()) {
        file << character.getName() << " "
             << character.getHealth() << " "
             << character.getLevel() << "\n";
    }

    std::cout << "Data saved to " << filename << std::endl;
}

// Функция для загрузки данных из файла
void loadFromFile(GameManager<Character>& manager, const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open file for reading.");
    }

    manager.clear(); // Очищаем текущие данные

    std::string name;
    int health, level;

    while (file >> name >> health >> level) {
        manager.addEntity(Character(name, health, level));
    }

    std::cout << "Data loaded from " << filename << std::endl;
}

int main() {
    // Создаем менеджер и добавляем персонажей
    GameManager<Character> manager;
    manager.addEntity(Character("Hero", 100, 1));
    manager.addEntity(Character("Mage", 80, 3));
    manager.addEntity(Character("Warrior", 120, 2));

    // Выводим информацию о персонажах
    std::cout << "Original characters:" << std::endl;
    manager.displayAll();

    // Сохраняем данные в файл
    try {
        saveToFile(manager, "characters_save.txt");
    } catch (const std::runtime_error& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    // Создаем новый менеджер и загружаем данные из файла
    GameManager<Character> loadedManager;
    try {
        loadFromFile(loadedManager, "characters_save.txt");
    } catch (const std::runtime_error& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    // Выводим загруженных персонажей
    std::cout << "\nLoaded characters:" << std::endl;
    loadedManager.displayAll();

    return 0;
}

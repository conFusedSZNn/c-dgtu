#include <iostream>
#include <string>

class Character {
private:
    std::string name;  // Приватное поле: имя персонажа
    int health;        // Приватное поле: уровень здоровья
    int attack;        // Приватное поле: уровень атаки
    int defense;       // Приватное поле: уровень защиты
    const int maxHealth = 100; // Максимальное значение здоровья

public:
    // Конструктор для инициализации данных
    Character(const std::string& n, int h, int a, int d)
        : name(n), health(h), attack(a), defense(d) {}

    // Метод для получения уровня здоровья
    int getHealth() const {
        return health;
    }

    // Метод для вывода информации о персонаже
    void displayInfo() const {
        std::cout << "Name: " << name << ", HP: " << health
                  << ", Attack: " << attack << ", Defense: " << defense << std::endl;
    }

    // Метод для атаки другого персонажа
    void attackEnemy(Character& enemy) {
        int damage = attack - enemy.defense;
        if (damage > 0) {
            enemy.takeDamage(damage); // Используем takeDamage вместо прямого изменения health
            std::cout << name << " attacks " << enemy.name << " for " << damage << " damage!" << std::endl;
        } else {
            std::cout << name << " attacks " << enemy.name << ", but it has no effect!" << std::endl;
        }
    }

    // Метод для лечения персонажа
    void heal(int amount) {
        if (amount > 0) {
            health += amount;
            if (health > maxHealth) {
                health = maxHealth;
            }
            std::cout << name << " healed for " << amount << " HP. Current HP: " << health << std::endl;
        } else {
            std::cout << "Healing amount must be positive!" << std::endl;
        }
    }

    // Метод для получения урона
    void takeDamage(int amount) {
        if (amount > 0) {
            health -= amount;
            if (health < 0) {
                health = 0;
            }
            std::cout << name << " took " << amount << " damage. Current HP: " << health << std::endl;
        } else {
            std::cout << "Damage amount must be positive!" << std::endl;
        }
    }
};

int main() {
    // Создаем объекты персонажей
    Character hero("Hero", 100, 20, 10);
    Character monster("Goblin", 50, 15, 5);

    // Выводим информацию о персонажах
    hero.displayInfo();
    monster.displayInfo();

    // Герой атакует монстра
    hero.attackEnemy(monster);
    monster.displayInfo();

    // Монстр лечится
    monster.heal(20);
    monster.displayInfo();

    // Попытка вылечиться сверх максимального здоровья
    monster.heal(50);
    monster.displayInfo();

    // Монстр получает урон
    monster.takeDamage(30);
    monster.displayInfo();

    // Попытка нанести отрицательный урон (ошибка)
    monster.takeDamage(-10);
    monster.displayInfo();

    // Монстр получает смертельный урон
    monster.takeDamage(100);
    monster.displayInfo();

    return 0;
}
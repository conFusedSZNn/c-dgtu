#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>

class Entity {
protected:
    std::string name;
    int health;
    int attack;
    int defense;
    const int maxHealth;

public:
    Entity(const std::string& n, int h, int a, int d)
        : name(n), health(h), attack(a), defense(d), maxHealth(h) {}

    // Виртуальный метод для атаки
    virtual void attack(Entity& target) {
        int damage = attack - target.defense;
        if (damage > 0) {
            target.health -= damage;
            std::cout << name << " attacks " << target.name << " for " << damage << " damage!\n";
        } else {
            std::cout << name << " attacks " << target.name << ", but it has no effect!\n";
        }
    }

    // Виртуальный метод для лечения
    virtual void heal(int amount) {
        health += amount;
        if (health > maxHealth) health = maxHealth;
        std::cout << name << " healed for " << amount << " HP. Current HP: " << health << "\n";
    }

    // Виртуальный метод для вывода информации
    virtual void displayInfo() const {
        std::cout << "Entity: " << name << ", HP: " << health
                  << "/" << maxHealth << ", Attack: " << attack 
                  << ", Defense: " << defense << std::endl;
    }

    virtual ~Entity() {}
};

class Character : public Entity {
public:
    Character(const std::string& n, int h, int a, int d)
        : Entity(n, h, a, d) {}

    void attack(Entity& target) override {
        int damage = attack - target.defense;
        if (damage > 0) {
            if (rand() % 100 < 20) { // 20% шанс крита
                damage *= 2;
                std::cout << "Critical hit! ";
            }
            target.health -= damage;
            std::cout << name << " attacks " << target.name << " for " << damage << " damage!\n";
        } else {
            std::cout << name << " attacks " << target.name << ", but it has no effect!\n";
        }
    }

    void heal(int amount) override {
        int actualHeal = std::min(amount, maxHealth - health);
        Entity::heal(actualHeal);
        std::cout << name << " used healing potion!\n";
    }

    void displayInfo() const override {
        std::cout << "Character: " << name << ", HP: " << health
                  << "/" << maxHealth << ", Attack: " << attack 
                  << ", Defense: " << defense << std::endl;
    }
};

class Monster : public Entity {
public:
    Monster(const std::string& n, int h, int a, int d)
        : Entity(n, h, a, d) {}

    void attack(Entity& target) override {
        int damage = attack - target.defense;
        if (damage > 0) {
            if (rand() % 100 < 30) { // 30% шанс яда
                damage += 5;
                std::cout << "Poisonous attack! ";
            }
            target.health -= damage;
            std::cout << name << " attacks " << target.name << " for " << damage << " damage!\n";
        } else {
            std::cout << name << " attacks " << target.name << ", but it has no effect!\n";
        }
    }

    void displayInfo() const override {
        std::cout << "Monster: " << name << ", HP: " << health
                  << "/" << maxHealth << ", Attack: " << attack 
                  << ", Defense: " << defense << std::endl;
    }
};

class Boss : public Monster {
private:
    std::string specialAbility;

public:
    Boss(const std::string& n, int h, int a, int d, const std::string& ability)
        : Monster(n, h, a, d), specialAbility(ability) {}

    void attack(Entity& target) override {
        int damage = attack - target.defense;
        if (damage > 0) {
            if (rand() % 100 < 40) { // 40% шанс спецспособности
                damage += 10;
                std::cout << specialAbility << "! ";
            }
            target.health -= damage;
            std::cout << name << " attacks " << target.name << " for " << damage << " damage!\n";
        } else {
            std::cout << name << " attacks " << target.name << ", but it has no effect!\n";
        }
    }

    void displayInfo() const override {
        std::cout << "BOSS: " << name << ", HP: " << health
                  << "/" << maxHealth << ", Attack: " << attack 
                  << ", Defense: " << defense << ", Ability: " 
                  << specialAbility << std::endl;
    }
};

int main() {
    srand(static_cast<unsigned>(time(0)));

    // Создание объектов
    Character hero("Hero", 100, 20, 10);
    Monster goblin("Goblin", 50, 15, 5);
    Boss dragon("Dragon", 200, 30, 20, "Fire Breath");

    // Массив указателей на базовый класс
    Entity* entities[] = { &hero, &goblin, &dragon };

    // Демонстрация полиморфизма
    std::cout << "=== Initial status ===\n";
    for (auto& entity : entities) {
        entity->displayInfo();
    }

    // Бой
    std::cout << "\n=== Battle starts ===\n";
    hero.attack(goblin);
    goblin.attack(hero);
    dragon.attack(hero);
    hero.attack(dragon);

    // Лечение
    std::cout << "\n=== Healing ===\n";
    hero.heal(30);

    // Финальный статус
    std::cout << "\n=== Final status ===\n";
    for (auto& entity : entities) {
        entity->displayInfo();
    }

    return 0;
}

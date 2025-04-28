#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <chrono>
#include <random>

class Character {
private:
    std::string name;
    int health;
    int attack;
    int defense;
public:
    Character(const std::string& n, int h, int a, int d) 
        : name(n), health(h), attack(a), defense(d) {}

    void takeDamage(int damage) {
        health -= damage;
        if (health < 0) health = 0;
    }

    void displayInfo() const {
        std::cout << name << " [HP: " << health << ", ATK: " << attack 
                  << ", DEF: " << defense << "]\n";
    }

    bool isAlive() const { return health > 0; }
    int getAttack() const { return attack; }
    int getDefense() const { return defense; }
};

class Monster {
private:
    std::string name;
    int health;
    int attack;
    int defense;
public:
    Monster(const std::string& n, int h, int a, int d) 
        : name(n), health(h), attack(a), defense(d) {}

    void takeDamage(int damage) {
        health -= damage;
        if (health < 0) health = 0;
    }

    void displayInfo() const {
        std::cout << name << " [HP: " << health << ", ATK: " << attack 
                  << ", DEF: " << defense << "]\n";
    }

    bool isAlive() const { return health > 0; }
    int getAttack() const { return attack; }
    int getDefense() const { return defense; }
};

// Глобальные переменные для синхронизации
Character hero("Hero", 100, 20, 10);
std::vector<Monster> monsters;
std::mutex battleMutex;
bool battleEnded = false;

// Функция генерации монстров
void generateMonsters() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> healthDist(30, 70);
    std::uniform_int_distribution<> attackDist(5, 25);
    std::uniform_int_distribution<> defenseDist(2, 15);

    const std::string monsterNames[] = {"Goblin", "Orc", "Troll", "Skeleton", "Zombie"};

    while (!battleEnded) {
        std::this_thread::sleep_for(std::chrono::seconds(3));
        
        std::lock_guard<std::mutex> lock(battleMutex);
        std::string name = monsterNames[gen() % 5];
        monsters.emplace_back(name, healthDist(gen), attackDist(gen), defenseDist(gen));
        std::cout << "A wild " << name << " appears!\n";
    }
}

// Функция боя
void battle() {
    while (!battleEnded) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        
        std::lock_guard<std::mutex> lock(battleMutex);
        
        if (!hero.isAlive()) {
            std::cout << "Hero has been defeated!\n";
            battleEnded = true;
            break;
        }

        for (auto& monster : monsters) {
            if (monster.isAlive()) {
                // Герой атакует монстра
                int heroDamage = std::max(1, hero.getAttack() - monster.getDefense() / 2);
                monster.takeDamage(heroDamage);
                std::cout << "Hero hits " << monster.getAttack() << " damage to monster!\n";

                // Монстр атакует героя
                if (monster.isAlive()) {
                    int monsterDamage = std::max(1, monster.getAttack() - hero.getDefense() / 2);
                    hero.takeDamage(monsterDamage);
                    std::cout << "Monster hits " << monsterDamage << " damage to hero!\n";
                } else {
                    std::cout << "Monster defeated!\n";
                }
            }
        }

        // Удаляем побежденных монстров
        monsters.erase(
            std::remove_if(monsters.begin(), monsters.end(), 
                [](const Monster& m) { return !m.isAlive(); }),
            monsters.end());
    }
}

int main() {
    // Запускаем генератор монстров
    std::thread generator(generateMonsters);
    
    // Запускаем поток боя
    std::thread battleThread(battle);

    // Главный поток выводит информацию о состоянии
    while (!battleEnded) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        
        std::lock_guard<std::mutex> lock(battleMutex);
        std::cout << "\n=== Battle Status ===\n";
        hero.displayInfo();
        std::cout << "Monsters:\n";
        for (const auto& monster : monsters) {
            monster.displayInfo();
        }
        std::cout << "====================\n\n";
    }

    // Ожидаем завершения потоков
    generator.join();
    battleThread.join();

    std::cout << "Game over!\n";
    return 0;
}

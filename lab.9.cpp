#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <fstream>
#include <stdexcept>
#include <ctime>
#include <random>
#include <algorithm> // нужно для std::remove и std::find

// Шаблонный класс Logger для записи логов
template<typename T>
class Logger {
private:
    std::ofstream logFile;
public:
    Logger(const std::string& filename) {
        logFile.open(filename, std::ios::app);
        if (!logFile) {
            throw std::runtime_error("Cannot open log file");
        }
    }

    ~Logger() {
        if (logFile.is_open()) {
            logFile.close();
        }
    }

    void log(const T& message) {
        time_t now = time(nullptr);
        logFile << ctime(&now) << ": " << message << std::endl;
    }
};

// Базовый класс Monster
class Monster {
protected:
    std::string name;
    int health;
    int attack;
    int defense;
public:
    Monster(const std::string& n, int h, int a, int d) 
        : name(n), health(h), attack(a), defense(d) {}

    virtual ~Monster() = default;

    virtual void takeDamage(int damage) {
        health -= damage;
        if (health < 0) health = 0;
    }

    virtual void displayInfo() const {
        std::cout << name << " [HP: " << health << ", ATK: " << attack 
                  << ", DEF: " << defense << "]";
    }

    bool isAlive() const { return health > 0; }
    int getAttack() const { return attack; }
    std::string getName() const { return name; }
};

// Производные классы монстров
class Goblin : public Monster {
public:
    Goblin() : Monster("Goblin", 30, 8, 2) {}
};

class Skeleton : public Monster {
public:
    Skeleton() : Monster("Skeleton", 40, 10, 5) {
        defense += 3;
    }
};

class Dragon : public Monster {
public:
    Dragon() : Monster("Dragon", 100, 25, 15) {}
};

// Класс Inventory
class Inventory {
private:
    std::vector<std::string> items;
public:
    void addItem(const std::string& item) {
        items.push_back(item);
    }

    void removeItem(const std::string& item) {
        items.erase(std::remove(items.begin(), items.end(), item), items.end());
    }

    void display() const {
        if (items.empty()) {
            std::cout << "Inventory is empty.\n";
            return;
        }

        std::cout << "Inventory:\n";
        for (size_t i = 0; i < items.size(); ++i) {
            std::cout << i+1 << ". " << items[i] << "\n";
        }
    }

    bool hasItem(const std::string& item) const {
        return std::find(items.begin(), items.end(), item) != items.end();
    }
};

// Класс Character
class Character {
private:
    std::string name;
    int health;
    int maxHealth;
    int attack;
    int defense;
    int level;
    int experience;
    Inventory inventory;

    void levelUp() {
        level++;
        maxHealth += 20;
        health = maxHealth;
        attack += 5;
        defense += 3;
        experience = 0;
        std::cout << name << " leveled up to level " << level << "!\n";
    }
public:
    Character(const std::string& n, int h = 100, int a = 10, int d = 5)
        : name(n), health(h), maxHealth(h), attack(a), defense(d), level(1), experience(0) {}

    void attackEnemy(Monster& enemy) {
        int damage = attack - enemy.getAttack() / 2;
        if (damage < 1) damage = 1;
        
        enemy.takeDamage(damage);
        std::cout << name << " attacks " << enemy.getName() << " for " << damage << " damage!\n";
    }

    void takeDamage(int damage) {
        health -= damage;
        if (health < 0) {
            health = 0;
            throw std::runtime_error(name + " has been defeated!");
        }
    }

    void heal(int amount) {
        health += amount;
        if (health > maxHealth) health = maxHealth;
        std::cout << name << " heals for " << amount << " HP!\n";
    }

    void gainExperience(int exp) {
        experience += exp;
        if (experience >= 100) {
            levelUp();
        }
    }

    void addToInventory(const std::string& item) {
        inventory.addItem(item);
    }

    void useItem(const std::string& item) {
        if (item == "Health Potion") {
            heal(30);
            inventory.removeItem(item);
        } else {
            std::cout << "Cannot use " << item << ".\n";
        }
    }

    void displayInfo() const {
        std::cout << name << " [Lvl " << level << " | HP: " << health << "/" << maxHealth 
                  << " | ATK: " << attack << " | DEF: " << defense 
                  << " | EXP: " << experience << "/100]\n";
    }

    void displayInventory() const {
        inventory.display();
    }

    void saveToFile(const std::string& filename) const {
        std::ofstream file(filename);
        if (!file) {
            throw std::runtime_error("Cannot open save file");
        }

        file << name << "\n"
             << health << "\n"
             << maxHealth << "\n"
             << attack << "\n"
             << defense << "\n"
             << level << "\n"
             << experience << "\n";
    }

    void loadFromFile(const std::string& filename) {
        std::ifstream file(filename);
        if (!file) {
            throw std::runtime_error("Cannot open save file");
        }

        if (!(file >> name >> health >> maxHealth >> attack >> defense >> level >> experience)) {
            throw std::runtime_error("Corrupted save file format");
        }
    }

    bool isAlive() const { return health > 0; }
    std::string getName() const { return name; }
    int getDefense() const { return defense; }  // добавлен getter
    bool hasItem(const std::string& item) const { return inventory.hasItem(item); }
};

// Класс Game
class Game {
private:
    Character player;
    Logger<std::string> logger;

    std::mt19937 gen;
    std::uniform_int_distribution<> monsterDist;

    std::unique_ptr<Monster> generateMonster() {
        int choice = monsterDist(gen);
        switch (choice) {
            case 1: return std::make_unique<Goblin>();
            case 2: return std::make_unique<Skeleton>();
            case 3: return std::make_unique<Dragon>();
            default: return std::make_unique<Goblin>();
        }
    }

    void battle(Monster& monster) {
        logger.log("Battle started with " + monster.getName());
        
        while (player.isAlive() && monster.isAlive()) {
            std::cout << "\n=== Your turn ===\n";
            player.displayInfo();
            monster.displayInfo();
            std::cout << "\n1. Attack\n2. Use Health Potion\n3. Flee\n";
            
            int choice;
            std::cin >> choice;

            try {
                switch (choice) {
                    case 1:
                        player.attackEnemy(monster);
                        break;
                    case 2:
                        if (player.hasItem("Health Potion")) {
                            player.useItem("Health Potion");
                        } else {
                            std::cout << "No Health Potions in inventory!\n";
                        }
                        break;
                    case 3:
                        std::cout << "You fled from battle!\n";
                        logger.log("Player fled from battle");
                        return;
                    default:
                        std::cout << "Invalid choice!\n";
                        continue;
                }
            } catch (const std::exception& e) {
                logger.log("Error in battle: " + std::string(e.what()));
                throw;
            }

            if (monster.isAlive()) {
                std::cout << "\n=== Monster's turn ===\n";
                int damage = monster.getAttack() - player.getDefense() / 2;
                if (damage < 1) damage = 1;

                try {
                    player.takeDamage(damage);
                    std::cout << monster.getName() << " attacks you for " << damage << " damage!\n";
                } catch (const std::exception& e) {
                    logger.log("Player defeated: " + std::string(e.what()));
                    throw;
                }
            }
        }

        if (!monster.isAlive()) {
            int exp = 20 + monster.getAttack() * 2;
            player.gainExperience(exp);
            std::cout << "You defeated the " << monster.getName() << " and gained " << exp << " EXP!\n";
            logger.log("Player defeated " + monster.getName() + " and gained " + std::to_string(exp) + " EXP");

            std::uniform_int_distribution<> dropDist(1, 3);
            if (dropDist(gen) == 1) {
                player.addToInventory("Health Potion");
                std::cout << "You found a Health Potion!\n";
            }
        }
    }

public:
    Game(const std::string& playerName) 
        : player(playerName), logger("game_log.txt"),
          gen(std::random_device{}()), monsterDist(1, 3) {}

    void start() {
        std::cout << "Welcome to Text RPG, " << player.getName() << "!\n";
        
        try {
            while (player.isAlive()) {
                std::cout << "\n=== Main Menu ===\n";
                std::cout << "1. Explore\n2. Check Inventory\n3. Save Game\n4. Load Game\n5. Quit\n";
                
                int choice;
                std::cin >> choice;
                
                switch (choice) {
                    case 1: {
                        auto monster = generateMonster();
                        std::cout << "You encountered a " << monster->getName() << "!\n";
                        battle(*monster);
                        break;
                    }
                    case 2:
                        player.displayInventory();
                        break;
                    case 3:
                        player.saveToFile("save.txt");
                        std::cout << "Game saved!\n";
                        break;
                    case 4:
                        player.loadFromFile("save.txt");
                        std::cout << "Game loaded!\n";
                        break;
                    case 5:
                        return;
                    default:
                        std::cout << "Invalid choice!\n";
                }
            }
        } catch (const std::exception& e) {
            std::cerr << "Game over: " << e.what() << "\n";
            logger.log("Game over: " + std::string(e.what()));
        }
    }
};

int main() {
    try {
        std::cout << "Enter your character name: ";
        std::string name;
        std::cin >> name;

        Game game(name);
        game.start();
    } catch (const std::exception& e) {
        std::cerr << "Fatal error: " << e.what() << "\n";
        return 1;
    }

    return 0;
}

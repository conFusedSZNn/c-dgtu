#include <iostream>
#include <string>

class Weapon {
private:
    std::string name;
    int damage;
    float weight;

public:
    // Конструктор
    Weapon(const std::string& n, int d, float w)
        : name(n), damage(d), weight(w) {}

    // Перегрузка оператора + (сложение урона)
    Weapon operator+(const Weapon& other) const {
        std::string newName = name + "+" + other.name;
        int newDamage = damage + other.damage;
        float newWeight = weight + other.weight;
        return Weapon(newName, newDamage, newWeight);
    }

    // Перегрузка оператора > (сравнение по урону)
    bool operator>(const Weapon& other) const {
        return damage > other.damage;
    }

    // Перегрузка оператора << для вывода
    friend std::ostream& operator<<(std::ostream& os, const Weapon& weapon) {
        os << "Weapon: " << weapon.name 
           << ", Damage: " << weapon.damage
           << ", Weight: " << weapon.weight << " kg";
        return os;
    }
};

int main() {
    // Создание оружия
    Weapon sword("Excalibur", 25, 3.5);
    Weapon bow("Longbow", 15, 1.2);
    Weapon axe("Battle Axe", 30, 5.0);

    // Демонстрация оператора +
    Weapon combined = sword + bow;
    std::cout << "Combined weapon:\n" << combined << "\n\n";

    // Демонстрация оператора >
    std::cout << "Comparison results:\n";
    if (sword > bow) {
        std::cout << sword.name << " is stronger than " << bow.name << "\n";
    } else {
        std::cout << bow.name << " is stronger than " << sword.name << "\n";
    }

    if (axe > sword) {
        std::cout << axe.name << " is stronger than " << sword.name << "\n";
    } else {
        std::cout << sword.name << " is stronger than " << axe.name << "\n";
    }

    return 0;
}

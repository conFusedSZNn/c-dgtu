#include <iostream>
#include <vector>
#include <memory>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <stdexcept>
#include <string>

class User {
protected:
    std::string name;
    int id;
    int accessLevel;

public:
    User(const std::string& name, int id, int accessLevel) {
        if (name.empty() || id < 0 || accessLevel < 0)
            throw std::invalid_argument("Некорректные данные пользователя.");
        this->name = name;
        this->id = id;
        this->accessLevel = accessLevel;
    }

    virtual ~User() = default;

    std::string getName() const { return name; }
    int getId() const { return id; }
    int getAccessLevel() const { return accessLevel; }

    void setName(const std::string& newName) {
        if (newName.empty()) throw std::invalid_argument("Имя не может быть пустым.");
        name = newName;
    }

    void setAccessLevel(int level) {
        if (level < 0) throw std::invalid_argument("Уровень доступа не может быть отрицательным.");
        accessLevel = level;
    }

    virtual void displayInfo() const {
        std::cout << "Имя: " << name << ", ID: " << id << ", Доступ: " << accessLevel << std::endl;
    }
};


class Student : public User {
    std::string group;
public:
    Student(const std::string& name, int id, int accessLevel, const std::string& group)
        : User(name, id, accessLevel), group(group) {}

    void displayInfo() const override {
        std::cout << "Студент -> ";
        User::displayInfo();
        std::cout << "Группа: " << group << std::endl;
    }
};

class Teacher : public User {
    std::string department;
public:
    Teacher(const std::string& name, int id, int accessLevel, const std::string& department)
        : User(name, id, accessLevel), department(department) {}

    void displayInfo() const override {
        std::cout << "Преподаватель -> ";
        User::displayInfo();
        std::cout << "Кафедра: " << department << std::endl;
    }
};

class Administrator : public User {
    std::string role;
public:
    Administrator(const std::string& name, int id, int accessLevel, const std::string& role)
        : User(name, id, accessLevel), role(role) {}

    void displayInfo() const override {
        std::cout << "Администратор -> ";
        User::displayInfo();
        std::cout << "Роль: " << role << std::endl;
    }
};


class Resource {
    std::string resourceName;
    int requiredAccessLevel;

public:
    Resource(const std::string& name, int accessLevel)
        : resourceName(name), requiredAccessLevel(accessLevel) {}

    bool checkAccess(const User& user) const {
        return user.getAccessLevel() >= requiredAccessLevel;
    }

    std::string getName() const {
        return resourceName;
    }

    void display() const {
        std::cout << "Ресурс: " << resourceName << ", Требуемый уровень доступа: " << requiredAccessLevel << std::endl;
    }
};


template<typename U, typename R>
class AccessControlSystem {
    std::vector<std::shared_ptr<U>> users;
    std::vector<std::shared_ptr<R>> resources;

public:
    void addUser(std::shared_ptr<U> user) {
        users.push_back(user);
    }

    void addResource(std::shared_ptr<R> resource) {
        resources.push_back(resource);
    }

    void showAllUsers() const {
        for (const auto& user : users) {
            user->displayInfo();
            std::cout << "---------------------\n";
        }
    }

    void showAllResources() const {
        for (const auto& res : resources) {
            res->display();
        }
    }

    void checkUserAccess(int userId, const std::string& resourceName) const {
        auto userIt = std::find_if(users.begin(), users.end(), [&](const std::shared_ptr<U>& u) {
            return u->getId() == userId;
        });

        auto resIt = std::find_if(resources.begin(), resources.end(), [&](const std::shared_ptr<R>& r) {
            return r->getName() == resourceName;
        });

        if (userIt == users.end()) {
            std::cout << "Пользователь с ID " << userId << " не найден.\n";
            return;
        }

        if (resIt == resources.end()) {
            std::cout << "Ресурс '" << resourceName << "' не найден.\n";
            return;
        }

        if ((*resIt)->checkAccess(**userIt)) {
            std::cout << "✅ Доступ разрешён к ресурсу: " << resourceName << std::endl;
        } else {
            std::cout << "⛔ Доступ запрещён к ресурсу: " << resourceName << std::endl;
        }
    }

    std::shared_ptr<U> findUserByName(const std::string& name) const {
        for (const auto& user : users) {
            if (user->getName() == name)
                return user;
        }
        return nullptr;
    }

    std::shared_ptr<U> findUserById(int id) const {
        for (const auto& user : users) {
            if (user->getId() == id)
                return user;
        }
        return nullptr;
    }

    void sortUsersByAccessLevel(bool ascending = true) {
        std::sort(users.begin(), users.end(), [=](const std::shared_ptr<U>& a, const std::shared_ptr<U>& b) {
            return ascending ? (a->getAccessLevel() < b->getAccessLevel())
                             : (a->getAccessLevel() > b->getAccessLevel());
        });
    }

    void saveToFile(const std::string& filename) const {
        std::ofstream out(filename);
        if (!out) throw std::runtime_error("Не удалось открыть файл.");

        for (const auto& user : users) {
            out << user->getId() << "," << user->getName() << "," << user->getAccessLevel() << "\n";
        }

        out.close();
    }

    void loadFromFile(const std::string& filename) {
        std::ifstream in(filename);
        if (!in) throw std::runtime_error("Не удалось открыть файл.");

        std::string line;
        while (getline(in, line)) {
            std::stringstream ss(line);
            std::string name;
            int id, access;
            char delim;

            std::getline(ss, name, ',');
            ss >> id >> delim >> access;

            users.push_back(std::make_shared<Student>(name, id, access, "Загрузка"));
        }

        in.close();
    }
};


int main() {
    try {
        AccessControlSystem<User, Resource> system;

        auto s1 = std::make_shared<Student>("Иван", 101, 1, "Группа A");
        auto t1 = std::make_shared<Teacher>("Сергей", 202, 2, "Физика");
        auto a1 = std::make_shared<Administrator>("Анна", 303, 3, "Сис. админ");

        system.addUser(s1);
        system.addUser(t1);
        system.addUser(a1);

        auto lab = std::make_shared<Resource>("Лаборатория", 2);
        auto lib = std::make_shared<Resource>("Библиотека", 1);

        system.addResource(lab);
        system.addResource(lib);

        std::cout << "=== Все пользователи ===\n";
        system.showAllUsers();

        std::cout << "\n=== Проверка доступа ===\n";
        system.checkUserAccess(101, "Лаборатория");
        system.checkUserAccess(202, "Лаборатория");

        std::cout << "\n=== Поиск по имени ===\n";
        auto foundUser = system.findUserByName("Анна");
        if (foundUser) {
            foundUser->displayInfo();
        }

        std::cout << "\n=== Сортировка по уровню доступа ===\n";
        system.sortUsersByAccessLevel(false);
        system.showAllUsers();

        std::cout << "\n=== Сохранение в файл ===\n";
        system.saveToFile("users.txt");

    } catch (const std::exception& e) {
        std::cerr << "Ошибка: " << e.what() << std::endl;
    }

    return 0;
}

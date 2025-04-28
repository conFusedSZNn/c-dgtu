#include <iostream>
#include <vector>
#include <string>

// Шаблонный класс Queue (очередь)
template <typename T>
class Queue {
private:
    std::vector<T> elements;

public:
    // Добавление элемента в очередь
    void push(const T& item) {
        elements.push_back(item);
    }

    // Удаление и возврат первого элемента очереди
    T pop() {
        if (elements.empty()) {
            throw std::out_of_range("Queue is empty!");
        }
        T first = elements.front();
        elements.erase(elements.begin());
        return first;
    }

    // Проверка, пуста ли очередь
    bool isEmpty() const {
        return elements.empty();
    }

    // Вывод всех элементов очереди (для демонстрации)
    void display() const {
        std::cout << "Queue contents: ";
        for (const auto& item : elements) {
            std::cout << item << " ";
        }
        std::cout << std::endl;
    }
};

int main() {
    // Пример 1: Очередь строк
    Queue<std::string> stringQueue;

    stringQueue.push("Hello");
    stringQueue.push("World");
    stringQueue.push("!");

    stringQueue.display(); // Вывод: Hello World !

    std::cout << "Popped: " << stringQueue.pop() << std::endl; // Hello
    std::cout << "Popped: " << stringQueue.pop() << std::endl; // World

    stringQueue.display(); // Вывод: !

    // Пример 2: Очередь чисел
    Queue<int> intQueue;

    intQueue.push(10);
    intQueue.push(20);
    intQueue.push(30);

    intQueue.display(); // Вывод: 10 20 30

    std::cout << "Popped: " << intQueue.pop() << std::endl; // 10
    std::cout << "Popped: " << intQueue.pop() << std::endl; // 20

    intQueue.display(); // Вывод: 30

    return 0;
}

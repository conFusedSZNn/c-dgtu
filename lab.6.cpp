#include <iostream>
#include <stdexcept>
#include <deque>

template <typename T>
class Queue {
private:
    std::deque<T> elements;

public:
    // Добавление элемента в очередь
    void push(const T& value) {
        elements.push_back(value);
    }

    // Извлечение элемента из очереди с проверкой на пустоту
    T pop() {
        if (elements.empty()) {
            throw std::runtime_error("Queue is empty - cannot pop");
        }
        T value = elements.front();
        elements.pop_front();
        return value;
    }

    // Проверка на пустоту
    bool empty() const {
        return elements.empty();
    }

    // Получение размера очереди
    size_t size() const {
        return elements.size();
    }
};

int main() {
    try {
        Queue<int> intQueue;

        // Попытка извлечь из пустой очереди
        intQueue.pop(); // Вызовет исключение

    } catch (const std::runtime_error& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    try {
        Queue<std::string> stringQueue;

        stringQueue.push("First");
        stringQueue.push("Second");

        std::cout << "Popped: " << stringQueue.pop() << std::endl;
        std::cout << "Popped: " << stringQueue.pop() << std::endl;
        
        // Очередь теперь пуста
        stringQueue.pop(); // Вызовет исключение

    } catch (const std::runtime_error& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}

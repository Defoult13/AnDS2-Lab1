#include <iostream>
#include <chrono>
#include <vector>
#include <algorithm>


size_t lcg() {
    static size_t x = 0;
    x = (1021 * x + 24631) % 116640;
    return x;
}

class IntSet {
private:
    struct Node {
        int key;
        Node* left;
        Node* right;

        Node(int key) : key(key), left(nullptr), right(nullptr) {}
    };

    Node* root;

    Node* insert(Node* node, int key) {
        if (node == nullptr)
            return new Node(key);

        if (key < node->key)
            node->left = insert(node->left, key);
        else if (key > node->key)
            node->right = insert(node->right, key);

        return node;
    }

    bool contains(Node* node, int key) const {
        if (node == nullptr)
            return false;
        if (key == node->key)
            return true;
        if (key < node->key)
            return contains(node->left, key);
        else
            return contains(node->right, key);
    }

    Node* erase(Node* node, int key) {
        if (node == nullptr)
            return nullptr;

        if (key < node->key)
            node->left = erase(node->left, key);
        else if (key > node->key)
            node->right = erase(node->right, key);
        else {
            if (node->left == nullptr) {
                Node* rightChild = node->right;
                delete node;
                return rightChild;
            }
            if (node->right == nullptr) {
                Node* leftChild = node->left;
                delete node;
                return leftChild;
            }
            Node* minRight = findMin(node->right);
            node->key = minRight->key;
            node->right = erase(node->right, minRight->key);
        }
        return node;
    }

    Node* findMin(Node* node) const {
        while (node->left != nullptr)
            node = node->left;
        return node;
    }

    void destroy(Node* node) {
        if (node != nullptr) {
            destroy(node->left);
            destroy(node->right);
            delete node;
        }
    }

public:
    IntSet() : root(nullptr) {}

   
    IntSet(const IntSet& other) {
        root = copy(other.root);
    }

    
    IntSet& operator=(const IntSet& other) {
        if (this != &other) {
            destroy(root);
            root = copy(other.root);
        }
        return *this;
    }

    ~IntSet() {
        destroy(root);
    }

    void print() const {
        printInOrder(root);
        std::cout << std::endl;
    }

    bool insert(int key) {
        if (contains(key))
            return false;
        root = insert(root, key);
        return true;
    }

    bool contains(int key) const {
        return contains(root, key);
    }

    bool erase(int key) {
        if (!contains(key))
            return false;
        root = erase(root, key);
        return true;
    }

    // Функция для определения количества элементов
    size_t size() const {
        return size(root);
    }

private:
    size_t size(Node* node) const {
        if (node == nullptr)
            return 0;
        return 1 + size(node->left) + size(node->right);
    }

    Node* copy(Node* node) const {
        if (node == nullptr)
            return nullptr;
        Node* newNode = new Node(node->key);
        newNode->left = copy(node->left);
        newNode->right = copy(node->right);
        return newNode;
    }

    void printInOrder(Node* node) const {
        if (node != nullptr) {
            printInOrder(node->left);
            std::cout << node->key << " ";
            printInOrder(node->right);
        }
    }
};

// Функция для вычисления объединения двух множеств
IntSet unionSet(const IntSet& set1, const IntSet& set2) {
    IntSet result = set1; // Копируем set1

    // Добавляем элементы из set2, которых нет в result
    for (int i = 0; i <= 100; ++i) {
        if (!result.contains(i) && set2.contains(i))
            result.insert(i);
    }

    return result;
}

// Функция для вычисления симметрической разности двух множеств
IntSet symmetricDifference(const IntSet& set1, const IntSet& set2) {
    IntSet result;

    // Проверяем каждый элемент из set1
    for (int i = 0; i <= 100; ++i) {
        if (set1.contains(i) && !set2.contains(i))
            result.insert(i);
    }

    // Проверяем каждый элемент из set2
    for (int i = 0; i <= 100; ++i) {
        if (set2.contains(i) && !set1.contains(i))
            result.insert(i);
    }

    return result;
}

// Функция для тестирования времени заполнения, поиска, добавления и удаления для заданного количества элементов для IntSet
void testIntSetPerformance(int numElements) {
    double totalFillTime = 0.0;
    for (int attempt = 0; attempt < 100; ++attempt) {
        IntSet set;
        auto start = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < numElements; ++i) {
            int num = lcg();
            set.insert(num);
        }
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> fillTime = end - start;
        totalFillTime += fillTime.count();
    }
    double averageFillTime = totalFillTime / 100.0;

    double totalSearchTime = 0.0;
    double totalModifyTime = 0.0;
    for (int attempt = 0; attempt < 1000; ++attempt) {
        IntSet set;
        for (int i = 0; i < numElements; ++i) {
            int num = lcg();
            set.insert(num);
        }

        // Поиск в IntSet
        auto start = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < 1000; ++i) {
            int num = lcg();
            set.contains(num);
        }
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> searchTime = end - start;
        totalSearchTime += searchTime.count();

        // Добавление и удаление в IntSet
        start = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < 1000; ++i) {
            int num = lcg();
            set.insert(num);
            set.erase(num);
        }
        end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> modifyTime = end - start;
        totalModifyTime += modifyTime.count();
    }
    double averageSearchTime = totalSearchTime / 1000.0;
    double averageModifyTime = totalModifyTime / 1000.0;

    std::cout << "IntSet - Number of elements: " << numElements << std::endl;
    std::cout << "IntSet - Fill time: " << averageFillTime << " seconds" << std::endl;
    std::cout << "IntSet - Search time: " << averageSearchTime << " seconds" << std::endl;
    std::cout << "IntSet - Modification time (addition and deletion): " << averageModifyTime << " seconds" << std::endl;
}

// Функция для тестирования времени заполнения, поиска, добавления и удаления для заданного количества элементов для std::vector<int>
void testVectorPerformance(int numElements) {
    double totalFillTime = 0.0;
    for (int attempt = 0; attempt < 100; ++attempt) {
        std::vector<int> vec;
        auto start = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < numElements; ++i) {
            int num = lcg();
            vec.push_back(num);
        }
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> fillTime = end - start;
        totalFillTime += fillTime.count();
    }
    double averageFillTime = totalFillTime / 100.0;

    double totalSearchTime = 0.0;
    double totalModifyTime = 0.0;
    for (int attempt = 0; attempt < 1000; ++attempt) {
        std::vector<int> vec;
        for (int i = 0; i < numElements; ++i) {
            int num = lcg();
            vec.push_back(num);
        }

        // Поиск в std::vector<int>
        auto start = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < 1000; ++i) {
            int num = lcg();
            std::find(vec.begin(), vec.end(), num);
        }
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> searchTime = end - start;
        totalSearchTime += searchTime.count();

        // Добавление и удаление в std::vector<int>
        start = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < 1000; ++i) {
            int num = lcg();
            vec.push_back(num);
            vec.erase(std::remove(vec.begin(), vec.end(), num), vec.end());
        }
        end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> modifyTime = end - start;
        totalModifyTime += modifyTime.count();
    }
    double averageSearchTime = totalSearchTime / 1000.0;
    double averageModifyTime = totalModifyTime / 1000.0;

    std::cout << "std::vector<int> - Number of elements: " << numElements << std::endl;
    std::cout << "std::vector<int> - Fill time: " << averageFillTime << " seconds" << std::endl;
    std::cout << "std::vector<int> - Search time: " << averageSearchTime << " seconds" << std::endl;
    std::cout << "std::vector<int> - Modification time (addition and deletion): " << averageModifyTime << " seconds" << std::endl;
}

// Функция для тестирования функции объединения множеств
void testUnionSet() {
    IntSet set1;
    IntSet set2;

    set1.insert(1);
    set1.insert(2);
    set1.insert(3);

    set2.insert(3);
    set2.insert(4);
    set2.insert(5);

    IntSet resultSet = unionSet(set1, set2);
    resultSet.print(); // Ожидаемый результат: 1 2 3 4 5
}

// Функция для тестирования функции симметрической разности множеств
void testSymmetricDifference() {
    IntSet set1;
    IntSet set2;

    set1.insert(1);
    set1.insert(2);
    set1.insert(3);

    set2.insert(3);
    set2.insert(4);
    set2.insert(5);

    IntSet resultSet = symmetricDifference(set1, set2);
    resultSet.print(); // Ожидаемый результат: 1 2 4 5
}

int main() {
    // Проверка функции объединения множеств
    std::cout << "Testing unionSet:" << std::endl;
    testUnionSet();

    // Проверка функции симметрической разности множеств
    std::cout << "Testing symmetricDifference:" << std::endl;
    testSymmetricDifference();

    // Тестирование производительности для разных размеров контейнера
    testIntSetPerformance(1000);
    testIntSetPerformance(10000);
    testIntSetPerformance(100000);

    testVectorPerformance(1000);
    testVectorPerformance(10000);
    testVectorPerformance(100000);

    return 0;
}

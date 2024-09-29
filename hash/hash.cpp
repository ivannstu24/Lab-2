#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

using namespace std;


// Структура Node для хранения пары ключ-значение в хеш-таблице
struct Node {
    string key;   // Ключ элемента
    string value; // Значение элемента
    Node* next;        // Указатель на следующий элемент в цепочке

    Node(const string& k, const string& v) : key(k), value(v), next(nullptr) {}
};

// Класс HashTable для реализации хеш-таблицы
class HashTable {
public:
    HashTable(int size = 10) : capacity(size) {
        table = new Node*[capacity];  // Выделение памяти для массива указателей
        for (int i = 0; i < capacity; ++i) {
            table[i] = nullptr;
        }
    }

    ~HashTable() {
        clear();  // Освобождение памяти при удалении
        delete[] table;
    }

    // Добавление или обновление элемента по ключу
    void hset(const string& key, const string& value) {
        int index = hashFunction(key);
        Node* prev = nullptr;
        Node* current = table[index];

        // Поиск ключа в цепочке
        while (current != nullptr && current->key != key) {
            prev = current;
            current = current->next;
        }

        if (current == nullptr) {  // Ключ не найден, создаем новый элемент
            Node* newNode = new Node(key, value);
            if (prev == nullptr) {  // Вставляем в начало цепочки
                table[index] = newNode;
            } else {                // Добавляем в конец цепочки
                prev->next = newNode;
            }
            cout << "Inserted: [" << key << "] -> " << value << endl;
        } else {  // Ключ уже существует, обновляем значение
            current->value = value;
            cout << "Updated: [" << key << "] -> " << value << endl;
        }
    }

    // Получение значения по ключу
    void hget(const string& key) const {
        int index = hashFunction(key);
        Node* current = table[index];

        // Поиск значения по ключу в цепочке
        while (current != nullptr) {
            if (current->key == key) {
                cout << "Found: [" << key << "] -> " << current->value << endl;
                return;
            }
            current = current->next;
        }
        cout << "Key [" << key << "] not found!" << endl;
    }

    // Удаление элемента по ключу
    void hdel(const string& key) {
        int index = hashFunction(key);
        Node* prev = nullptr;
        Node* current = table[index];

        // Поиск ключа в цепочке
        while (current != nullptr && current->key != key) {
            prev = current;
            current = current->next;
        }

        if (current == nullptr) {
            cout << "Key [" << key << "] not found!" << endl;
            return;
        }

        if (prev == nullptr) {  // Удаление первого элемента в цепочке
            table[index] = current->next;
        } else {                // Удаление элемента в середине или конце цепочки
            prev->next = current->next;
        }

        delete current;  // Освобождение памяти
        cout << "Deleted: [" << key << "]" << endl;
    }

    // Очистка всей хеш-таблицы
    void clear() {
        for (int i = 0; i < capacity; ++i) {
            Node* current = table[i];
            while (current != nullptr) {
                Node* temp = current;
                current = current->next;
                delete temp;
            }
            table[i] = nullptr;
        }
    }

    // Сохранение хеш-таблицы в файл
    void saveToFile(const string& filename) {
        ofstream outFile(filename);
        if (outFile.is_open()) {
            for (int i = 0; i < capacity; ++i) {
                Node* current = table[i];
                while (current != nullptr) {
                    outFile << current->key << " " << current->value << endl;
                    current = current->next;
                }
            }
            outFile.close();
        } else {
            cerr << "Unable to open file for writing!" << endl;
        }
    }

    // Загрузка хеш-таблицы из файла
    void loadFromFile(const string& filename) {
        ifstream inFile(filename);
        if (inFile.is_open()) {
            string key, value;
            clear();  // Сбрасываем текущую хеш-таблицу перед загрузкой
            while (inFile >> key >> value) {
                hset(key, value);
            }
            inFile.close();
        } else {
            cerr << "Unable to open file for reading!" << endl;
        }
    }

private:
    // Хеш-функция для вычисления индекса на основе ключа
    int hashFunction(const string& key) const {
        int hash = 0;
        for (char ch : key) {
            hash = (hash * 31 + ch) % capacity;
        }
        return hash;
    }

    Node** table;  // Массив указателей на Node* (хеш-таблица)
    int capacity;  // Емкость таблицы
};

// Обработка команд для хеш-таблицы
void processCommand(HashTable& hashTable, const string& command) {
    string cmd, key, value;
    istringstream iss(command);
    iss >> cmd;

    if (cmd == "HSET") {
        iss >> key >> value;
        hashTable.hset(key, value);
    } else if (cmd == "HGET") {
        iss >> key;
        hashTable.hget(key);
    } else if (cmd == "HDEL") {
        iss >> key;
        hashTable.hdel(key);
    } else {
        cout << "Unknown command: " << command << endl;
    }
}

int main(int argc, char* argv[]) {
    if (argc != 5) {
        cerr << "Usage: " << argv[0] << " --file filename --query 'COMMAND'" << endl;
        return 1;
    }

    string fileFlag = argv[1];
    string filename = argv[2];
    string queryFlag = argv[3];
    string query = argv[4];

    if (fileFlag != "--file" || queryFlag != "--query") {
        cerr << "Invalid flags!" << endl;
        return 1;
    }

    HashTable hashTable;
    hashTable.loadFromFile(filename);  // Загружаем хеш-таблицу из файла
    processCommand(hashTable, query);  // Обрабатываем команду
    hashTable.saveToFile(filename);    // Сохраняем изменения в файл

    return 0;
}

   // ./hash_program --file hash_table.data --query 'HSET mykey1 value1'     # Добавление элемента с ключом mykey1 и значением value1
   // ./hash_program --file hash_table.data --query 'HGET mykey1'            # Получение значения по ключу mykey1
   // ./hash_program --file hash_table.data --query 'HDEL mykey1'            # Удаление элемента по ключу mykey1

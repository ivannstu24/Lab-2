
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>


using namespace std;


//./array_program --file array.data --query 'MPUSH 15'    # Добавление 15 в конец
//./array_program --file array.data --query 'MADD 1 20'   # Добавление 20 на позицию 1
//./array_program --file array.data --query 'MGET 1'      # Получение значения по индексу 1
//./array_program --file array.data --query 'MSET 1 30'   # Замена значения по индексу 1 на 30
//./array_program --file array.data --query 'MDEL 2'      # Удаление элемента по индексу 2
//./array_program --file array.data --query 'MLEN'        # Длина массива





// Интерфейс для структуры "массив"
class ArrayInterface {
public:
    virtual void push(int value) = 0;
    virtual void addByIndex(int index, int value) = 0;
    virtual void deleteByIndex(int index) = 0;
    virtual void setByIndex(int index, int value) = 0;
    virtual void getValue(int index) = 0;
    virtual int length() = 0;
    virtual void displayArray() = 0;
    virtual void saveToFile(const string& filename) = 0;
    virtual void loadFromFile(const string& filename) = 0;
};

// Реализация массива на основе динамического выделения памяти
class Array : public ArrayInterface {
public:
    Array() : size(0), capacity(10) {
        array = new int[capacity];  // Изначально выделяем память на 10 элементов
    }

    ~Array() {
        delete[] array;  // Освобождаем память
    }

    // Добавление элемента в конец
    void push(int value) override {
        if (size >= capacity) {
            resize();
        }
        array[size++] = value;
    }

    // Добавление элемента по индексу
    void addByIndex(int index, int value) override {
        if (index < 0 || index > size) {
            cout << "Invalid index!" << endl;
            return;
        }
        if (size >= capacity) {
            resize();
        }
        for (int i = size; i > index; --i) {
            array[i] = array[i - 1];
        }
        array[index] = value;
        size++;
    }

    // Удаление элемента по индексу
    void deleteByIndex(int index) override {
        if (index < 0 || index >= size) {
            cout << "Index out of bounds!" << endl;
            return;
        }
        for (int i = index; i < size - 1; ++i) {
            array[i] = array[i + 1];
        }
        size--;
    }

    // Замена элемента по индексу
    void setByIndex(int index, int value) override {
        if (index < 0 || index >= size) {
            cout << "Index out of bounds!" << endl;
            return;
        }
        array[index] = value;
    }

    // Получение элемента по индексу
    void getValue(int index) override {
        if (index < 0 || index >= size) {
            cout << "Index out of bounds!" << endl;
        } else {
            cout << "Element at index " << index << ": " << array[index] << endl;
        }
    }

    // Получение длины массива
    int length() override {
        return size;
    }

    // Печать всех элементов массива
    void displayArray() override {
        for (int i = 0; i < size; ++i) {
            cout << array[i] << " ";
        }
        cout << endl;
    }

    // Сохранение массива в файл
    void saveToFile(const string& filename) override {
        ofstream outFile(filename);
        if (outFile.is_open()) {
            for (int i = 0; i < size; ++i) {
                outFile << array[i] << endl;
            }
            outFile.close();
        } else {
            cerr << "Unable to open file for writing!" << endl;
        }
    }

    // Загрузка массива из файла
    void loadFromFile(const string& filename) override {
        ifstream inFile(filename);
        if (inFile.is_open()) {
            int value;
            size = 0;  // Сбрасываем текущий размер
            while (inFile >> value) {
                push(value);
            }
            inFile.close();
        } else {
            cerr << "Unable to open file for reading!" << endl;
        }
    }

private:
    int* array;    // Указатель на массив
    int size;      // Текущий размер массива
    int capacity;  // Емкость массива

    // Увеличение размера массива в 2 раза
    void resize() {
        capacity *= 2;
        int* newArray = new int[capacity];
        for (int i = 0; i < size; ++i) {
            newArray[i] = array[i];
        }
        delete[] array;  // Освобождаем старую память
        array = newArray;
    }
};

// Обработка команд
void processCommand(ArrayInterface& array, const string& command) {
    string cmd;
    int index, value;
    istringstream iss(command);
    iss >> cmd;

    if (cmd == "MPUSH") {
        iss >> value;
        array.push(value);
        cout << "Added " << value << " to array" << endl;
    } else if (cmd == "MADD") {
        iss >> index >> value;
        array.addByIndex(index, value);
    } else if (cmd == "MDEL") {
        iss >> index;
        array.deleteByIndex(index);
    } else if (cmd == "MGET") {
        iss >> index;
        array.getValue(index);
    } else if (cmd == "MSET") {
        iss >> index >> value;
        array.setByIndex(index, value);
    } else if (cmd == "MLEN") {
        cout << "Length of array: " << array.length() << endl;
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

    Array array;
    array.loadFromFile(filename);
    processCommand(array, query);
    array.displayArray();
    array.saveToFile(filename);

    return 0;
}

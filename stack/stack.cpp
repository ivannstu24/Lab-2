#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

using namespace std;

// Структура ноды для стека (связанный список)
struct Node {
    int data;       // Данные ноды
    Node* next;     // Указатель на следующий элемент

    Node(int value) : data(value), next(nullptr) {}
};

// Класс Stack для реализации стека
class Stack {
public:
    Stack() : top(nullptr), size(0) {}

    ~Stack() {
        clear();  // Очистка памяти при удалении стека
    }

    // Добавление элемента на вершину стека
    void push(int value) {
        Node* newNode = new Node(value);
        newNode->next = top;  // Устанавливаем указатель на текущую вершину
        top = newNode;        // Вершина теперь указывает на новый элемент
        size++;
    }

    // Удаление элемента с вершины стека
    void pop() {
        if (isEmpty()) {
            cout << "Stack is empty!" << endl;
            return;
        }
        Node* temp = top;     // Временный указатель на текущую вершину
        top = top->next;      // Перемещаем вершину на следующий элемент
        delete temp;          // Удаляем старую вершину
        size--;
    }

    // Чтение всех элементов стека
    void readStack() const {
        Node* current = top;
        cout << "Stack elements: ";
        while (current != nullptr) {
            cout << current->data << " ";
            current = current->next;
        }
        cout << endl;
    }

    // Проверка, пуст ли стек
    bool isEmpty() const {
        return top == nullptr;
    }

    // Очистка стека (удаление всех элементов)
    void clear() {
        while (!isEmpty()) {
            pop();
        }
    }

    // Сохранение стека в файл
    void saveToFile(const string& filename) {
        ofstream outFile(filename);
        if (outFile.is_open()) {
            Node* current = top;
            while (current != nullptr) {
                outFile << current->data << endl;
                current = current->next;
            }
            outFile.close();
        } else {
            cerr << "Unable to open file for writing!" << endl;
        }
    }

    // Загрузка стека из файла
    void loadFromFile(const string& filename) {
        ifstream inFile(filename);
        if (inFile.is_open()) {
            int value;
            clear();  // Сбрасываем текущий стек перед загрузкой
            while (inFile >> value) {
                push(value);
            }
            inFile.close();
        } else {
            cerr << "Unable to open file for reading!" << endl;
        }
    }

private:
    Node* top;  // Вершина стека (указатель на последний добавленный элемент)
    int size;   // Текущий размер стека
};

// Обработка команд для стека
void processCommand(Stack& stack, const string& command) {
    string cmd;
    int value;
    istringstream iss(command);
    iss >> cmd;

    if (cmd == "SPUSH") {
        iss >> value;
        stack.push(value);
        cout << "Pushed " << value << " to stack" << endl;
    } else if (cmd == "SPOP") {
        stack.pop();
        cout << "Popped top element from stack" << endl;
    } else if (cmd == "SREAD") {
        stack.readStack();
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

    Stack stack;
    stack.loadFromFile(filename);   // Загружаем данные из файла
    processCommand(stack, query);   // Обрабатываем команду
    stack.saveToFile(filename);     // Сохраняем изменения в файл

    return 0;
}

//./stack_program --file stack.data --query 'SPUSH 10'     # Добавление 10 на вершину стека
//./stack_program --file stack.data --query 'SPUSH 20'     # Добавление 20 на вершину стека
//./stack_program --file stack.data --query 'SREAD'        # Чтение всех элементов стека
//./stack_program --file stack.data --query 'SPOP'         # Удаление элемента с вершины
//./stack_program --file stack.data --query 'SREAD'        # Чтение всех элементов стека после удаления


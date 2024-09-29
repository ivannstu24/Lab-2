#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

using namespace std;

//./dbms2 --file queue.data --query 'QPUSH 10'
//./dbms2 --file queue.data --query 'QPUSH 20'



// Узел для очереди
struct QueueNode {
    int data;
    QueueNode* next;

    QueueNode(int value) : data(value), next(nullptr) {}
};

// Интерфейс для общих операций структуры данных
class QueueInterface {
public:
    virtual void enqueue(int value) = 0;
    virtual void dequeue() = 0;
    virtual void peek() = 0;
    virtual void displayQueue() = 0;
    virtual void saveToFile(const string& filename) = 0;
    virtual void loadFromFile(const string& filename) = 0;
};

// Реализация очереди
class Queue : public QueueInterface {
public:
    Queue() : head(nullptr), tail(nullptr) {}

    // Добавление элемента в конец
    void enqueue(int value) override {
        QueueNode* newNode = new QueueNode(value);
        if (tail == nullptr) { // Если очередь пуста
            head = tail = newNode;
        } else {
            tail->next = newNode;
            tail = newNode;
        }
    }

    // Удаление элемента с начала
    void dequeue() override {
        if (head == nullptr) {
            cout << "Queue is empty!" << endl;
            return;
        }
        QueueNode* temp = head;
        head = head->next;
        if (head == nullptr) { // Если очередь опустела
            tail = nullptr;
        }
        cout << "Removed: " << temp->data << endl;
        delete temp;
    }

    // Получение элемента с начала очереди без удаления
    void peek() override {
        if (head != nullptr) {
            cout << "Front of queue: " << head->data << endl;
        } else {
            cout << "Queue is empty!" << endl;
        }
    }

    // Печать всех элементов
    void displayQueue() override {
        QueueNode* current = head;
        while (current != nullptr) {
            cout << current->data << " ";
            current = current->next;
        }
        cout << endl;
    }

    // Сохранение очереди в файл
    void saveToFile(const string& filename) override {
        ofstream outFile(filename);
        if (outFile.is_open()) {
            QueueNode* current = head;
            while (current != nullptr) {
                outFile << current->data << endl;
                current = current->next;
            }
            outFile.close();
        } else {
            cerr << "Unable to open file for writing!" << endl;
        }
    }

    // Загрузка очереди из файла
    void loadFromFile(const string& filename) override {
        ifstream inFile(filename);
        if (inFile.is_open()) {
            int value;
            while (inFile >> value) {
                enqueue(value);
            }
            inFile.close();
        } else {
            cerr << "Unable to open file for reading!" << endl;
        }
    }

private:
    QueueNode* head;
    QueueNode* tail;
};

// Обработка команд
void processCommand(QueueInterface& queue, const string& command) {
    string cmd;
    int value;
    istringstream iss(command);
    iss >> cmd;

    if (cmd == "QPUSH") {
        iss >> value;
        queue.enqueue(value);
        cout << "Added " << value << " to queue" << endl;
    } else if (cmd == "QPOP") {
        queue.dequeue();
    } else if (cmd == "QPEEK") {
        queue.peek();
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

    Queue queue;

    queue.loadFromFile(filename);
    processCommand(queue, query);
    queue.displayQueue();
    queue.saveToFile(filename);

    return 0;
}

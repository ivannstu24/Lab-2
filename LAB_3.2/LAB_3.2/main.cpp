#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

using namespace std;

//./dbms --file list.txt --type single --query 'LPUSH 5'
//./dbms --file list.txt --type double --query 'LGET 10'




// Узел для двусвязного списка
struct Node {
    int data;
    Node* next;
    Node* prev;

    Node(int value) : data(value), next(nullptr), prev(nullptr) {}
};

// Узел для односвязного списка
struct SingleNode {
    int data;
    SingleNode* next;

    SingleNode(int value) : data(value), next(nullptr) {}
};

// Интерфейс для общих операций списка
class ListInterface {
public:
    virtual void addToHead(int value) = 0;
    virtual void deleteByValue(int value) = 0;
    virtual void getValue(int value) = 0;
    virtual void displayList() = 0;
    virtual void saveToFile(const string& filename) = 0;
    virtual void loadFromFile(const string& filename) = 0;
};

// Реализация двусвязного списка
class DoublyLinkedList : public ListInterface {
public:
    DoublyLinkedList() : head(nullptr), tail(nullptr) {}

    void addToHead(int value) override {
        Node* newNode = new Node(value);
        if (head == nullptr) {
            head = tail = newNode;
        } else {
            newNode->next = head;
            head->prev = newNode;
            head = newNode;
        }
    }

    void deleteByValue(int value) override {
        Node* current = head;
        while (current != nullptr && current->data != value) {
            current = current->next;
        }
        if (current == nullptr) return;

        if (current == head) {
            head = head->next;
            if (head != nullptr) head->prev = nullptr;
        } else if (current == tail) {
            tail = tail->prev;
            if (tail != nullptr) tail->next = nullptr;
        } else {
            current->prev->next = current->next;
            current->next->prev = current->prev;
        }
        delete current;
    }

    void getValue(int value) override {
        Node* current = head;
        while (current != nullptr) {
            if (current->data == value) {
                cout << "Element found: " << value << endl;
                return;
            }
            current = current->next;
        }
        cout << "Element not found: " << value << endl;
    }

    void displayList() override {
        Node* current = head;
        while (current != nullptr) {
            cout << current->data << " ";
            current = current->next;
        }
        cout << endl;
    }

    void saveToFile(const string& filename) override {
        ofstream outFile(filename);
        if (outFile.is_open()) {
            Node* current = head;
            while (current != nullptr) {
                outFile << current->data << endl;
                current = current->next;
            }
            outFile.close();
        } else {
            cerr << "Unable to open file for writing!" << endl;
        }
    }

    void loadFromFile(const string& filename) override {
        ifstream inFile(filename);
        if (inFile.is_open()) {
            int value;
            while (inFile >> value) {
                addToHead(value);
            }
            inFile.close();
        } else {
            cerr << "Unable to open file for reading!" << endl;
        }
    }

private:
    Node* head;
    Node* tail;
};

// Реализация односвязного списка
class SinglyLinkedList : public ListInterface {
public:
    SinglyLinkedList() : head(nullptr) {}

    void addToHead(int value) override {
        SingleNode* newNode = new SingleNode(value);
        newNode->next = head;
        head = newNode;
    }

    void deleteByValue(int value) override {
        if (head == nullptr) return;
        
        if (head->data == value) {
            SingleNode* temp = head;
            head = head->next;
            delete temp;
            return;
        }

        SingleNode* current = head;
        while (current->next != nullptr && current->next->data != value) {
            current = current->next;
        }

        if (current->next == nullptr) return;

        SingleNode* temp = current->next;
        current->next = current->next->next;
        delete temp;
    }

    void getValue(int value) override {
        SingleNode* current = head;
        while (current != nullptr) {
            if (current->data == value) {
                cout << "Element found: " << value << endl;
                return;
            }
            current = current->next;
        }
        cout << "Element not found: " << value << endl;
    }

    void displayList() override {
        SingleNode* current = head;
        while (current != nullptr) {
            cout << current->data << " ";
            current = current->next;
        }
        cout << endl;
    }

    void saveToFile(const string& filename) override {
        ofstream outFile(filename);
        if (outFile.is_open()) {
            SingleNode* current = head;
            while (current != nullptr) {
                outFile << current->data << endl;
                current = current->next;
            }
            outFile.close();
        } else {
            cerr << "Unable to open file for writing!" << endl;
        }
    }

    void loadFromFile(const string& filename) override {
        ifstream inFile(filename);
        if (inFile.is_open()) {
            int value;
            while (inFile >> value) {
                addToHead(value);
            }
            inFile.close();
        } else {
            cerr << "Unable to open file for reading!" << endl;
        }
    }

private:
    SingleNode* head;
};

// Обработка команд
void processCommand(ListInterface& list, const string& command) {
    string cmd;
    int value;
    istringstream iss(command);
    iss >> cmd;

    if (cmd == "LPUSH") {
        iss >> value;
        list.addToHead(value);
        cout << "Added " << value << " to head" << endl;
    } else if (cmd == "LDEL") {
        iss >> value;
        list.deleteByValue(value);
        cout << "Deleted " << value << endl;
    } else if (cmd == "LGET") {
        iss >> value;
        list.getValue(value);
    } else {
        cout << "Unknown command: " << command << endl;
    }
}

int main(int argc, char* argv[]) {
    if (argc != 7) {
        cerr << "Usage: " << argv[0] << " --file filename --type single|double --query 'COMMAND'" << endl;
        return 1;
    }

    string fileFlag = argv[1];
    string filename = argv[2];
    string typeFlag = argv[3];
    string listType = argv[4];
    string queryFlag = argv[5];
    string query = argv[6];

    if (fileFlag != "--file" || typeFlag != "--type" || queryFlag != "--query") {
        cerr << "Invalid flags!" << endl;
        return 1;
    }

    ListInterface* list = nullptr;

    if (listType == "single") {
        list = new SinglyLinkedList();
    } else if (listType == "double") {
        list = new DoublyLinkedList();
    } else {
        cerr << "Invalid list type!" << endl;
        return 1;
    }

    list->loadFromFile(filename);
    processCommand(*list, query);
    list->displayList();
    list->saveToFile(filename);

    delete list;
    return 0;
}


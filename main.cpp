// 18463339 Ndumezulu Ngwenya
#include <iostream>
#include <fstream>
#include <string>
#include <regex> // Library for regular expressions

using namespace std;

// Define a structure for a node in the linked stack
struct Node {
    string data;
    Node* next;
    Node(const string& d) : data(d), next(nullptr) {} // Constructor
};

// Class for a linked stack
class LinkedStack {
private:
    Node* top;

public:
    LinkedStack() : top(nullptr) {} // Constructor

    ~LinkedStack() { // Destructor to free memory
        while (!isEmpty()) {
            pop();
        }
    }

    void push(const string& url) { // Function to push a URL onto the stack
        Node* newNode = new Node(url); // Create a new node
        newNode->next = top; // Point new node to current top
        top = newNode; // Set new node as top
    }

    string pop() { // Function to pop a URL from the stack
        if (isEmpty()) { // Check if stack is empty
            cerr << "Error: Stack is empty." << endl;
            return "";
        }
        string data = top->data; // Get data from top node
        Node* temp = top; // Temporary pointer to top node
        top = top->next; // Move top to next node
        delete temp; // Free memory
        return data; // Return popped URL
    }

    bool isEmpty() const { // Function to check if stack is empty
        return top == nullptr;
    }

    string peek() const { // Function to peek at the top URL without popping
        if (isEmpty()) { // Check if stack is empty
            cerr << "Error: Stack is empty." << endl;
            return "";
        }
        return top->data; // Return data from top node
    }
};

// Function to validate URL using regular expression
bool validateURL(const string& url) {
    regex pattern("www\\..+\\..+"); // Regular expression pattern for URL validation
    return regex_match(url, pattern); // Check if URL matches pattern
}

// Function to read initial URLs from file
void readInitialURLs(LinkedStack& history) {
    ifstream file("C:\\Data\\BrowserHistory.txt"); // Open file
    if (file.is_open()) { // Check if file is open
        string url;
        while (getline(file, url)) { // Read each line from file
            if (validateURL(url)) { // Validate URL
                history.push(url); // Push valid URL onto history stack
            } else {
                cerr << "Invalid URL found: " << url << endl; // Error message for invalid URL
            }
        }
        file.close(); // Close file
    } else {
        cerr << "Error: Unable to open file." << endl; // Error message for file open failure
    }
}

int main() {
    LinkedStack history; // Create history stack
    readInitialURLs(history); // Read initial URLs from file

    LinkedStack forwardHistory; // Stack for forward history

    int choice; // Variable to store user choice
    do {
        cout << "Menu:" << endl;
        cout << "1. Add a new URL" << endl;
        cout << "2. Navigate back to the previous URL" << endl;
        cout << "3. Navigate forward to the next URL" << endl;
        cout << "4. Exit" << endl;
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1: { // Add a new URL
                string newURL;
                cout << "Enter the new URL: ";
                cin >> newURL;
                if (validateURL(newURL)) { // Validate new URL
                    history.push(newURL); // Push new URL onto history stack
                    cout << "URL added successfully." << endl;
                    // Clear forward history when a new URL is added
                    while (!forwardHistory.isEmpty()) {
                        forwardHistory.pop();
                    }
                } else {
                    cerr << "Invalid URL entered." << endl; // Error message for invalid URL
                }
                break;
            }
            case 2: { // Navigate back
                if (!history.isEmpty()) {
                    // Move current URL to forward history before popping
                    forwardHistory.push(history.peek());
                    cout << "Navigating back to: " << history.pop() << endl;
                } else {
                    cerr << "No previous URLs in history." << endl; // Error message for empty history
                }
                break;
            }
            case 3: { // Navigate forward
                if (!forwardHistory.isEmpty()) {
                    // Move current URL to history before pushing
                    history.push(forwardHistory.peek());
                    cout << "Navigating forward to: " << forwardHistory.pop() << endl;
                } else {
                    cerr << "No forward URLs in history." << endl; // Error message for empty forward history
                }
                break;
            }
            case 4:
                cout << "Exiting program." << endl; // Exit program
                break;
            default:
                cerr << "Invalid choice. Please enter a number between 1 and 4." << endl; // Error message for invalid choice
        }
    } while (choice != 4); // Continue loop until user chooses to exit

    return 0;
}

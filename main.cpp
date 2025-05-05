
#include "Inventory.h"
#include <limits>

using namespace std;

// Helper functions for input validation
int getValidIntInput(const string& prompt) {
    int value;
    cout << prompt;
    while (!(cin >> value)) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid input. Please enter a number: ";
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    return value;
}

string getLineInput(const string& prompt) {
    string input;
    cout << prompt;
    getline(cin, input);
    return input;
}

Position getPositionInput() {
    int shelf, compartment;
    
    shelf = getValidIntInput("Enter shelf number (0-2): ");
    while (shelf < 0 || shelf >= 3) {
        shelf = getValidIntInput("Invalid shelf. Please enter a number between 0-2: ");
    }
    
    compartment = getValidIntInput("Enter compartment number (0-14): ");
    while (compartment < 0 || compartment >= 15) {
        compartment = getValidIntInput("Invalid compartment. Please enter a number between 0-14: ");
    }
    
    return Position(shelf, compartment);
}

int main() {
    // Create inventory
    Inventory inv;
    int menuChoice;
    int nextId = 1000; // Starting ID for items

    do {
        cout
        << "\n=== Library Inventory System Menu ===\n"
        << "1. Add a Book\n"
        << "2. Add a Magazine\n"
        << "3. Add a Movie\n"
        << "4. Check Out Item\n"
        << "5. Check In Item\n"
        << "6. Swap Items\n"
        << "7. Print All Items\n"
        << "8. Print Checked Out Items\n"
        << "0. Exit\n"
        << "=======================================\n"
        << "Enter your choice: ";
        cin >> menuChoice; cout << endl;
        cin.ignore(); cin.clear();

        switch (menuChoice) {
                case 0:
                    cout << "Exiting program..." << endl;
                    break;

                case 1: { // Add a Book
                    string name = getLineInput("Enter name: ");
                    string description = getLineInput("Enter description: ");
                    string title = getLineInput("Enter title: ");
                    string author = getLineInput("Enter author: ");
                    string copyright = getLineInput("Enter copyright date: ");

                    Book book(name, description, nextId++, title, author, copyright);
                    Position pos = getPositionInput();

                    inv.addItem(pos, book);
                    cout << "Book added successfully!" << endl;
                    break;
                }

                case 2: { // Add a Magazine
                    string name = getLineInput("Enter name: ");
                    string description = getLineInput("Enter description: ");
                    string edition = getLineInput("Enter edition: ");
                    string title = getLineInput("Enter title of main article: ");

                    Magazine magazine(name, description, nextId++, edition, title);
                    Position pos = getPositionInput();

                    inv.addItem(pos, magazine);
                    cout << "Magazine added successfully!" << endl;
                    break;
                }

                case 3: { // Add a Movie
                    string name = getLineInput("Enter name: ");
                    string description = getLineInput("Enter description: ");
                    string title = getLineInput("Enter title: ");
                    string director = getLineInput("Enter director: ");

                    int numActors = getValidIntInput("Enter number of main actors: ");
                    vector<string> actors;
                    for (int i = 0; i < numActors; i++) {
                        string actor = getLineInput("Enter actor " + to_string(i+1) + ": ");
                        actors.push_back(actor);
                    }

                    Movie movie(name, description, nextId++, title, director, actors);
                    Position pos = getPositionInput();

                    inv.addItem(pos, movie);
                    cout << "Movie added successfully!" << endl;
                    break;
                }

                case 4: { // Check Out Item
                    string itemId = getLineInput("Enter item ID to check out: ");
                    string checkOutBy = getLineInput("Enter name of person checking out: ");

                    Item* item = inv.checkoutItem(itemId, checkOutBy);
                    cout << "Item checked out successfully:" << endl;
                    cout << *item << endl;
                    break;
                }

                case 5: { // Check In Item
                    string itemId = getLineInput("Enter item ID to check in: ");
                    int id = stoi(itemId);

                    // Create a dummy item with just the ID for checking in
                    Item dummyItem("", "", id);
                    inv.checkinItem(dummyItem);
                    cout << "Item checked in successfully!" << endl;
                    break;
                }

                case 6: { // Swap Items
                    cout << "First position:" << endl;
                    Position pos1 = getPositionInput();

                    cout << "Second position:" << endl;
                    Position pos2 = getPositionInput();

                    inv.swapItems(pos1, pos2);
                    cout << "Items swapped successfully!" << endl;
                    break;
                }
                case 7: // Print All Items
                    cout << inv;
                    break;
                case 8: // Print Checked Out Items
                    inv.printCheckedOutItems();
                    break;

                default:
                    cout << "Invalid choice. Please try again." << endl;
            }

    } while (menuChoice != 0);
    
    return 0;
}
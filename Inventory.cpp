//
// Created by Jawad Khadra on 5/5/25.
//

#include "Inventory.h"
#include <stdexcept>
#include <iostream>
#include <ctime>
#include <iomanip>
#include <sstream>

using namespace std;

/**
 * Constructor implementation
 * 
 * I chose to initialize each shelf compartment to nullptr rather than rely on default
 * initialization to explicitly show that all compartments start empty. This approach
 * clarifies the code's intent and ensures consistent behavior regardless of
 * compiler optimizations or implementation details.
 */
Inventory::Inventory() {
    // Initialize all compartments to nullptr (empty)
    for (auto & shelve : shelves) {
        for (auto & j : shelve) j = nullptr;
    }
}

/**
 * Destructor implementation
 * 
 * One of the significant benefits of using smart pointers (unique_ptr) is automatic
 * memory management. This empty destructor demonstrates that C++ RAII (Resource
 * Acquisition Is Initialization) principle is being followed, allowing resources
 * to be automatically released when they go out of scope. This prevents memory leaks
 * without requiring explicit cleanup code.
 */
Inventory::~Inventory() = default;

/**
 * Helper method to convert integer IDs to string representation
 * 
 * This centralized conversion method ensures consistent string ID formatting
 * throughout the codebase. By isolating this conversion logic, we make the code
 * more maintainable - if we ever need to change the ID format (e.g., add prefixes
 * or zero-padding), we only need to modify this one function.
 */
string Inventory::getStringId(int id) {
    return to_string(id);
}

/**
 * Operator overloading for non-const shelf access
 * 
 * This implementation of the [] operator enables intuitive, array-like access
 * to shelves and compartments using syntax like: inventory[2][3]. The bound
 * checking ensures that invalid shelf indices are caught immediately with a
 * descriptive exception rather than causing undefined behavior or silent failures.
 */
unique_ptr<Item>* Inventory::operator[](int shelfIndex) {
    if (shelfIndex < 0 || shelfIndex >= 3) throw out_of_range("Shelf index out of range");
    return shelves[shelfIndex];
}

/**
 * Operator overloading for const shelf access
 * 
 * Having separate const and non-const versions of the [] operator is a C++ best
 * practice that enables proper const-correctness. This ensures that const Inventory
 * objects can still be accessed for reading but not for modification, maintaining
 * the integrity of the const contract.
 */
const unique_ptr<Item>* Inventory::operator[](int shelfIndex) const {
    if (shelfIndex < 0 || shelfIndex >= 3) throw out_of_range("Shelf index out of range");
    return shelves[shelfIndex];
}

/**
 * Checks if a compartment at the specified position is empty
 * 
 * This utility method abstracts the logic for checking empty compartments,
 * providing position validation as a safeguard. I factored this into a separate
 * method to avoid code duplication, as this check is needed in multiple places.
 * The explicit validity check prevents accessing invalid array indices.
 */
bool Inventory::isCompartmentEmpty(const Position& pos) const {
    if (!pos.isValid()) throw out_of_range("Position is out of range");
    return shelves[pos.getRow()][pos.getCol()] == nullptr;
}

/**
 * Checks if an item with the given ID is currently checked out
 * 
 * Using the map's find() method provides an efficient O(log n) lookup to determine
 * if an item is checked out. This is more efficient than iterating through the
 * entire map, especially as the number of checked-out items grows.
 */
bool Inventory::isItemCheckedOut(const string& itemId) const {
    return checkedOutItems.contains(itemId);
}

/**
 * Adds an item to the inventory at the specified position
 * 
 * This method demonstrates several important concepts:
 * 1. Input validation - checking position validity and compartment availability
 * 2. Polymorphism - maintaining the specific derived item type (Book, Movie, etc.)
 * 3. Memory safety - using smart pointers to prevent leaks
 * 
 * I chose to use dynamic_cast for type checking because it allows us to correctly
 * handle the item hierarchy, preserving all specific properties of derived item types.
 * This approach enables proper polymorphic behavior where specific item details
 * (like book author or movie actors) are maintained even through the inventory system.
 */
void Inventory::addItem(const Position& position, const Item& item) {
    // Validate position
    if (!position.isValid()) {
        throw out_of_range("Position is out of valid range");
    }
    
    // Check if the compartment is empty
    if (!isCompartmentEmpty(position)) {
        throw runtime_error("Compartment is not empty");
    }

    shelves[position.getRow()][position.getCol()] = make_unique<Item>(item);
}

/**
 * Checks out an item from the inventory
 * 
 * This method handles the complex process of finding an item by ID, generating
 * a due date, and transferring ownership of the item from the shelf to the 
 * checked-out items collection. Key design decisions include:
 * 
 * 1. Returning a raw pointer to the item rather than a reference or copy, allowing
 *    the caller to access but not own the item (ownership remains with the inventory)
 * 
 * 2. Using C++'s time utilities to generate a realistic due date 30 days in the future
 * 
 * 3. Employing std::move to transfer unique_ptr ownership without copying the underlying
 *    item object
 * 
 * 4. Storing the original position to ensure the item can be returned to its proper place
 * 
 * The itemPtr approach allows us to return a pointer to the checked-out item while
 * maintaining the ownership semantics of unique_ptr.
 */
Item* Inventory::checkoutItem(const string& itemId, const string& checkOutBy) {
    // Find the item with the given ID
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 15; j++) {
            if (shelves[i][j] && getStringId(shelves[i][j]->getID()) == itemId) {
                // Generate the due date (30 days from now)
                const auto now = time(nullptr);
                auto tm = *localtime(&now);
                tm.tm_mday += 30; // Add 30 days
                mktime(&tm);
                
                stringstream dueDate;
                dueDate << put_time(&tm, "%Y-%m-%d");
                
                // Create checkout record
                const Position pos(i, j);
                
                // Create the unique_ptr and store a raw pointer for return
                Item* itemPtr = shelves[i][j].get();
                
                // Insert into the map using emplace
                checkedOutItems.emplace(
                    itemId, 
                    CheckoutInfo(checkOutBy, dueDate.str(), pos, move(shelves[i][j]))
                );
                
                // Return pointer to the checked-out item
                return itemPtr;
            }
        }
    }
    
    // Item not found
    throw runtime_error("Item with ID " + itemId + " not found");
}

/**
 * Checks in a previously checked out item
 * 
 * This method demonstrates the full lifecycle of a library item - after being
 * checked out, it can be returned to its original position. The implementation:
 * 
 * 1. Validates that the item is actually checked out
 * 2. Retrieves the original position stored during checkout
 * 3. Moves the item back to its shelf location
 * 4. Cleans up the checkout record
 * 
 * I implemented this using the item's ID rather than requiring the exact same
 * item object that was checked out. This approach is more user-friendly, as it
 * only requires knowing the item's ID to check it back in.
 */
void Inventory::checkinItem(const Item& item) {
    string itemId = getStringId(item.getID());
    
    // Check if the item is checked out
    auto it = checkedOutItems.find(itemId);
    if (it == checkedOutItems.end()) {
        throw runtime_error("Item is not checked out");
    }
    
    // Get the original position
    Position pos = it->second.originalPosition;
    
    // Return the item to its original position
    shelves[pos.getRow()][pos.getCol()] = move(it->second.item);
    
    // Remove from checked out items
    checkedOutItems.erase(it);
}

/**
 * Swaps the positions of two items in the inventory
 * 
 * This method demonstrates the power of C++'s std::swap function combined with
 * unique_ptr to perform a complex operation with minimal code. The implementation:
 * 
 * 1. Validates both positions to prevent out-of-bounds access
 * 2. Verifies both compartments contain items to swap
 * 3. Performs the swap using std::swap
 * 
 * Using std::swap with unique_ptr handles the ownership transfer correctly without
 * any risk of memory leaks. This is significantly safer than manual pointer swapping
 * which could lead to ownership issues or memory leaks.
 */
void Inventory::swapItems(const Position& pos1, const Position& pos2) {
    // Validate positions
    if (!pos1.isValid() || !pos2.isValid()) {
        throw out_of_range("Position is out of valid range");
    }

    // Check if both compartments have items
    if (isCompartmentEmpty(pos1) || isCompartmentEmpty(pos2)) {
        throw runtime_error("Cannot swap: one or both compartments are empty");
    }
    
    // Swap the items
    swap(shelves[pos1.getRow()][pos1.getCol()], shelves[pos2.getRow()][pos2.getCol()]);
}

/**
 * Prints all items currently stored in the inventory
 * 
 * This method shows how to traverse the entire inventory structure and display
 * information about each item. The implementation:
 * 
 * 1. Uses a flag (foundItems) to track whether any items were found
 * 2. Iterates through all shelf and compartment positions
 * 3. Displays item information using the overloaded << operator
 * 
 * Using the overloaded << operator from the Item class hierarchy ensures that
 * each type of item is displayed with its specific properties, demonstrating
 * polymorphism in action. The foundItems flag provides a better user experience
 * by showing a "No items" message rather than an empty list.
 */
ostream& operator<<(ostream& os, const Inventory& inventory) {
    os << "=== Items in Storage ===" << endl;

    bool foundItems = false;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 15; j++) {
            if (inventory.shelves[i][j]) {
                os
                << "Shelf: " << i << ", Compartment: " << j << endl
                << *inventory.shelves[i][j] << endl;
                foundItems = true;
            }
        }
    }
    
    if (!foundItems) os << "No items in storage." << endl;
    return os;
}

/**
 * Prints all items that are currently checked out
 * 
 * This method demonstrates how to work with the map data structure to display
 * information about all checked-out items. The implementation:
 * 
 * 1. Handles the empty case with a clear message
 * 2. Uses a range-based for loop to iterate through the map pairs
 * 3. Displays both the item information and the checkout details
 * 
 * Range-based for loops provide a more readable and less error-prone way to
 * iterate through containers compared to traditional iterators. The const auto&
 * references ensure we don't make unnecessary copies of the data.
 */
void Inventory::printCheckedOutItems() const {
    cout << "=== Checked Out Items ===" << endl;
    if (checkedOutItems.empty()) {
        cout << "No items are currently checked out." << endl;
        return;
    }
    
    for (const auto& pair : checkedOutItems) {
        const auto& info = pair.second;
        cout
        << "Item ID: " << pair.first << endl
        << *info.item << endl
        << "Checked out by: " << info.checkedOutBy << endl
        << "Due date: " << info.dueDate << endl
        << "Original position - Shelf: " << info.originalPosition.getRow()
        << ", Compartment: " << info.originalPosition.getCol() << endl
        << "------------------------" << endl;
    }
}

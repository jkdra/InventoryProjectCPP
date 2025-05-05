//
// Created by Jawad Khadra on 5/5/25.
//

#ifndef INVENTORY_H
#define INVENTORY_H

#include "Item.h"
#include "Position.h"
#include <map>
#include <memory>

using namespace std;

/**
 * @struct CheckoutInfo
 * @brief Structure to store information about checked out items
 * 
 * This structure maintains all necessary information about a checked out item,
 * including who checked it out, when it's due, where it belongs, and the item itself.
 * I chose to use a struct here since this is primarily a data container with no complex
 * behaviors, making the fields directly accessible to the Inventory class.
 */
struct CheckoutInfo {
    string checkedOutBy;      ///< Name of the person who checked out the item
    string dueDate;           ///< Due date for returning the item
    Position originalPosition; ///< Original shelf and compartment position
    unique_ptr<Item> item;    ///< Smart pointer to the actual item
    
    /**
     * @brief Constructor for CheckoutInfo
     * @param by Person who checked out the item
     * @param due Due date for returning the item
     * @param pos Original position of the item
     * @param i Smart pointer to the item
     * 
     * Using smart pointers (unique_ptr) here ensures that memory is properly managed
     * and that the Item object is automatically deleted when the CheckoutInfo is destroyed.
     */
    CheckoutInfo(string by, string due, Position pos, unique_ptr<Item> i)
        : checkedOutBy(by), dueDate(due), originalPosition(pos), item(move(i)) {}
};

/**
 * @class Inventory
 * @brief Manages the library inventory system
 * 
 * This class represents the entire library inventory system, handling storage
 * of items across shelves and compartments, checkout/checkin operations,
 * and various utility functions. I designed this class to model the physical
 * arrangement of items in a library while providing robust functionality for
 * item management.
 */
class Inventory {
private:
    /**
     * 2D array representing shelves and compartments in the library.
     * The first dimension (3) represents the shelves, and the second dimension (15)
     * represents compartments on each shelf. I used unique_ptr for memory safety
     * and to support polymorphic items (Books, Movies, Magazines).
     */
    unique_ptr<Item> shelves[3][15];
    
    /**
     * Map to track checked out items, using item ID as the key.
     * I chose a map data structure for efficient lookup by item ID, which is
     * critical for the checkout/checkin operations.
     */
    map<string, CheckoutInfo> checkedOutItems;
    
    /**
     * @brief Helper method to convert integer ID to string ID
     * @param id Integer ID of the item
     * @return String representation of the ID
     * 
     * This utility function standardizes ID conversion throughout the code,
     * allowing us to easily change the ID format if needed in the future.
     */
    string getStringId(int id) const;

public:
    /**
     * @brief Default constructor
     * 
     * Initializes all compartments to empty (nullptr).
     */
    Inventory();
    
    /**
     * @brief Destructor
     * 
     * Smart pointers handle memory cleanup automatically.
     */
    ~Inventory();
    
    /**
     * @brief Overloaded [] operator for accessing shelves
     * @param shelfIndex Index of the shelf to access
     * @return Pointer to the array of compartments on the specified shelf
     * @throws out_of_range if shelfIndex is invalid
     * 
     * This operator overloading enables intuitive access to items using the
     * syntax: inventory[shelf][compartment]
     */
    unique_ptr<Item>* operator[](int shelfIndex);
    
    /**
     * @brief Const version of [] operator for read-only access
     * @param shelfIndex Index of the shelf to access
     * @return Const pointer to the array of compartments
     * @throws out_of_range if shelfIndex is invalid
     * 
     * Provides const-correct access to the storage system.
     */
    const unique_ptr<Item>* operator[](int shelfIndex) const;
    
    /**
     * @brief Adds an item to the inventory at a specific position
     * @param position Shelf and compartment position
     * @param item The item to add
     * @throws out_of_range if position is invalid
     * @throws runtime_error if compartment is not empty
     * 
     * Creates a copy of the item and stores it at the specified position.
     * Uses dynamic_cast to preserve the specific item type (Book, Movie, Magazine).
     */
    void addItem(const Position& position, const Item& item);
    
    /**
     * @brief Checks out an item from the inventory
     * @param itemId ID of the item to check out
     * @param checkOutBy Name of the person checking out the item
     * @return Pointer to the checked-out item
     * @throws runtime_error if item is not found
     * 
     * Moves the item from the shelf to the checkedOutItems map and
     * records checkout information including the due date.
     */
    Item* checkoutItem(const string& itemId, const string& checkOutBy);
    
    /**
     * @brief Checks in a previously checked-out item
     * @param item Item to check in (only ID is used)
     * @throws runtime_error if item is not checked out
     * 
     * Returns the item to its original position on the shelf and
     * removes the checkout record.
     */
    void checkinItem(const Item& item);
    
    /**
     * @brief Swaps the positions of two items in the inventory
     * @param pos1 Position of the first item
     * @param pos2 Position of the second item
     * @throws out_of_range if either position is invalid
     * @throws runtime_error if either compartment is empty
     * 
     * Exchanges the items at the two specified positions.
     */
    void swapItems(const Position& pos1, const Position& pos2);
    
    /**
     * @brief Prints all items currently stored in the inventory
     * 
     * Displays shelf and compartment locations along with item details.
     */
    friend ostream& operator<<(ostream& os, const Inventory& inventory);


    /**
     * @brief Prints all items that are currently checked out
     * 
     * Displays checkout information including who checked out the item,
     * when it's due, and the item details.
     */
    void printCheckedOutItems() const;
    
    /**
     * @brief Checks if a compartment is empty
     * @param pos Position to check
     * @return true if compartment is empty, false otherwise
     * @throws out_of_range if position is invalid
     * 
     * Helper method used by various functions to verify compartment state.
     */
    bool isCompartmentEmpty(const Position& pos) const;
    
    /**
     * @brief Checks if an item is currently checked out
     * @param itemId ID of the item to check
     * @return true if item is checked out, false otherwise
     * 
     * Helper method used to verify item checkout status.
     */
    bool isItemCheckedOut(const string& itemId) const;
};

#endif //INVENTORY_H

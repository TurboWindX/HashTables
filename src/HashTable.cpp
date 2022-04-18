//============================================================================
// Name        : HashTable.cpp
// Author      : John Watson
// Version     : 1.0
// Copyright   : Copyright © 2017 SNHU COCE
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <algorithm>
#include <climits>
#include <iostream>
#include <string> // atoi
#include <time.h>
#include <vector>

#include "CSVparser.hpp"

using namespace std;

//============================================================================
// Global definitions visible to all methods and classes
//============================================================================

const unsigned int DEFAULT_SIZE = 179;

// forward declarations
double strToDouble(string str);

// define a structure to hold bid information
struct Bid {
    string bidId; // unique identifier
    string title;
    string fund;
    double amount = 0.00;
};

//============================================================================
// Hash Table class definition
//============================================================================

/**
 * Define a class containing data members and methods to
 * implement a hash table with chaining.
 */
class HashTable {

private:
    // Define structures to hold bids
    struct Node {
        Bid bid;
        unsigned int key;
        Node *next;

        // default constructor
        Node() {
            key = UINT_MAX;
            next = nullptr;
        }

        // initialize with a bid
        Node(Bid aBid) : Node() {
            bid = aBid;
        }

        // initialize with a bid and a key
        Node(Bid aBid, unsigned int aKey) : Node(aBid) {
            key = aKey;
        }
    };

    unsigned int tableSize = DEFAULT_SIZE;
    vector<Node>* nodes;

    unsigned int Hash(int key);

public:
    HashTable();
    HashTable(unsigned int size);
    virtual ~HashTable();
    void Insert(Bid bid);
    void PrintAll();
    void Remove(string bidId);
    Bid Search(string bidId);
};

/**
 * Default constructor
 */
HashTable::HashTable() {
    // FIXME (1): Initialize the structures used to hold bids
    // Initalize node structure by resizing tableSize
    //FIXED
    nodes = new vector<Node>[DEFAULT_SIZE];
    //nodes->resize(179);
}

/**
 * Constructor for specifying size of the table
 * Use to improve efficiency of hashing algorithm
 * by reducing collisions without wasting memory.
 */
HashTable::HashTable(unsigned int size) {
    // invoke local tableSize to size with this->
    // resize nodes size
    nodes = new vector<Node>[size];
}


/**
 * Destructor
 */
HashTable::~HashTable() {
    // FIXME (2): Implement logic to free storage when class is destroyed
    // erase nodes beginning
    //FIXED
    //free(nodes);
}



/**
 * Calculate the hash value of a given key.
 * Note that key is specifically defined as
 * unsigned int to prevent undefined results
 * of a negative list index.
 *
 * @param key The key to hash
 * @return The calculated hash
 */
unsigned int HashTable::Hash(int key) {
    // FIXME (3): Implement logic to calculate a hash value
    // return key tableSize
    //FIXED
    unsigned bucketIndex;
    bucketIndex = key % tableSize;
    return bucketIndex;
}

/**
 * Insert a bid
 * @param bid The bid to insert
 */
void HashTable::Insert(Bid bid) {
    int theBidId = atoi(bid.bidId.c_str());

    nodes[Hash(theBidId)].push_back(Node(bid, Hash(theBidId)));
    
}

/**
 * Print all bids
 */
void HashTable::PrintAll() {

    for (int i = 0; i < tableSize; ++i) {
        if (!nodes[i].empty()) {
            cout << "index " << i << ": " << endl;
            cout << "Bid Id      |      Title      |       Fund      |      Amount       " << endl;
            cout << "----------------------------------------------------------------" << endl;
            for (Node n : nodes[i]) {
                cout << n.bid.bidId << "  |  " << n.bid.title << "  |  " << n.bid.fund << "  |  " << n.bid.amount << endl;
                cout << "----------------------------------------------------------------" << endl;
            }
            cout << endl;
        }
    }
    
}

/**
 * Remove a bid
 *
 * @param bidId The bid id to search for
 */
void HashTable::Remove(string bidId) {
    // FIXME (7): Implement logic to remove a bid
    // set key equal to hash atoi bidID cstring
    // erase node begin and key

    int theBidId = atoi(bidId.c_str());
    unsigned key = Hash(theBidId);
    for (Node n : nodes[key]) {
        int i = 0;
        if (n.bid.bidId == bidId) {
            //found the bid
            nodes[key].erase(nodes[key].begin() + i);
        }
        i++;
    }
}

/**
 * Search for the specified bidId
 *
 * @param bidId The bid id to search for
 */
Bid HashTable::Search(string bidId) {
    Bid bid;
    int theBidId = atoi(bidId.c_str());
    unsigned key = Hash(theBidId);
    for (Node n : nodes[key]) {
        if (n.bid.bidId == bidId) {
            //found the bid
            bid = n.bid;
        }
    }
        
    return bid;
}

//============================================================================
// Static methods used for testing
//============================================================================

/**
 * Display the bid information to the console (std::out)
 *
 * @param bid struct containing the bid info
 */
void displayBid(Bid bid) {
    cout << bid.bidId << ": " << bid.title << " | " << bid.amount << " | "
            << bid.fund << endl;
    return;
}

/**
 * Load a CSV file containing bids into a container
 *
 * @param csvPath the path to the CSV file to load
 * @return a container holding all the bids read
 */
void loadBids(string csvPath, HashTable bidTable) {
    cout << "Loading CSV file " << csvPath << endl;

    // initialize the CSV Parser using the given path
    csv::Parser file = csv::Parser(csvPath);
    int co = 0;
    vector<string> header = file.getHeader();
    for (auto const& c : header) {
        cout << c << " | ";
        co++;
    }


    switch (co) {
    case(9):
        try {
            // loop to read rows of a CSV file
            for (unsigned int i = 0; i < file.rowCount(); i++) {

                // Create a data structure and add to the collection of bids
                Bid bid;
                bid.bidId = file[i][1];
                bid.title = file[i][0];
                bid.fund = file[i][8];
                bid.amount = strToDouble(file[i][4]);

                //cout << "Item: " << bid.title << ", Fund: " << bid.fund << ", Amount: " << bid.amount << endl;

                // push this bid to the end
                bidTable.Insert(bid);
            }
        }
        catch (csv::Error& e) {
            std::cerr << e.what() << std::endl;
        }
        break;
    case(20):
        try {
            // loop to read rows of a CSV file
            for (unsigned int i = 0; i < file.rowCount(); i++) {

                // Create a data structure and add to the collection of bids
                Bid bid;
                bid.bidId = file[i][1];
                bid.title = file[i][0];
                bid.fund = file[i][19];
                bid.amount = strToDouble(file[i][4]);

                //cout << "Item: " << bid.title << ", Fund: " << bid.fund << ", Amount: " << bid.amount << endl;

                // push this bid to the end
                bidTable.Insert(bid);
            }
        }
        catch (csv::Error& e) {
            std::cerr << e.what() << std::endl;
        }
        break;
    default:
        break;
    }
}

/**
 * Simple C function to convert a string to a double
 * after stripping out unwanted char
 *
 * credit: http://stackoverflow.com/a/24875936
 *
 * @param ch The character to strip out
 */
double strToDouble(string str) {
    str.erase(remove(str.begin(), str.end(), '$'), str.end());
    return atof(str.c_str());
}

/**
 * The one and only main() method
 */
int main(int argc, char* argv[]) {

    // process command line arguments
    string csvPath, bidKey;
    switch (argc) {
    case 2:
        csvPath = argv[1];
        bidKey = "98269";
        break;
    case 3:
        csvPath = argv[1];
        bidKey = argv[2];
        break;
    default:
        csvPath = "./res/eBid_Monthly_Sales_Dec_2016.csv";
        bidKey = "98269";
    }

    // Define a timer variable
    clock_t ticks;

    // Define a hash table to hold all the bids
    HashTable bidTable = HashTable(); //FIXED: Potentially uninintinzall local pointer var

    Bid bid;

    int choice = 0;
    while (choice != 9) {
        cout << "Menu:" << endl;
        cout << "  1. Load Bids" << endl;
        cout << "  2. Display All Bids" << endl;
        cout << "  3. Find Bid" << endl;
        cout << "  4. Remove Bid" << endl;
        cout << "  9. Exit" << endl;
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {

        case 1:
            //bidTable = HashTable();

            // Initialize a timer variable before loading bids
            ticks = clock();

            // Complete the method call to load the bids
            loadBids(csvPath, bidTable);

            // Calculate elapsed time and display result
            ticks = clock() - ticks; // current clock ticks minus starting clock ticks
            cout << "time: " << ticks << " clock ticks" << endl;
            cout << "time: " << ticks * 1.0 / CLOCKS_PER_SEC << " seconds" << endl;
            break;

        case 2:
            bidTable.PrintAll();
            break;

        case 3:
            ticks = clock();

            bid = bidTable.Search(bidKey);

            ticks = clock() - ticks; // current clock ticks minus starting clock ticks

            if (!bid.bidId.empty()) {
                displayBid(bid);
            } else {
                cout << "Bid Id " << bidKey << " not found." << endl;
            }

            cout << "time: " << ticks << " clock ticks" << endl;
            cout << "time: " << ticks * 1.0 / CLOCKS_PER_SEC << " seconds" << endl;
            break;

        case 4:
            
            bidTable.Remove(bidKey);
            break;
        }
    }

    cout << "Good bye." << endl;

    return 0;
}

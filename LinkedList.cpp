#include "LinkedList.h"
#include <sstream>
#include <iostream>

// Constructor
LinkedList::LinkedList() : head(nullptr), tail(nullptr), size(0) {}

// Destructor
LinkedList::~LinkedList() 
{
    clear();
}

// Copy constructor
LinkedList::LinkedList(const LinkedList& other)
    : head(nullptr), tail(nullptr), size(0) 
{
  Node* current = other.head;
  while (current != nullptr) 
    {
      addBack(current->getTile());
      current = current->getNext();
    }
}

// Copy assignment operator
LinkedList& LinkedList::operator=(const LinkedList& other) 
{
  if (this != &other) {
    clear();
    Node* current = other.head;
    while (current != nullptr) {
      addBack(current->getTile());
      current = current->getNext();
    }
  }
  return *this;
}

// Move constructor
LinkedList::LinkedList(LinkedList&& other)
    : head(other.head), tail(other.tail), size(other.size) {
  // Reset the other list to a safe empty state.
  other.head = nullptr;
  other.tail = nullptr;
  other.size = 0;
}

// Move assignment operator
LinkedList& LinkedList::operator=(LinkedList&& other) {
  if (this != &other) {
    // Clean up the current state
    clear();    
    head = other.head;
    tail = other.tail;
    size = other.size;    
    other.head = nullptr;
    other.tail = nullptr;
    other.size = 0;
  }
  return *this;
}

// Adds a tile node at the end of the list.
void LinkedList::addBack(Tile* tile) {
    Node* newNode = new Node(tile);
    newNode->setNext(nullptr);
    if (head == nullptr) {
        head = newNode;
        tail = newNode;
    } else {
        tail->setNext(newNode);
        tail = newNode;
    }
    size++;
}

// Adds a tile node at the front of the list.
void LinkedList::addFront(Tile* tile) {
    Node* newNode = new Node(tile);
    newNode->setNext(head);
    head = newNode;
    if (tail == nullptr) {
        tail = newNode;
    }
    size++;
}

// Removes the tile node from the list and returns the removed tile.
Tile* LinkedList::remove(Tile* tile) {
    Node* current = head;
    Node* previous = nullptr;

    while (current != nullptr && *(current->getTile()) != *tile) {
        previous = current;
        current = current->getNext();
    }

    if (current == nullptr) {
        return nullptr;
    }

    if (previous == nullptr) {
        head = current->getNext();
    } else {
        previous->setNext(current->getNext());
    }

    if (current == tail) {
        tail = previous;
    }

    Tile* removedTile = current->getTile();
    current->setTile(nullptr);
    delete current;

    size--;

    return removedTile;
}

// Removes and returns the first node's tile from the list.
Tile* LinkedList::removeFront() {
    if (head == nullptr) {
        return nullptr;
    }

    Node* oldHead = head;
    head = head->getNext();
    if (head == nullptr) {
        tail = nullptr;
    }

    Tile* removedTile = oldHead->getTile();
    oldHead->setTile(nullptr);
    delete oldHead;

    size--;

    return removedTile;
}

// Removes and returns the last node's tile from the list.
Tile* LinkedList::removeEnd() {
    if (head == nullptr) {
        throw std::underflow_error("List is empty");
    }

    if (head == tail) {
        Tile* removedTile = head->getTile();
        delete head;
        head = nullptr;
        tail = nullptr;
        size--;
        return removedTile;
    }

    Node* current = head;
    while (current->getNext() != tail) {
        current = current->getNext();
    }

    Tile* removedTile = tail->getTile();
    delete tail;
    tail = current;
    tail->setNext(nullptr);

    size--;

    return removedTile;
}

// Clears the entire list by deleting all nodes.
void LinkedList::clear() {
    Node* current = head;
    while (current != nullptr) {
        Node* next = current->getNext();
        delete current;
        current = next;
    }
    head = nullptr;
    tail = nullptr;
    size = 0;
}

// Returns the head node of the list.
Node* LinkedList::getHead() const {
    return head;
}

// Returns the current size of the list.
int LinkedList::getLength() const {
    return size;
}

// Converts the list to a string.
std::string LinkedList::toString() const {
    std::ostringstream oss;
    Node* current = head;
    while (current != nullptr) {
        oss << *(current->getTile());
        if (current->getNext() != nullptr) {
            oss << ", ";
        }
        current = current->getNext();
    }
    return oss.str();
}

// Returns true if the list is empty, false otherwise.
bool LinkedList::isEmpty() const {
    return head == nullptr;
}

// Retrieves the tile at a given index in the list.
Tile* LinkedList::get(int index) const {
  if (index < 0 || index >= size) {
    throw std::out_of_range("Index out of range");
  }
  Node* current = head;
  for (int i = 0; i < index; ++i) {
    current = current->getNext();
  }
  return current->getTile();
}

// Removes the first node in the list (alias for removeFront).
void LinkedList::deleteFront() { removeFront(); }

// Removes the last node in the list (alias for removeEnd).
void LinkedList::deleteBack() { removeEnd(); }

// Inserts a node containing 'tile' at a specific index in the list.
void LinkedList::addAt(Tile* tile, int index) {
  if (index < 0 || index > size) {
    throw std::out_of_range("Index out of range");
  }

  if (index == 0) {
    addFront(tile);
    return;
  }

  if (index == size) {
    addBack(tile);
    return;
  }

  Node* newNode = new Node(tile);
  Node* current = head;
  for (int i = 0; i < index - 1; ++i) {
    current = current->getNext();
  }
  newNode->setNext(current->getNext());
  current->setNext(newNode);
  size++; // Increase the size of the list.
}

// Deletes the node at a specific index in the list.
void LinkedList::deleteAt(int index) {
  if (index < 0 || index >= size) {
    throw std::out_of_range("Index out of range");
  }

  if (index == 0) {
    removeFront();
    return;
  }

  if (index == size - 1) {
    removeEnd();
    return;
  }

  Node* current = head;
  for (int i = 0; i < index - 1; ++i) {
    current = current->getNext();
  }
  Node* toDelete = current->getNext();
  current->setNext(toDelete->getNext());
  toDelete->setTile(nullptr);
  delete toDelete;
  size--;
}
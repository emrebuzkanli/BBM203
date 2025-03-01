#include "Stack.h"
#include <iostream>

// Constructor to initialize an empty stack
Stack::Stack() {
    // TODO: Your code here
    top = -1;

}

// Adds a province to the top of the stack
void Stack::push(int province) {
    // TODO: Your code here
    if (top >= MAX_SIZE - 1) {
        return;
    }
    top =top+1;
    data[top] = province;
}

// Removes and returns the top province from the stack
int Stack::pop() {
    // TODO: Your code here
    if (isEmpty()) {
        return -1;
    }
    int result = data[top];
    top = top - 1;
    return result;
}

// Returns the top province without removing it
int Stack::peek() const {
    // TODO: Your code here
    if (isEmpty()) {
        return -1;
    }
    int result = data[top];
    return result;

}

// Checks if the stack is empty
bool Stack::isEmpty() const {
    // TODO: Your code here
    if (top==-1) {
        return true;
    }
    return false;
}

// Function to get the current size of the stack
int Stack::getSize() const {
    // TODO: Your code here
    int size = top + 1;
    return size;
}

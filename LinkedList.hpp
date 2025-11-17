#ifndef _LINKED_LIST_HPP_
#define _LINKED_LIST_HPP_

#include <stdexcept>

template <typename T>
class LinkedList {
private:
    struct Node {
        Node* next;
        T data;

        Node(T d, Node* n) : next(n), data(d) {}
    }* head_;

public:
    class Iterator {
    private:
        Node* current_;

    public:
        Iterator(Node* head) : current_(head) {}

        T* current() {
            if (current_ == nullptr) {
                return nullptr;
            }
            return &current_->data;
        }

        void next() {
            if (current_ != nullptr) {
                current_ = current_->next;
            }
        }

        bool hasNext() {
            return current_ != nullptr;
        }

        bool operator!=(const Iterator& other) const {
            return current_ != other.current_;
        }

        T& operator*() const {
            return current_->data;
        }

        Iterator& operator++() {
            current_ = current_->next;
            return *this;
        }
    };

    LinkedList() : head_(nullptr) {}

    bool empty() const {
        return head_ == nullptr;
    }

    size_t size() const {
        size_t count = 0;
        Node* iter = head_;
        while (iter != nullptr) {
            iter = iter->next;
            ++count;
        }
        return count;
    }

    void pushFront(T value) {
        head_ = new Node(value, head_);
    }

    void pushBack(T value) {
        Node* newNode = new Node(value, nullptr);
        if (empty()) {
            head_ = newNode;
        } else {
            Node* current = head_;
            while (current->next != nullptr) {
                current = current->next;
            }
            current->next = newNode;
        }
    }

    T at(size_t index) const {
        if (index >= size()) {
            throw std::out_of_range("Index is out of range");
        }
        Node* current = head_;
        for (size_t i = 0; i < index; i++) {
            current = current->next;
        }
        return current->data;
    }

    T popFront() {
        if (empty()) {
            throw std::logic_error("List is empty");
        }
        Node* temp = head_;
        head_ = head_->next;
        T value = temp->data;
        delete temp;
        return value;
    }

    bool removeAt(size_t index) {
        if (index >= size() || empty()) {
            return false; // Index out of bounds or empty list
        }

        if (index == 0) {
            // Removing the head
            Node* temp = head_;
            head_ = head_->next;
            delete temp;
            return true;
        }

        Node* current = head_;
        for (size_t i = 0; i < index - 1; i++) {
            current = current->next;
        }

        Node* temp = current->next;
        current->next = temp->next;
        delete temp;

        return true;
    }

    void clear() {
        while (!empty()) {
            popFront();
        }
    }

    ~LinkedList() {
        clear();
    }

    Iterator begin() const {
        return Iterator(head_);
    }

    Iterator end() const {
        return Iterator(nullptr);
    }

    Iterator generateIterator() const {
        return Iterator(head_);
    }
};

#endif

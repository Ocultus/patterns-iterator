#ifndef LAB4_LINKEDLIST_H
#define LAB4_LINKEDLIST_H

#include <iterator>
#include "LinkedListException.h"

template <typename T>
class LinkedList {
public:
    struct Node{
        T data;
        Node* next;
    };
    using PNode = Node*;
    LinkedList();
    void add(T elem);
    void deleteElem(T elem);
    bool empty();
    void clear() noexcept;
    ~LinkedList();
    LinkedList(const LinkedList<T> &obj);
    LinkedList(LinkedList<T> &&obj) noexcept;
    LinkedList& operator=(const LinkedList<T>& obj);
    LinkedList& operator=(LinkedList<T>&& obj) noexcept;
    bool operator==(const LinkedList<T> &obj);
    bool operator!=(const LinkedList<T> &obj);
    class listIterator: public std::iterator<std::input_iterator_tag, T>{
        private:
            PNode current;
        public:
            listIterator();
            explicit listIterator(const LinkedList<T> &list);
            listIterator(const listIterator &other);
            listIterator &operator=(const listIterator& other);
            listIterator &operator++();
            listIterator operator++(int);
            bool hasNext();
            bool operator==(const listIterator &it) const;
            bool operator!=(const listIterator &it) const;
            void setNullptr();
            T operator*() const;
            friend class LinkedList<T>;
            ~listIterator() = default;
        };
    listIterator begin(){ return listIterator(*this); };
    listIterator end(){ return listIterator();}
    PNode head;
private:

};

template<typename T>
LinkedList<T>::LinkedList(): head(nullptr) {}

template<typename T>
void LinkedList<T>::add(T elem) {
    Node *cell = new Node;
    cell -> data = elem;
    empty() ? cell->next = nullptr : cell->next = head;
    head = cell;
}

template<typename T>
void LinkedList<T>::deleteElem(T elem) {
    PNode tempHead = head;
    if(tempHead -> data == elem){
        head = head ->next;
        delete tempHead;
    }
    bool notFound = true;
    while(tempHead -> next != nullptr && notFound){
        PNode temp = tempHead;
        tempHead = tempHead -> next;
        if(tempHead -> data == elem){
            temp -> next  = tempHead ->next;
            delete tempHead;
            notFound = false;
        }
    }
}

template<typename T>
bool LinkedList<T>::empty() {
    return head == nullptr;
}

template<typename T>
void LinkedList<T>::clear() noexcept {
    if(!empty()){
        PNode it = head->next;
        while(it != nullptr){
            delete head;
            head = it;
            it = head -> next;
        }
        head = nullptr;
    }
}

template<typename T>
LinkedList<T>::~LinkedList() {
    clear();
    delete head;
}

template<typename T>
LinkedList<T>::LinkedList(const LinkedList<T> &obj):head(new Node) {
    head = new Node;
    head->data = obj.head->data;
    if(obj.head->next != nullptr){
        PNode it = head;
        PNode oldNode = obj.head->next;
        while(oldNode != nullptr){
            PNode cell = new Node;
            it->next = cell;
            cell->data = oldNode->data;
            oldNode = oldNode -> next;
            it = it ->next;
        }
        it->next = nullptr;
    }
    else{
        head->next == nullptr;
    }
}

template<typename T>
LinkedList<T>::LinkedList(LinkedList<T> &&obj) noexcept:head(obj.head){
    obj.head = nullptr;
}

template<typename T>
LinkedList<T> &LinkedList<T>::operator=(const LinkedList<T>& obj) {
    if(this != obj){
        delete head;
        head = new Node;
        head->data = obj.head->data;
        if(obj.head->next != nullptr){
            PNode it = head;
            PNode oldNode = obj.head->next;
            while(oldNode != nullptr){
                PNode cell = new Node;
                it->next = cell;
                cell->data = oldNode->data;
                oldNode = oldNode -> next;
                it = it ->next;
            }
            it->next = nullptr;
        }
        else{
            head->next == nullptr;
        }
    }
    return *this;
}

template<typename T>
LinkedList<T> &LinkedList<T>::operator=(LinkedList<T>&& obj) noexcept {
    if(*this != obj){
        std::cout << 1;
        delete head;
        head = obj.head;
        obj.head = nullptr;
    }
    return *this;
}

template<typename T>
bool LinkedList<T>::operator==(const LinkedList<T> &obj) {
    PNode node1 = head;
    PNode node2 = obj.head;
    while(node1 != nullptr && node2 != nullptr){
        if(node1->data != node2->data || node1->next != node2-> next){
            return false;
        }
        node1 = node1 -> next;
        node2 = node2 -> next;
    }
    return true;
}

template<typename T>
bool LinkedList<T>::operator!=(const LinkedList<T> &obj) {
    return !(*this == obj);
}


template<typename T>
LinkedList<T>::listIterator::listIterator(const LinkedList<T> &list) {
    current = list.head;
}

template<typename T>
typename LinkedList<T>::listIterator &LinkedList<T>::listIterator::operator++() {
    this->hasNext() ? current = current -> next : current = nullptr;
    return (*this);
}

template<typename T>
T LinkedList<T>::listIterator::operator*() const {
    return current->data;
}

template<typename T>
bool LinkedList<T>::listIterator::operator==(const LinkedList::listIterator &it) const {
    if(it.current == nullptr && this->current == nullptr){ return true;}
    else if(it.current == nullptr || this->current == nullptr){ return false;}
    else{return this->current->data == it.current->data && this->current->next == it.current->next;}
}

template<typename T>
bool LinkedList<T>::listIterator::operator!=(const LinkedList::listIterator &it) const {
    return !(*this == it);
}

template<typename T>
typename LinkedList<T>::listIterator &LinkedList<T>::listIterator::operator=(const LinkedList::listIterator &other) {
    this->current = other.current;
    return (*this);
}

template<typename T>
LinkedList<T>::listIterator::listIterator(): current(nullptr){}

template<typename T>
LinkedList<T>::listIterator::listIterator(const LinkedList::listIterator &other): current(other.current){}

template<typename T>
bool LinkedList<T>::listIterator::hasNext() {
    return current -> next != nullptr;
}

template<typename T>
void LinkedList<T>::listIterator::setNullptr() {
    current = nullptr;
}

template<typename T>
typename LinkedList<T>::listIterator LinkedList<T>::listIterator::operator++(int) {
    const listIterator old = *this;
    ++(*this);
    return old;
}

#endif //LAB4_LINKEDLIST_H

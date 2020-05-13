#ifndef LAB4_HASHTABLE_HPP
#define LAB4_HASHTABLE_HPP

#include "LinkedList.hpp"

using namespace std;

template <typename Value, class Hash = hash<Value>>
class HashTable {
    public:
        using _pair = pair<Value,size_t>;
        using _bucket = LinkedList<_pair>;
        HashTable();
        explicit HashTable(unsigned size);
        void add(Value elem);
        void deleteElem(Value elem);
        bool hasElem(Value elem);
        void clear();
        bool empty();
        HashTable(const HashTable<Value, Hash>&);
        HashTable(HashTable<Value, Hash>&&) noexcept ;
        HashTable& operator= (const HashTable<Value, Hash>&);
        HashTable& operator= (HashTable<Value,Hash>&&) noexcept ;
        ~HashTable();
        class tableIterator: public std::iterator<std::input_iterator_tag, Value>{
            private:
                unsigned writeIndex;
                typename LinkedList<_pair>::listIterator current;
                HashTable<Value> *_hashTable;
            public:
                tableIterator();
                explicit tableIterator(HashTable<Value> &hashTable);
                tableIterator(const tableIterator &other);
                tableIterator& operator=(const tableIterator& other);
                tableIterator& operator++();
                tableIterator operator++(int);
                bool operator==(const tableIterator &it) const;
                bool operator!=(const tableIterator &it) const;
                Value operator*() const;
                friend class HashTable<Value>;
                ~tableIterator() = default;
        };
        tableIterator begin(){return tableIterator(*this);};
        tableIterator end(){return tableIterator();};
    private:
        unsigned int size;
        unsigned int realElem;
        Hash hashFunc;
        _bucket *arr;
};

template<typename Value, class Hash>
HashTable<Value, Hash>::tableIterator::tableIterator(HashTable<Value> &hashTable) {
    bool notFound = true;
    for(unsigned i = 0; hashTable.size && notFound; i++){
        if(!hashTable.arr[i].empty()){
            writeIndex = i;
            current = hashTable.arr[i].begin();
            _hashTable = (&hashTable);
            notFound = false;
        }
    }
}

template<typename Value, class Hash>
typename HashTable<Value, Hash>::tableIterator &HashTable<Value, Hash>::tableIterator::operator++() {
    if(current.hasNext()){
        ++current;
    }
    else{
        bool notFound = true;
        for(unsigned i = writeIndex + 1; i < _hashTable->size && notFound; i++){
            if(!(_hashTable->arr[i].empty())){
                writeIndex = i;
                current = _hashTable->arr[i].begin();
                notFound = false;
            }
        }
        if(notFound){
            _hashTable = nullptr;
            current.setNullptr();
        }
    }
    return (*this);
}

template<typename Value, class Hash>
bool HashTable<Value, Hash>::tableIterator::operator==(const HashTable::tableIterator &it) const {
    if(_hashTable == nullptr && it._hashTable == nullptr){
        return true;
    }
    else if (_hashTable == nullptr || it._hashTable == nullptr){
        return false;
    }
    return writeIndex == it.writeIndex && current == it.current && _hashTable == it._hashTable;
}

template<typename Value, class Hash>
bool HashTable<Value, Hash>::tableIterator::operator!=(const HashTable::tableIterator &it) const {
    return !(*this == it);
}

template<typename Value, class Hash>
Value HashTable<Value, Hash>::tableIterator::operator*() const {
    return (*current).first;
}

template<typename Value, class Hash>
typename HashTable<Value, Hash>::tableIterator HashTable<Value, Hash>::tableIterator::operator++(int) {
    const tableIterator old = (*this);
    ++(*this);
    return old;
}

template<typename Value, class Hash>
typename HashTable<Value, Hash>::tableIterator &HashTable<Value, Hash>::tableIterator::operator=(const HashTable::tableIterator &other) {
    this->current = other.current;
    return (*this);
}

template<typename Value, class Hash>
HashTable<Value, Hash>::tableIterator::tableIterator(const HashTable::tableIterator &other)
:current(other.current), writeIndex(other.writeIndex), _hashTable(other._hashTable){}

template<typename Value, class Hash>
HashTable<Value, Hash>::tableIterator::tableIterator(): _hashTable(nullptr),writeIndex{}{}

template<typename Value, class Hash>
HashTable<Value, Hash>::HashTable(unsigned _size): arr(new _bucket[_size]), realElem(0), size(_size) {}

template<typename Value, class Hash>
void HashTable<Value, Hash>::add(Value elem) {
    size_t hashElem = hashFunc(elem);
    arr[hashElem % size].add(_pair(elem, hashElem));
    realElem++;
    float capacity = 100. * realElem / size;
    if(capacity > 69){
        _bucket *rehashTable = new _bucket[size * 2];
        for(Value temp:*this){
            hashElem = hashFunc(temp);
            rehashTable[hashElem % (size*2)].add(_pair(temp, hashElem));
        }
        size = size * 2;
        _bucket *old = arr;
        arr = rehashTable;
        delete[] old;
    }

}

template<typename Value, class Hash>
bool HashTable<Value, Hash>::empty() {
    return realElem > 0;
}

template<typename Value, class Hash>
bool HashTable<Value, Hash>::hasElem(Value elem) {
    size_t elemHash = hashFunc(elem);
    if(arr[elemHash % size].empty()){
        return false;
    }
    else{
        for(_pair temp: arr[elemHash % size]){
            if(elem == temp.first){
                return true;
            }
        }
        return false;
    }
}

template<typename Value, class Hash>
void HashTable<Value, Hash>::deleteElem(Value elem) {
    size_t hashElem = hashFunc(elem);
    if(arr[hashElem % size].empty()){
        throw LinkedListException("");
    }
    else{
        arr[hashElem % size].deleteElem(_pair(elem, hashElem));
        realElem--;
    }
}

template<typename Value, class Hash>
void HashTable<Value, Hash>::clear() {
    for(unsigned i = 0; i < size; i++){
        if(!arr[i].empty()){
            arr[i].clear();
        }
    }
}

template<typename Value, class Hash>
HashTable<Value, Hash>::~HashTable() {
    clear();
    delete [] arr;
}

template<typename Value, class Hash>
HashTable<Value, Hash>::HashTable() : arr(new _bucket[10]), realElem(0), size(10){}

template<typename Value, class Hash>
HashTable<Value, Hash>::HashTable(const HashTable<Value, Hash>& other):realElem(other.realElem), size(other.size),
arr(new _bucket[other.size]){
    for(unsigned i = 0; i < size; i++){
        arr[i] = other[i];
    }
}

template<typename Value, class Hash>
HashTable<Value, Hash>::HashTable(HashTable<Value, Hash>&& other) noexcept:arr(other.arr),
size(other.size), realElem(other.size){
    other.arr = nullptr;
    other.size = 0;
    other.realElem = 0;
}

template<typename Value, class Hash>
HashTable<Value, Hash> &HashTable<Value, Hash>::operator=(HashTable<Value, Hash>&& other) noexcept{
    if(this != other){
        delete [] arr;
        arr = other.arr;
        size = other.size;
        realElem = other.realElem;
        other.arr = nullptr;
        other.size = 0;
        other.realElem = 0;
    }
    return *this;
}

template<typename Value, class Hash>
HashTable<Value, Hash> &HashTable<Value, Hash>::operator=(const HashTable<Value, Hash>& other) {
    if(this != other){
        delete[] arr;
        size = other.size;
        realElem = other.realElem;
        arr = new _bucket[size];
        for(unsigned i = 0; i < size; i++){
            arr[i] = other[i];
        }
    }
    return *this;
}


#endif //LAB4_HASHTABLE_HPP

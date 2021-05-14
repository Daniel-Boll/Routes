#include <cstdint>
#include <functional>
#include <iostream>
#include <string>

template <typename T> 
class DLinkedList {
  private:
    // Auxiliar functions
    T lowerHalf(T data, int32_t index);
    T upperHalf(T data, int32_t index);

    inline bool equal(T a, T b) {
      return a == b;
    }
  public:
    // TODO: Extract node from here.
    // prob at /usr/local/include.
    // downside: node<T> everywhere
    class node {
      private:
        node *next;
        node *prev;
        T info;

        // Now sentinel can access
        // private members of node
        friend class sentinel;
      public:
        node() : next(nullptr), prev(nullptr) {}        

        // TODO: Apply lambda to all functions
       
        /* std::function<void(T data)> setData = [&](T data) mutable -> void {this->info = data;}; */
        /* std::function<void(T data)> setNext = [&](node *ptr) mutable -> void {this->next = ptr;}; */
        /* std::function<void(T data)> setPrev = [&](node *ptr) mutable -> void {this->next = ptr;}; */

        inline void setData(T data)    { this->info = data; }
        inline void setNext(node *ptr) { this->next = ptr; }
        inline void setPrev(node *ptr) { this->prev = ptr; }

        /* std::function<T()> getData = [=]() -> T { return this->info; }; */

        inline T getData()     const { return this->info; }
        inline node *getNext() const { return this->next; }
        inline node *getPrev() const { return this->prev; }
       
        /* ~node(); */
    };

    class sentinel : public node {
      private:
        int32_t size;
      public:
        sentinel() : size(0) {}

        inline void increment() { this->size++; }
        inline void decrement() { this->size--; }
        inline int32_t getSize() const { return this->size; }

        inline node *getFirst() const { return this->getPrev(); }
        inline node *getLast() const { return this->getNext(); }

        inline void setFirst(node* ptr) { this->setPrev(ptr); }
        inline void setLast(node* ptr) { this->setNext(ptr); }
    };

    sentinel *sent;
    DLinkedList() : sent(new sentinel()) {} 

    inline bool emptyList() { return sent->getSize() == 0; } 

    node* position(T data);
    node* contains(T data);

    T add(T data);
    T unshift(T data);
    T addAt(T data, int32_t index);

    T remove();
    T shift();
    T removeAt(int32_t index);
    T removeSpecific(T data);

    inline sentinel getSentinel() { return *(this->sent); };

    void show();
    
    ~DLinkedList();
};

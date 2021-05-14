#include "DLinkedList.hpp"

#include <vector>
#include <cmath>
#include <iostream>

template <typename T>
T DLinkedList<T>::add(T data) {
  node *aux = new node();
  
  aux->setPrev(DLinkedList::sent->getLast());
  aux->setNext(nullptr);
  aux->setData(data);
  if(emptyList()) {
    DLinkedList::sent->setFirst(aux);
  } else {
    DLinkedList::sent->getLast()->setNext(aux);
  }

  DLinkedList::sent->setLast(aux);
  DLinkedList::sent->increment();
  return data;
}

template <typename T>
T DLinkedList<T>::unshift(T data) {
  node *aux = new node();
  
  aux->setPrev(nullptr);
  aux->setNext(DLinkedList::sent->getFirst());
  aux->setData(data);
  if(emptyList()) {
    DLinkedList::sent->setNext(aux);
  } else {
    DLinkedList::sent->getFirst()->setPrev(aux);
  }

  DLinkedList::sent->setFirst(aux);
  DLinkedList::sent->increment();
  return data;
}

template <typename T>
T DLinkedList<T>::addAt(T data, int32_t index) {
  node *aux = new node();
  aux->setData(data);

  int32_t currentSize =  DLinkedList::sent->getSize(); 
 
  // Refactor better than IFs
  if(index == 0) {
    return unshift(data);
  } else if(index >= currentSize) {
    return add(data);
  } else if(index <= (currentSize/2)){
    return lowerHalf(data, index);
  } else if(index > (currentSize/2)) {
    return upperHalf(data, index);
  } else {
    std::cout << "Error" << std::endl;
  }
    
  return data;
}

template <typename T>
T DLinkedList<T>::lowerHalf(T data, int32_t index) {
  node *aux = new node();
  node *cur = DLinkedList::sent->getFirst();

  while(cur->getNext() && index-- > 0) {
    cur = cur->getNext();
  }
  
  aux->setData(data);
  aux->setPrev(cur->getPrev());
  aux->setNext(cur);

  cur->getPrev()->setNext(aux);
  cur->setPrev(aux);

  DLinkedList::sent->increment();
  return data;
}

template <typename T>
T DLinkedList<T>::upperHalf(T data, int32_t index) {
  node *aux = new node();
  node *cur = DLinkedList::sent->getLast();
  index = abs(index - (DLinkedList::sent->getSize() - 1));
  
  while(cur->getPrev() && index-- > 0) cur = cur->getPrev();

  aux->setData(data);
  aux->setPrev(cur->getPrev());
  aux->setNext(cur);

  aux->getPrev()->setNext(aux);
  cur->setPrev(aux);

  DLinkedList::sent->increment();
  return data;
}

template <typename T>
T DLinkedList<T>::remove() {
  node* cur = DLinkedList::sent->getLast();
  T data = cur->getData();
  if(cur->getPrev()) cur->getPrev()->setNext(nullptr);
  
  DLinkedList::sent->setLast(cur->getPrev());
  // Free memory here :P
  
  DLinkedList::sent->decrement();
  return data;
}

template <typename T>
T DLinkedList<T>::shift() {
  node *cur = DLinkedList::sent->getFirst();
  T data = cur->getData();
  if(cur->getNext()) cur->getNext()->setPrev(nullptr);

  DLinkedList::sent->setFirst(cur->getNext());
  // Free memory here :P
  
  DLinkedList::sent->decrement();
  return data;
}

template <typename T>
T DLinkedList<T>::removeAt(int32_t index) {
  int32_t currentSize = DLinkedList::sent->getSize(); 
  T data;
  
  if(index == 0) return shift();
  if(index >= currentSize) return remove();

  if(index <= (currentSize / 2)) {
    node *cur = DLinkedList::sent->getFirst();
   
    while(cur->getNext() && index--) cur = cur->getNext();
   
    data = cur->getData();
    if(cur->getPrev()) cur->getPrev()->setNext(cur->getNext());
    if(cur->getNext()) cur->getNext()->setPrev(cur->getPrev());

    // Free memory here :P

    DLinkedList::sent->decrement();
  } else {
    node *cur = DLinkedList::sent->getLast();

    index = abs(index - (currentSize - 1));
    
    while(cur->getPrev() && index) cur = cur->getPrev();

    data = cur->getData();
    if(cur->getPrev()) cur->getPrev()->setNext(cur->getNext());
    if(cur->getNext()) cur->getNext()->setPrev(cur->getPrev());

    // Free memory here :P

    DLinkedList::sent->decrement();
  }

  return data;
}

template <typename T>
T DLinkedList<T>::removeSpecific(T data) {
  node* cur = DLinkedList::sent->getFirst();
  T removed;
 
  // Greedy check here.
  if(equal(DLinkedList::sent->getFirst()->getData(), data)) return shift();
  if(equal(DLinkedList::sent->getLast()->getData(), data)) return remove();

  while(cur->getNext() && !equal(cur->getData(), data)) cur = cur->getNext();

  if(equal(cur->getData(), data)) {
    removed = cur->getData();
   
    if(cur->getPrev()) cur->getPrev()->setNext(cur->getNext());
    if(cur->getNext()) cur->getNext()->setPrev(cur->getPrev());

    // Free memory here :P

    DLinkedList::sent->decrement();
  }

  return removed;
}

template <typename T>
void DLinkedList<T>::show() {
  std::cout << "(" << DLinkedList::sent->getSize() << ") -> ";
  if(emptyList()) std::cout << "| " << "Empty List" << " |" << std::endl;
  else {
    node *cur;
    cur = DLinkedList::sent->getFirst();
    std::cout << "| ";
    while(cur != nullptr) {
      std::cout << cur->getData() << " | ";
      cur = cur->getNext();
    }
    std::cout << std::endl;
  }
}

template <>
void DLinkedList<std::vector<std::string>>::show() {
  std::cout << "(" << DLinkedList::sent->getSize() << ") -> \n" << std::endl;
  if(emptyList()) std::cout << "Empty route" << std::endl;
  else {
    node *cur;
    cur = DLinkedList::sent->getFirst();
    while(cur != nullptr) {
      std::cout << "\t\t" << cur->getData().front() << std::endl;
      std::cout << cur->getData().back() << std::endl;
      cur = cur->getNext();
    }
  }
}

template <>
void DLinkedList<std::vector<std::wstring>>::show() {

}

template <typename T>
DLinkedList<T>::~DLinkedList() {
  /* node *aux = DLinkedList::sent->getFirst()->getNext(); */

  /* while(aux) { */
  /*   aux = DLinkedList::sent->getFirst()->getNext(); */
  /*   delete DLinkedList::sent->getFirst(); */
  /*   DLinkedList::sent->setFirst(aux); */
  /* } */

  /* DLinkedList::sent->setFirst(nullptr); */
  /* DLinkedList::sent->setLast(nullptr); */
}

template class DLinkedList<std::string>;
template class DLinkedList<std::vector<std::string>>;
template class DLinkedList<std::vector<std::wstring>>;

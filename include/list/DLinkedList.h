/*
 * File:   DLinkedList.h
 */

#ifndef DLINKEDLIST_H
#define DLINKEDLIST_H

#include "list/IList.h"

#include <sstream>
#include <iostream>
#include <type_traits>
using namespace std;

template <class T>
class DLinkedList : public IList<T>
{
public:
    class Node;        // Forward declaration
    class Iterator;    // Forward declaration
    class BWDIterator; // Forward declaration

protected:
    Node *head; // this node does not contain user's data
    Node *tail; // this node does not contain user's data
    int count;
    bool (*itemEqual)(T &lhs, T &rhs);        // function pointer: test if two items (type: T&) are equal or not
    void (*deleteUserData)(DLinkedList<T> *); // function pointer: be called to remove items (if they are pointer type)

public:
    DLinkedList(
        void (*deleteUserData)(DLinkedList<T> *) = 0,
        bool (*itemEqual)(T &, T &) = 0);
    DLinkedList(const DLinkedList<T> &list);
    DLinkedList<T> &operator=(const DLinkedList<T> &list);
    ~DLinkedList();

    // Inherit from IList: BEGIN
    void add(T e);
    void add(int index, T e);
    T removeAt(int index);
    bool removeItem(T item, void (*removeItemData)(T) = 0);
    bool empty();
    int size();
    void clear();
    T &get(int index);
    int indexOf(T item);
    bool contains(T item);
    string toString(string (*item2str)(T &) = 0);
    // Inherit from IList: END

    void println(string (*item2str)(T &) = 0)
    {
        cout << toString(item2str) << endl;
    }
    void setDeleteUserDataPtr(void (*deleteUserData)(DLinkedList<T> *) = 0)
    {
        this->deleteUserData = deleteUserData;
    }

    bool contains(T array[], int size)
    {
        int idx = 0;
        for (DLinkedList<T>::Iterator it = begin(); it != end(); it++)
        {
            if (!equals(*it, array[idx++], this->itemEqual))
                return false;
        }
        return true;
    }

    /*
     * free(DLinkedList<T> *list):
     *  + to remove user's data (type T, must be a pointer type, e.g.: int*, Point*)
     *  + if users want a DLinkedList removing their data,
     *      he/she must pass "free" to constructor of DLinkedList
     *      Example:
     *      DLinkedList<T> list(&DLinkedList<T>::free);
     */
    static void free(DLinkedList<T> *list)
    {
        typename DLinkedList<T>::Iterator it = list->begin();
        while (it != list->end())
        {
            delete *it;
            it++;
        }
    }

    /* begin, end and Iterator helps user to traverse a list forwardly
     * Example: assume "list" is object of DLinkedList

     DLinkedList<char>::Iterator it;
     for(it = list.begin(); it != list.end(); it++){
            char item = *it;
            std::cout << item; //print the item
     }
     */
    Iterator begin()
    {
        return Iterator(this, true);
    }
    Iterator end()
    {
        return Iterator(this, false);
    }

    /* last, beforeFirst and BWDIterator helps user to traverse a list backwardly
     * Example: assume "list" is object of DLinkedList

     DLinkedList<char>::BWDIterator it;
     for(it = list.last(); it != list.beforeFirst(); it--){
            char item = *it;
            std::cout << item; //print the item
     }
     */
    BWDIterator bbegin()
    {
        return BWDIterator(this, true);
    }
    BWDIterator bend()
    {
        return BWDIterator(this, false);
    }

protected:
    static bool equals(T &lhs, T &rhs, bool (*itemEqual)(T &, T &))
    {
        if (itemEqual == 0)
            return lhs == rhs;
        else
            return itemEqual(lhs, rhs);
    }
    void copyFrom(const DLinkedList<T> &list);
    void removeInternalData();
    Node *getPreviousNodeOf(int index);
    void checkIndex(int index);
    //////////////////////////////////////////////////////////////////////
    ////////////////////////  INNER CLASSES DEFNITION ////////////////////
    //////////////////////////////////////////////////////////////////////
public:
    class Node
    {
    public:
        T data;
        Node *next;
        Node *prev;
        friend class DLinkedList<T>;

    public:
        Node(Node *next = 0, Node *prev = 0)
        {
            this->next = next;
            this->prev = prev;
        }
        Node(T data, Node *next = 0, Node *prev = 0)
        {
            this->data = data;
            this->next = next;
            this->prev = prev;
        }
    };

    //////////////////////////////////////////////////////////////////////
    class Iterator
    {
    private:
        DLinkedList<T> *pList;
        Node *pNode;

    public:
        Iterator(DLinkedList<T> *pList = 0, bool begin = true)
        {
            if (begin)
            {
                if (pList != 0)
                    this->pNode = pList->head->next;
                else
                    pNode = 0;
            }
            else
            {
                if (pList != 0)
                    this->pNode = pList->tail;
                else
                    pNode = 0;
            }
            this->pList = pList;
        }

        Iterator &operator=(const Iterator &iterator)
        {
            this->pNode = iterator.pNode;
            this->pList = iterator.pList;
            return *this;
        }
        void remove(void (*removeItemData)(T) = 0)
        {
            pNode->prev->next = pNode->next;
            pNode->next->prev = pNode->prev;
            Node *pNext = pNode->prev; // MUST prev, so iterator++ will go to end
            if (removeItemData != 0)
                removeItemData(pNode->data);
            delete pNode;
            pNode = pNext;
            pList->count -= 1;
        }

        T &operator*()
        {
            return pNode->data;
        }
        bool operator!=(const Iterator &iterator)
        {
            return pNode != iterator.pNode;
        }
        // Prefix ++ overload
        Iterator &operator++()
        {
            pNode = pNode->next;
            return *this;
        }
        // Postfix ++ overload
        Iterator operator++(int)
        {
            Iterator iterator = *this;
            ++*this;
            return iterator;
        }
    };

 class BWDIterator 
  {
    // TODO implement
  private:
    DLinkedList<T> *pList;   
    Node *cur_node;              

  public:
    BWDIterator(DLinkedList<T> *pList = 0, bool begin = true) {
        if (begin) 
        {
            if (pList != nullptr)
                this->cur_node = pList->tail->prev; 
            else
                cur_node = nullptr;
        } else 
        {
            if (pList != nullptr)
                this->cur_node = pList->head;      
            else
                cur_node = nullptr;
        }
        this->pList = pList;
    }


    void remove(void (*removeItemData)(T) = 0) {
        cur_node->prev->next = cur_node->next;
        cur_node->next->prev = cur_node->prev;
        Node *pPrev = cur_node->next; 
        if (removeItemData != nullptr) 
        {
            removeItemData(cur_node->data); 
        }
        delete cur_node;
        cur_node = pPrev;
        pList->count--;
    }

    BWDIterator &operator=(const BWDIterator &iterator) 
    {
        this->cur_node = iterator.cur_node;
        this->pList = iterator.pList;
        return *this;
    }

    BWDIterator &operator--() 
    {
        cur_node = cur_node->prev;
        return *this;
    }

    T &operator*() 
    {
        return cur_node->data;
    }

    bool operator!=(const BWDIterator &iterator) 
    {
        return cur_node != iterator.cur_node;
    }
    
    BWDIterator operator--(int) 
    {
        BWDIterator temp = *this;
        ++(*this); 
        return temp;
    }

  };
};

template <class T>
using List = DLinkedList<T>;

//! ////////////////////////////////////////////////////////////////////
//! //////////////////////     METHOD DEFNITION      ///////////////////
//! ////////////////////////////////////////////////////////////////////

template <class T>
DLinkedList<T>::DLinkedList(void (*deleteUserData)(DLinkedList<T> *), bool (*itemEqual)(T &, T &)) 
{
  // TODO implement
  head = new Node();
  tail = new Node();
  count = 0;
  head->next = tail;
  tail->prev = head;
  this->deleteUserData = deleteUserData;
  this->itemEqual = itemEqual;
}

template <class T>
DLinkedList<T>::DLinkedList(const DLinkedList<T> &list) 
{
  // TODO implement
    this->count = 0;
    tail = new Node();
    head = new Node();
    tail->prev = head;
    head->next = tail;
    Node *temp = list.head->next;
    while(temp!=list.tail)
    {
        add(temp->data);
        temp = temp->next;
    }
    this->deleteUserData = list.deleteUserData;
    this->itemEqual = list.itemEqual;
}

template <class T>
DLinkedList<T> &DLinkedList<T>::operator=(const DLinkedList<T> &list) 
{
  // TODO implement
  if(this == &list) 
  {
    return *this; 
  }else
  { 
    this->clear(); 
    Node* current = list.head->next; 
    while(current!=list.tail)
    {
      this->add(current->data);
      current = current->next;
    }
    this->deleteUserData = list.deleteUserData;
    this->itemEqual = list.itemEqual;
    return *this;
  }
}

template <class T>
DLinkedList<T>::~DLinkedList() 
{
  // TODO implement
  removeInternalData();
}

template <class T>
void DLinkedList<T>::add(T e) 
{
  // TODO implement
  Node* newNode = new Node(e);
    newNode->next = tail;         
    newNode->prev = tail->prev; 
    if(tail->prev == head)
    {
      head->next = newNode;
    }
    else
    {
      tail->prev->next = newNode;
    }  
    tail->prev = newNode;        
    count++; 
}

template <class T>
void DLinkedList<T>::add(int index, T e) 
{
  // TODO implement
  if (index < 0 || index > count) 
  {
        throw std::out_of_range("Index is out of range!");
  }

    Node* newNode = new Node(e);
    Node* current = getPreviousNodeOf(index);
    newNode->next = current->next;
    newNode->prev = current;
    current->next->prev = newNode;
    current->next = newNode;
    count++;
}

template <class T>
T DLinkedList<T>::removeAt(int index) 
{
  // TODO implement
    if (index < 0 || index >= count) 
    {
        throw std::out_of_range("Index is out of range!");
    }
    Node* node_to_remove = getPreviousNodeOf(index)->next;
    node_to_remove->prev->next = node_to_remove->next;
    node_to_remove->next->prev = node_to_remove->prev;
    T temp = node_to_remove->data;
    delete node_to_remove;
    count--;
    return temp;
}

template <class T>
bool DLinkedList<T>::empty() 
{
  // TODO implement
  return this->count == 0;
}

template <class T>
int DLinkedList<T>::size() 
{
  // TODO implement
  return this->count;
}

template <class T>
void DLinkedList<T>::clear() 
{
  // TODO implement
  Node *current = this->head->next;
  if(deleteUserData == nullptr)
  {
    while(current!=tail)
    {
      Node *nextNode = current->next;
      delete current;
      current = nextNode;
    }
    head->next = tail;
    tail->prev = head;
    count = 0;
  }else
  {
    deleteUserData(this);
    while(current!=tail)
    {
      Node *nextNode = current->next;
      delete current;
      current = nextNode;
    }
    head->next = tail;
    tail->prev = head;
    count = 0;
  }
}

template <class T>
T &DLinkedList<T>::get(int index) 
{
  // TODO implement
  checkIndex(index);
  Node* temp = getPreviousNodeOf(index)->next;
  return temp->data;
}

template <class T>
int DLinkedList<T>::indexOf(T item) 
{
  // TODO implement
  int index_find = -1;
  Node *current = head->next;
  for(int i = 0; i < count; i ++)
  {
    if(equals(item, current->data , itemEqual) == 1)
    {
      index_find = i;
      break;
    }
    current = current->next;
  }
  return index_find;  
}

template <class T>
bool DLinkedList<T>::removeItem(T item, void (*removeItemData)(T)) 
{
  // TODO implement
  int index_remove = 0;
  Node *current = head->next;
    for (int i = 0; i < count; ++i) 
    {
      if(equals(item, current->data, itemEqual) == 1)
      {
        if(removeItemData)
        {
          removeItemData(current->data);
        }
        removeAt(i);
        return 1; 
      }
      current = current->next;
    }
    return index_remove; ;
}

template <class T>
bool DLinkedList<T>::contains(T item) 
{
  // TODO implement
  if(indexOf(item) == -1)
  {
    return 0;
  }else return 1;
}

template <class T>
string DLinkedList<T>::toString(string (*item2str)(T &)) 
{
  // TODO implement
    stringstream string;
    string<<"[";
    int temp = 0;
    Node *current = head->next;
    while(current!=tail){
        if(item2str == nullptr)
        {
          string << current->data;
        }
        else
        {
          string << item2str(current->data);
        }
        if(temp + 1 < count){
            string<< ", ";
        }
        temp++;
        current = current->next;
    }
    string << "]";
    return string.str();
}

//! ////////////////////////////////////////////////////////////////////
//! ////////////////////// (private) METHOD DEFNITION //////////////////
//! ////////////////////////////////////////////////////////////////////
template <class T>
void DLinkedList<T>::copyFrom(const DLinkedList<T> &list) 
{
  /**
   * Copies the contents of another doubly linked list into this list.
   * Initializes the current list to an empty state and then duplicates all data
   * and pointers from the source list. Iterates through the source list and
   * adds each element, preserving the order of the nodes.
   */
  // TODO implement

  // this->removeInternalData();
  // Node* current = list.head->next;
  // while (current != nullptr) 
  // {
  //   this->add(current->data); 
  //   current = current->next;
  // }
  //   this->itemEqual = list.itemEqual;
  //   this->deleteUserData = list.deleteUserData;
}

template <class T>
void DLinkedList<T>::removeInternalData() 
{
  /**
   * Clears the internal data of the list by deleting all nodes and user-defined
   * data. If a custom deletion function is provided, it is used to free the
   * user's data stored in the nodes. Traverses and deletes each node between
   * the head and tail to release memory.
   */
  // TODO implement 
  Node* current = head->next;
  if (deleteUserData != nullptr) 
  {
    deleteUserData(this);  
  } 
  while (current != tail) 
  {
    Node* nextNode = current->next;
    delete current;
    current = nextNode;
  }
  head->next = tail;
  tail->prev = head;
  delete head;
  delete tail;
  count = 0;
}

template <class T>
typename DLinkedList<T>::Node *DLinkedList<T>::getPreviousNodeOf(int index) 
{
  /**
   * Returns the node preceding the specified index in the doubly linked list.
   * If the index is in the first half of the list, it traverses from the head;
   * otherwise, it traverses from the tail. Efficiently navigates to the node by
   * choosing the shorter path based on the index's position.
   */
  // TODO implement
  if (index < 0 || (index > count )) 
  {
    throw std::out_of_range("Index is out of range!");
  }

  Node* current;
  if(index == 0)
  {
    return head;
  }
  if (index < count / 2) 
  {  
    current = head->next;
    for (int i = 0; i < index - 1; i++) 
    {
      current = current->next;
    }
  } else 
  { 
    current = tail->prev;
    for (int i = count; i > index; i--) 
    
      current = current->prev;
  }
  return current;
}
template <class T>
void DLinkedList<T>::checkIndex(int index) {
  /**
   * Validates whether the given index is within the valid range of the list.
   * Throws an std::out_of_range exception if the index is negative or exceeds
   * the number of elements. Ensures safe access to the list's elements by
   * preventing invalid index operations.
   */
  // TODO implement
  if (index < 0 || index >= count) 
  {
    throw std::out_of_range("Index is out of range!");
  }
}

#endif /* DLINKEDLIST_H */

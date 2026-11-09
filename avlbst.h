#ifndef AVLBST_H
#define AVLBST_H

#include <iostream>
#include <exception>
#include <cstdlib>
#include <cstdint>
#include <algorithm>
#include "bst.h"

struct KeyError { };

/**
* A special kind of node for an AVL tree, which adds the balance as a data member, plus
* other additional helper functions. You do NOT need to implement any functionality or
* add additional data members or helper functions.
*/
template <typename Key, typename Value>
class AVLNode : public Node<Key, Value>
{
public:
    // Constructor/destructor.
    AVLNode(const Key& key, const Value& value, AVLNode<Key, Value>* parent);
    virtual ~AVLNode();

    // Getter/setter for the node's height.
    int8_t getBalance () const;
    void setBalance (int8_t balance);
    void updateBalance(int8_t diff);

    // Getters for parent, left, and right. These need to be redefined since they
    // return pointers to AVLNodes - not plain Nodes. See the Node class in bst.h
    // for more information.
    virtual AVLNode<Key, Value>* getParent() const override;
    virtual AVLNode<Key, Value>* getLeft() const override;
    virtual AVLNode<Key, Value>* getRight() const override;

protected:
    int8_t balance_;    // effectively a signed char
};

/*
  -------------------------------------------------
  Begin implementations for the AVLNode class.
  -------------------------------------------------
*/

/**
* An explicit constructor to initialize the elements by calling the base class constructor
*/
template<class Key, class Value>
AVLNode<Key, Value>::AVLNode(const Key& key, const Value& value, AVLNode<Key, Value> *parent) :
    Node<Key, Value>(key, value, parent), balance_(0)
{

}

/**
* A destructor which does nothing.
*/
template<class Key, class Value>
AVLNode<Key, Value>::~AVLNode()
{

}

/**
* A getter for the balance of a AVLNode.
*/
template<class Key, class Value>
int8_t AVLNode<Key, Value>::getBalance() const
{
    return balance_;
}

/**
* A setter for the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::setBalance(int8_t balance)
{
    balance_ = balance;
}

/**
* Adds diff to the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::updateBalance(int8_t diff)
{
    balance_ += diff;
}

/**
* An overridden function for getting the parent since a static_cast is necessary to make sure
* that our node is a AVLNode.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getParent() const
{
    return static_cast<AVLNode<Key, Value>*>(this->parent_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getLeft() const
{
    return static_cast<AVLNode<Key, Value>*>(this->left_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getRight() const
{
    return static_cast<AVLNode<Key, Value>*>(this->right_);
}


/*
  -----------------------------------------------
  End implementations for the AVLNode class.
  -----------------------------------------------
*/


template <class Key, class Value>
class AVLTree : public BinarySearchTree<Key, Value>
{
public:
    virtual void insert (const std::pair<const Key, Value> &new_item); // TODO
    virtual void remove(const Key& key);  // TODO
protected:
    virtual void nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2);

    // Add helper functions here
    void insertFix(AVLNode<Key, Value>* parent, AVLNode<Key, Value>* child);
    void rotateRight(AVLNode<Key, Value>* parent, AVLNode<Key, Value>* grand);
    void rotateLeft(AVLNode<Key, Value>* parent, AVLNode<Key, Value>* grand);
    void removeFix(AVLNode<Key, Value>* node, int diff);
};

/*
 * Recall: If key is already in the tree, you should 
 * overwrite the current value with the updated value.
 */
template<class Key, class Value>
void AVLTree<Key, Value>::rotateRight(AVLNode<Key, Value>* parent, AVLNode<Key, Value>* grand){
  AVLNode<Key, Value>* ggrand = grand->getParent();
  AVLNode<Key, Value>* parent_right = parent->getRight();
  if(ggrand == NULL){
    parent->setParent(NULL);
    this->root_ = parent;
  }
  else{
    if(ggrand->getLeft() == grand){
      ggrand->setLeft(parent);
    }
    else{
      ggrand->setRight(parent);
    }
  }
  parent->setParent(ggrand);
  parent->setRight(grand);
  grand->setParent(parent);
  grand->setLeft(parent_right);
  if (parent_right != NULL) {
    parent_right->setParent(grand);
  }
}

template<class Key, class Value>
void AVLTree<Key, Value>::rotateLeft(AVLNode<Key, Value>* parent, AVLNode<Key, Value>* grand){
  AVLNode<Key, Value>* ggrand = grand->getParent();
  AVLNode<Key, Value>* parent_left = parent->getLeft();
  if(ggrand == NULL){
    parent->setParent(NULL);
    this->root_ = parent;
  }
  else{
    if(ggrand->getLeft() == grand){
      ggrand->setLeft(parent);
    }
    else{
      ggrand->setRight(parent);
    }
  }
  parent->setParent(ggrand);
  parent->setLeft(grand);
  grand->setParent(parent);
  grand->setRight(parent_left);
  if (parent_left != NULL) {
    parent_left->setParent(grand);
  }
}


template<class Key, class Value>
void AVLTree<Key, Value>::insertFix(AVLNode<Key, Value>* parent, AVLNode<Key, Value>* child){
  AVLNode<Key, Value>* grand = parent->getParent();
  if(grand == NULL){
    return;
  }
  if(grand->getLeft() == parent){
    grand->updateBalance(-1);
    int gbalance = grand->getBalance();
    if(gbalance == 0){
      return;
    }
    else if(gbalance == -1){
      insertFix(grand, parent);
    }
    else{
      if(parent->getLeft() == child){
        rotateRight(parent, grand);
        parent->setBalance(0);
        grand->setBalance(0);
      }
      else if(parent->getRight() == child){
        int child_balance = child->getBalance();
        rotateLeft(child, parent);
        rotateRight(child, grand);
        if(child_balance == -1){
          parent->setBalance(0);
          grand->setBalance(1);
          child->setBalance(0);
        }
        else if(child_balance == 0){
          parent->setBalance(0);
          grand->setBalance(0);
          child->setBalance(0);
        }
        else if(child_balance == 1){
          parent->setBalance(-1);
          grand->setBalance(0);
          child->setBalance(0);
        }
        return;
      }
    }
  }
  else if(grand->getRight() == parent){
    grand->updateBalance(1);
    int gbalance = grand->getBalance();
    if(gbalance == 0){
      return;
    }
    else if(gbalance == 1){
      insertFix(grand, parent);
    }
    else{
      if(parent->getRight() == child){
        rotateLeft(parent, grand);
        parent->setBalance(0);
        grand->setBalance(0);
      }
      else if(parent->getLeft() == child){
        int child_balance = child->getBalance();
        rotateRight(child, parent);
        rotateLeft(child, grand);
        if(child_balance == -1){
          parent->setBalance(1);
          grand->setBalance(0);
          child->setBalance(0);
        }
        else if(child_balance == 0){
          parent->setBalance(0);
          grand->setBalance(0);
          child->setBalance(0);
        }
        else if(child_balance == 1){
          parent->setBalance(0);
          grand->setBalance(-1);
          child->setBalance(0);
        }
        return;
      }
    }
  }
}

template<class Key, class Value>
void AVLTree<Key, Value>::insert (const std::pair<const Key, Value> &new_item)
{
    // TODO
    if(this->root_ == NULL){
      AVLNode<Key, Value>* newRoot = new AVLNode<Key, Value>(new_item.first, new_item.second, NULL);
      newRoot->setBalance(0);
      this->root_ = newRoot;
      return;
    }
    typename BinarySearchTree<Key, Value>::iterator it = this->find(new_item.first);
    Node<Key, Value>* temp = this->root_;
    AVLNode<Key, Value>* child = NULL;
    if(it == this->end()){
      while(true){
        if(new_item.first < temp->getKey()){
          if(temp->getLeft() != NULL){
            temp = temp->getLeft();
          }
          else{
            child = new AVLNode<Key, Value>(new_item.first, new_item.second, static_cast<AVLNode<Key, Value>*>(temp));
            child->setBalance(0);
            temp->setLeft(child);
            break;
          }
        }
        else if(new_item.first > temp->getKey()){
          if(temp->getRight() != NULL){
            temp = temp->getRight();
          }
          else{
            child = new AVLNode<Key, Value>(new_item.first, new_item.second, static_cast<AVLNode<Key, Value>*>(temp));
            temp->setRight(child); 
            break;
          }
        }
      }
      AVLNode<Key, Value>* parent = static_cast<AVLNode<Key, Value>*>(temp);
      if(parent->getRight() == child){
        parent->updateBalance(1);
      }
      else if(parent->getLeft() == child){
        parent->updateBalance(-1);
      }
      if(parent->getBalance() == 0){
        return;
      }
      else{
        insertFix(parent, child);
      }
      return;
    }
    else{
      it->second = new_item.second;
      return;
    }
}

/*
 * Recall: The writeup specifies that if a node has 2 children you
 * should swap with the predecessor and then remove.
 */
template<class Key, class Value>
void AVLTree<Key, Value>::removeFix(AVLNode<Key, Value>* node, int diff){
  if(node == NULL){
    return;
  }
  AVLNode<Key, Value>* parent = node->getParent();
  int ndiff = 0;
  if(parent != NULL){
    if(parent->getLeft() == node){
      ndiff = 1;
    }
    else{
      ndiff = -1;
    }
  }
  if(diff == -1){
    int bn = node->getBalance();
    if(bn + diff == -2){
      AVLNode<Key, Value>* left = node->getLeft();
      int bl = left->getBalance();
      if(bl == -1){
        rotateRight(left, node);
        node->setBalance(0);
        left->setBalance(0);
        if (parent != NULL) {
          removeFix(parent, ndiff);
        }
      }
      else if(bl == 0){
        rotateRight(left, node);
        node->setBalance(-1);
        left->setBalance(1);
      }
      else if(bl == 1){
        AVLNode<Key, Value>* rightOfLeft = left->getRight();
        int br = rightOfLeft->getBalance();
        rotateLeft(rightOfLeft, left);
        rotateRight(rightOfLeft, node);
        if(br == 1){
          node->setBalance(0);
          left->setBalance(-1);
          rightOfLeft->setBalance(0);
        }
        else if(br == 0){
          node->setBalance(0);
          left->setBalance(0);
          rightOfLeft->setBalance(0);
        }
        else if(br == -1){
          node->setBalance(1);
          left->setBalance(0);
          rightOfLeft->setBalance(0);
        }
        if (parent != NULL) {
          removeFix(parent, ndiff);
        }
      }
    }
    else if(bn + diff == -1){
      node->setBalance(-1);
      return;
    }
    else if(bn + diff == 0){
      node->setBalance(0);
      if(parent != NULL){
        removeFix(parent, ndiff);
      }
    }
  }
  // The left subtree shrank
  if (diff == 1) {
    int bn = node->getBalance();

    if (bn + diff == 2) {
      AVLNode<Key, Value>* right = node->getRight();
      int br = right->getBalance();

      if (br == 1) {
        rotateLeft(right, node);
        node->setBalance(0);
        right->setBalance(0);

        if (parent != NULL) {
          removeFix(parent, ndiff);
        }
      }
      else if (br == 0) {
        rotateLeft(right, node);
        node->setBalance(1);
        right->setBalance(-1);
        // The subtree height did not decrease, so stop here.
      }
      else if (br == -1) {
        AVLNode<Key, Value>* leftOfRight = right->getLeft();
        int bp = leftOfRight->getBalance();

        rotateRight(leftOfRight, right);
        rotateLeft(leftOfRight, node);

        if (bp == 1) {
          node->setBalance(-1);
          right->setBalance(0);
          leftOfRight->setBalance(0);
        }
        else if (bp == 0) {
          node->setBalance(0);
          right->setBalance(0);
          leftOfRight->setBalance(0);
        }
        else if (bp == -1) {
          node->setBalance(0);
          right->setBalance(1);
          leftOfRight->setBalance(0);
        }

        if (parent != NULL) {
          removeFix(parent, ndiff);
        }
      }
    }
    else if (bn + diff == 1) {
      node->setBalance(1);
      return;
    }
    else if (bn + diff == 0) {
      node->setBalance(0);
      if (parent != NULL) {
        removeFix(parent, ndiff);
      }
    }
  }
}

template<class Key, class Value>
void AVLTree<Key, Value>:: remove(const Key& key)
{
  // TODO
AVLNode<Key, Value>* avl_rem = static_cast<AVLNode<Key, Value>*>(this->internalFind(key));
AVLNode<Key, Value>* parent = avl_rem->getParent();
  if (avl_rem == NULL) {
    return;
  }
  int diff = 0;
    if(avl_rem != NULL){
      if(avl_rem->getLeft() == NULL && avl_rem->getRight() == NULL){
        if(parent == NULL){
          this->root_ = NULL;
          delete avl_rem;
          return;
        }
        if(parent->getLeft() == avl_rem){
          diff = 1;
          parent->updateBalance(1);
          avl_rem->getParent()->setLeft(NULL);
        }
        else{
          diff = -1;
          parent->updateBalance(-1);
          parent->setRight(NULL);
        }
        delete avl_rem;
        return;
      }
      else if(avl_rem->getLeft()!= NULL && avl_rem->getRight() == NULL){
        AVLNode<Key, Value>* left = avl_rem->getLeft();
        if(parent == NULL){
          this->root_= left;
          this->root_->setParent(NULL);
          delete avl_rem;
          return;
        }
        left->setParent(parent);
        if(avl_rem->getKey() > parent->getKey()){
          diff = -1;
          parent->updateBalance(-1);
          parent->setRight(left);
        }
        else if(avl_rem->getKey() < parent->getKey()){
          diff = 1;
          parent->updateBalance(1);
          parent->setLeft(left);
        }
      }
      else if(avl_rem->getLeft()== NULL && avl_rem->getRight() != NULL){
        AVLNode<Key, Value>* right = avl_rem->getRight();
        if(parent == NULL){
          this->root_= right;
          this->root_->setParent(NULL);
          delete avl_rem;
          return;
        }
        right->setParent(parent);
        if(avl_rem->getKey() > parent->getKey()){
          diff = -1;
          parent->updateBalance(-1);
          parent->setRight(right);
        }
        else if(avl_rem->getKey() < parent->getKey()){
          diff = 1;
          parent->updateBalance(1);
          parent->setLeft(right);
        }
      }
      else{
        AVLNode<Key, Value>* pred = static_cast<AVLNode<Key, Value>*>(this->predecessor(avl_rem));
        nodeSwap(avl_rem, pred);
        AVLNode<Key, Value>* child = avl_rem->getLeft();

        if (parent->getLeft() == avl_rem) {
          diff = 1;
          parent->updateBalance(1);
          parent->setLeft(child);
        } else {
          diff = -1;
          parent->updateBalance(-1);
          parent->setRight(child);
        }
        if (child != NULL) {
          child->setParent(parent);
        }
      }
      delete avl_rem;
      removeFix(parent, diff);
    }
    return;
}

template<class Key, class Value>
void AVLTree<Key, Value>::nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2)
{
    BinarySearchTree<Key, Value>::nodeSwap(n1, n2);
    int8_t tempB = n1->getBalance();
    n1->setBalance(n2->getBalance());
    n2->setBalance(tempB);
}


#endif

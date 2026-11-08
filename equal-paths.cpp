#ifndef RECCHECK
//if you want to add any #includes like <iostream> you must do them here (before the next endif)
#include<iostream>
#include<algorithm>
#endif

#include "equal-paths.h"
using namespace std;


// You may add any prototypes of helper functions here
int help_paths(Node* root){
  if(root == NULL){return 0;}
  int left_h = help_paths(root->left);
  int right_h = help_paths(root->right);
  if((left_h == -1) || (right_h == -1)){
    return -1;
  }
  if(left_h != 0 && right_h != 0){
    if(left_h != right_h){
      return -1;
    }
  }
  return 1 + max(left_h, right_h);
}


bool equalPaths(Node * root)
{
    // Add your code below
    if(root == NULL){
      return true;
    }
    int result = help_paths(root);
    if(result == -1){
      return false;
    }
    return true;
}


#ifndef TREE_H

#define TREE_H


#include <iostream>

#include "list.h"



template <typename T>
class BinaryTree;


template <typename T>
class BinaryTreeNode{

friend class BinaryTree<T>;

private:
	T data;
	BinaryTreeNode<T>* Left;
	BinaryTreeNode<T>* Right;
	
	int degree;

//methods
//constructor destuctor
	BinaryTreeNode(T ddata);

//mutators
	void set_left(BinaryTreeNode<T>* Left);
	void set_right(BinaryTreeNode<T>* Right);

public:
//accessors
	T get_data();
	BinaryTreeNode<T>* get_left();
	BinaryTreeNode<T>* get_right();
	int get_deg();
	
//predicates
	bool has_left();
	bool has_right();
	bool has_children();
};


template <typename T>
class BinaryTree{
private:
	BinaryTreeNode<T>* Root;
	int  n_nodes;
	List<BinaryTreeNode<T>*> Leafs;
	
public:
//constructor destructor
	BinaryTree();
	~BinaryTree();
	
	void clear_pointers();

//debug
	void print_leafs(std::ostream& out = std::cout);

//accessors
	BinaryTreeNode<T>* get_root() const;
	int get_n_nodes();
	
	List<BinaryTreeNode<T>*> get_leafs();
	
//predicates
	bool empty() const;

//expand
	void append_root(T item);
	void append_left(BinaryTreeNode<T>* Node, T item);
	void append_right(BinaryTreeNode<T>* Node, T item);
	void append_left_right(BinaryTreeNode<T>* Node, T left, T right);
};


//Show
template <typename T>
std::ostream& operator<<(std::ostream& out, const BinaryTree<T>& B);

template <typename T>
void print_tree(std::ostream& out, BinaryTreeNode<T>* Root);	//use it with causious


#include "tree.tpp"


#endif

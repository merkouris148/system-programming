#include <cstdlib>
#include <cassert>


const bool tree_warnings_on = true;

/*
 ********************
 * Binary Tree Node *
 ********************
 */
 
//constructor destructor
template <typename T>
BinaryTreeNode<T>::BinaryTreeNode(T ddata) : data(ddata), Left(NULL), Right(NULL), degree(0){}

//mutators
template <typename T>
void BinaryTreeNode<T>::set_left(BinaryTreeNode<T>* LLeft){
	if(tree_warnings_on and has_left())  std::cout << "Binary Tree Warnings: Unsafe set_left!" << std::endl; 
	
	if(!has_left() && LLeft != NULL)  		++degree;
	else if(has_left() && LLeft == NULL)	--degree;
	Left = LLeft;
}

template <typename T>
void BinaryTreeNode<T>::set_right(BinaryTreeNode<T>* RRight){
	if(tree_warnings_on and has_right())  std::cout << "Binary Tree Warnings: Unsafe set_left!" << std::endl; 
	
	if(!has_right() && RRight != NULL) 		++degree;
	else if(has_right() && RRight == NULL)	--degree;
	Right = RRight;
}


//accessors
template <typename T>
T BinaryTreeNode<T>::get_data(){
	return data;
}

template <typename T>
BinaryTreeNode<T>* BinaryTreeNode<T>::get_left(){
	assert(has_left());
	
	return Left;
}

template <typename T>
BinaryTreeNode<T>* BinaryTreeNode<T>::get_right(){
	assert(has_right());
	
	return Right;
}

template <typename T>
int BinaryTreeNode<T>::get_deg(){
	return degree;
}

//predicates
template <typename T>
bool BinaryTreeNode<T>::has_left(){
	return Left != NULL;
}

template <typename T>
bool BinaryTreeNode<T>::has_right(){
	return Right != NULL;
}

template <typename T>
bool BinaryTreeNode<T>::has_children(){
	return has_right() or has_left();
}


/*
 ***************
 * Binary Tree *
 ***************
 */

//constructor destructor
template <typename T>
BinaryTree<T>::BinaryTree() : Root(NULL), n_nodes(0) {}

template <typename T>
void delete_tree(BinaryTreeNode<T>* Root){
	//BFS delete
	if(Root->has_right()) 	delete_tree(Root->get_right());
	if(Root->has_left())	delete_tree(Root->get_left());
	
	delete Root;
}

template <typename T>
BinaryTree<T>::~BinaryTree(){
	if(!empty()) delete_tree(get_root());
}

template <typename T>
void clear_pointer_tree(BinaryTreeNode<T>* Root){
	//BFS
	if(Root->has_right())	clear_pointer_tree(Root->get_right());
	if(Root->has_left())	clear_pointer_tree(Root->get_left());
	
	delete Root->get_data();
}

template <typename T>
void BinaryTree<T>::clear_pointers(){
	if(!empty()) clear_pointer_tree(get_root());
}

//debug
template <typename T>
void BinaryTree<T>::print_leafs(std::ostream& out /*= cout*/){
	ListNode<BinaryTreeNode<T>*>* cursor = Leafs.get_begin();
	out << "[ ";
	while(cursor->has_next()){out << cursor->get_data()->get_data() << ", "; cursor = cursor->get_next();}
	out << cursor->get_data()->get_data();
	out << " ]";
}

//accessors
template <typename T>
BinaryTreeNode<T>* BinaryTree<T>::get_root() const{
	assert(!empty());
	
	return Root;
}

template <typename T>
int BinaryTree<T>::get_n_nodes(){
	return n_nodes;
}

template <typename T>
List<BinaryTreeNode<T>*> BinaryTree<T>::get_leafs(){
	return Leafs;
}

//predicates
template <typename T>
bool BinaryTree<T>::empty() const{
	return n_nodes == 0;
}

//expand
template <typename T>
void BinaryTree<T>::append_root(T item){
	assert(empty());
	
	Root = new BinaryTreeNode<T>(item);
	++n_nodes;
	
	Leafs.append(Root);
}

template <typename T>
void BinaryTree<T>::append_left(BinaryTreeNode<T>* Node, T item){
	assert(Node != NULL && !empty());
	//assert(Leafs.exists(Node));					//we can append nodes only to leafs
	
	if(Leafs.exists(Node))  Leafs.erase(Leafs.get_first(Node));														//old leaf no longer a leaf
	BinaryTreeNode<T>* NewLeft = new BinaryTreeNode<T>(item);  Node->set_left(NewLeft);		//create new node
	Leafs.insert(NewLeft);																	//the new node is a leaf
	
	++n_nodes;
}

template <typename T>
void BinaryTree<T>::append_right(BinaryTreeNode<T>* Node, T item){
	assert(Node != NULL && !empty());
	//assert(Leafs.exists(Node));					//we can append nodes only to leafs
	
	if(Leafs.exists(Node))  Leafs.erase(Leafs.get_first(Node));														//old leaf no longer a leaf
	BinaryTreeNode<T>* NewRight = new BinaryTreeNode<T>(item); Node->set_right(NewRight);	//create new node
	Leafs.insert(NewRight);																	//the new node is a leaf
	
	++n_nodes;
}

template <typename T>
void BinaryTree<T>::append_left_right(BinaryTreeNode<T>* Node, T left, T right){
	append_left(Node, left);
	append_right(Node, right);
}



//Show
template <typename T>
void print_tree(std::ostream& out, BinaryTreeNode<T>* Root){
	assert(Root != NULL);
	
	out << "( " << Root->get_data() << ", ";
	
	if(Root->has_left())	print_tree(out, Root->get_left());
	
	out << ", ";
	
	if(Root->has_right())	print_tree(out, Root->get_right());
	
	out << " )";
}

template <typename T>
std::ostream& operator<<(std::ostream& out, const BinaryTree<T>& B){
	//out << "[ ";
	
	if(!B.empty()) print_tree(out, B.get_root());
	
	//out << " ]";
	
	return out;
}

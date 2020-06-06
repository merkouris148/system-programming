#include <cstdlib>
#include <cassert>
#include <climits>

const bool warnings_on = false;

/*
 *************
 * List Node *
 *************
 */

//methods
//constructor destructor
template <typename T>
ListNode<T>::ListNode(T ddata) : data(ddata), next(NULL){}

//accessors
template <typename T>
T ListNode<T>::get_data() const {return data;}

template <typename T>
T& ListNode<T>::get_data_handle() {return data;}

template <typename T>
ListNode<T>* ListNode<T>::get_next(){
	assert(has_next());
	
	return next;
}

//mutators
template <typename T>
void ListNode<T>::set_next(ListNode<T>* nnext){
	if(warnings_on and has_next()) std::cout << "List Warning: Unsafe set_next!" << std::endl;
		
	next = nnext;
}

//predicates
template <typename T>
bool ListNode<T>::has_next() const{
	return next != NULL;
}


/*
 ********
 * List *
 ********
 */

//constructor destructor
template <typename T>
List<T>::List() : head(NULL), last(NULL), size(0){}

template <typename T>
void delete_list(ListNode<T>* Node){
	if(Node->has_next()) delete_list(Node->get_next());
	delete Node;
}

template <typename T>
List<T>::~List(){
	if(!empty()){
		delete_list(get_begin());
	}
}

template <typename T>
List<T>::List(const List& R) : head(NULL), last(NULL), size(0){
	append(R);
}

template <typename T>
const List<T>& List<T>::operator=(const List<T>& R){
	if(this != &R){
		//delete list
		if(not empty()){
			delete_list(get_begin());
			size = 0;
		}
		
		//append new list
		append(R);
	}
	
	return *this;
}



template <typename T>
void clear_pointer_list(ListNode<T>* Node){
	if(Node->has_next()) clear_pointer_list(Node->get_next());
	delete Node->get_data();
}

template <typename T>
void List<T>::clear_pointers(){
	if(!empty()){
		clear_pointer_list(get_begin());
	}
}


//debug
template <typename T>
void List<T>::print(void (*print_elem) (T,std::ostream&), std::ostream& out /*= std::cout*/) const{
	//out << "[ ";
	
	if(!empty()){
		ListNode<T>* Head = get_begin();
		print_elem(Head->get_data(), out);
	
		while(Head->has_next()){
			Head = Head->get_next();
			print_elem(Head->get_data(), out);
		}
	}

	//out << " ]";
}

//accessors
template <typename T>
ListNode<T>* List<T>::get_begin() const{
	assert(!empty());
	
	return head;
}

template <typename T>
int List<T>::get_size() const {return size;}

template <typename T>
ListNode<T>* List<T>::get_last() const {return last;}

template <typename T>
List<T> List<T>::get_tail() const{
	List<T> tail;
	
	ListNode<T>* tail_cur = get_begin()->get_next();
	while(tail_cur->has_next()){
		tail.append(tail_cur->get_data());
		
		tail_cur = tail_cur->get_next();
	}
	
	//last iteration
	tail.append(tail_cur->get_data());
	
	return tail;
}

//predicates
template <typename T>
bool List<T>::empty() const {return size == 0;}


template <typename T>
bool exists_in_list(ListNode<T>* Head, T item){
	if(Head->get_data() == item) 	return true;
	if(!Head->has_next())			return false;
	else 							return exists_in_list(Head->get_next(), item);
}

template <typename T>
bool List<T>::exists(T item) const{
	if(empty()) return false;
	return 		exists_in_list(get_begin(), item);
}

template <typename T>
bool exists_in_list_eq(ListNode<T>* Head, T item, bool (*equal) (T, T)){
	if(equal(Head->get_data(), item)) 	return true;
	if(!Head->has_next())				return false;
	else 								return exists_in_list_eq(Head->get_next(), item, equal);
}

template <typename T>
bool List<T>::exists(T item, bool (*equal) (T, T)) const{
	if(empty()) return false;
	return 		exists_in_list_eq(get_begin(), item, equal);
}

//search
template <typename T>
ListNode<T>* get_first_in_list(ListNode<T>* Head, T item){
	if(Head->get_data() == item) 	return Head;
	if(!Head->has_next())			assert(("List Error: search_first(): Item wasn't found in list!", false));
	else 							return get_first_in_list(Head->get_next(), item);
}

template <typename T>
ListNode<T>* List<T>::get_first(T item) const{
	assert(!empty());
	return get_first_in_list(get_begin(), item);
}

template <typename T>
ListNode<T>* get_first_in_list_eq(ListNode<T>* Head, T item, bool (*equal) (T, T)){
	if(equal(Head->get_data(), item)) 	return Head;
	if(!Head->has_next())				assert(("List Error: search_first(): Item wasn't found in list!", false));
	else 								return get_first_in_list_eq(Head->get_next(), item, equal);
}

template <typename T>
ListNode<T>* List<T>::get_first(T item, bool (*equal) (T, T)) const{
	assert(!empty());
	return get_first_in_list_eq(get_begin(), item, equal);
}

//max/min
/* let f:T -> Int maximum() returns,
 * for a l in [T], the v in f(l)
 * s.t. v >= f(i) for every i in l*/
template <typename T>
int List<T>::maximum(int (*f)(const T&)){
	int max = -INT_MAX;
	
	auto cursor = get_begin();
	for(; cursor->has_next(); cursor = cursor->get_next()){
		int v = f(cursor->get_data());
		if(v > max)  max = v;
	}
	
	//last iteration
	int v = f(cursor->get_data());
	if(v > max)  max = v;
	
	return max;
}

//expand
template <typename T>
void List<T>::append(T item){
	ListNode<T>* Node = new ListNode<T>(item);
	
	if(!empty()) last->set_next(Node);
	if(empty()) head = Node;
	
	last = Node;
	size++; 
}

template <typename T>
void List<T>::append(const List<T>& L){
	if(not L.empty()){
		
		ListNode<T>* cursor = L.get_begin();
		while(cursor->has_next()){
			append(cursor->get_data());
			
			cursor = cursor->get_next();
		}
		
		append(cursor->get_data());
	}
}

template <typename T>
bool List<T>::insert(T item){
	if(exists(item)) return false;
	else 			{append(item); return true;}
}

template <typename T>
void List<T>::insert(const List<T>& L){
	if(!L.empty()){
		
		ListNode<T>* cursor = L.get_begin();
		while(cursor->has_next()){
			insert(cursor->get_data());
			
			cursor = cursor->get_next();
		}
		
		insert(cursor->get_data());
	}
}

//erase
template <typename T>
bool is_list_node(ListNode<T>* Head, ListNode<T>* Query){
	assert(Query != NULL);
	
	if(Head == NULL)		return false;
	if(Head == Query)		return true;
	if(Head->has_next())	return is_list_node(Head->get_next(), Query);
}

//This functions should be called after the is_list_node() function
//has assured that Node is in *this* List
template <typename T>
ListNode<T>* get_prev(ListNode<T>* Head, ListNode<T>* Node){
	//we won't call is_list_node() here so to keep complexity O(n)	//If (i) Node is this list's node,
	assert(Head != NULL);											//the list is (ii) non empty,
	assert(Head != Node);											//and (iii) the node isn't the head
																	//then the previous node of Node exists
	
	if(Head->get_next() == Node)	return Head;
	else 							return get_prev(Head->get_next(), Node);
}

template <typename T>
void List<T>::erase(ListNode<T>* Node){
	if(is_list_node(head, Node)){
									//if Node->has_prev()
		ListNode<T>* Prev = NULL;	if(Node != head) 		Prev = get_prev(head, Node);
		ListNode<T>* Next = NULL;	if(Node->has_next())	Next = Node->get_next();
		if(Prev != NULL) Prev->set_next(Next);

		if(Node == head)	head = Next;
		if(Node == last)	last = Prev;
	
		delete Node;
		--size;
	}
}

template <typename T>
void List<T>::erase(List< ListNode<T>* >& Nodes){
	if(!Nodes.empty()){
		ListNode< ListNode<T>* >* l = Nodes.get_begin();
		while(l->has_next()){erase(l->get_data()); l = l->get_next();}
		erase(l->get_data());
	}
}

template <typename T>
void List<T>::erase_first(T elem){
	erase(get_first(elem));
}

template <typename T>
T List<T>::pop_head(){
	ListNode<T>* head = get_begin();
	T data = head->get_data();
	
	erase(head);
	
	return data;
}


//sublist
template <typename T>
List<T> List<T>::filter(bool (*predicate) (const T&)){
	List<T> sublist;
	
	auto cursor = get_begin();
	for(; cursor->has_next(); cursor = cursor->get_next())  if(predicate(cursor->get_data()))  sublist.append(cursor->get_data());
	
	//last iteration
	if(predicate(cursor->get_data()))  sublist.append(cursor->get_data());
	
	return sublist;
}

//conversions
template <typename T>
List<T>::operator List<int>() const{
	if(empty()) {List<int> e; return e;}
	
	List<int> R;
	auto cursor = get_begin();
	for(; cursor->has_next(); cursor = cursor->get_next()) R.append(cursor->get_data());
	//lats it
	R.append(cursor->get_data());
	
	return R;
}


//Show
template <typename T>
std::ostream& operator<<(std::ostream& out, const List<T>& L){
	out << "[ ";
	
	if(!L.empty()){
		ListNode<T>* Head = L.get_begin();
		out << Head->get_data() << ", ";
	
		while(Head->has_next()){
			Head = Head->get_next();
			out << Head->get_data() << ", ";
		}
	}

	out << " ]";
	
	return out;
}


//operations
template <typename T>
List<List<T>> tokenize_list(const List<T>& L, const T& delim){
	auto cur = L.get_begin();
	
	List<List<T>> tokens;
	
	while(cur->has_next()){
	
		List<T> token;
		while(cur->get_data() != delim and cur->has_next()){
			token.append(cur->get_data());
		
			cur = cur->get_next();
		}
	
		//last iteration{
			if(cur->get_data() != delim) 	token.append(cur->get_data());
			if(cur->has_next())				cur = cur->get_next();
		//}
		
		tokens.append(token);		
	}
	
	return tokens;

}



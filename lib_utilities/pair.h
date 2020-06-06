#ifndef PAIR_H

#define PAIR_H

#include <iostream>

template <typename T, typename S>
struct Pair{	
	T fst;
	S snd;

//methods
//constructor destructor
	Pair(T t, S s) 							: fst(t), snd(s) {};
	Pair(const Pair& P) 					{fst = P.fst; snd = P.snd;};
	const Pair& operator=(const Pair& P)  	{if(&P != this){fst = P.fst; snd = P.snd;}};
};

//relations
template <typename T, typename S>
bool operator==(const Pair<T, S>& P, const Pair<T, S>& Q){
	return P.fst == Q.fst and P.snd == Q.snd;
}

//show
template <typename T, typename S>
std::ostream& operator<<(std::ostream& out, Pair<T, S> P){
	out << "(" <<  P.fst << ", " << P.snd << ")";
	
	return out;
};

#endif

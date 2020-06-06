#ifndef TRANSACTIONS_H

#define TRANSACTIONS_H

#include <iostream>

#include "../lib_containers/list.h"
#include "../lib_containers/string.h"
#include "../lib_containers/tree.h"
#include "../lib_utilities/time.h"



/*
 ***************
 * Transaction *
 ***************
 */


class Wallet;

class Transaction{
private:
	String Id;
	Wallet* sent;
	Wallet* receiv;
	int amount;
	Time date;

	bool done;
public:
//constructor destructor
	Transaction(const String& IId, Wallet* ssent, Wallet* rreceiv, int aamount, Time ddate)
		:	Id((assert(not IId.empty()), IId)),
			sent((assert(ssent != NULL), ssent)),
			receiv((assert(rreceiv != NULL), rreceiv)),
			amount((assert(aamount >= 1), aamount)),
			date(ddate),
			done(false)
		{};

//accessors
	String 	get_Id()		const 	{return Id;};
	Wallet* get_sent() 		const	{return sent;};
	Wallet* get_receiv()	const 	{return receiv;};
	int 	get_amount()	const	{return amount;};
	Time	get_date()		const	{return date;};

//predicates
	bool feasible() const;

//operations
	void do_transaction();
};


//Show
std::ostream& operator<<(std::ostream& out, const Transaction& t);


/*
 ***********
 * Bitcoin *
 ***********
 */



//History Tree Node Record
class HTNRecord{
friend class BitCoin;

//data
	Transaction* t;
	Wallet* w;
	int amount;

//constructor destructor
	HTNRecord(Transaction* tt, Wallet* ww, int aamount)
		:	t(tt),											//a HTN record may have a NULL transaction
			w((assert(ww != NULL), ww)),					//though, only the root will have a null transaction
			amount((assert(aamount >= 1), aamount))
		{};

public:
//accessors
	Transaction* 	get_transaction()	const	{return t;};
	Wallet*			get_wallet()		const	{return w;};
	int				get_amount()		const	{return amount;};
};



//Show
std::ostream& operator<<(std::ostream& out, const HTNRecord& h);


class BitCoin : public BinaryTree<HTNRecord>{
private:
	String Id;

public:
//constructor destructor
	BitCoin(const String& IId) : Id((assert(!IId.empty()), IId)) {};
	void initialize(Wallet* w, int init_amount) {assert(w != NULL); append_root(HTNRecord(NULL, w, init_amount));};

//debug
	void print_tree(std::ostream& out = std::cout) const;

//accessors
	String get_Id() const {return Id;};

//operations
	int do_transaction(Wallet* sent, Wallet* rec, int amount, Transaction* t, bool& still_owns);  //returns the reminder of the transaction
	List<Transaction*> get_history();
	int get_unspent_amount() const;
};

//relations
bool bitcoin_equal_by_name(const BitCoin& b1, const BitCoin& b2);
bool bitcoin_equal_by_name_ptr(BitCoin* b1, BitCoin* b2);

//Show
std::ostream& operator<<(std::ostream& out, const BitCoin& b);

/*
 **********
 * Wallet *
 **********
 */


class Wallet : public List<BitCoin*>{
friend void Transaction::do_transaction();

private:
	String Id;
	int amount;

//mutators
	void add_amount(int a) {amount += a;};

public: 
//constructor destructor
	Wallet(const String& IId, List<BitCoin*>& bcs, int init_amount);
	Wallet(const String& IId)
		: Id(IId), amount(0) {}; 

//accessor
	int 			get_amount()	const	{return amount;};
	String			get_Id()		const	{return Id;};
	List<BitCoin*>	get_coins()		const;
};

//Show
std::ostream& operator<<(std::ostream& out, const Wallet& w);

//relations
bool wallet_equal_by_name(const Wallet& w1, const Wallet& w2);
bool wallet_equal_by_name_ptr(Wallet* w1, Wallet* w2);


#endif

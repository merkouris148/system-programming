#include "transactions.h"

using namespace std;

/*
 ***************
 * Transaction *
 ***************
 */

//accessors
bool Transaction::feasible() const {return sent->get_amount() >= amount;}

//operations
void Transaction::do_transaction(){
	assert(not done);
	assert(("Transaction: Error: Invalid transaction!", amount <= sent->get_amount()));
	
	//find which coins are going to be tranfered from *sent to *receiv{
		auto coins = sent->get_coins();	//traverse *sent's coins
		auto cursor = coins.get_begin();
		
		int current_amount = amount;		//reminder amount
	
		List<BitCoin*> transfered_coins; 		//the coins which will be tranfered from *sent to *receiv
		while(current_amount > 0){
			BitCoin* Coin = cursor->get_data();
			
			bool still_owns;
			current_amount = Coin->do_transaction(sent, receiv, current_amount, this, still_owns);	//if *sent spent all money from a coin
			if(not still_owns) sent->erase(sent->get_first(Coin));									//*sent no longer posese the coin
																									//also current_amount (reminder amount)
																									//is updated
		
			transfered_coins.append(Coin);		//add to the list of the coins to be tranfered from *sent to *receiv
		
			if(cursor->has_next()) cursor = cursor->get_next();
		}
	//}
	
	
	receiv->insert(transfered_coins);	//sent coins to *receiv
	receiv->add_amount(amount);			//sent amount
	
	sent->add_amount(-amount);			//take amount from *sent
	
	//the transaction is done!
	done = true;
}

//Show
std::ostream& operator<<(std::ostream& out, const Transaction& t){
	out << "Transaction: " << t.get_Id() << endl; 
	out	<< "Sent: " << t.get_sent()->get_Id() << endl;
	out	<< "Receiv: " << t.get_receiv()->get_Id() << endl;
	out	<< "Amount: " << t.get_amount() << endl;
	out	<< "Date: " << t.get_date() << endl;
	
	return out;
}

/*
 ***********
 * Bitcoin *
 ***********
 */

//HTNRecord

//Show
std::ostream& operator<<(std::ostream& out, const HTNRecord& h){
	out << "(" << h.get_wallet()->get_Id() << "," <<  h.get_amount() << ")";
	
	return out;
}

//bitcoin

//debug
void BitCoin::print_tree(std::ostream& out /*=cout*/) const{
	if(!empty()) ::print_tree(out, get_root());
}

//operations
int BitCoin::do_transaction(Wallet* sent, Wallet* receiv, int amount, Transaction* t, bool& still_owns){
	
	auto leafs = get_leafs(); auto cursor = leafs.get_begin();	//traverse leafs
	int current_amount = amount;								//reminder amount
	still_owns = false;											//*sent still_owns *this coin after the end of transaction
	
	while(cursor->has_next() and current_amount > 0){
		
		HTNRecord parent_rec = cursor->get_data()->get_data();	//possible parent record
		auto parent_node = cursor->get_data();					//possible parent node
		
		if(parent_rec.get_wallet() == sent){
			
			int parent_amount = parent_rec.get_amount();
			if(parent_amount > current_amount){
				/* if *sent doesn't give all of his/her *this coin's shares to
				 * the *receiv still owns *this coin*/
				
				append_left_right(parent_node,
												HTNRecord(t, receiv, current_amount),
												HTNRecord(t, sent, parent_amount - current_amount)
								);
				current_amount = 0;
				still_owns = true;				//Owners has at least 1 node at the next level
			}									//So he/she still owns the coin
			
			//parent_amount <= current_amount
			else{
				append_left(parent_node, HTNRecord(t, receiv, parent_amount));
				current_amount -= parent_amount;
			}
		}
		
		cursor = cursor->get_next();
	}
	
	/*repeat for the last node*/
	if(current_amount > 0){
		HTNRecord parent_rec = cursor->get_data()->get_data();	//possible parent record
		auto parent_node = cursor->get_data();					//possible parent node
		
		if(parent_rec.get_wallet() == sent){
			
			int parent_amount = parent_rec.get_amount();
			if(parent_amount > current_amount){
				/* if *sent doesn't give all of his/her *this coin's shares to
				 * the *receiv still owns *this coin*/
				
				append_left_right(parent_node,
												HTNRecord(t, receiv, current_amount),
												HTNRecord(t, sent, parent_amount - current_amount)
								);
				current_amount = 0;
				still_owns = true;				//Owners has at least 1 node at the next level
			}									//So he/she still owns the coin
			//parent_amount <= current_amount
			else{
				append_left(parent_node, HTNRecord(t, receiv, parent_amount));
				current_amount -= parent_amount;
			}
		}
	}
	
	
	
	return current_amount;
}

void bitcoin_tree_get_history(BinaryTreeNode<HTNRecord>* Root, List<Transaction*>&  H){
	H.insert(Root->get_data().get_transaction());
	if(Root->has_left())	bitcoin_tree_get_history(Root->get_left(), H);
	if(Root->has_right())	bitcoin_tree_get_history(Root->get_right(), H);
}

List<Transaction*> BitCoin::get_history(){
	List<Transaction*> H;
	bitcoin_tree_get_history(get_root(), H);
	
	return H;
}

BinaryTreeNode<HTNRecord>* get_rightest_path(BinaryTreeNode<HTNRecord>* Root){
	if(Root->has_right())		get_rightest_path(Root->get_right());
	else if(Root->has_left())	return NULL;
	else 						return Root;
}

int BitCoin::get_unspent_amount() const{
	auto Rightest = get_rightest_path(get_root());
	if(Rightest == NULL)	return 0;
	else 					return Rightest->get_data().get_amount();
}

//Show
std::ostream& operator<<(std::ostream& out, const BitCoin& b){
	out << "BitCoinId: " << b.get_Id() <<  " Transaction Tree: "; b.print_tree(); out << "\n";

	return out;
}

//relations
bool bitcoin_equal_by_name(const BitCoin& b1, const BitCoin& b2) {return b1.get_Id() == b2.get_Id();}
bool bitcoin_equal_by_name_ptr(BitCoin* b1, BitCoin* b2) {return bitcoin_equal_by_name(*b1, *b2);}

/*
 **********
 * Wallet *
 **********
 */
 
//constructor destructor
Wallet::Wallet(const String& IId, List<BitCoin*>& bcs, int init_amount)
	:	Id((assert(!IId.empty()), IId)) {
		
		//intialize bitcoin list{
		if(!bcs.empty()){
			auto cursor = bcs.get_begin();
			while(cursor->has_next()){
				append( cursor->get_data() );						//the user owns the coin
				cursor->get_data()->initialize(this, init_amount);	//the coin is owned by the user
			
				cursor = cursor->get_next();
			}
		
			append( cursor->get_data() );						//the user owns the coin
			cursor->get_data()->initialize(this, init_amount);	//the coin is owned by the user
		}
		
		//initialize amount
		amount = bcs.get_size() * init_amount;
	}

//accessors
List<BitCoin*> Wallet::get_coins() const{
	return *this;
}
	
//Show
void print_bcn_ptr(BitCoin* b, std::ostream& out){
	out << *b;
}

std::ostream& operator<<(std::ostream& out, const Wallet& w){
	out << "WalletId: " << w.get_Id() << endl;
	out << "Current Amount: " << w.get_amount() << endl;
	out << "Coins in possession: " << endl;
	w.print(print_bcn_ptr, out);
	
	return out;
}	

//relations
bool wallet_equal_by_name(const Wallet& w1, const Wallet& w2) {return w1.get_Id() == w2.get_Id();}
bool wallet_equal_by_name_ptr(Wallet* w1, Wallet* w2) {return wallet_equal_by_name(*w1, *w2);}


#ifndef APP_H

#define APP_H

#include "../lib_containers/list.h"
#include "../lib_utilities/time.h"

#include "transactions.h"

class App{
private:
	List<BitCoin*> 		all_coins;
	List<Wallet*>		all_wallets;
	List<Transaction*>	all_transactions;
	
	bool exit_flag;
	
	int transaction_new_id;
	
public:
//constructor destructor
	App(
		const char* bitCoinBalanceFile,
		const char* transactionsFile,
		int bitCoinValue
	);
	
	~App();

//prompt
	void initiate_prompt();
	
//commands
	bool requestTransaction(const List<String>& args);
	bool requestTransaction_single(const List<String>& args);
	bool requestTransaction_list(const List<String>& args);
	bool requestTransaction_file(const List<String>& args);
	
	bool findEarnings(const List<String>& args);		//needs hashtable
	bool findPayments(const List<String>& args);		//needs hashtable
	
	bool walletStatus(const List<String>& args);
	bool bitCoinStatus(const List<String>& args);

	bool traceCoin(const List<String>& args);
	
	bool exit(const List<String>& args);
	
//debug
	bool dbg_print_all(const List<String>& args);
	bool dbg_print_coin(const List<String>& args);
	bool dbg_print_wallet(const List<String>& args);
};





#endif

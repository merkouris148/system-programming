#include "bitcoin_app.h"

#include <cassert>
#include <cstdlib>

#include <iostream>

#include "../lib_system/file_io.h"
#include "../lib_containers/string.h"
#include "../lib_containers/array.h"

#include "bitcoin_app_helper.h"

const char* delimeter = "\t -:;";

using namespace std;

App::App(
		const char* bitCoinBalanceFile,
		const char* transactionsFile,
		int bitCoinValue
	){
		assert(bitCoinValue >= 1);
		assert(bitCoinBalanceFile != NULL and transactionsFile != NULL);
		
		
		handle_bitCoinBalanceFile(bitCoinBalanceFile, bitCoinValue, all_coins, all_wallets);
		handle_transactionsFile(transactionsFile, all_wallets, all_transactions);
		do_all_transactions(all_transactions);
		
		transaction_new_id = 1;
		exit_flag = false;
	}
	
App::~App(){
	all_coins.clear_pointers();
	all_wallets.clear_pointers();
	all_transactions.clear_pointers();	
}

//prompt
void App::initiate_prompt(){
	exit_flag = false;
	
	file_in in;
	
	const int command_num = 10;
	
	Array<String>								command_name(command_num);
	Array<bool (App::*) (const List<String>&) >	command_action(command_num);
	
	command_name[0] = "/requestTransaction";	command_action[0] = &App::requestTransaction;
	
	command_name[1] = "/findEarnings";			command_action[1] = &App::findEarnings;
	command_name[2] = "/findPayments";			command_action[2] = &App::findPayments;
	
	command_name[3] = "/walletStatus";			command_action[3] = &App::walletStatus;
	command_name[4] = "/bitCoinStatus";			command_action[4] = &App::bitCoinStatus;
	command_name[5] = "/traceCoin";				command_action[5] = &App::traceCoin;
	
	command_name[6] = "/exit";					command_action[6] = &App::exit;
	
	//debug
	command_name[7] = "/dbg_print_all";			command_action[7] = &App::dbg_print_all;
	command_name[8] = "/dbg_print_coin";		command_action[8] = &App::dbg_print_coin;
	command_name[9] = "/dbg_print_wallet";		command_action[9] = &App::dbg_print_wallet;
	
	cout << "How can I help?" << endl;
	while(not exit_flag){
		String command = in.getline();										/* read command from user*/		if(command.empty()) continue;
		List<String> tokens = tokenize_string(command, delimeter);			//tokenize command
		auto command_head = tokens.get_begin()->get_data();					//get command name
		
		if( not command_name.exists(command_head) ){
			cout << "Error: such command doesn't exists!" << endl;
			continue;
		}
		int action_i = command_name.get_first(command_head);				//find command in commands
		
		
		//special treatment for /requestTransaction list version{
		//----------------------------------------------------------------------------------------------------------------------------			
					if(action_i == 0 and command[command.get_length()-1] == ';'){
						tokens.append(";");
						command = in.getline();
					
						while(!command.empty()){
							tokens.append(tokenize_string(command, delimeter));
							tokens.append(";");
						
							command = in.getline();
						}
					}	
			/* /requestTransaction list version arguments will be in the form <transaction><;>(<transaction><;>)*  */
		//-----------------------------------------------------------------------------------------------------------------------------
		//}
		
		
		bool succ = (this->*command_action[action_i])(tokens);
		
		if(not succ) 	cout << "\nError: command " << command_name[action_i] << " failed!" << endl;
		else 			{if(action_i == 6) cout << "\nBye!" << endl; else cout << "\nDone!" << endl;}
	}
}



//commands

bool App::requestTransaction(const List<String>& args){
	//debug
	//cout << "debug: " << args << endl;
	
	if(args.get_size() == 2)										return requestTransaction_file(args);
	if(args.get_size() == 9 )										return requestTransaction_single(args);
	if(args.get_size() >= 9 /*and (args.get_size()-1) % 9 == 0*/ )	return requestTransaction_list(args);
	
	//else
	cout << "Error: invalid number of arguments!" << endl;
	
	return false;
}


bool App::requestTransaction_file(const List<String>& args){
	String filename = args.get_begin()->get_next()->get_data();
	if(not file_exists(filename)){		
		cout << "Error: file does not exists!" << endl;
		return false;
	}
	
	List<List<String>> many_tokens;
	gettokens(filename, many_tokens, delimeter);
	
	List<Transaction*> Trans;
	transaction_new_id = make_transactions(all_wallets, Trans, many_tokens, transaction_new_id);
	
	all_transactions.append(Trans); do_all_transactions(Trans);
	
	return true;
}

bool App::requestTransaction_single(const List<String>& args){
	Transaction* T;
	bool err;
		transaction_new_id = make_transaction(all_wallets, T, args.get_tail(), transaction_new_id, err);
	if(err) return false;
	
	if(T->feasible())	{all_transactions.append(T); T->do_transaction();}
	else 				{cout << "Warning: transaction not feasible!" << endl; return false;}
	
	return true;
}

bool App::requestTransaction_list(const List<String>& args){
	List<List<String>> many_tokens = tokenize_list(args.get_tail(), String(";"));
	
	//debug
	//cout << "debug: " << many_tokens << endl;
	
	List<Transaction*> Trans;
	transaction_new_id = make_transactions(all_wallets, Trans, many_tokens, transaction_new_id);
	
	all_transactions.append(Trans); do_all_transactions(Trans);
	
	return true;
}
	
bool App::findEarnings(const List<String>& args){ cout << "I'm sorry, not implemented!" << endl; return true; }
bool App::findPayments(const List<String>& args){ cout << "I'm sorry, not implemented!" << endl; return true; }
	

bool App::walletStatus(const List<String>& args){
	String walletId = args.get_begin()->get_next()->get_data();
	
	Wallet* q = new Wallet(walletId);
	if(not all_wallets.exists(q, wallet_equal_by_name_ptr)){
		cout << "Warning: wallet Id doesn't exist" << endl;
		delete q;
		return false;
	}
	
	Wallet* w = all_wallets.get_first(q, wallet_equal_by_name_ptr)->get_data();
	cout << "the remainig amount of wallet: " << walletId << " = " << w->get_amount() << endl;
	
	delete q;
	return true;
}

bool App::bitCoinStatus(const List<String>& args){
	String coinId = args.get_begin()->get_next()->get_data();
	
	BitCoin* q = new BitCoin(coinId);
	if(not all_coins.exists(q, bitcoin_equal_by_name_ptr)){
		cout << "Warning: bitcoin Id doesn't exist" << endl;
		delete q;
		return false;
	}
	
	BitCoin* b = all_coins.get_first(q, bitcoin_equal_by_name_ptr)->get_data();
	auto History = b->get_history();
	cout << "Bitcoin " << coinId << " used in " << History.get_size()-1 << " transaction(s)." << endl;
	cout << "The amount left unspent is " << b->get_unspent_amount() << endl;
	
	delete q;
	return true;
}


bool App::traceCoin(const List<String>& args){
	String coinId = args.get_begin()->get_next()->get_data();
	
	BitCoin* q = new BitCoin(coinId);
	if(not all_coins.exists(q, bitcoin_equal_by_name_ptr)){
		cout << "Warning: bitcoin Id doesn't exist" << endl;
		delete q;
		return false;
	}
	
	BitCoin* b = all_coins.get_first(q, bitcoin_equal_by_name_ptr)->get_data();
	auto History = b->get_history();
	History.print([](Transaction* t, ostream& out){if(t != NULL) out << *t << endl;});
	
	delete q;
	return true;
}


bool App::exit(const List<String>& args){
	exit_flag = true;
	
	if(args.get_size() > 1) cout << "Warning: /exit command takes no arguments, arguments given are discarted!" << endl;
	
	return true;
}

//debug
bool App::dbg_print_all(const List<String>& args){
	cout << "\nCoins" << endl; 			all_coins.print([](BitCoin* b, ostream& out){ out << *b;});
	cout << "\nWallet" << endl;			all_wallets.print([](Wallet* b, ostream& out){ out << *b;});
	cout << "\nTransactions" << endl;	all_transactions.print([](Transaction* b, ostream& out){ out << *b;});
	
	return true;
}

bool App::dbg_print_coin(const List<String>& args){
	String coinId = args.get_begin()->get_next()->get_data();
	
	BitCoin* q = new BitCoin(coinId);
	if(not all_coins.exists(q, bitcoin_equal_by_name_ptr)){
		cout << "Warning: bitcoin Id doesn't exist" << endl;
		delete q;
		return false;
	}
	
	BitCoin* b = all_coins.get_first(q, bitcoin_equal_by_name_ptr)->get_data();
	cout << *b << endl;
	
	delete q;
	return true;
}


bool App::dbg_print_wallet(const List<String>& args){
	String walletId = args.get_begin()->get_next()->get_data();
	
	Wallet* q = new Wallet(walletId);
	if(not all_wallets.exists(q, wallet_equal_by_name_ptr)){
		cout << "Warning: wallet Id doesn't exist" << endl;
		delete q;
		return false;
	}
	
	Wallet* w = all_wallets.get_first(q, wallet_equal_by_name_ptr)->get_data();
	cout << *w << endl;
	
	delete q;
	return true;
}

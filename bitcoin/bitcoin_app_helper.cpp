#include "bitcoin_app_helper.h"

#include <cassert>

#include <iostream>

#include "../lib_system/file_io.h"
#include "../lib_containers/string.h"

using namespace std;

const char* help_delimeter = "\t -:;";

//constructor destructor
void handle_bitCoinBalanceFile(
								const char* bitCoinBalanceFile,
								int bitCoinValue,
								List<BitCoin*>& all_coins,
								List<Wallet*>& all_wallets
							){ 
	file_in in(bitCoinBalanceFile);
	List<String> lines; in.getlines(lines);
	
	auto cursor = lines.get_begin();
	while(cursor->has_next()){												//skip empty lines
		String line = cursor->get_data();									if(line.empty()) continue;
		auto tokens = tokenize_string(line, " \t");
		
		//the first token is the wallet ID
		auto token_cur = tokens.get_begin();
		auto walletID = token_cur->get_data();
		List<BitCoin*> wallet_coins; //<-- []				//initialize wallets coins to [] empty list
		
		//if wallet has coins
		if(tokens.get_size() > 1){
			//initialize coins first
			token_cur = token_cur->get_next();
			while(token_cur->has_next()){
				wallet_coins.append(new BitCoin(token_cur->get_data()));
			
				token_cur = token_cur->get_next();
			}
			
			//last iteration
			wallet_coins.append(new BitCoin(token_cur->get_data()));
			
			//append wallet's coins to all coins
			all_coins.append(wallet_coins);
		}
		
		//itiniatize wallet
		all_wallets.append(new Wallet(walletID, wallet_coins, bitCoinValue));
		
		cursor = cursor->get_next();
	}
	
	
	
	//last iteration{
		String line = cursor->get_data();									if(line.empty()) return;
		auto tokens = tokenize_string(line, " \t");
		
		//the first token is the wallet ID
		auto token_cur = tokens.get_begin();
		auto walletID = token_cur->get_data();
		List<BitCoin*> wallet_coins; //<-- []				//initialize wallets coins to [] empty list
		
		//if wallet has coins
		if(tokens.get_size() > 1){
			//initialize coins first
			token_cur = token_cur->get_next();
			while(token_cur->has_next()){
				wallet_coins.append(new BitCoin(token_cur->get_data()));
			
				token_cur = token_cur->get_next();
			}
			
			//last iteration
			wallet_coins.append(new BitCoin(token_cur->get_data()));
			
			//append wallet's coins to all coins
			all_coins.append(wallet_coins);
		}
		
		//itiniatize wallet
		all_wallets.append(new Wallet(walletID, wallet_coins, bitCoinValue));
	//}
}


void handle_transactionsFile(
							const char* transactionsFile,
							const List<Wallet*>& all_wallets,
							List<Transaction*>& all_transactions
							){
	file_in in(transactionsFile);
	List<String> lines; in.getlines(lines);
	
	auto cursor = lines.get_begin();
	while(cursor->has_next()){												//skip empty lines
		String line = cursor->get_data();									if(line.empty()) continue;
		auto tokens = tokenize_string(line, help_delimeter);				if(tokens.get_size() != 9) {cout << "debug" << tokens << endl; exit(1);}
		auto token_cur = tokens.get_begin();
		
		String transID 	= token_cur->get_data();	token_cur = token_cur->get_next();
		String sentID	= token_cur->get_data();	token_cur = token_cur->get_next();
		String receID	= token_cur->get_data();	token_cur = token_cur->get_next();
		String amos		= token_cur->get_data();	token_cur = token_cur->get_next();
		String days		= token_cur->get_data();	token_cur = token_cur->get_next();
		String mons		= token_cur->get_data();	token_cur = token_cur->get_next();
		String yeas		= token_cur->get_data();	token_cur = token_cur->get_next();
		String hous		= token_cur->get_data();	token_cur = token_cur->get_next();
		String mins		= token_cur->get_data();
		
		int amount	= amos.stoi();
		
		int day 	= days.stoi();		if(not is_day(day)) 	{cout << "Warning: wrong date format!" << endl; cursor = cursor->get_next(); continue;}
		int mon 	= mons.stoi();		if(not is_mon(mon)) 	{cout << "Warning: wrong date format!" << endl; cursor = cursor->get_next(); continue;}
		int year	= yeas.stoi();		if(not is_yea(year)) 	{cout << "Warning: wrong date format!" << endl; cursor = cursor->get_next(); continue;}
		int hour 	= hous.stoi();		if(not is_hou(hour)) 	{cout << "Warning: wrong date format!" << endl; cursor = cursor->get_next(); continue;}
		int min		= mins.stoi();		if(not is_min(min)) 	{cout << "Warning: wrong date format!" << endl; cursor = cursor->get_next(); continue;}
		
		
		
		Wallet* q = new Wallet(sentID);
			if(not all_wallets.exists(q, wallet_equal_by_name_ptr)) {cout << "Warning: senter not found!" << endl; delete q; cursor = cursor->get_next(); continue;}
			Wallet* sent = all_wallets.get_first(q, wallet_equal_by_name_ptr)->get_data();
		delete q;
		
		Wallet* p = new Wallet(receID);
			if(not all_wallets.exists(p, wallet_equal_by_name_ptr)) {cout << "Warning: receiver not found!" << endl; delete p; cursor = cursor->get_next(); continue;}
			Wallet* receiv = all_wallets.get_first(p, wallet_equal_by_name_ptr)->get_data();
		delete p;
		
		
		
		all_transactions.append( new Transaction( transID, sent, receiv, amount, Time(day, mon, year, hour, min) ) );
		
		cursor = cursor->get_next();
	}
	
	
	//last iteration{
		String line = cursor->get_data();									if(line.empty()) return;
		auto tokens = tokenize_string(line, help_delimeter);				assert(tokens.get_size() == 9);
		auto token_cur = tokens.get_begin();
		
		String transID 	= token_cur->get_data();	token_cur = token_cur->get_next();
		String sentID	= token_cur->get_data();	token_cur = token_cur->get_next();
		String receID	= token_cur->get_data();	token_cur = token_cur->get_next();
		String amos		= token_cur->get_data();	token_cur = token_cur->get_next();
		String days		= token_cur->get_data();	token_cur = token_cur->get_next();
		String mons		= token_cur->get_data();	token_cur = token_cur->get_next();
		String yeas		= token_cur->get_data();	token_cur = token_cur->get_next();
		String hous		= token_cur->get_data();	token_cur = token_cur->get_next();
		String mins		= token_cur->get_data();
		
		int amount	= amos.stoi();
		
		int day 	= days.stoi();		if(not is_day(day)) 	{cout << "Warning: wrong date format!" << endl; return;}
		int mon 	= mons.stoi();		if(not is_mon(mon)) 	{cout << "Warning: wrong date format!" << endl; return;}
		int year	= yeas.stoi();		if(not is_yea(year)) 	{cout << "Warning: wrong date format!" << endl; return;}
		int hour 	= hous.stoi();		if(not is_hou(hour)) 	{cout << "Warning: wrong date format!" << endl; return;}
		int min		= mins.stoi();		if(not is_min(min)) 	{cout << "Warning: wrong date format!" << endl; return;}
		
		Wallet* q = new Wallet(sentID);
			if(not all_wallets.exists(q, wallet_equal_by_name_ptr)) {cout << "Warning: senter not found!" << endl; delete q; return;}
			Wallet* sent = all_wallets.get_first(q, wallet_equal_by_name_ptr)->get_data();
		delete q;
		
		Wallet* p = new Wallet(receID);
			if(not all_wallets.exists(p, wallet_equal_by_name_ptr)) {cout << "Warning: receiver not found!" << endl; delete p; return;}
			Wallet* receiv = all_wallets.get_first(p, wallet_equal_by_name_ptr)->get_data();
		delete p;
		
		
		
		all_transactions.append( new Transaction( transID, sent, receiv, amount, Time(day, mon, year, hour, min) ) );
	//}
}

void do_all_transactions(List<Transaction*>& all_transactions){
	if(all_transactions.empty())  return;
	
	auto cursor = all_transactions.get_begin();
	while(cursor->has_next()){
		if(cursor->get_data()->feasible()) 	cursor->get_data()->do_transaction();
		else 								cout << "Warning: transaction " << cursor->get_data()->get_Id() <<  " isn't feasible!" << endl;
		
		cursor = cursor->get_next();
	}
	
	if(cursor->get_data()->feasible()) 	cursor->get_data()->do_transaction();
	else 								cout << "Warning: transaction " << cursor->get_data()->get_Id() <<  " isn't feasible!" << endl;
}

int make_transaction(
						const List<Wallet*>& all_wallets,
						Transaction*& T,
						const List<String>& args,
						int transaction_new_id,
						
						bool& err
					){
	err = false;
	
	//if(args.get_size() != 8) {cout << "Error: Wrong inline transactions format: " << args << endl; return transaction_new_id;}
	auto token_cur = args.get_begin();
		
	String transID("o"); transID += itoa(transaction_new_id);
	
	String sentID	= token_cur->get_data();	token_cur = token_cur->get_next();
	String receID	= token_cur->get_data();	token_cur = token_cur->get_next();
	String amos		= token_cur->get_data();	token_cur = token_cur->get_next();
	String days		= token_cur->get_data();	token_cur = token_cur->get_next();
	String mons		= token_cur->get_data();	token_cur = token_cur->get_next();
	String yeas		= token_cur->get_data();	token_cur = token_cur->get_next();
	String hous		= token_cur->get_data();	token_cur = token_cur->get_next();
	String mins		= token_cur->get_data();
	
	int amount	= amos.stoi();
	
	int day 	= days.stoi();		if(not is_day(day)) 	{cout << "Warning: wrong date format!" << endl; err = true; return transaction_new_id;}
	int mon 	= mons.stoi();		if(not is_mon(mon)) 	{cout << "Warning: wrong date format!" << endl; err = true; return transaction_new_id;}
	int year	= yeas.stoi();		if(not is_yea(year)) 	{cout << "Warning: wrong date format!" << endl; err = true; return transaction_new_id;}
	int hour 	= hous.stoi();		if(not is_hou(hour)) 	{cout << "Warning: wrong date format!" << endl; err = true; return transaction_new_id;}
	int min		= mins.stoi();		if(not is_min(min)) 	{cout << "Warning: wrong date format!" << endl; err = true; return transaction_new_id;}
	
	
	Wallet* q = new Wallet(sentID);
		if(not all_wallets.exists(q, wallet_equal_by_name_ptr)) {cout << "Warning: senter not found!" << endl; delete q; err = true; return transaction_new_id;}
		Wallet* sent = all_wallets.get_first(q, wallet_equal_by_name_ptr)->get_data();
	delete q;
	
	Wallet* p = new Wallet(receID);
		if(not all_wallets.exists(p, wallet_equal_by_name_ptr)) {cout << "Warning: receiver not found!" << endl; delete p; err = true; return transaction_new_id;}
		Wallet* receiv = all_wallets.get_first(p, wallet_equal_by_name_ptr)->get_data();
	delete p;
	
	
	T = new Transaction( transID, sent, receiv, amount, Time(day, mon, year, hour, min) );
	
	return transaction_new_id+1;
}

int make_transactions(
						const List<Wallet*>& all_wallets,
						List<Transaction*>& Trans,
						const List<List<String>>& many_tokens,
						int transaction_new_id
					){
	auto cur = many_tokens.get_begin();
	while(cur->has_next()){
		Transaction* T;
		bool err;
			transaction_new_id = make_transaction(all_wallets, T, cur->get_data(), transaction_new_id, err);
		if(err) {cur = cur->get_next(); continue;}
		
		Trans.append(T);
		
		cur = cur->get_next();
	}
	
	//last iteration{
		Transaction* T;
		bool err;
			transaction_new_id = make_transaction(all_wallets, T, cur->get_data(), transaction_new_id, err);
		if(err) return transaction_new_id;
		
		Trans.append(T);
	//}
	
	return transaction_new_id;
}

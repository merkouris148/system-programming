#ifndef APP_HELPER

#define APP_HELPER

#include "../lib_containers/list.h"
#include "transactions.h"

void handle_bitCoinBalanceFile(
								const char* bitCoinBalanceFile,
								int bitCoinValue,
								List<BitCoin*>& all_coins,
								List<Wallet*>& all_wallets
							);

void handle_transactionsFile(
							const char* transactionsFile,
							const List<Wallet*>& all_wallets,
							List<Transaction*>& all_transactions
							);
							
void do_all_transactions(List<Transaction*>& all_transactions);

int make_transaction(
						const List<Wallet*>& all_wallets,
						Transaction*& T,
						const List<String>& args,
						int transaction_new_id,
						
						bool& err
					);

int make_transactions(
						const List<Wallet*>& all_wallets,
						List<Transaction*>& Trans,
						const List<List<String>>& many_tokens,
						int transaction_new_id
					);

#endif

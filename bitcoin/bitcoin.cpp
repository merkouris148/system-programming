#include "bitcoin_app.h"

#include <cstring>

using namespace std;

/*
 *********************
 * Helping Functions *
 *********************
 */

bool check_arguments(int argc, char** argv){
	if(!(argc % 2 == 1))	return false;
	if(argc < 2)			return false;	
	
	//skip first argument (name of the func)
	//check if there is "-a", "-v", "-t" in args
	//check only odds args
	bool exist_bitcoin_balance_file	= false;
	bool exist_bitcoin_value		= false;
	bool exist_transaction_file		= false;
	for(int i = 1; i <= (argc-1); i += 2){
		if(strcmp(argv[i], "-a") == 0)		  	exist_bitcoin_balance_file	= true;
		else if(strcmp(argv[i], "-v") == 0)		exist_bitcoin_value			= true;
		else if(strcmp(argv[i], "-t") == 0)		exist_transaction_file		= true;
	}
	
	return exist_bitcoin_balance_file and exist_bitcoin_value and exist_transaction_file;
}

void get_args(
				int argc,
				char** argv,
				char*& bitcoin_balance_file,
				char*& transaction_file,
				int& bitcoin_value
			){
	for(int i = 1; i <= (argc-1); i += 2){
		if(strcmp(argv[i], "-a") == 0)  		bitcoin_balance_file	= argv[i+1];
		else if(strcmp(argv[i], "-t") == 0)		transaction_file	 	= argv[i+1];
		else if(strcmp(argv[i], "-v") == 0)		bitcoin_value			= atoi(argv[i+1]);
	}
}

int main(int argc, char** argv){
	
	char* bitcoin_balance_file;
	char* transaction_file;
	int bitcoin_value;
	
	assert(check_arguments(argc, argv));
	get_args(argc, argv, bitcoin_balance_file, transaction_file, bitcoin_value);
	
	App A(bitcoin_balance_file, transaction_file, bitcoin_value);
	A.initiate_prompt();
	
	return 0;
}

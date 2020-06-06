###########################################################
# This code has been tested in Python, version 3.6.9
#
# Input: num_of_users,	the number of records in the 
#						balance input file,
#						sug. value: 100
#
#		num_of_transactions, 	the number of records in
#								the transaction input file,
#								sug. value: 100
#		
#		number_of_bitcoins,	the number of bitcoins
#							available,
#							sug. value: 1000
#
# Output:	a balance file of name "balance.in"
#			a transactions file of name "transactions.in"
#
# Author: Merkouris Papamichail
# email: sdi1400148@di.uoa.gr
###########################################################

import sys
from random import randint

num_of_users = int(sys.argv[1])
num_of_transactions = int(sys.argv[2])
num_of_bitcoins = int(sys.argv[3])

max_num_bc_for_user = 10
assert(num_of_bitcoins > max_num_bc_for_user)

bitcoinIds = range(1, num_of_bitcoins + 1)

bitcoinAvailable = [True]*num_of_bitcoins
user_prefix = "User"
balance_file = open("balance.in", "w")

# create balance.in
for i in range(1, num_of_users+1):
	line = user_prefix + str(i) + " "
	# add bitcoins
	bitcoins_n = randint(1, max_num_bc_for_user)
	bitcoins = ""
	for j in range(1, bitcoins_n+1):
		k = randint(0, num_of_bitcoins-1)
		if bitcoinAvailable[k]:
			bitcoinAvailable[k] = False
			bitcoins = bitcoins + " " + str(bitcoinIds[k])
	
	line = line + bitcoins + "\n"
	balance_file.write(line)

balance_file.close()

transactions_file = open("transactions.in", "w")
max_trans_amount = 1000
# create transactions.in
for i in range(1, num_of_transactions+1):
	user1 = randint(1, num_of_users)
	user2 = randint(1, num_of_users)
	amount = randint(1, max_trans_amount)
	day = randint(1, 30)
	month = randint(1, 12)
	year = randint(1900, 2020)
	hour = randint(0, 24)
	minute = randint(0, 59)
	line = str(i) + " " + user_prefix + str(user1) + " " + user_prefix + str(user2) + " " + str(amount) + " " + str(day) + "-" + str(month) + "-" + str(year) + " " + str(hour) + ":" + str(minute) + "\n"
	transactions_file.write(line)

transactions_file.close()

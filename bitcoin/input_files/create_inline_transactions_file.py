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
# Output: 	an inline transactions file of name
#			"inline_transactions.in"
#
# Author: Merkouris Papamichail
# email: sdi1400148@di.uoa.gr
###########################################################

import sys
from random import randint

num_of_users = int(sys.argv[1])
num_of_transactions = int(sys.argv[2])

user_prefix = "User"

transactions_file = open("inline_transactions.in", "w")
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
	line = user_prefix + str(user1) + " " + user_prefix + str(user2) + " " + str(amount) + " " + str(day) + "-" + str(month) + "-" + str(year) + " " + str(hour) + ":" + str(minute) + ";" + "\n"
	transactions_file.write(line)

transactions_file.close()

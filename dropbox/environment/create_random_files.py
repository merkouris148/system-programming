###########################################################
# This code has been tested in Python, version 3.6.9
#
# Input: prefix, the prefix of all files to be created
#
#		num_of_files, the number of files to be created
#
#		max_file_size, the maximum size (in chars), of
#						a file.
#
# Output: num_of_files number of random generated files
#
# Author: Merkouris Papamichail
# email: sdi1400148@di.uoa.gr
###########################################################

import sys
from random import randint, choices

lorem_ipsum = [
				"Lorem ipsum dolor sit amet consectetur adipiscing elit sed",
				"do eiusmod tempor incididun ut labore et dolore magna aliqua.",
				"Ut enim ad minim veniam quis nostrud exercitation ullamco laboris",
				"nisi ut aliquip ex ea commodo consequat Duis aute irure dolor",
				"in reprehenderit in voluptate velit esse cillum dolore eu fugiat",
				"nulla pariatur. Excepteur sint occaecat cupidatat non proident sunt",
				"in culpa qui officia deserunt mollit anim id est laborum."
			]

## Main

prefix = sys.argv[1]
num_of_files = int(sys.argv[2])
max_file_size = int(sys.argv[3])

assert(num_of_files > 0 and max_file_size > 0)

for i in range(1, num_of_files+1):
	file_name = prefix + str(i) + ".txt"
	file_size = randint(1, max_file_size)
	random_text = "\n".join(choices(lorem_ipsum, k=file_size))
	f = open(file_name, "w")
	f.write(random_text)
	f.close()

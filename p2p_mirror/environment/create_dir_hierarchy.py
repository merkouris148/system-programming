###########################################################
# This code has been tested in Python, version 3.6.9
#
# Input: base, the name of the base directory		
# 
# 		 max_depth,	the maximum hight of the directory
#					tree
#
#		max_children, the maximum number of subdirectories
#						or files a direcctory may have
#
#		max_file_size, the maximum size (in chars), of
#						a file.
#
# Output: A random generated hierarchy of directories
#			under base dir
#
# Author: Merkouris Papamichail
# email: sdi1400148@di.uoa.gr
###########################################################

import sys, os
from random import randint, choices

def queue_pop(Q):
	elem = Q[0]
	Q.remove(Q[0])
	return elem

def queue_insert(Q, elem):
	Q.append(elem)

def create_subdirs(Queue, prefix, max_depth, max_children, max_file_size, current_depth):
	head = queue_pop(Queue)
	os.mkdir(head)
	num_children = randint(1, max_children)
	for i in range(1, num_children):
		make_dir = randint(0,1)
		if make_dir:
			dir_name = head + prefix + str(i) + "/"
			queue_insert(Queue, dir_name)
		else:
			file_name = head + prefix + str(i) + ".txt"
			file_size = randint(1, max_file_size)
			random_text = "\n".join(choices(lorem_ipsum, k=file_size))
			f = open(file_name, "w")
			f.write(random_text)
			f.close()
	
	current_depth = current_depth + 1
	if current_depth < max_depth and Queue != []:
		create_subdirs(Queue, prefix, max_depth, max_children, max_file_size, current_depth)

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

base = sys.argv[1]
max_depth = int(sys.argv[2])
max_children = int(sys.argv[3])
max_file_size = int(sys.argv[4])

assert(base[0:1] != "./" and base[-1] != "/")

prefix = base
base = "./" + base + "/"

Queue = [base]
create_subdirs(Queue, prefix, max_depth, max_children, max_file_size, 0)

#include "senter_receiver.h"

#include <iostream>

#include "../lib_system/directory.h"
#include "../lib_system/path.h"

const bool stdout_messeges = true;

using namespace std;

/*
 **********
 * Senter *
 **********
 */
 
//private methods
void Senter::write_to_log(const String& messege){
	String log_messege = "Senter [" + senter_id + " : " + receiver_id + "] " + messege + "\n";
	log_file.putline(log_messege);
	
	if(stdout_messeges)  cout << log_messege << endl;
}

//constructor destructor
Senter::Senter(
				const String& iinput_dir_path,
				const String& ssenter_id,
				const String& rreceiver_id,
				
				const String& pipe_name,
				
				file_app& llog_file
			) :
				input_dir_path(iinput_dir_path),
				senter_id(ssenter_id),
				receiver_id(rreceiver_id),
							
				log_file(llog_file),
				
				pipe(pipe_name)
			{}

//operations
void Senter::sent_files(){
	auto files = get_leafs(input_dir_path);
	auto cursor = files.get_begin();
	
	for(;cursor->has_next(); cursor = cursor->get_next()){
		String path = cursor->get_data();
		
		//write file paths length
		pipe.write_short_int(path.get_length());  			write_to_log(itoa(path.get_length()));
		
		//write path
		pipe.write(path);  									write_to_log(path);
		
		//get file
		String sfile;
		if(is_file(path)) 	sfile = getfile(path);
		//else  			sfile <- e
		
		//write file's length
		pipe.write_int(sfile.get_length());  				write_to_log(itoa(sfile.get_length()));
		
		//write file
		pipe.write(sfile);  								write_to_log(sfile);
	}
	
	//last iteraton{
		String path = cursor->get_data();
		
		//write file paths length
		pipe.write_short_int(path.get_length());  			write_to_log(itoa(path.get_length()));
		
		//write path
		pipe.write(path);									write_to_log(path);
		
		//get file
		String sfile;
		if(is_file(path)) 	sfile = getfile(path);
		//else  			sfile <- e
		
		//write file's length
		pipe.write_int(sfile.get_length());  				write_to_log(itoa(sfile.get_length()));
		
		//write file
		pipe.write(sfile);  								write_to_log(sfile);
	//}
	
	//write last 00
	pipe.write_short_int(0);	  							write_to_log("00");  write_to_log("SENTER END");
}


/*
 ************
 * Receiver *
 ************
 */

//private methods
void Receiver::write_to_log(const String& messege){
	String log_messege = "Receiver [" + senter_id + " : " + receiver_id + "] " + messege + "\n";
	log_file.putline(log_messege);
	
	if(stdout_messeges)  cout << log_messege << endl;
}

//constructor destructor
Receiver::Receiver(
				const String& mmirror_path,
				const String& ssenter_id,
				const String& rreceiver_id,
				
				const String& pipe_name,
				
				file_app& llog_file
			) :
				mirror_path(mmirror_path),
				senter_id(ssenter_id),
				receiver_id(rreceiver_id),
			
				log_file(llog_file),
			
				pipe(pipe_name)
			{}

//operations

void Receiver::receive_files(){
	while(true){
		//read path len
		int path_len = pipe.read_short_int();		write_to_log(itoa(path_len));
		
		//00 reached
		if(path_len == 0)	break;
		
		//read path
		String path = pipe.read(path_len);			write_to_log(path);
		
		//read file len
		int file_len = pipe.read_int();				write_to_log(itoa(file_len));
		
		//read file
		String file = pipe.read(file_len);			write_to_log(file);
		
		//create file
		create_path(mirror_path, path, file);
	}
	
	write_to_log("RECEIVER END");
}

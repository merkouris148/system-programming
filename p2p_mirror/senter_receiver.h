#ifndef SENTER_RECEIVER_H

#define SENTER_RECEIVER_H

#include "../lib_system/pipes.h"
#include "../lib_system/file_io.h"
#include "../lib_containers/string.h"

/*
 **********
 * Senter *
 **********
 */

class Senter{
private:
	String input_dir_path;
	String senter_id;
	String receiver_id;
		
	pipe_w pipe;
	
	file_app& log_file;

//private methods
	void write_to_log(const String& messege);

public:
//constructor destructor
	Senter(
			const String& iinput_dir_path,
			const String& ssenter_id,
			const String& rreceiver_id,
			
			const String& pipe_name,	//a relative or absolute path to common_dir in the form
										//./<common dir name> WITHOUT '/' at end!!
			
			file_app& log_file
		);
	
	//~Senter();

//operations
	void sent_files();
};


/*
 ************
 * Receiver *
 ************
 */

class Receiver{
private:
	String mirror_path;
	String receiver_id;
	String senter_id;
	
	pipe_r pipe;
	
	file_app& log_file;

//private methods
	void write_to_log(const String& messege);

public:
//constructor destructor
	Receiver(
			const String& mmirror_path,
			const String& ssenter_id,
			const String& rreceiver_id,
			
			const String& pipe_name,
			
			file_app& log_file
		);
	
	//~Senter();

//operations
	void receive_files();
	void create_file(const String& path, const String& file);
};

#endif

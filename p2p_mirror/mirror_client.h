#ifndef MIRROR_CLIENT_H

#define MIRROR_CLIENT_H

#include "../lib_containers/list.h"
#include "../lib_containers/string.h"
#include "../lib_system/file_io.h"

class MirrorClient{
private:
	String Id;
	List<String> current_ids;
	
	String senter_path;
	String receiver_path;
	
	String mirror_dir;
	String input_dir;
	String common_dir;

	String log_file;
	file_app log;

//private methods
	void sent_files(const List<String>& ids);
	String make_senter_args(const String& receiver_id);
	
	void receive_files(const List<String>& ids);
	String make_receiver_args(const String& senter_id);
	
	List<String> get_current_ids();

public:
//constructor destructor
	MirrorClient(
					const String& IId,
					
					const String& ssenter_path,
					const String& rreceiver_path,
					
					const String& mmirror_dir,
					const String& iinput_dir,
					const String& ccommon_dir,
					
					const String& llog_file
				);

	~MirrorClient();

//operations
	void operate();
	
//log
	void write_to_log(const String& messege);
};


//operations
bool is_id_file(const String& path);
List<String> get_current_ids(const String& path);

#endif

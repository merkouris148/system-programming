#include "mirror_client.h"

#include <sys/types.h>
#include <unistd.h>

#include "../lib_containers/list_set.h"
#include "../lib_system/path.h"
#include "../lib_system/pipes.h"

#include "../lib_system/spawn_child_process.h"
#include "../lib_system/directory.h"

#include <iostream>

using namespace std;

//consts
const bool client_stdout_messeges = true;

//private methods
void MirrorClient::sent_files(const List<String>& ids){
	auto cursor = ids.get_begin();
	for(; cursor->has_next(); cursor = cursor->get_next()){		
		SpawnChildProcess senter(senter_path, make_senter_args(cursor->get_data()));		write_to_log("spawn senter to " + cursor->get_data());
		int i; for(i = 1; i <= 3; ++i){
			
			if(waitpid(senter.spawn()) == 0) {break;										write_to_log("senter to " + cursor->get_data() + " done!");}
		}
		if(i >= 4)																			write_to_log("senter to " + cursor->get_data() + " failed!");
	}
	//last iteration
	SpawnChildProcess senter(senter_path, make_senter_args(cursor->get_data()));			write_to_log("spawn senter to " + cursor->get_data());
	int j; for(j = 1; j <= 3; ++j){
		if(waitpid(senter.spawn()) == 0) {break;											write_to_log("senter to " + cursor->get_data() + " done!");}
	}
	if(j >= 4)																				write_to_log("senter to " + cursor->get_data() + " failed!");
}

String MirrorClient::make_senter_args(const String& receiver_id){
	return 	Id 			+ " " +
			receiver_id	+ " " +
			common_dir 	+ " " +
			input_dir 	+ " " +
			log_file;
}

void MirrorClient::receive_files(const List<String>& ids){
	auto cursor = ids.get_begin();
	for(; cursor->has_next(); cursor = cursor->get_next()){		
		//create dir for senter id
		mkdir(mirror_dir + cursor->get_data());
		
		SpawnChildProcess receiver(receiver_path, make_receiver_args(cursor->get_data()));		write_to_log("spawn receiveer from " + cursor->get_data());
		int i; for(i = 1; i <= 3; ++i){
			if(waitpid(receiver.spawn()) == 0) {break;											write_to_log("receiver from " + cursor->get_data() + " done!");}
		}
		if(i >= 4)																				write_to_log("receiver from " + cursor->get_data() + " failed!");
	}
	//last iteration
	//create dir for senter id
	mkdir(mirror_dir + cursor->get_data());
	SpawnChildProcess receiver(receiver_path, make_receiver_args(cursor->get_data()));			write_to_log("spawn receiver from " + cursor->get_data());
	int j; for(j = 1; j <= 3; ++j){
		if(waitpid(receiver.spawn()) == 0) {break;												write_to_log("receiver from " + cursor->get_data() + " done!");}
	}
	if(j >= 4)																					write_to_log("receiver from " + cursor->get_data() + " failed!");

}

String MirrorClient::make_receiver_args(const String& senter_id){
	return 	senter_id						+ " " +
			Id								+ " " +
			common_dir						+ " " +
			mirror_dir 	+ senter_id	+ "/"	+ " " +
			log_file;
}

List<String> MirrorClient::get_current_ids(){
	auto l = ::get_current_ids(common_dir);
	
	//DEGUB
	//cout << l << endl;
	
	l.erase_first(Id);
	
	return l;
}

//log
void MirrorClient::write_to_log(const String& messege){
	String log_messege = "Client " + Id + ": "  + messege + "\n";
	log.putline(log_messege);
	
	if(client_stdout_messeges)  cout << log_messege << endl;
}

//constructor destructor
MirrorClient::MirrorClient(
							const String& IId,
					
							const String& ssenter_path,
							const String& rreceiver_path,
					
							const String& mmirror_dir,
							const String& iinput_dir,
							const String& ccommon_dir,
					
							const String& llog_file
						) :
							Id(IId),
							
							senter_path(( assert(not path_is_dir(ssenter_path)), ssenter_path )),
							receiver_path(( assert(not path_is_dir(rreceiver_path)), rreceiver_path )),
							
							mirror_dir(( assert(path_is_dir(mmirror_dir)), mmirror_dir )),
							input_dir(( assert(path_is_dir(iinput_dir)), iinput_dir )),
							common_dir(( assert(path_is_dir(ccommon_dir)), ccommon_dir )),
							
							log_file(( assert(path_is_file(llog_file)), llog_file )),
							log(log_file)
						{
	
	
	//create id file{
		String id_file = common_dir + Id + ".id";
		if(file_exists(id_file))  { write_to_log("Error: id file already exists");  exit(1); }
		
		create_path(common_dir,  Id + ".id", itoa(getpid()));		write_to_log("create id file");
	//}
	
	//create mirror dir
	mkdir(mirror_dir);
	
	//are there existing clients ?
	current_ids = get_current_ids();
	
	//if there are existing clients
	if(not current_ids.empty()){
		sent_files(current_ids);			//senting my input dir..
		receive_files(current_ids);			//receiving theirs dirs and files..
	}
}

MirrorClient::~MirrorClient(){
	rmdir_r(common_dir + Id + ".id");
	rmdir_r(mirror_dir);
}

//operations
void MirrorClient::operate(){
	List<String> new_ids = get_current_ids();
		
	//some clients left
	if(new_ids < current_ids){
		List<String> to_delete = current_ids - new_ids;
			
		auto cursor = to_delete.get_begin();
		for(; cursor->has_next(); cursor = cursor->get_next())  rmdir_r(mirror_dir + cursor->get_data() + "/");
		//last iteration
		rmdir_r(mirror_dir + cursor->get_data() + "/");
	}
		
	//some clients appeared
	else if(new_ids > current_ids){
		List<String> to_receive =  new_ids - current_ids;
			
		receive_files(to_receive);
		sent_files(to_receive);
	}
				
	//update ids
	current_ids = new_ids;
}

//methods
bool is_id_file(const String& path){
	return is_suffix(path, ".id");
}

List<String> safe_get_entries(const String& path){
	List<String> l = get_entries(path);
	
	//DEBUG
	//cout << l << endl;
	
	List<String> r;
	if(not l.empty()){
		auto cursor = l.get_begin();
		for(; cursor->has_next(); cursor = cursor->get_next()){
			
			String path = cursor->get_data();
			while(path_get_size(path) > 1)  path = path_pop_head(path);
			
			r.append(path);
		}
		//last it
		String path = cursor->get_data();
		while(path_get_size(path) > 1)  path = path_pop_head(path);
			
		r.append(path);
		
		//DEGUG
		//cout << path_get_size(path) << endl;
		
	}
	
	return r;
}

List<String> get_current_ids(const String& path){
	List<String> l = safe_get_entries(path).filter(is_id_file);
		
	//get only the id
	auto cursor = l.get_begin();
	List<String> t;
	for(; cursor->has_next(); cursor = cursor->get_next()){
		String s = cursor->get_data();
		t.append(s.get_prefix('.'));
	}
	
	//last iteration
	String s = cursor->get_data();
	t.append(s.get_prefix('.'));


	return t;
}

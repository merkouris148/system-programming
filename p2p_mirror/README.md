# p2p-mirror

A peer to peer application, where each process owns a directory and every other process mirrors the process's directory. Written in C++. (A project about [Linux]([Linux - Wikipedia](https://en.wikipedia.org/wiki/Linux))/[Unix]([Unix - Wikipedia](https://en.wikipedia.org/wiki/Unix)) [inter-process communication]([Inter-process communication - Wikipedia](https://en.wikipedia.org/wiki/Inter-process_communication)).)

In this application the user may launch multiple `mirror_clients` . Every mirror_client has a directory with various files and subdirectories. Every time a new mirror_client is launched, the existing mirror_clients request it's files and copy them to their `mirror_directory`. On the other hand, the new mirror_client, also copies the existing mirror_clients' files. This way, every client has a complete copy of every other in it's mirror_directory. If the user terminates a mirror_client, then the others delete it's files from their mirror_dirs.

## Input

A mirror_client takes 5 arguments:

1. A **client's Id**. Each client must have a unique Id. (Not to be confused with a process id, see bellow.)

2. The path to the **common directory**, where every client puts a file with it's [process id](https://en.wikipedia.org/wiki/Process_identifier). *The common directory should be the same for every client*.

3. The path to the **input directory**, where exist the client's files (subdirectories etc.).

4. The path to the **mirror directory**, where the client will put the copies of the other clients' input directories. The mirror directory is *created by the client*.

5. A **log file**. The log file *must already exists*. The clients can have the same or different log files.

The arguments are passed to the client with the following command:

`./mirror_client -n id -c ./common -i ./input_1 -m ./mirror_1 -l log.txt &`

The last character (`&`) is so the client runs in the background. Example input directories can be found in the folder `environment` , along with an empty `common` directory and a `log.txt`  empty file. There also can be found a Python3 script (`create_dir_hierarchy.py`) to generate a random directory hierarchy, as input directory for a client. The script takes 4 arguments:

1. the name of the base (level 0) directory,

2. the maximum depth of the hierarchy, 

3. the maximum number of files and subdirectories a directory may have

4. and the maximum size of a file.

### Environment

The application takes place in the `environment` directory. There will be sent the executables after compilation. There, also, can be found the input files and directories, mentioned above.

## Communication Protocol

We describe, now, the communication protocol the clients follow:

1. The client, firstly creates it's mirror_directory.

2. Then it creates an `id file`, under the common directory, of the form `<id>.id` and writes there it's process id.

3. After that the client waits for other clients to be launch. The client periodically checks for any changes in the common directory, namely if a new id file is created or an id file is deleted.

4. If a new id file is created, the client behaves as follows:
   
   1. [Forks]([Fork (system call) - Wikipedia](https://en.wikipedia.org/wiki/Fork_(system_call))) two child-processes, a `sender` and a `receiver`. The sender creates a [named pipe]([Named pipe - Wikipedia](https://en.wikipedia.org/wiki/Named_pipe)) (aka fifo) under the common directory. Let `id1` be the existing client's id and `id2` be the new client's id. Then the named pipe name follows the format `id1_to_id2.fifo`. Similarly behaves the receiver. The receiver creates a named pipe of the format `id2_to_id1.fifo`.
   
   2. These two pipes will be used for the interprocess communication. The **senter** will traverse through the contents of the input directory, for every item (file or directory), it will write:
      
      1. The size of the items path
      
      2. The path of the item
      
      3. The size of the item. If the item is a directory, then the size equals zero `0`.
      
      4. Should the item be a file, the sender writes the contents of the file to the pipe.
      
      After sending all the items the sender writes `00` to the pipe.
   
   3. Now, the **receiver** copies all the items the new client has sent, under the `mirror_id1/id2/` directory.
   
   4. After ending the two child processes terminate.

5. If an id file is deleted, the client deletes the corresponding directory from its mirror directory.

### Client Termination

In order to terminate a client you have to use the `fg` command. Then *kill* the process with `Ctrl+C`. Then the client with receive a `SIGINT` or `SIGQUIT` [signal]([Signal (IPC) - Wikipedia](https://en.wikipedia.org/wiki/Signal_(IPC))) and delete it's mirror directory and it's id file from the common directory.

## Notes

- This code is written in C++11 and compiled with [GNU g++ compiler](https://en.wikipedia.org/wiki/GNU_Compiler_Collection#Languages), version 7.5.0.
- All the shell commands described above are for the [BASH]([Bash (Unix shell) - Wikipedia](https://en.wikipedia.org/wiki/Bash_(Unix_shell))) Unix Shell.
- To compile this application use the `make` command.
- A more detailed description of this assignment can be found in the file `hw2-spring-2019.pdf` (in Greek).

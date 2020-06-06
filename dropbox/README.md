# dropbox

A drobox simulator, written in C++. (A project about [Linux]([Linux - Wikipedia](https://en.wikipedia.org/wiki/Linux))/[Unix]([Unix - Wikipedia](https://en.wikipedia.org/wiki/Unix)) [network sockets]([Network socket - Wikipedia](https://en.wikipedia.org/wiki/Network_socket)) and [threads]([Thread (computing) - Wikipedia](https://en.wikipedia.org/wiki/Thread_(computing))).) This project is similar to `p2p-mirror`.

In this project we implemented two entities, a `dropbox_server` and a `dropbox_client`. Each dropbox_client has a directory with some files in it. Every time a dropbox_client is launched, it connects to the dropbox_server. The dropbox_server, then, informs the new dropbox_client about any other existing dropobox_client. Then the dropbox_clients communicate with each other in order to exchange their files. In the end every dropbox_client has a copy of every other dropbox_client's files in it's `mirror_directory`.

## Input

The dropbox_server takes 1 argument, it's [**port number**]([Port (computer networking) - Wikipedia](https://en.wikipedia.org/wiki/Port_(computer_networking))), where it listens for dropbox_clients requests. This argument is passed to the server with the following command:

`./dropbox_server -p <port>`

Whereas the dropbox_client takes 5 arguments:

1. A **port number**, where the dropbox_client listens for request from other clients and the dropbox_server.

2. It's [**ip address**]([IP address - Wikipedia](https://en.wikipedia.org/wiki/IP_address)), which will sent to the dropbox_server in order to other clients find it. This argument *may be omitted*, the default value is `localhost` .

3. An **input directory**, under which are the client's files. The input directories must not cointain any subdirectories (see **Issues** bellow).

4. A **mirror directory**, where the copies of the other clients' files will be saved. The mirror directories should already be created and be empty (see **Issues** bellow).

5. The number of **working threads**, that will be created by the client and will serve the incoming requests. This argument *may be omitted*, the default value is `4`.

6. The **server port**, in which the dropbox_server listens for requests.

7. The **server's ip address**. This argument *may be omitted*, the default value is `localhost`.

The arguments are passed to the dropbox_client with the following command:

`./dropbox_client -p <port> -ip <ip> -d <input_dir> -m <mirror_dir> -w <num_worker_threads> -sp <server_port> -sip <server_ip> &`

The last character (`&`) is so the client runs in the background. Example input directories can be found in the folder `environment` . There also can be found a Python3 script (`create_random_files.py`) to generate random files. The script takes 3 arguments:

1. A prefix wich will be common for all the files. You can also use the prefix to determine a path. For example the prefix `./input_1/input_1` will create multiple text files of the form `input_1<n>.txt`, under the directory `input_1`.

2. The number of files to be created.

3. The maximum size of each file.

### Environment

The application takes place in the `environment` directory. There will be sent the executables after compilation. There, also, can be found the input directories.

## Communication Protocol

We describe, now, the communication protocol between the server and the clients, and among the clients.

### Server Protocol

1. After the server is launched, it listens for client requests in it's port

2. There are 3 requests the server handles:
   
   1. **LOG_ON** request. When a new client is launch, it sends to the server a log on request, along with it's ip address and it's port. Then the server is informed of the new client and keeps these contacting information (client's ip address and port).
      
      When a new client connects to the server, the server sends the contacting information to all the other existing clients.
   
   2. **GET_CLIENTS** request. When the server gets a get_clients request it sends a list of the contacting information of all the existing clients to the client.
   
   3. **LOG_OFF** request. When a client is terminated, it sends the server this request. Then the server informs all the other clients of the leaving client.

### Client Protocol

1. After a client is launched, it sends the server a log_on request (see above) and a get_clients request.

2. Then the client creates a `task queue` in order to handle the various tasks it needs to perform. The client stores to the queue all the contact information it received from the server.

3. Then the client creates the `worker threads`. The task queue mentioned above is shared among the multiple threads. Every worker thread pops a tasks from the task queue in order to execute it. The main (parent) thread continues listening to it's port for request from other clients, or the server, and updates the task queue. There are 5 types of tasks the worker thread handles.
   
   1. If the item the worker took from the task queue is a tuple with the **communication information** of another client, the worker sends a get_file_list request (see bellow) to other client. After the worker receives the file list it pushes to the queue the *file information* it received, namely the file's paths, and the other client's ip address and port number.
   
   2. If the item the worker took from the task queue is a tuple with the **file information**, the worker sends a get_file request to the other client. After, receiving the file from the other client, it stores the file to the mirror directory.
   
   3. If the item the worker took from the queue is **GET_FILE_LIST** request, then it sends the other client a list of it's files.
   
   4. If the item the worker took from the queue is **GET_FILE** request, then it sends to the other user the requested file.
   
   5. There can also be a **LOG_OFF** request in the task queue. This request is sent by the server and informs the client that another client left. The leaving clients files are *not deleted* from the mirror file.

### Server & Client Termination

In order to terminate the server or a client you have to use the `fg` command. Then *kill* the process with `Ctrl+C`. Then the server or client with receive a `SIGINT` or `SIGQUIT` [signal]([Signal (IPC) - Wikipedia](https://en.wikipedia.org/wiki/Signal_(IPC))) and terminate safely. The clients should be terminated **before** the server.

## Issues

* As mentioned above, the mirror directories are not deleted, after a client *dies*. Thus, the user has to delete the mirror directories manually.

* The mirror directories must already exist, in order to the application works properly. Also, they must be empty.

* There should only be files in the input directories, no other subdirectories.

## Notes

- This code is written in C++11 and compiled with [GNU g++ compiler](https://en.wikipedia.org/wiki/GNU_Compiler_Collection#Languages), version 7.5.0.
- All the shell commands described above are for the [BASH]([Bash (Unix shell) - Wikipedia](https://en.wikipedia.org/wiki/Bash_(Unix_shell))) Unix Shell.
- To compile this application use the `make` command.
- A more detailed description of this assignment can be found in the file `hw3-spring-2019.pdf` (in Greek).

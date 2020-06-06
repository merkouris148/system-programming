# system programming

In this repository are placed 3 projects that were assignments of the [system programming](https://en.wikipedia.org/wiki/Systems_programming) course in [Department of Informatics and Telecommunications of University of Athens](https://www.di.uoa.gr/en), written in C++. In these assignments the use of the most C++'s libraries were prohibited (along with the STL libraries). So the functionality of these libraries was implemented from scratch.

## Contents

We list here the components of this repository, along with a discription of every component. Every project (of the 3) has it's own `Makefile` and can be compiled separately. 

### Libraries

Firstly, we discribe the libraries made for the projects.

1. **lib_containers** In this directory are placed all the source files reguarding [cointainer](https://en.wikipedia.org/wiki/Container_(abstract_data_type)) libraries. The libraries make heavy use of [C++ templates](http://www.cplusplus.com/doc/oldtutorial/templates/). An effort has been made to provide a simple and easy to use interface, that meets the STL libraires' feeling. The data structures implemented here are used throughout the whole project.

2. **lib_safe_c** This directory includes only a hearder file with some usefull macros that provide extra safety to the use of tricky C functions.

3. **lib_system** In this directory are placed all the source files reguarding [system calls](https://en.wikipedia.org/wiki/System_call). The main concern this library was to make the C system calls more reliable and easy to use. This is achived by implementing a higher level of abstraction. The various functions of this library take as arguments complex data structures, defined by the lib_containers. Thus, making compatible the use of containers with low level system calls.

4. **lib_utilities** In this directory are placed some source files reguarding utilities that are used throughout the whole project.

### Projects

Now, we desrcibe the projects that make use of the libraries described above. Every project has it's own readme file, were it is explained the application usage.

1. **bitcoin** A virtual bank for handling bitcoin transactions. (A project about data structures in C/C++.) See `./bitcoin/README.md` for more information.

2. **p2p_mirror** A peer to peer application, where each process owns a directory and every other process mirrors the process's directory. (A project about [Linux](https://en.wikipedia.org/wiki/Linux)/[Unix](https://en.wikipedia.org/wiki/Unix) [inter-process communication](https://en.wikipedia.org/wiki/Inter-process_communication).) See `./p2p_mirror/README.md` for more information.

3. **dropbox** A drobox simulator. (A project about [Linux](https://en.wikipedia.org/wiki/Linux)/[Unix](https://en.wikipedia.org/wiki/Unix) [network sockets](https://en.wikipedia.org/wiki/Network_socket) and [threads]([https://en.wikipedia.org/wiki/Thread_(computing)).) This project is similar to `p2p-mirror`.  See `./dropbox/README.md` for more information.

## Notes

This code is written in C++11 and compiled with [GNU g++ compiler](https://en.wikipedia.org/wiki/GNU_Compiler_Collection#Languages), version 7.5.0.

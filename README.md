# ethernetTool

A Windows console TCP/IP diagnostic app. UDP functions include listen, message, echo, and counter stream. TCP functions include listen and echo servers and client message. The app is built on the Winsock API and supports multithreading.

## Functions
The app opens a console window on startup. Enter the *command* keywords separated by a space. Commands are case-sensitive. Function names are entered first, followed by parameters. For example:
>*functionName parameter1 parameter2 ...* 

## UDP Functions

### Listen
To start or stop a UDP listening thread, enter:
> *listen start localPortNumber*

> *listen stop*

where *localPortNumber* is the port you want to listen on. An incoming packet's source address/port, payload size (bytes), and payload are printed to the console.

### Message
To send a UDP message to a specific address/port, enter:
> *message destIPaddress destPortNumber yourMessage*

where *destIPaddress* is the destination address in IPv4 dot-decimal notation (x.x.x.x), *destPortNumber* is the destination port number, and *yourMessage* is your message. For example, enter:

> *message 192.168.1.6 345 Yeah buddy 123#$%?*

to send "Yeah buddy 123#$%?" to 192.168.1.6 port 345.

### Echo
Similar to the listen function, start or stop a UDP echo thread using:
> *echo start localPortNumber*

> *echo stop*

Incoming packet data is displayed in the console. The payload is echoed back to to the source's address/port.

### Counter
For troubleshooting embedded systems, its often helpful to stream a counter to a system under test to, say, print to LEDs. To start or stop a UDP counter thread, enter:
> *counter destIPaddress destPortNumber startInteger endInteger delay loop*

> *counter stop*

where *destIPaddress* is the destination address, *destPortNumber* is the destination port, *startInteger* is the counter's starting integer, *endInteger* is the counter's ending integer, *delay* is time in milliseconds between counter increments, and entering "loop" for *loop* triggers the counter to repeat continuously. *startInteger* and *endInteger* are 4-byte integers with range from -2147483648 to 2147483647. *delay* is an unsigned 4-byte integer ranging from 0 to 4294967295. Omit the *loop* command to stream the counter once.

For example, enter:

> *counter 192.168.1.6 345 0 1000 500*

to stream a counter once from 0 to 1000 in 500 ms time intervals to 192.168.1.6 port 345. Similarly, 

> *counter 192.168.1.6 345 -1234 4321 2500 loop*

streams a looping counter from -1234 to 4321 in 2.5 second time intervals.

## TCP Functions
TCP functions first start a thread running an instance of the server class, which spawns a second thread running a session for either the listen or echo functions. TCP threads cannot currently be stopped individually, only as a group, by entering:
> *server stop*

### Listen Server
To start or stop a server thread running the listen function, enter:
> *server listen localPortNumber*

### Echo Server
To start or stop a server thread running the echo function, enter:
> *server echo localPortNumber*

### Client Message
To start a client thread to send a message to a server, enter:
> *client message serverIP serverPort yourMessage*

### Client-Server Ping-Pong
Just for fun, you can ping-pong a message between a client and server. First, start a server that will start a "pong" sessions when a client connects by entering:
> *server pong portNumber*

When a client connects, the session runs the session class *pong* function that echos messages received and prints "Server: pong" to the console. 

Next, start a client running the client class *ping* function by entering:
> *client ping serverIP serverPortNumber message*

The client sends *message* to the server and then echos subsequent messages received, printing "Client: ping" to the console. The client and server will ping-pong the message indefinitely, with 1 second delays between volleys. You can stop the volleys on the server side by entering:
> *session stop*

or on the client side entering:
> *client stop*

### App Control
To stop all active threads, enter:
> *stop all*

To stop all active threads and exit the app, enter:
> *exit*

## Future Updates
- More TCP client functions.
- GUI version.
- Command case-insensitivity.

## Download
[ethernetTool v1.3.0](https://github.com/JohnWSweeney/ethernetTool/releases/download/v1.3.0/ethernetTool_v1_3_0.exe) [57 kB]

## ChangeLog
v1.3.0
- Added client-sever ping-pong feature.
	- Added ping function to client class.
	- Added pong function to session class.

v1.2.5
- Updated tcp class.
	- Merged shutdown, closeSocket functions into new closeConnection function.
	- Moved socket close responsibility to calling functions, tcp::closeConnection.
- Updated client/session/server class functions with socket close responsibilities.

v1.2.4
- Updated populateServerCmds and populateClientCmds functions in tcpCommands.cpp to fix bug causing crash with too few client/server commands.

v1.2.3
- Updated tcp class rx,tx functions and return codes.
- Updated server class listen, echo and client class message functions.

v1.2.2
- Updated tcp, server, session, and client classes.
- Added tcpCommands .h/cpp to handle server, client commands.

v1.2.1
- Updated sortClientCommands function in sortFunctions.h/cpp, added supporting functions.

v1.2.0
- Added client class with message function.
- Updated tcp class.

v1.1.0
- Added TCP, server, session classes.
	- Session class supports listen and echo functions.

v1.0.3
- Updated error handling in:
	- udp class.
	- listen, message, echo, and counter functions in udpFunctions.cpp.
	- listen, message, echo, and counter thread functions in udpThreads.cpp.
- Updated datagram payload, recvfrom buffer to maximum possible size of 65,527 bytes.

v1.0.2
- Compliled for "release" instead of "debug" (oops). 
- Added (rxReady < 0) condition to socketReadStatus function in udp.cpp to prevent looping on listen, echo funcitons on release build.
- Moved atomic bool function status variables to separate .h/.cpp files.

v1.0.1
- Each .cpp now has its own header.
- Fixed typo in counter instruction examples.

v1.0.0
- Added counter function.

v0.1.0
- Added UDP echo function.
- Moved thread start functions to separate file.
- Added stop all threads function.
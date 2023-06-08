# ethernetTool

A Windows console TCP/IP diagnostic app. Functions include listen, message, echo, and counter stream. The app is built on the Winsock API and supports multithreading. The app currently only supports UDP (TCP in progress).

## Functions
Enter the command keywords separated by a space. Commands are currently case-sensitive and must be lower-case.

### Listen
To start or stop a thread listening on a local port, enter:
> *listen start localPortNumber* <br/>
> *listen stop*

where *localPortNumber* is the port you want to listen on. An incoming packet's source address/port, payload size (bytes), and payload are printed to the console.

### Message
To send a message to a specific address/port, enter:
> *message destIPaddress destPortNumber yourMessage*

where *destIPaddress* is the destination address in IPv4 dot-decimal notation (x.x.x.x), *destPortNumber* is the destination port number, and *yourMessage* is your message. For example, enter:

> *message 192.168.1.6 345 Yeah buddy 123#$%?*

to send "Yeah buddy 123#$%?" to 192.168.1.6 port 345.

### Echo
Similar to the listen function, start or stop an echo thread using:
> *echo start localPortNumber* <br/>
> *echo stop*

Incoming packet data is displayed in the console. The payload is echoed back to to the source's address/port.

### Counter
For troubleshooting embedded systems, its often helpful to stream a counter to a system under test to, say, print to LEDs. To start or stop a counter thread, enter:
> *counter destIPaddress destPortNumber startInteger endInteger delay loop* <br/>
> *counter stop*

where *destIPaddress* is the destination address, *destPortNumber* is the destination port, *startInteger* is the counter's starting integer, *endInteger* is the counter's ending integer, *delay* is time in milliseconds between counter increments, and entering "loop" for *loop* triggers the counter to repeat continuously. *startInteger* and *endInteger* are 4-byte integers with range from -2147483648 to 2147483647. *delay* is an unsigned 4-byte integer ranging from 0 to 4294967295. Omit the *loop* command to stream the counter once.

For example, enter:

> *counter 192.168.1.6 345 0 1000 500*

to stream a counter once from 0 to 1000 in 500 ms time intervals to 192.168.1.6 port 345. Similarly, 

> *counter 192.168.1.6 345 -1234 4321 2500 loop*

streams a looping counter from -1234 to 4321 in 2.5 second time intervals.

### App Control
To stop all active threads, enter:
> *stop all*

To stop all active threads and exit the app, enter:
> *exit*

## Future Updates
- TCP support.
- GUI version.
- Command case-insensitivity.

## Download ##
[ethernetTool v1.0.3](https://github.com/JohnWSweeney/ethernetTool/releases/download/v1.0.3/ethernetTool_v1_0_3.exe) [39 kB]

## ChangeLog <br/>
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
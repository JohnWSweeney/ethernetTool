# ethernetTool

A Windows console TCP/IP diagnostic app. Functions include listen, message, echo, and counter stream. The app is built on the Winsock API and supports multithreading. The app currently only supports UDP (TCP in progress).

## Functions
Enter the command keywords separated by a space. Commands are currently case-sensitive and must be lower-case.

### Listen
To start or stop a thread listening on a local port, enter:
> *listen start portNumber*

> *listen stop*

where *portNumber* is the port you want to listen on. An incoming packet's source address/port, payload size (bytes), and payload are printed to the console.

### Message
To send a message to a specific address/port, enter:
> *message ipAddress portNumber yourMessage*

where *ipAddress* is the destination address in IPv4 dot-decimal notation (x.x.x.x), *portNumber* is the destination port number, and *yourMessage* is your message. For example, enter:

> *message 192.168.1.6 345 Yeah buddy 123#$%?*

to send "Yeah buddy 123#$%?" to 192.168.1.6 port 345.

### Echo
Similar to the listen function, start or stop an echo thread using:
> *echo start portNumber*

> *echo stop*

Incoming packet data is displayed in the console. The payload is echoed back to to the source's address/port.

### Counter
For troubleshooting embedded systems, its often helpful to stream a counter to a system under test to, say, print to LEDs. To start or stop a counter thread, enter:
> *counter ipAddress portNumber startInteger endInteger delay loop*

> *counter stop*

where *ipAddress* is the destination address, *portNumber* is the destination port, *startInteger* is the counter's starting integer, *endInteger* is the counter's ending integer, *delay* is time in milliseconds between counter increments, and entering "loop" for *loop* triggers the counter to repeat continuously. *startInteger* and *endInteger* are 4-byte integers with range from -2147483648 to 2147483647. *delay* is an unsigned 4-byte integer ranging from 0 to 4294967295. Omit the *loop* command to stream the counter once.

For example, enter:

> *message 192.168.1.6 345 0 1000 500*

to stream a counter once from 0 to 1000 in 500 ms time intervals to 192.168.1.6:345. Similarly, 

> *message 192.168.1.6 345 -1234 4321 2500 loop*

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

## ChangeLog
v1.0.0
- Added counter function.

v0.1.0
- Added UDP echo function.
- Moved thread start functions to separate file.
- Added stop all threads function.


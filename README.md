# ethernetTool

A Windows console app providing multiple ethernet diagnostic functions, including listen, message, and echo. The app is built on the Winsock API and supports multithreading. 

To start or stop a listen thread, enter:
> *listen start portNumber*

> *listen stop*

where *portNumber* is the local port you want to listen on. An incoming packet's source address/port, payload size (bytes), and payload are printed to the console.

To send a message to a specific address/port, enter:
> *message ipAddress portNumber yourMessage*

where *ipAddress* is the destination address in IPv4 dot-decimal notation (x.x.x.x), *portNumber* is the destination port number, and *yourMessage* is your message. For example, enter:

> *message 192.168.1.6 345 Yeah buddy 123#$%?*

to send "Yeah buddy 123#$%?" to 192.168.1.6:345.

Similar to the listen function, start or stop an echo thread using:
> *echo start portNumber*

> *echo stop*

Incoming packet data is displayed in the console.

To stop all active threads, enter:
> *stop all*

To stop all active threads and exit the program, enter:
> *exit*

Commands are case-sensitive.

### Future Updates
- TCP support.
- GUI version.

### ChangeLog
v0.1.0
- Added UDP echo function.
- Moved thread start functions to separate file.
- Added stop all threads function.

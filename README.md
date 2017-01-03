This is a simple implementation of publisher/subscriber mechanism for multi-thread program. The code had been written and tested in Ubuntu 16.04 and passed Google's cpplint. To run the code, please do it as follows

1. unzip my_pubsub.zip
2. cd my_pubsub
3. make
4. ./test


The document structure of this code is

├── main.cpp
├── Makefile
├── message.hpp
├── module.cpp
├── module.hpp
├── readme.txt
├── subscriber.cpp
└── subscriber.hpp


The basic idea of this publisher/subscriber mechanism is

1. The main() function brings up a message_server, and passes the handle of the message_server to all active threads.

2. The message_server maintains a vector of Topic objects.

3. Each Topic object maintains a vector of pointers to every subscribed threads.

4. Any thread or main() function can access the subscriber vectors via the handle of message_server. 

5. To subscribe/unsubscribe, just add/remove corresponding thread pointer to/from the subscriber vectors.

6. To publish a message, 

   a) A thread contact the message_server with topic name.

   b) Message_server accesses to the subscriber vector of that topic.

   c) Use that subscriber vector, message_server push the message to the message buffer of every subscribed thread.

   d) Every thread moniters its own message buffer in a for-loop in run() method.
   
   e) When new message comes, the thread fetch the message from the message buffer, then remeove the message from the buffer.


Due to the limited time, several possible improvements remain untouched in this version of the code:

1. Consider real-time requirements. For example, in current code, if hard interupt is invoked, the message haven't been fully publish might be lost or demaged. 

2. Consider lock in parallel threads. For example, in current code, if two messages arrive at the same time, the program might behavior abnormally or crash.

3. Code refactoring is needed. In "subscriber.hpp", cross references are needed to resolved. Because, class MessageServer uses class Subscriber, while Subscribe also uses class MessageServer.

4. Message buffer in every thread might be carefully re-design. Because the buffer might overflow or some message might be lost.

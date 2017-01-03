This is a simple implementation of **publisher/subscriber** mechanism for multi-thread program. The code had been written and tested in Ubuntu 16.04 and passed Google's **cpplint**. 

# Installation
To run the code, please do it as follows

```bash
unzip my_pubsub.zip
cd my_pubsub
make
./test
```

# Document Structure
The document structure of this code is

```bash
├── main.cpp
├── Makefile
├── message.hpp
├── module.cpp
├── module.hpp
├── readme.txt
├── subscriber.cpp
└── subscriber.hpp
```

# Basic Idea
The basic idea of this **publisher/subscriber** mechanism is

1. The `main()` function brings up a `message_server`, and passes the handle of the `message_server` to all active threads.

1. The `message_server` maintains a vector of `Topic` objects.

1. Each Topic object maintains a vector of pointers to every subscribed threads.

1. Any thread or `main()` function can access the subscriber vectors via the handle of `message_server`. 

1. To `subscribe/unsubscribe`, just `add/remove` corresponding thread pointer `to/from` the subscriber vectors.

1. To publish a message, 

   1. A thread contact the `message_server` with topic name.

   1. `Message_server` accesses to the subscriber vector of that topic.

   1. Use that subscriber vector, `message_server` push the message to the message buffer of every subscribed thread.

   1. Every thread moniters its own message buffer in a `for`-loop in `run()` method.
   
   1. When new message comes, the thread fetch the message from the message buffer, then remeove the message from the buffer.

# Future Work
Due to the limited time, several possible improvements remain untouched in this version of the code:

1. Consider **real-time** requirements. For example, in current code, if hard interupt is invoked, the message haven't been fully publish might be lost or demaged. 

1. Consider **lock** in parallel threads. For example, in current code, if two messages arrive at the same time, the program might behavior abnormally or crash.

1. **Code refactoring** is needed. In `subscriber.hpp`, cross references are needed to resolved. Because, class `MessageServer` uses class `Subscriber`, while `Subscribe` also uses class `MessageServer`.

1. Message buffer in every thread might be carefully re-design. Because the buffer might overflow or some message might be lost.

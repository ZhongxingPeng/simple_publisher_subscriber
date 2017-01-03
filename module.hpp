// NONLINT(legal/copyright)

#ifndef _MODULE_H_  // NOLINT(build/header_guard)
#define _MODULE_H_
#include <pthread.h>
#include <unistd.h>
#include "subscriber.hpp"


// Class Module implements the multi-thread mechanism
class Module {
 public:
  Module();
  virtual ~Module();

  // Detcah the thread
  int detach();

  // Add a thread
  int join();

  // Every instance of Module will run the for-loop in method run() continuesly
  virtual void* run() = 0;

  // Return the ID of the thread
  pthread_t self();

  // Start the thread
  int start();

 private:
  // Indicate whether a thread has been detached
  // m_detached = 1: deteched
  // m_detached = 0: undeteched
  int m_detached;

  // Indicate whether a thread is running
  // m_running = 1: it is running
  // m_running = 0: it does not run
  int m_running;

  // The ID of the thread
  pthread_t m_tid;
};


// A toy sub-class of Module to demostrate our intra-module message
// publisher/subscriber mechanism, which inherits from two classes:
// 1. class Module implements the multi-thread mechanism
// 2. class Subscribe implements publisher/subscriber mechanism between modules
class ModuleA : public Module, public Subscriber {
 public:
  ModuleA() { is_message_server_exist = false; }
  void* run();

 private:
  // indicate whether the message server has been start
  bool is_message_server_exist;
};


// Another toy sub-class of Module to demostrate our intra-module message
// publisher/subscriber mechanism, which inherits from two classes:
// 1. class Module implements the multi-thread mechanism
// 2. class Subscribe implements publisher/subscriber mechanism between modules
class ModuleB : public Module, public Subscriber {
 public:
  ModuleB() { is_message_server_exist = false; }
  void* run();

 private:
  // indicate whether the message server has been start
  bool is_message_server_exist;
};

#endif  // _MODULE_H_

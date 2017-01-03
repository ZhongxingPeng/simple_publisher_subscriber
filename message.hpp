// NONLINT(legal/copyright)

#ifndef _MESSAGE_H_  // NOLINT(build/header_guard)
#define _MESSAGE_H_
#include <stdint.h>
#include <sys/time.h>
#include <string>


// Class Message define a message object
class Message {
 public:
  // Construct message with current time-stamp and content of input_msg_content
  explicit Message(std::string input_msg_content) {
    time_stamp = getCurrentTimeInMilliseconds();
    topic_name = "";
    message_content = input_msg_content;
  }
  ~Message() {}

  // Get the current time from Epoch in milliseconds
  uint64_t getCurrentTimeInMilliseconds() {
    struct timeval current_time;
    uint64_t mtime;
    gettimeofday(&current_time, NULL);
    // Convert current time from seconds and micro seconds to milliseconds
    mtime = current_time.tv_sec*1000 + current_time.tv_usec/1000.0 + 0.5;
    return mtime;
  }

  // Get the message content
  std::string getMessageContent() { return message_content; }
  // Get the current time stamp
  uint64_t getTimeStamp() { return time_stamp; }
  // Get topic name of this message
  std::string getTopicName() { return topic_name; }
  // Set content to the message
  void setMessageContent(std::string input_msg_content) {
    message_content = input_msg_content; }
  // Set a topic name to the message
  void setTopicName(std::string input_topic_name) {
    topic_name = input_topic_name; }

 private:
  std::string message_content;
  uint64_t time_stamp;
  std::string topic_name;
};

#endif  // _MESSAGE_H_

// NONLINT(legal/copyright)

#include "subscriber.hpp"


////////////////////////////////////////////////////////////////////////////////
////////////////// Class Topic /////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

void Topic::subscribeToThisTopic(Subscriber* s) {
    // If no thread already subscribed to this topic. Create a new vector of
    // subscribers. Otherwise,
    // --- a) subscribe the given thread if it doesn't subscribe,
    // --- b) ignore the thread if it already subscribed to the topic.
    if (getNumSubscriberToThisTopic() == 0) {
        subscribers.push_back(s);
        std::cout << "Add new subscriber, now we have "
        << getNumSubscriberToThisTopic() << " subscribers to topic "
        << getTopicName() << std::endl;
    } else {
        std::vector<Subscriber*>::iterator it = std::find(
            subscribers.begin(), subscribers.end(), s);
        if (*it != s) {
            subscribers.push_back(s);
            std::cout << "Add new subscriber, now we have "
            << getNumSubscriberToThisTopic() << " subscribers to topic "
            << getTopicName() << std::endl;
        } else {
            std::cout << "The thread already subscribed to topic: "
            << getTopicName() << std::endl;
        }
    }
}


// Use "Erase–remove idiom" to remove thread from the vector of subscribers
void Topic::unsubscribeFromThisTopic(Subscriber* s) {
    subscribers.erase(std::remove(subscribers.begin(), subscribers.end(), s),
        subscribers.end());
    std::cout << "Unsubscribe from topic: " << getTopicName() << std::endl;
}


////////////////////////////////////////////////////////////////////////////////
////////////////// Class MessageServer /////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

std::vector<Topic>::iterator MessageServer::checkTopicStatus(
    std::string input_topic_name) {
    // Use unary function
    std::vector<Topic>::iterator it = find_if(topics.begin(),
        topics.end(), MatchString(input_topic_name));
    return it;
}


void MessageServer::createTopic(Subscriber* s, std::string input_topic_name) {
    Topic topic;
    topic.setTopicName(input_topic_name);
    topic.subscribeToThisTopic(s);
    topics.push_back(topic);
    std::cout << "Create new topic: " << input_topic_name << std::endl;
}


void MessageServer::publishMessage(std::string input_topic_name, Message msg) {
    // Check the existence of the given topic
    std::vector<Topic>::iterator it = checkTopicStatus(input_topic_name);
    // If the topic exists, push the message to the message buffer of every
    // subscriber to this topic
    if (it != topics.end()) {
        std::vector<Subscriber*> subscribers = it->getSubscribers();

        std::vector<Subscriber*>::iterator p;
        for (p = subscribers.begin(); p != subscribers.end(); p++) {
            (*p)->pushToMessageBuffer(msg);
        }
    } else {
        std::cout << "Publish message failed: No such topic --- " <<
        input_topic_name << std::endl;
    }
}


void MessageServer::subscribe(Subscriber* s, std::string input_topic_name) {
    // Check the existence of the given topic
    std::vector<Topic>::iterator it = checkTopicStatus(input_topic_name);
    // If the topic exists, subscribe the given thread to the topic.
    // If not, create a new topic, then subscribe the given thread
    if (it != topics.end()) {
        std::cout << "subscribeToThisTopic: " << input_topic_name << std::endl;
        it->subscribeToThisTopic(s);
    } else {
        createTopic(s, input_topic_name);
    }
}


void MessageServer::unsubscribe(Subscriber* s, std::string input_topic_name) {
    // Check the existence of the given topic
    std::vector<Topic>::iterator it = checkTopicStatus(input_topic_name);
    if (it != topics.end()) {
        std::cout << "Unsubscribing topic..." << std::endl;
        it->unsubscribeFromThisTopic(s);

        if (it->getNumSubscriberToThisTopic() == 0) {
            topics.erase(it);
            std::cout << "Unsubscribe: delete empty topic: " <<
            input_topic_name << std::endl;
        }
    } else {
        std::cout << "Unsubscribe: No such topic called " << input_topic_name
        << std::endl;
    }
}


void MessageServer::unsubscribeOneThreadFromAllTopics(Subscriber* s) {
    // Iterate through the whole topics vector to unsubscribe the given thread
    for (std::vector<Topic>::iterator it = topics.begin() ; it != topics.end();
        ++it) {
        it->unsubscribeFromThisTopic(s);
    }
}


////////////////////////////////////////////////////////////////////////////////
////////////////// Class Subscriber ////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

Message Subscriber::fetchAndClearMessageBuffer() {
    // Fetch the oldes message in the message buffer
    Message temp_msg = message_buffer.front();
    // Delete the message from message buffer
    message_buffer.erase(message_buffer.begin());
    return temp_msg;
}

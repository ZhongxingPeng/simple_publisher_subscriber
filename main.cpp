// NONLINT(legal/copyright)

#include "module.hpp"
#include "subscriber.hpp"


int main(int argc, char** argv) {
    // Create 4 threads:
    // --- a) thread1 and thread2 from ModuleA
    // --- b) thread3 and thread4 from ModuleB
    ModuleA* thread1 = new ModuleA();
    ModuleA* thread2 = new ModuleA();
    ModuleB* thread3 = new ModuleB();
    ModuleB* thread4 = new ModuleB();

    // Start these threads
    thread1->start();
    thread2->start();
    thread3->start();
    thread4->start();

    // Create message server, which maintains lists of subscribers to different
    // topics
    MessageServer* msg_server = new MessageServer();

    // Send the handle of message server to every thread
    thread1->setServerHandle(msg_server);
    thread2->setServerHandle(msg_server);
    thread3->setServerHandle(msg_server);
    thread4->setServerHandle(msg_server);

    // Test: unsubscribe thread1 from all topics. The method of
    // unsubscribeOneThreadFromAllTopics() can be invoked before destroying a
    // thread, to clean up subscriber lists without pointing to dead threads
    msg_server->unsubscribeOneThreadFromAllTopics(thread1);

    // Waitting for 2 seconds
    usleep(2000000);

    // Subscribe these 4 threads to two topics:
    // --- a) thread1 and thread2 subscribe to topic "Topic_for_moduleA"
    // --- b) thread3 and thread4 subscribe to topic "Topic_for_moduleB"
    //
    // Subscription and publish can also be done in class, please refer to the
    // commented code in "module.cpp"
    msg_server->subscribe(thread1, "Topic_for_moduleA");
    msg_server->subscribe(thread2, "Topic_for_moduleA");
    msg_server->subscribe(thread3, "Topic_for_moduleB");
    msg_server->subscribe(thread4, "Topic_for_moduleB");

    // Create and publish a new message to triger the cross publishing between 
    // the threads of ModuleA and ModuleB. More details can be referred to the
    // methods of "run()" in ModuleA and ModuleB.
    //
    // Since in "run()", ModuleA waitting for 1 sec but ModuleB waitting for 2 
    // sec During a same time peorid, threads of ModuleA will have two times of 
    // msg than those of ModuleB.
    // Output sequence might be similar to a following order:
    // thread 140354979768064, reciever msg --- A_welcome_from_ModuleA
    // thread 140354900719360, reciever msg --- A_welcome_from_ModuleA
    // thread 140354988160768, reciever msg --- A_welcome_from_ModuleB
    // thread 140354996553472, reciever msg --- A_welcome_from_ModuleB
    // thread 140354988160768, reciever msg --- A_welcome_from_ModuleB
    // thread 140354996553472, reciever msg --- A_welcome_from_ModuleB
    // where
    // --- thread1: 140354988160768
    // --- thread2: 140354996553472
    // --- thread3: 140354979768064
    // --- thread4: 140354900719360
    Message msg1("A_welcome_from_ModuleB");
    msg_server->publishMessage("Topic_for_moduleA", msg1);

    // To demostrate to unsubscribe from a non-exist topic.
    // Nothing, but "Unsubscribe: No such topic called" shows in your terminal
    msg_server->unsubscribe(thread1, "test_topic");

    // Waitting for 10 seconds
    usleep(10000000);
    // Unsubscribe thread4 from topic "Topic_for_moduleB"
    // Output sequence might be similar to a following order:
    // thread 140354979768064, reciever msg --- A_welcome_from_ModuleA
    // thread 140354988160768, reciever msg --- A_welcome_from_ModuleB
    // thread 140354996553472, reciever msg --- A_welcome_from_ModuleB
    msg_server->unsubscribe(thread4, "Topic_for_moduleB");
    std::cout << "-------------------------------------------------------------"
    << std::endl;
    std::cout << "-----------unsubscribe(thread4, Topic_for_moduleB)-----------"
    << std::endl;
    std::cout << "-------------------------------------------------------------"
    << std::endl;

    // Waitting for 40 seconds
    usleep(40000000);
    // Unsubscribe thread2 from all topics
    // Output sequence might be similar to a following order:
    // thread 140354979768064, reciever msg --- A_welcome_from_ModuleA
    // thread 140354996553472, reciever msg --- A_welcome_from_ModuleB
    msg_server->unsubscribeOneThreadFromAllTopics(thread2);
    std::cout << "-------------------------------------------------------------"
    << std::endl;
    std::cout << "----------unsubscribeOneThreadFromAllTopics(thread2)---------"
    << std::endl;
    std::cout << "-------------------------------------------------------------"
    << std::endl;

    while (true) {}
        std::cout << "done" << std::endl;

    delete thread1;
    delete thread2;
    delete thread3;
    delete msg_server;

    exit(0);
}

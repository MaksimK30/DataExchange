//
// Created by maksimk30 on 29.01.2024.
//

#ifndef DATA_SENDER_RECIEVER_H
#define DATA_SENDER_RECIEVER_H

#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstring>
#include <chrono>
#include <thread>

#define ERROR_S "SERVER ERROR: "
#define DEFAULT_PORT 8566

class reciever {
public:
    void recieve_data();

    ~reciever();


private:
    int client;
    int server;
    struct sockaddr_in server_address;

    int wait_for_connection(int &client, int &server, struct sockaddr_in &server_address, int delay = 500);
};


#endif //DATA_SENDER_RECIEVER_H

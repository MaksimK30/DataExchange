//
// Created by maksimk30 on 29.01.2024.
//

#ifndef DATA_SENDER_TRANSMITTER_H
#define DATA_SENDER_TRANSMITTER_H
#include "transmitter.h"
#include <iostream>
#include <string>
#include <cstdlib>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <cstring>
#include <chrono>
#include <thread>
#include <vector>
#include <mutex>
#include <algorithm>
#include <regex>

#define SERVER_IP "127.0.0.1"
#define ERROR_S "SERVER ERROR: "
#define DEFAULT_PORT 8566

class transmitter {
public:
    transmitter();
    ~transmitter();

    void get_data();
    void send_data();

private:
    int client;
    bool buffer_is_empty;
    struct sockaddr_in server_address;
    std::vector<std::string> buffer;
    std::mutex buffer_mutex;

    void wait_for_connection(int &client, struct sockaddr_in &server_address, int delay = 500);
};


#endif //DATA_SENDER_TRANSMITTER_H

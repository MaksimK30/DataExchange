//
// Created by maksimk30 on 29.01.2024.
//
#include "../Headers/reciever.h"


reciever::~reciever(){
    close(client);
    close(server);
}

//Метод
int reciever::wait_for_connection(int &client, int &server, struct sockaddr_in &server_address, int delay){

    client = socket(AF_INET, SOCK_STREAM, 0);

    if(client < 0){
        std::cout << ERROR_S << "не удалось создать сокет";
        return -1;
    }

    std::cout << "SERVER: сокет успешно создан\n";

    server_address.sin_port = htons(DEFAULT_PORT);
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = htons(INADDR_ANY);

    //Связывание сокета
    int bind_res = bind(client, reinterpret_cast<struct sockaddr*>(&server_address), sizeof(server_address));

    while(bind_res < 0){
        std::cout << ERROR_S << "ошибка связывания сокета, пробуем снова\n";
        std::this_thread::sleep_for(std::chrono::seconds(5));
        bind_res = bind(client, reinterpret_cast<struct sockaddr*>(&server_address), sizeof(server_address));
    }

    std::cout << "SERVER: сокет успешно связан\n";
    std::cout << "SERVER: ожидание клиентов....\n";

    socklen_t size = sizeof(server_address);

    listen(client, 1);
    server = accept(client, reinterpret_cast<struct sockaddr*>(&server_address), &size);

    //При возникновении проблем при принятии подключения
    if(server < 0){
        std::cout << ERROR_S << "Клиент отклонён";
        return -1;
    }

    std::cout << "=> Соединено с клиентом\n";
    return 0;
}

//Метод приёма и обработки данных
void reciever::recieve_data() {

    //Установка соединения с клиентом
    int conn_res = wait_for_connection(client, server, server_address);

    //При возникновении ошибки подключения
    if(conn_res == -1){
        close(client);
        close(server);
        exit(0);
    }

    char buffer[1024]{0};
    long recieved_value;

    while(server > 0){
        int bytes_recieved = recv(server, buffer, 1024,0);

        //При ошибке приёма данных
        if(bytes_recieved == 0){
            close(client);
            close(server);
            std::cout << "Клиент недоступен, ожидание клиента..." << std::endl;
            wait_for_connection(client, server, server_address);
            continue;
        }

        //Обработка полученного значения
        recieved_value = std::stoi(buffer);

        if(recieved_value > 99 && recieved_value % 32 == 0){
            std::cout << "Получено: " << buffer << std::endl;
        }else{
            std::cout << "Ошибка передачи: " << buffer << std::endl;
        }
    }
}
//
// Created by maksimk30 on 29.01.2024.
//
#include "../Headers/transmitter.h"

transmitter::transmitter() {
    buffer_is_empty = true;
}

transmitter::~transmitter(){
    close(client);
}

//Метод подключения к серверу
void transmitter::wait_for_connection(int &client, struct sockaddr_in &server_address, int delay){

    client = socket(AF_INET, SOCK_STREAM, 0);

    if (client < 0) {
        std::cout << ERROR_S << "не удалось создать сокет";
        exit(0);
    }

    std::cout << "SERVER: сокет успешно создан\n";

    server_address.sin_port = htons(DEFAULT_PORT);
    server_address.sin_family = AF_INET;
    inet_pton(AF_INET, SERVER_IP, &server_address.sin_addr);

    std::cout
            << "Подключение к серверу "
            << inet_ntoa(server_address.sin_addr)
            << " с портом "
            << DEFAULT_PORT
            << std::endl;

    std::cout << "Ожидание подключения к серверу...\n";

    int connect_res = connect(client, reinterpret_cast<struct sockaddr *>(&server_address), sizeof(server_address));

    while (connect_res < 0) {
        connect_res = connect(client, reinterpret_cast<struct sockaddr *>(&server_address), sizeof(server_address));
        std::this_thread::sleep_for(std::chrono::milliseconds(delay));
    }
    std::cout << "Подключено к серверу\n";
}

//Метод получения данных от пользователя
void transmitter::get_data() {
    std::string user_input;
    std::string temp_string;
    bool bad_string = false;

    while(true){
        std::getline(std::cin, user_input);

        if(user_input.length() > 64 || user_input[0] == '\0'){
            std::cout << "Строка должна быть длинной от 1 до 64 символов\n";
            continue;
        }

        for(char c : user_input){
            if(!isdigit(c)){
                bad_string = true;
                break;
            }
        }

        //Если строка содержит символы - переход к следующей итерации
        if(bad_string){
            std::cout << "Строка должна содержать только цифры\n";
            bad_string = false;
            continue;
        }

        //Сортировка по убыванию
        std::sort(user_input.begin(), user_input.end(), std::greater<int>());

        //Замена чётных символов
        for(char c : user_input){
            if(((short) c) % 2 == 0){
                temp_string += "KB";
            }else{
                temp_string += c;
            }
        }

        //Блокировка и изменение данных буффера
        buffer_mutex.lock();

        buffer.push_back(temp_string);
        buffer_is_empty = false;

        buffer_mutex.unlock();

        //Обнуление собранной строки
        temp_string = "";
    }
}

//Метод обработки данных в буффере
void transmitter::send_data() {
    std::string send_string;
    long sum;
    int select_res;
    fd_set s_set;
    timeval timeout = {0, 100000};

    //Подключение к серверу
    wait_for_connection(client, server_address);

    while(true){

        //Проверка заполненности буффера
        if(buffer_is_empty == true){
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            continue;
        }

        /*//Проверка соединения с сервером
        select_res = select(client + 1, &s_set, 0, 0, &timeout);

        //Если соединение отсутствует - ожидание переподключения
        if(select_res != 0){
            std::cout << "Сервер недоступен, пытаемся переподключиться..." << std::endl;
            wait_for_connection(client, server_address);
            continue;
        }*/

        FD_ZERO(&s_set);
        FD_SET(client, &s_set);

        //Блоировка ресурсов для обработки данных
        buffer_mutex.lock();

        send_string = buffer.at(0);

        std::cout << "Отправлено: " << send_string << std::endl;

        //Исключение из строки KB
        send_string = std::regex_replace(send_string, std::regex("KB"), "");


        for(char c : send_string){
            sum += std::atoi(&c);
        }

        //Проверка соединения с сервером
        select_res = select(client + 1, &s_set, 0, 0, &timeout);

        //Если соединение отсутствует - ожидание переподключения
        if(select_res != 0){
            std::cout << "Сервер недоступен, пытаемся переподключиться..." << std::endl;
            wait_for_connection(client, server_address);
            buffer_mutex.unlock();
            continue;
        }

        buffer.erase(buffer.begin());

        buffer_is_empty = buffer.empty();

        buffer_mutex.unlock();

        //Отправка на сервер обработанных данных
        int send_res = send(client, std::to_string(sum).c_str(), sizeof(sum), 0);

        //Повторная отправка, если сообщение не дошло
        while(send_res == 0){
            send_res = send(client, std::to_string(sum).c_str(), sizeof(sum), 0);
        }

        sum = 0;
    }
}
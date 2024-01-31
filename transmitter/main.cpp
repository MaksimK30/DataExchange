#include <iostream>
#include "Classes/Headers/transmitter.h"

int main(){
    transmitter tm;

    std::thread get_data_thread(&transmitter::get_data, std::ref(tm));
    std::thread send_data_thread(&transmitter::send_data, std::ref(tm));

    get_data_thread.join();
    send_data_thread.join();

    return 0;
}
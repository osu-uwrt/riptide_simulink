#include <stdlib.h>
#include <iostream>
#include <cmath>
#include <chrono>
#include <unistd.h>

using namespace std;

int main(){
    double initial_time = chrono::system_clock::now().time_since_epoch().count() * pow(10, -9);

    double previous_time = chrono::system_clock::now().time_since_epoch().count() * pow(10, -9);
    double previous_velocity = 0;
    double previous_position = 0;

    while(time(0) - 10 < initial_time){
        double time_step = chrono::system_clock::now().time_since_epoch().count()  * pow(10, -9) - previous_time;
        previous_time = chrono::system_clock::now().time_since_epoch().count() * pow(10, -9);
        double base_acceleration =  rand() % 20 - 9;

        double velocity = base_acceleration * time_step + previous_velocity;
        double position = velocity * time_step + previous_position;

        std::cout << position << endl;

        previous_position = position;
        previous_velocity = velocity;

        usleep(100000);
    }
    


}
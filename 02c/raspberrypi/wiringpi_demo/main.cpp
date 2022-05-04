#include<wiringPi.h>
#include<iostream>

const int LEDpin=1;

int main(){

    if(-1==wiringPiSetup())
    {
        std::cerr<<"setup error\n";
    } 
    pinMode(LEDpin,OUTPUT);      


    for(size_t i=0;i<10;++i)
    {
            digitalWrite(LEDpin,HIGH); 
            delay(600);
            digitalWrite(LEDpin,LOW);
            delay(600);

    }
    
    std::cout<<"------------bye-------------"<<std::endl;
    return 0; 
}
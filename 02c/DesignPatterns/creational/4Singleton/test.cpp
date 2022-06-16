
#include"call_once_SingLeton.h"
// #include "LazySingLeton.h"

#include<iostream>
#include<thread>

int main()
{
    std::thread t([]{});  //什么都没有做
    t.join();
    std::cout<<call_once_SingLeton::getInstance().teststring<<std::endl;
    // std::cout<<LazySingleton::getInstance().a<<std::endl;
    return 0;
}
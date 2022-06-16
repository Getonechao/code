#include"FactoryMethod.h"
#include<iostream>

void Apple::run()
{
    std::cout<<"this is Apple"<<std::endl;
}

void Banana::run()
{
    std::cout<<"this is Banana"<<std::endl;
}



Product* AppleFactory::createProduct()
{
    return new Apple();
}

Product* BananaFactory::createProduct()
{
    return new Banana();
}
#include"simpleFactory.h"

int main()
{
    simpleFactory* Factory=new simpleFactory();

    Product* Product=Factory->createProduct("Banana");

    Product->run();


    return 0;

}
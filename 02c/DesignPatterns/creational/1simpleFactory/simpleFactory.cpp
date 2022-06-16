#include"simpleFactory.h"


void Apple::run()
{
    std::cout<<"this is Apple"<<std::endl;
}

void Banana::run()
{
    std::cout<<"this is Banana"<<std::endl;
}

/**
 * @brief 映射产品名字与产品code
 * 
 * @param name 
 * @return uint16_t 
 */
uint16_t simpleFactory::mapping(std::string name)
{
    if("Apple"==name){return 1;}
    if("Banana"==name){return 2;}
}



Product* simpleFactory::createProduct(std::string product_name){
    //映射<product_name---product_code>
    uint16_t product_code=mapping(product_name);

    switch (product_code)
    {
    case 1:
        return new Apple();
        break;
    case 2:
        return new Banana();
    default:
        break;
    }
}
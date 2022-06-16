#include<memory>
#include"FactoryMethod.h"

int main()
{
 
    // 1.创建工厂
    auto  Factory=std::make_shared<BananaFactory>();
    // 2.生产产品
    std::shared_ptr<Product> Product(Factory->createProduct());
    // 3.执行产品功能 
    Product->run();
    
    return 0;
}
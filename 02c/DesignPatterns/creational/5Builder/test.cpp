#include"builder.h"

int main(){
    // 1. 创建一个建造者实例
    builder* builder_obj=new concretebuilder_1();
    // 2. 创建一个导向器实例
    director director_obj(builder_obj);
    // 3. 参数设置
    director_obj.createProduct();
    // 4. 获取产品
    product* Product_obj=builder_obj->getProduct();

    Product_obj->show();
}
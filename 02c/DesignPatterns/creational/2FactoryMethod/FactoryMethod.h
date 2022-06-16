/**
 * @file Factory.h
 * @author zhangchao (you@domain.com)
 * @brief 工厂方法模式
 * @version 0.1
 * @date 2022-06-12
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#ifndef FACTORY_METHOD_H
#define FACTORY_METHOD_H


/**
 * @brief 产品的基类
 * 
 */
class Product
{
private:
  
public:
    Product(){};
    virtual ~Product(){};

    virtual void run()=0;
};

class Apple:public Product
{
private:
    
public:
    Apple(){};
    ~Apple(){};
    void run();
};

class Banana:public Product
{
private:
    
public:
    Banana(){};
    ~Banana(){};
    void run();
};


/**
 * @brief 工厂的基类
 * 
 */
class Factory
{
private:
    
public:
    Factory()=default;
    virtual ~Factory(){};
    virtual Product* createProduct()=0;
};


class AppleFactory:public Factory
{
private:
public:
    AppleFactory()=default;
    ~AppleFactory(){};
    Product* createProduct();
};


class BananaFactory:public Factory
{
private:
public:
    BananaFactory()=default;
    ~BananaFactory(){};
    Product* createProduct();
};

#endif
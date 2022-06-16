/**
 * @file simpleFactory.h
 * @author zhangchao (you@domain.com)
 * @brief 简单工厂模式
 * @version 0.1
 * @date 2022-06-12
 * 
 * @copyright Copyright (c) 2022
 * 
 */


#ifndef SIMPLE_FACTORY
#define SIMPLE_FACTORY

#include<iostream>
#include<map>
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




class simpleFactory
{
private:
    uint16_t mapping(std::string name);
public:
    simpleFactory(){};
    ~simpleFactory(){};
    Product* createProduct(std::string product_name);


};



#endif
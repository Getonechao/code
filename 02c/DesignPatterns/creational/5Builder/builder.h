/**
 * @file builder.h
 * @author zhang chao (you@domain.com)
 * @brief builder pattern 适用于复杂对象的建立
 * @version 0.1
 * @date 2022-06-12
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#ifndef BUILDER_H
#define BUILDER_H
#include<iostream>
/**
 * @brief 产品类 对象的具有很多属性，选取固定的一组数据成为具体的builder构建的东西
 * 
 */
class product
{
private:
    std::string Head;
    std::string Body;
    std::string Hand;
public:
    product()=default;
    ~product()=default;

    void setHead(std::string Head_para){Head=Head_para;}
    void setBody(std::string Body_para){Body=Body_para;}
    void setHand(std::string Hand_para){Hand=Hand_para;}

    void show(){std::cout<<Head<<" "<<Body<<" "<<Hand<<std::endl;}
};




/**
 * @brief 生成器
 * 
 */
class builder
{
private:
    
public:
    builder(){};
    virtual ~builder(){};
    virtual void build_Head();
    virtual void build_Body();
    virtual void build_Hand();
    virtual product*  getProduct();

};

/**
 * @brief 具体的生成器1
 *   big man
 */
class concretebuilder_1:public builder
{
private:
    product* product_instance;
public:
    concretebuilder_1(){product_instance=new product();};
    ~concretebuilder_1(){delete product_instance;};
    /**
     * @brief Get the Product object
     * 
     * @return product* 
     */
    product*  getProduct(){return product_instance;}
    
    void build_Head(){ product_instance->setHead("big Head");}

    void build_Body(){product_instance->setBody("big Body");}

    void build_Hand(){product_instance->setHand("big Hand");}

};

/**
 * @brief 具体的生成器2
 *  small man
 */
class concretebuilder_2:public builder
{
private:
    product* product_instance;
public:
    concretebuilder_2(){product_instance=new product();};
    ~concretebuilder_2(){delete product_instance;};
    /**
     * @brief Get the Product object
     * 
     * @return product* 
     */
    product*  getProduct(){return product_instance;}
    
    void build_Head(){ product_instance->setHead("small Head");}

    void build_Body(){product_instance->setBody("small Body");}

    void build_Hand(){product_instance->setHand("small Hand");}


};

/**
 * @brief 导向器
 * 
 */
class director
{
private:
    builder* builder_instance;
public:
     director(builder* builder_parm){builder_instance=builder_parm;};
     ~director()=default;
    void  createProduct()
    {
        builder_instance->build_Head();
        builder_instance->build_Body();
        builder_instance->build_Hand();
    }
};




#endif
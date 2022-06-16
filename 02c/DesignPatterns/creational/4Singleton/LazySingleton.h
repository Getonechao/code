/**
 * @file LazySingleton.h
 * @author zhang chao (you@domain.com)
 * @brief 懒汉版单例模式
 * @version 0.1
 * @date 2022-06-11
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef LAZY_SINGLETON_H
#define LAZY_SINGLETON_H


#include<istream>

/**
 * @brief 静态局部变量的懒汉单例(c++11线程安全)
 * 
 */
class LazySingleton
{
private:
    //禁止copy
    LazySingleton(const LazySingleton&)=delete;
    const LazySingleton &operator=(const LazySingleton&)=delete;

protected:
     LazySingleton()=default;
     virtual ~LazySingleton()=default;
    

public:
  
    static LazySingleton& getInstance();
    std::string a="hello world";
};


#endif
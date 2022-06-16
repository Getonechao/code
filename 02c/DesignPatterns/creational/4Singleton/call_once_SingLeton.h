/**
 * @file call_once_SingLeton.h
 * @author zhang chao (you@domain.com)
 * @brief  call_once 单例模式
 * @version 0.1
 * @date 2022-06-11
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#ifndef CALL_ONCE_SINGLETON_H
#define CALL_ONCE_SINGLETON_H

#include <memory>
#include <mutex>
#include <iostream>
/**
 * @brief call_once 实现
 * 
 */
class call_once_SingLeton
{
private:
    //禁止copy
    call_once_SingLeton(const call_once_SingLeton&);
    const call_once_SingLeton &operator=(const call_once_SingLeton&);
    call_once_SingLeton(){};
    ~call_once_SingLeton(){};
    
    //实例指针（只声明不定义）
    static call_once_SingLeton* Instance; 
    static std::once_flag singletonFlag;
    //创建实例
    static void CreateInstance();
public:
    
    static call_once_SingLeton& getInstance();
    std::string teststring="hello world";
};


#endif
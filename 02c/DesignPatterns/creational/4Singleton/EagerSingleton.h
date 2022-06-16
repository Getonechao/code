/**
 * @file EagerSingleton.h
 * @author zhang chao (you@domain.com)
 * @brief 饿汉版单例模式
 * @version 0.1
 * @date 2022-06-11
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef EAGER_SINGLETON_H
#define EAGER_SINGLETON_H
#include <memory>
#include <mutex>

/**
 * @brief c++11线程安全版
 * 
 */
class EagerSingleton
{
private:
    //禁止copy
    EagerSingleton(const EagerSingleton&);
    const EagerSingleton &operator=(const EagerSingleton&);
    //
    static EagerSingleton* Instance;
public:
    static EagerSingleton* getIntance();

protected:
    EagerSingleton();
    ~EagerSingleton();
};



#endif

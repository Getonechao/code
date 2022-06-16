#include"EagerSingLeton.h"


//静态变量类外初始化
EagerSingleton* EagerSingleton::Instance=new EagerSingleton;

EagerSingleton* EagerSingleton::getIntance()
{
    return EagerSingleton::Instance;
}
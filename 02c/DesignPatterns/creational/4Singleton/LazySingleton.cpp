#include "LazySingleton.h"


LazySingleton& LazySingleton::getInstance()
{
    static LazySingleton Instance;
    
    return Instance;

}
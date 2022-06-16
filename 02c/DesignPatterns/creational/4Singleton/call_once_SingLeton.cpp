#include"call_once_SingLeton.h"
#include <iostream>



call_once_SingLeton* call_once_SingLeton::Instance; 
std::once_flag call_once_SingLeton::singletonFlag;

void call_once_SingLeton::CreateInstance()
{
    std::cout<<"2\n";
    call_once_SingLeton::Instance = new call_once_SingLeton();
};

call_once_SingLeton& call_once_SingLeton::getInstance()
{
    std::cout<<"1\n";
    /**
     * @brief
     *          异常:
     *           terminate called after throwing an instance of 'std::system_error'
     *           what():  Unknown error -1
     *           Aborted
     *          原因：std::call_once中使用了系统线程
     *           解决方案：在程序中使用一次thread线程类，并且CMakeLists.txt中添加pthrad或 find_package(Threads REQUIRED) target_link_libraries(${PROJECT_NAME} PRIVATE Threads::Threads)
     */
    // 异常std::system_error std::call_once需要链接pthread
    std::call_once(call_once_SingLeton::singletonFlag,[](){
        call_once_SingLeton::Instance = new call_once_SingLeton();
    });
    std::cout<<"3\n";
    return *(call_once_SingLeton::Instance);
};

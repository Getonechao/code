
#include "unistd.h"
#include "string.h"
#include <stdio.h>


ENV_t ENV;


void initexe(bool option)
{
    /*EXEPATH*/
    readlink("/proc/self/exe",ENV.EXEPATH,sizeof(ENV.EXEPATH));
    /*CWDPATH*/
    getcwd(ENV.CWDPATH,sizeof(ENV.CWDPATH));
    /*EXENAME*/
    char* temp_EXENAME=strrchr(ENV.EXEPATH,'/')+1;
    memcpy(ENV.EXENAME,temp_EXENAME,sizeof(ENV.EXENAME));
    /*EXEDIR*/
    memcpy(ENV.EXEDIR,ENV.EXEPATH,sizeof(ENV.EXEPATH));
    for (int i = sizeof(ENV.EXEDIR)-1; i >= 0; i--) {
		if (ENV.EXEDIR[i] == '/') {
			ENV.EXEDIR[i + 1] = '\0';
			break;
        }
    }
    /* 打印信息 */
    if(option)
    {
        printf("\
    -----------------------------------当前程序环境---------------------------------\
        \r\n\t\033[34mEXEPATH\033[0m:%s\
        \r\n\t\033[34mEXENAME\033[0m:%s\
        \r\n\t\033[34mEXEDIR\033[0m:%s\
        \r\n\t\033[34mCWDPATH\033[0m:%s\n\
    --------------------------------------------------------------------------------\r\n\
        ",ENV.EXEPATH,ENV.EXENAME,ENV.EXEDIR,ENV.CWDPATH);
    }
}
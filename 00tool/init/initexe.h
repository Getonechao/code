#ifndef CH_INITEXE_H
#define CH_INITEXE_H

#define CHAR_LENGTH 128

#ifndef ON
#define ON 1
#endif

#ifndef OFF
#define OFF 0
#endif



/*************/



/*程序环境*/
typedef struct ENV_t
{
    char EXEPATH[CHAR_LENGTH];
    char CWDPATH[CHAR_LENGTH];
    char EXEDIR[CHAR_LENGTH];
    char EXENAME[CHAR_LENGTH];
}ENV_t;

extern ENV_t ENV;

void initexe(bool option);

#endif
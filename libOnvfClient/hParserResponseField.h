#ifndef _HPARSERRESPONSEFIELD_H_
#define _HPARSERRESPONSEFIELD_H_

#include <string>
#include <typeinfo>


template<class Type>
void ParserResponseField(char *pcResponseBuf, const char *pcField, Type *ptType, const char *pcEnd = "\0")
{
    char *pcStartParam, *pcEndParam;
    pcStartParam = strstr(pcResponseBuf, pcField);
    if(pcStartParam != NULL)
    {
        pcStartParam += strlen(pcField);
        if(strcmp(typeid(Type).name(), typeid(int).name()) == 0  ||
                strcmp(typeid(Type).name(), typeid(LONG).name()) == 0   )
        {
            int *pType = (int *)ptType;
            *pType = atoi(pcStartParam);
        }
        else if(strcmp(typeid(Type).name(), typeid(bool).name()) == 0)
        {
            bool *pType = (bool *)ptType;
            *pType = (atoi(pcStartParam)!=0);
        }
        else if(strcmp(typeid(Type).name(), typeid(std::string).name()) == 0)
        {
            std::string *pType = (std::string *)ptType;
            pType->clear();
            pcEndParam = strstr(pcStartParam, pcEnd);
            pType->append(pcStartParam, (pcEndParam)?pcEndParam-pcStartParam:strlen(pcStartParam));
            pType->append("\0", 1);
        }           /*
		else if(strcmp(typeid(Type).name(), typeid(AnsiString).name()) == 0)
		{
		    pcEndParam = strstr(pcStartParam, pcEnd);
		    if(pcEndParam)
                *(AnsiString *)ptType = AnsiString(pcStartParam, pcEndParam - pcStartParam) + "\0";
            else
                *(AnsiString *)ptType = "\0";
		}              */
        else if(strcmp(typeid(Type).name(), typeid(char).name()) == 0)
        {
            char *pType = (char *)ptType;
            //*pType='\0';

            if(*pcEnd != '\0')
            {
                pcEndParam = strstr(pcStartParam, pcEnd);
                if(pcEndParam)
                {
                    memcpy(pType, pcStartParam, pcEndParam-pcStartParam);
                    pType[pcEndParam-pcStartParam] = '\0';
                }
                else
                {
                    memcpy(pType, pcStartParam, strlen(pcStartParam));
                    pType[strlen(pcStartParam)] = '\0';
                }
            }
            else
            {
                memcpy(pType, pcStartParam, strlen(pcStartParam));
                pType[strlen(pcStartParam)] = '\0';
            }
        }
        else if(strcmp(typeid(Type).name(), typeid(unsigned char).name()) == 0)
        {
            unsigned char *pType = (unsigned char *)ptType;
            //*pType='\0';

            if(*pcEnd != '\0')
            {
                pcEndParam = strstr(pcStartParam, pcEnd);
                if(pcEndParam)
                {
                    memcpy(pType, pcStartParam, pcEndParam-pcStartParam);
                    pType[pcEndParam-pcStartParam] = '\0';
                }
                else
                {
                    memcpy(pType, pcStartParam, strlen(pcStartParam));
                    pType[strlen(pcStartParam)] = '\0';
                }
            }
            else
            {
                memcpy(pType, pcStartParam, strlen(pcStartParam));
                pType[strlen(pcStartParam)] = '\0';
            }
        }
    }
}


template<class Type>
void AddParamter(char *pcCmd,char *name,Type value)
{
    char buf[32];
    buf[0]='\0';
    char *pcPos(buf);
    if(strcmp(typeid(Type).name(), typeid(int).name()) == 0  ||
            strcmp(typeid(Type).name(), typeid(long).name()) == 0 )
        sprintf(pcPos, "%d",value);
    else if(strcmp(typeid(Type).name(), typeid(unsigned int).name()) == 0 ||
            strcmp(typeid(Type).name(), typeid(unsigned long).name()) == 0 )
        sprintf(pcPos, "%u",value);
    else if(strcmp(typeid(Type).name(), typeid(bool).name()) == 0)
        sprintf(pcPos, "%b",value);
    else if(strcmp(typeid(Type).name(), typeid(char).name()) == 0)
        sprintf(pcPos, "%c",value);
    else if(strcmp(typeid(Type).name(), typeid(char *).name()) == 0)
        pcPos = (char *)value;
    else if(strcmp(typeid(Type).name(), typeid(const char *).name()) == 0)
        pcPos = (char *)(value);
    else if(strcmp(typeid(Type).name(), typeid(unsigned char).name()) == 0)
        sprintf(pcPos, "%c", value);
    else if(strcmp(typeid(Type).name(), typeid(unsigned char *).name()) == 0)
        pcPos = (char *)(value);
    else if(strcmp(typeid(Type).name(), typeid(const unsigned char *).name()) == 0)
        pcPos = (char *)value;

    if(*pcPos != '\0')
    {
        if(pcCmd[0] != '\0')
            strcat(pcCmd,"&");
        strcat(pcCmd, name);
        strcat(pcCmd, "=");
        strcat(pcCmd, pcPos);
    }
}




#endif      // end of #ifndef _HPARSERRESPONSEFIELD_H_





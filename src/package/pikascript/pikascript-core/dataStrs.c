#include "dataStrs.h"
#include "dataString.h"
#include <stdio.h>
#include <stdarg.h>

Args *New_strBuff(void)
{
    return New_args(NULL);
}

char *strsRemovePrefix(Args *buffs, char *inputStr, char *prefix)
{
    int32_t size = strGetSize(inputStr);
    char *buff = args_getBuff(buffs, size);
    return strRemovePrefix(inputStr, prefix, buff);
}

char *strsGetDirectStr(Args *buffs, char *argPath)
{
    char *directStr = NULL;
    directStr = strsCut(buffs, argPath, '"', '"');
    if (NULL != directStr)
    {
        return directStr;
    }
    directStr = strsCut(buffs, argPath, '\'', '\'');
    if (NULL != directStr)
    {
        return directStr;
    }
    return NULL;
}


char *strsAppend(Args *buffs, char *strOrigin, char *strToAppend)
{
    int32_t size = strGetSize(strOrigin) + strGetSize(strToAppend);
    char *buff = args_getBuff(buffs, size);
    char *strOut = strCopy(buff, strOrigin);
    strAppend(strOut, strToAppend);
    return strOut;
}

char *strsGetLastToken(Args *buffs, char *argPath, char sign)
{
    int32_t size = strGetSize(argPath);
    char *buff = args_getBuff(buffs, size);
    return strGetLastToken(buff, argPath, sign);
}

char *strsCut(Args *buffs, char *strIn, char startSign, char endSign)
{
    int32_t size = strGetSize(strIn);
    char *buff = args_getBuff(buffs, size);
    return strCut(buff, strIn, startSign, endSign);
}

char *strsDeleteChar(Args *buffs, char *strIn, char ch)
{
    int32_t size = strGetSize(strIn);
    return strDeleteChar(args_getBuff(buffs, size), strIn, ch);
}

char *strsGetFirstToken(Args *buffs, char *strIn, char sign)
{
    int32_t size = strGetSize(strIn);
    return strGetFirstToken(args_getBuff(buffs, size), strIn, sign);
}

char *strsPopToken(Args *buffs, char *tokens, char sign)
{
    int32_t size = strGetSize(tokens);
    char *buff = args_getBuff(buffs, size);
    return strPopToken(buff, tokens, sign);
}

char *strsCopy(Args *buffs, char *source)
{
    int32_t size = strGetSize(source);
    char *buff = args_getBuff(buffs, size);
    return strCopy(buff, source);
}

char *strsFormat(Args *buffs, uint16_t buffSize, const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    char *res = args_getBuff(buffs, buffSize);
    vsnprintf(res, buffSize, fmt, args);
    va_end(args);
    return res;
}
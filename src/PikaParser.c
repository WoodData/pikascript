#include "PikaParser.h"
#include "BaseObj.h"
#include "PikaObj.h"
#include "dataQueue.h"
#include "dataQueueObj.h"
#include "dataStack.h"
#include "dataStrs.h"

char* strsPopStmts(Args* buffs, char* stmts) {
    int32_t size = strGetSize(stmts);
    if (0 == size) {
        return NULL;
    }
    char* strOut = args_getBuff(buffs, size);
    int32_t iOut = 0;
    char delChar = ' ';
    int32_t stmtEnd = 0;
    uint8_t isGetSign = 0;
    int32_t parentheseDeepth = 0;
    for (int32_t i = 0; i < size; i++) {
        if ('(' == stmts[i]) {
            parentheseDeepth++;
        }
        if (')' == stmts[i]) {
            parentheseDeepth--;
        }
        if (parentheseDeepth == 0) {
            if (',' == stmts[i]) {
                stmtEnd = i;
                isGetSign = 1;
                break;
            }
        }
    }
    if (!isGetSign) {
        stmtEnd = size;
    }
    for (int32_t i = 0; i < stmtEnd; i++) {
        strOut[i] = stmts[i];
    }
    memmove(stmts, stmts + stmtEnd + 1, size);
    strOut[stmtEnd] = 0;
    return strOut;
}

AST* AST_parseStmt(AST* ast, char* stmt) {
    Args* buffs = New_strBuff();
    char* assignment = strsGetFirstToken(buffs, stmt, '(');
    char* direct = NULL;
    char* method = NULL;
    char* ref = NULL;
    char* str = NULL;
    char* num = NULL;
    char* right = NULL;
    uint8_t directExist = 0, isMethod = 0, isRef = 0, isStr = 0, isNum = 0;
    if (strIsContain(assignment, '=')) {
        directExist = 1;
    }
    if (directExist) {
        right = strsGetLastToken(buffs, assignment, '=');
    } else {
        right = assignment;
    }
    if (strIsContain(stmt, '(') || strIsContain(stmt, ')')) {
        isMethod = 1;
        isStr = 0;
        isNum = 0;
        isRef = 0;
    }
    if (!isMethod && (strIsContain(right, '\'') || strIsContain(right, '\"'))) {
        isMethod = 0;
        isStr = 1;
        isNum = 0;
        isRef = 0;
    }
    if (!isMethod && !isStr && (right[0] >= '0' && right[0] <= '9')) {
        isMethod = 0;
        isStr = 0;
        isNum = 1;
        isRef = 0;
    }
    if (!isMethod && !isStr && !isNum && !strEqu(stmt, "")) {
        isMethod = 0;
        isStr = 0;
        isNum = 0;
        isRef = 1;
    }
    if (directExist) {
        direct = strsGetFirstToken(buffs, assignment, '=');
        obj_setStr(ast, (char*)"direct", direct);
    }
    char* subStmts = NULL;
    if (isMethod) {
        method = right;
        obj_setStr(ast, (char*)"method", method);
        subStmts = strsCut(buffs, stmt, '(', ')');
        while (1) {
            char* subStmt = strsPopStmts(buffs, subStmts);
            if (NULL == subStmt) {
                break;
            }
            queueObj_pushObj(ast, (char*)"stmt");
            AST_parseStmt(queueObj_getCurrentObj(ast), subStmt);
        }
        goto exit;
    }
    if (isRef) {
        ref = right;
        obj_setStr(ast, (char*)"ref", ref);
        goto exit;
    }
    if (isStr) {
        str = right;
        str = strsDeleteChar(buffs, str, '\'');
        str = strsDeleteChar(buffs, str, '\"');
        obj_setStr(ast, (char*)"str", str);
        goto exit;
    }
    if (isNum) {
        num = right;
        obj_setStr(ast, (char*)"num", num);
        goto exit;
    }
exit:
    args_deinit(buffs);
    return ast;
}

static int32_t getPyLineBlockDeepth(char* line) {
    uint32_t size = strGetSize(line);
    for (int i = 0; i < size; i++) {
        if (line[i] != ' ') {
            uint32_t spaceNum = i;
            if (0 == spaceNum % 4) {
                return spaceNum / 4;
            }
            /* space Num is not 4N, error*/
            return -1;
        }
    }
}

AST* pikaParseLine(char* line, Stack* blockStack) {
    AST* ast = New_queueObj();
    Args* buffs = New_strBuff();
    uint8_t blockDeepth = getPyLineBlockDeepth(line);
    uint8_t blockDeepthLast = blockDeepth;
    if (NULL != blockStack) {
        blockDeepthLast = args_getInt(blockStack, "top");
        for (int i = 0; i < blockDeepthLast - blockDeepth; i++) {
            QueueObj* exitBlock = obj_getObj(ast, "exitBlock", 0);
            if (NULL == exitBlock) {
                obj_newObj(ast, "exitBlock", "", TinyObj);
                exitBlock = obj_getObj(ast, "exitBlock", 0);
                queueObj_init(exitBlock);
            }
            char buff[10] = {0};
            char* blockType = stack_popStr(blockStack, buff);
            queueObj_pushStr(exitBlock, blockType);
        }
    }
    obj_setInt(ast, "blockDeepth", blockDeepth);
    char* lineStart = line + blockDeepth * 4;
    char* stmt = lineStart;
    if (0 == strncmp(lineStart, (char*)"while ", 6)) {
        stmt = strsCut(buffs, lineStart, ' ', ':');
        obj_setStr(ast, "contralFlow", "while");
        if (NULL != blockStack) {
            stack_pushStr(blockStack, "while");
        }
    }
    if (0 == strncmp(lineStart, (char*)"if ", 3)) {
        stmt = strsCut(buffs, lineStart, ' ', ':');
        obj_setStr(ast, "contralFlow", "if");
        if (NULL != blockStack) {
            stack_pushStr(blockStack, "if");
        }
    }
    stmt = strsGetCleanCmd(buffs, stmt);
    ast = AST_parseStmt(ast, stmt);
    goto exit;
exit:
    args_deinit(buffs);
    return ast;
}

char* pikaParseLineToAsm(Args* buffs, char* line, Stack* blockStack) {
    AST* ast = pikaParseLine(line, blockStack);
    char* pikaAsm = AST_toPikaAsm(ast, buffs);
    AST_deinit(ast);
    return pikaAsm;
}

char* pikaParseMultiLineToAsm(Args* outBuffs, char* multiLine) {
    Stack* blockStack = New_Stack();
    Arg* pikaAsmBuff = arg_setStr(NULL, "", "");
    Arg* multiLineBuff = arg_setStr(NULL, "", multiLine);
    while (1) {
        Args* singleRunBuffs = New_strBuff();
        char* multiLine = strsCopy(singleRunBuffs, arg_getStr(multiLineBuff));
        arg_deinit(multiLineBuff);
        char* line = strsPopToken(singleRunBuffs, multiLine, '\n');
        multiLineBuff = arg_setStr(NULL, "", multiLine);
        char* singleAsm = pikaParseLineToAsm(singleRunBuffs, line, blockStack);
        char* pikaAsm = arg_getStr(pikaAsmBuff);
        pikaAsm = strsAppend(singleRunBuffs, pikaAsm, singleAsm);
        arg_deinit(pikaAsmBuff);
        pikaAsmBuff = arg_setStr(NULL, "", pikaAsm);
        args_deinit(singleRunBuffs);
        if (strGetSize(multiLine) == 0) {
            break;
        }
    }
    char* multiAsm = strsCopy(outBuffs, arg_getStr(pikaAsmBuff));
    arg_deinit(multiLineBuff);
    arg_deinit(pikaAsmBuff);
    stack_deinit(blockStack);
    return multiAsm;
}

char* AST_appandPikaAsm(AST* ast, AST* subAst, Args* buffs, char* pikaAsm) {
    uint32_t deepth = obj_getInt(ast, "deepth");
    while (1) {
        QueueObj* subStmt = queueObj_popObj(subAst);
        if (NULL == subStmt) {
            break;
        }
        obj_setInt(ast, "deepth", deepth + 1);
        pikaAsm = AST_appandPikaAsm(ast, subStmt, buffs, pikaAsm);
    }
    char* method = obj_getStr(subAst, "method");
    char* ref = obj_getStr(subAst, "ref");
    char* direct = obj_getStr(subAst, "direct");
    char* str = obj_getStr(subAst, "str");
    char* num = obj_getStr(subAst, "num");
    if (NULL != ref) {
        char buff[32] = {0};
        sprintf(buff, "%d REF %s\n", deepth, ref);
        pikaAsm = strsAppend(buffs, pikaAsm, buff);
    }
    if (NULL != method) {
        char buff[32] = {0};
        sprintf(buff, "%d RUN %s\n", deepth, method);
        pikaAsm = strsAppend(buffs, pikaAsm, buff);
    }
    if (NULL != str) {
        char buff[32] = {0};
        sprintf(buff, "%d STR %s\n", deepth, str);
        pikaAsm = strsAppend(buffs, pikaAsm, buff);
    }
    if (NULL != num) {
        char buff[32] = {0};
        sprintf(buff, "%d NUM %s\n", deepth, num);
        pikaAsm = strsAppend(buffs, pikaAsm, buff);
    }

    if (NULL != direct) {
        char buff[32] = {0};
        sprintf(buff, "%d OUT %s\n", deepth, direct);
        pikaAsm = strsAppend(buffs, pikaAsm, buff);
    }
    obj_setInt(ast, "deepth", deepth - 1);
    return pikaAsm;
}

static char* addBlockDeepth(AST* ast,
                            Args* buffs,
                            char* pikaAsm,
                            uint8_t deepthOffset) {
    pikaAsm = strsAppend(buffs, pikaAsm, (char*)"B");
    char buff[11];
    pikaAsm = strsAppend(
        buffs, pikaAsm,
        fast_itoa(buff, obj_getInt(ast, "blockDeepth") + deepthOffset));
    pikaAsm = strsAppend(buffs, pikaAsm, (char*)"\n");
}

char* AST_toPikaAsm(AST* ast, Args* buffs) {
    Args* runBuffs = New_strBuff();
    char* pikaAsm = strsCopy(runBuffs, "");
    QueueObj* exitBlock = obj_getObj(ast, "exitBlock", 0);
    if (NULL != exitBlock) {
        while (1) {
            uint8_t blockTypeNum = obj_getInt(exitBlock, "top") -
                                   obj_getInt(exitBlock, "bottom") - 1;
            char* blockType = queueObj_popStr(exitBlock);
            if (NULL == blockType) {
                break;
            }
            /* goto the while start when exit while block */
            if (strEqu(blockType, "while")) {
                pikaAsm = addBlockDeepth(ast, buffs, pikaAsm, blockTypeNum);
                pikaAsm = strsAppend(buffs, pikaAsm, (char*)"0 JMP -1\n");
            }
        }
    }
    pikaAsm = addBlockDeepth(ast, buffs, pikaAsm, 0);
    obj_setInt(ast, "deepth", 0);
    pikaAsm = AST_appandPikaAsm(ast, ast, runBuffs, pikaAsm);
    if (strEqu(obj_getStr(ast, "contralFlow"), "while")) {
        pikaAsm = strsAppend(runBuffs, pikaAsm, "0 JEZ 2\n");
    }
    if (strEqu(obj_getStr(ast, "contralFlow"), "if")) {
        pikaAsm = strsAppend(runBuffs, pikaAsm, "0 JEZ 1\n");
    }
    pikaAsm = strsCopy(buffs, pikaAsm);
    args_deinit(runBuffs);
    return pikaAsm;
}

int32_t AST_deinit(AST* ast) {
    return obj_deinit(ast);
}
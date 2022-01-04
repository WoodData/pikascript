#include "gtest/gtest.h"
extern "C" {
#include "PikaMain.h"
#include "PikaStdLib_MemChecker.h"
#include "dataArgs.h"
#include "dataMemory.h"
#include "mem_pool_config.h"
#include "pikaScript.h"
}
extern PikaMemInfo pikaMemInfo;
TEST(pikaMain, init) {
    pikaMemInfo.heapUsedMax = 0;
    PikaObj* pikaMain = pikaScriptInit();
    obj_deinit(pikaMain);
    EXPECT_EQ(pikaMemNow(), 0);
}

TEST(pikaMain, new_mem) {
    pikaMemInfo.heapUsedMax = 0;
    PikaObj* pikaMain = newRootObj((char*)"pikaMain", New_PikaMain);
    VM_Parameters* globals =
        obj_runDirect(pikaMain, (char*)
        "mem = PikaStdLib.MemChecker()\n"
        "mem.max()\n"
        );
    PikaObj* mem = (PikaObj*)args_getPtr(globals->list, (char*)"mem");
    PikaObj* memClassPtr = (PikaObj*)obj_getPtr(mem, (char*)"_clsptr");
    EXPECT_EQ((void*)memClassPtr, (void*)New_PikaStdLib_MemChecker);
    obj_deinit(pikaMain);
    // obj_deinit(globals);
    EXPECT_EQ(pikaMemNow(), 0);
}

TEST(pikaMain, list_new) {
    /* init */
    pikaMemInfo.heapUsedMax = 0;
    PikaObj* pikaMain = newRootObj((char*)"pikaMain", New_PikaMain);
    /* run */
    VM_Parameters* globals =
        obj_runDirect(pikaMain, (char*)
        "list = PikaStdData.List()\n"
        "list.append(7)\n"
        "list.append('eee')\n"
        "len = list.len()\n"
        "a1 = list.get(0)\n"
        "a2 = list.get(1)\n"
        );
    /* collect */
    int len = obj_getInt(globals, (char*)"len");
    int a1 = obj_getInt(globals, (char*)"a1");
    char* a2 = obj_getStr(globals, (char*)"a2");

    /* assert */
    EXPECT_EQ(len, 2);
    EXPECT_EQ(a1, 7);
    EXPECT_STREQ(a2, "eee");

    /* deinit */
    // obj_deinit(globals);
    obj_deinit(pikaMain);
    EXPECT_EQ(pikaMemNow(), 0);
}

TEST(pikaMain, int_float_convert) {
    /* init */
    pikaMemInfo.heapUsedMax = 0;
    PikaObj* pikaMain = newRootObj((char*)"pikaMain", New_PikaMain);
    /* run */
    VM_Parameters* globals =
        obj_runDirect(pikaMain, (char*)
        "a = 1\n"
        "a_f = float(a)\n"
        "b = 1.3\n"
        "b_i = int(b)\n"
        );
    /* collect */
    int a = obj_getInt(globals, (char*)"a");
    float a_f = obj_getFloat(globals, (char*)"a_f");
    float b = obj_getFloat(globals, (char*)"b");
    int b_i = obj_getInt(globals, (char*)"b_i");

    /* assert */
    EXPECT_EQ(a, 1);
    EXPECT_FLOAT_EQ(a_f, 1);
    EXPECT_FLOAT_EQ(b, 1.3);
    EXPECT_FLOAT_EQ(b_i, 1);

    /* deinit */
    // obj_deinit(globals);
    obj_deinit(pikaMain);
    EXPECT_EQ(pikaMemNow(), 0);
}

TEST(pikaMain, type_) {
    /* init */
    pikaMemInfo.heapUsedMax = 0;
    PikaObj* pikaMain = newRootObj((char*)"pikaMain", New_PikaMain);
    /* run */
    obj_runDirect(pikaMain, (char*)
        "type(1)\n"
        "b = 1.4\n"
        "type(b)\n"
        );
    /* collect */

    /* assert */

    /* deinit */
    // obj_deinit(globals);
    obj_deinit(pikaMain);
    EXPECT_EQ(pikaMemNow(), 0);
}

TEST(pikaMain, a_signed) {
    /* init */
    pikaMemInfo.heapUsedMax = 0;
    PikaObj* pikaMain = newRootObj((char*)"pikaMain", New_PikaMain);
    /* run */
    obj_runDirect(pikaMain, (char*)"a = -1\n");
    /* collect */
    int a = obj_getInt(pikaMain, (char*)"a");

    /* assert */
    EXPECT_EQ(-1, a);

    /* deinit */
    obj_deinit(pikaMain);
    EXPECT_EQ(pikaMemNow(), 0);
}

TEST(pikaMain, a_mm) {
    /* init */
    pikaMemInfo.heapUsedMax = 0;
    PikaObj* pikaMain = newRootObj((char*)"pikaMain", New_PikaMain);
    /* run */
    obj_runDirect(pikaMain, (char*)"a = -1.1 ** 3\n");
    /* collect */
    float a = obj_getFloat(pikaMain, (char*)"a");

    /* assert */
    EXPECT_FLOAT_EQ(-1.331, a);

    /* deinit */
    obj_deinit(pikaMain);
    EXPECT_EQ(pikaMemNow(), 0);
}

TEST(pikaMain, a_n1) {
    /* init */
    pikaMemInfo.heapUsedMax = 0;
    PikaObj* pikaMain = newRootObj((char*)"pikaMain", New_PikaMain);
    /* run */
    obj_runDirect(pikaMain, (char*)"a = ~1\n");
    /* collect */
    int a = obj_getInt(pikaMain, (char*)"a");

    /* assert */
    EXPECT_EQ(-2, a);

    /* deinit */
    obj_deinit(pikaMain);
    EXPECT_EQ(pikaMemNow(), 0);
}

TEST(pikaMain, a_l4) {
    /* init */
    pikaMemInfo.heapUsedMax = 0;
    PikaObj* pikaMain = newRootObj((char*)"pikaMain", New_PikaMain);
    /* run */
    obj_runDirect(pikaMain, (char*)"a = 1<<4\n");
    /* collect */
    int a = obj_getInt(pikaMain, (char*)"a");

    /* assert */
    EXPECT_EQ(16, a);

    /* deinit */
    obj_deinit(pikaMain);
    EXPECT_EQ(pikaMemNow(), 0);
}

TEST(pikaMain, less_equ) {
    /* init */
    pikaMemInfo.heapUsedMax = 0;
    PikaObj* pikaMain = newRootObj((char*)"pikaMain", New_PikaMain);
    /* run */
    obj_runDirect(pikaMain, (char*)
    "a = 0\n"
    "while a <= 10:\n"
    "    a = a + 1\n"
    "\n"
    );
    /* collect */
    int a = obj_getInt(pikaMain, (char*)"a");

    /* assert */
    EXPECT_EQ(11, a);

    /* deinit */
    obj_deinit(pikaMain);
    EXPECT_EQ(pikaMemNow(), 0);
}

TEST(pikaMain, and_or_not) {
    /* init */
    pikaMemInfo.heapUsedMax = 0;
    PikaObj* pikaMain = newRootObj((char*)"pikaMain", New_PikaMain);
    /* run */
    obj_runDirect(pikaMain, (char*)
    "a = 0\n"
    "b = 0\n"
    "c = 0\n"
    "if (1>2) or (2>1):\n"
    "    a = 1\n"
    "\n"
    "if (1>2) and (2>1):\n"
    "    b = 1\n"
    "\n"
    "if not (1>2):\n"
    "    c = 1\n"
    "\n"
    );
    /* collect */
    int a = obj_getInt(pikaMain, (char*)"a");
    int b = obj_getInt(pikaMain, (char*)"b");
    int c = obj_getInt(pikaMain, (char*)"c");

    /* assert */
    EXPECT_EQ(1, a);
    EXPECT_EQ(0, b);
    EXPECT_EQ(1, c);

    /* deinit */
    obj_deinit(pikaMain);
    EXPECT_EQ(pikaMemNow(), 0);
}

TEST(pikaMain, err_scop) {
    /* init */
    pikaMemInfo.heapUsedMax = 0;
    PikaObj* pikaMain = newRootObj((char*)"pikaMain", New_PikaMain);
    /* run */
    obj_run(pikaMain, (char*)"print('test'");
    /* collect */

    /* assert */

    /* deinit */
    obj_deinit(pikaMain);
    EXPECT_EQ(pikaMemNow(), 0);
}

TEST(pikaMain, __init__) {
    /* init */
    pikaMemInfo.heapUsedMax = 0;
    PikaObj* pikaMain = newRootObj((char*)"pikaMain", New_PikaMain);
    /* run */
    obj_run(pikaMain, (char*)"pin = PikaStdDevice.GPIO()\n");
    /* collect */
    char* a = obj_getStr(pikaMain, (char*)"pin.pin");
    /* assert */
    EXPECT_STREQ(a, "PA0");

    /* deinit */
    obj_deinit(pikaMain);
    EXPECT_EQ(pikaMemNow(), 0);
}

TEST(pikaMain, PikaStdData) {
    /* init */
    pikaMemInfo.heapUsedMax = 0;
    PikaObj* pikaMain = newRootObj((char*)"pikaMain", New_PikaMain);
    /* run */
    obj_run(pikaMain, (char*)
    "dict = PikaStdData.Dict()\n"
    "dict.set('a', 1)\n"
    "dict.set('b', 2)\n"
    "dict.remove('b')\n"
    "a = dict.get('a')\n"
    "b = dict.get('b')\n"
    );
    /* collect */
    int a = obj_getInt(pikaMain, (char*)"a");
    int b = obj_getInt(pikaMain, (char*)"b");
    /* assert */
    EXPECT_EQ(a, 1);
    EXPECT_EQ(b, 0);

    /* deinit */
    obj_deinit(pikaMain);
    EXPECT_EQ(pikaMemNow(), 0);
}

TEST(pikaMain, iter) {
    /* init */
    pikaMemInfo.heapUsedMax = 0;
    PikaObj* pikaMain = newRootObj((char*)"pikaMain", New_PikaMain);
    /* run */
    obj_runDirect(pikaMain, (char*)
        "list = PikaStdData.List()\n"
        "list.append(7)\n"
        "list.append('eee')\n"
        "len = list.len()\n"
        "a1 = list.get(0)\n"
        "a2 = list.get(1)\n"
        "res = iter(list)\n"
        "\n"
        );
    /* collect */
    PikaObj* res = (PikaObj*)obj_getPtr(pikaMain, (char*)"res");
    PikaObj* list = (PikaObj*)obj_getPtr(pikaMain, (char*)"list");
    /* assert */
    EXPECT_EQ(res, list);
    /* deinit */
    // obj_deinit(globals);
    obj_deinit(pikaMain);
    EXPECT_EQ(pikaMemNow(), 0);
}

TEST(pikaMain, list_for_loop) {
    /* init */
    pikaMemInfo.heapUsedMax = 0;
    PikaObj* pikaMain = newRootObj((char*)"pikaMain", New_PikaMain);
    /* run */
    obj_runDirect(pikaMain, (char*)
        "list = PikaStdData.List()\n"
        "list.append(7)\n"
        "list.append('eee')\n"
        "len = list.len()\n"
        "for item in list:\n"
        "    print(item)\n"
        "    a = item\n"
        "\n"
        );
    /* collect */
    char* a = obj_getStr(pikaMain, (char*)"a");
    /* assert */
    EXPECT_STREQ(a, (char*)"eee");
    /* deinit */
    // obj_deinit(globals);
    obj_deinit(pikaMain);
    EXPECT_EQ(pikaMemNow(), 0);
}

TEST(pikaMain, range) {
    /* init */
    pikaMemInfo.heapUsedMax = 0;
    PikaObj* pikaMain = newRootObj((char*)"pikaMain", New_PikaMain);
    /* run */
    obj_runDirect(pikaMain, (char*)
        "r = range(10, 0)\n"
        "ir = iter(r)\n"
        "\n"
        );
    /* collect */
    /* assert */
    obj_deinit(pikaMain);
    /* mem check */
    EXPECT_EQ(pikaMemNow(), 0);
}

TEST(pikaMain, for_in_range) {
    /* init */
    pikaMemInfo.heapUsedMax = 0;
    PikaObj* pikaMain = newRootObj((char*)"pikaMain", New_PikaMain);
    /* run */
    obj_runDirect(pikaMain, (char*)
        "a = 0\n"
        "for i in range(0, 10):\n"
        "    print(i)\n"
        "    a = a + i\n"
        "\n"
        );
    /* collect */
    int a = obj_getInt(pikaMain, (char*)"a");
    /* assert */
    EXPECT_EQ(a, 45);
    /* deinit */
    obj_deinit(pikaMain);
    /* mem check */
    EXPECT_EQ(pikaMemNow(), 0);
}

TEST(pikaMain, for_for_in_range) {
    /* init */
    pikaMemInfo.heapUsedMax = 0;
    PikaObj* pikaMain = newRootObj((char*)"pikaMain", New_PikaMain);
    /* run */
    obj_runDirect(pikaMain, (char*)
         "a = 0\n"
         "for i in range(0, 10):\n"
         "    for k in range(0, 3):\n"
         "        print(k)\n"
         "        a = a + k\n"
         "\n"
        );
    /* collect */
    int a = obj_getInt(pikaMain, (char*)"a");
    /* assert */
    EXPECT_EQ(a, 30);
    /* deinit */
    obj_deinit(pikaMain);
    /* mem check */
    EXPECT_EQ(pikaMemNow(), 0);
}

TEST(pikaMain, for_if_break) {
    /* init */
    pikaMemInfo.heapUsedMax = 0;
    PikaObj* pikaMain = newRootObj((char*)"pikaMain", New_PikaMain);
    /* run */
    obj_runDirect(pikaMain, (char*)
         "a = 0\n"
         "for i in range(0, 10):\n"
         "    if i == 5:\n"
         "        break\n"
         "    a = a + i\n"
         "\n"
        );
    /* collect */
    int a = obj_getInt(pikaMain, (char*)"a");
    /* assert */
    EXPECT_EQ(a, 10);
    /* deinit */
    obj_deinit(pikaMain);
    /* mem check */
    EXPECT_EQ(pikaMemNow(), 0);
}

TEST(pikaMain, for_if_continue) {
    /* init */
    pikaMemInfo.heapUsedMax = 0;
    PikaObj* pikaMain = newRootObj((char*)"pikaMain", New_PikaMain);
    /* run */
    obj_run(pikaMain, (char*)
         "a = 0\n"
         "for i in range(0, 10):\n"
         "    if i == 5:\n"
         "        continue\n"
         "    a = a + i\n"
         "\n"
        );
    /* collect */
    int a = obj_getInt(pikaMain, (char*)"a");
    /* assert */
    EXPECT_EQ(a, 40);
    /* deinit */
    obj_deinit(pikaMain);
    /* mem check */
    EXPECT_EQ(pikaMemNow(), 0);
}

/* the log_buff of printf */
extern char log_buff[LOG_BUFF_MAX][LOG_SIZE];
TEST(pikaMain, print_in_def) {
    /* init */
    pikaMemInfo.heapUsedMax = 0;
    PikaObj* pikaMain = newRootObj((char*)"pikaMain", New_PikaMain);
    /* run */
    /* the test is used to fix too many print in def */
    /* clear the log_buff */
    memset(log_buff, 0, LOG_BUFF_MAX * LOG_SIZE);
    obj_runDirect(pikaMain, (char*)
        "def test_print():\n"
        "    print('test')\n"
        "test_print()\n"
        );
    /* collect */
    /* assert */
    /* should only print once, so the second log (log_buff[1]) shuold be '\0' */
    EXPECT_STREQ(log_buff[0], "test\r\n");
    EXPECT_STREQ(log_buff[1], "");
    /* deinit */
    obj_deinit(pikaMain);
    /* mem check */
    EXPECT_EQ(pikaMemNow(), 0);
}

// TEST(pikaMain, prime_100) {
//     /* init */
//     pikaMemInfo.heapUsedMax = 0;
//     PikaObj* pikaMain = newRootObj((char*)"pikaMain", New_PikaMain);
//     /* run */
//     obj_run(pikaMain, (char*)
//             "num = 0\n"
//             "i = 2\n"
//             "for i in range(2 , 100):\n"
//             "    j=2\n"
//             "    is_prime = 1\n"
//             "    for j in range(2 , i):\n"
//             "        if i%j==0 :\n"
//             "            is_prime = 0\n"
//             "            break\n"
//             "    if is_prime:\n"
//             "        num = num + i\n"
//             "\n");
//     /* collect */
//     int num = obj_getInt(pikaMain, (char*)"num");
//     /* assert */
//     EXPECT_EQ(num, 40);
//     /* deinit */
//     obj_deinit(pikaMain);
//     /* mem check */
//     EXPECT_EQ(pikaMemNow(), 0);
// }
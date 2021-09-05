#include "gtest/gtest.h"
extern "C"
{
#include "PikaBlock.h"
#include "dataStrs.h"
#include "PikaIf.h"
#include "TinyObj.h"
#include "PikaObj.h"
}

TEST(block, init)
{
    PikaObj *block = block_init(NULL);
    block_deinit(block);
    EXPECT_EQ(pikaMemNow(), 0);
}

TEST(block, push)
{
    PikaObj *block = block_init(NULL);
    block_pushLine(block, (char *)"line1");
    block_pushLine(block, (char *)"line2");
    block_deinit(block);
    EXPECT_EQ(pikaMemNow(), 0);
}

TEST(block, pop)
{
    PikaObj *block = block_init(NULL);
    block_pushLine(block, (char *)"line1");
    block_pushLine(block, (char *)"line2");
    char *line1 = block_popLine(block);
    ASSERT_STREQ(line1, (char *)"line1");
    char *line2 = block_popLine(block);
    ASSERT_STREQ(line2, (char *)"line2");
    char *line3 = block_popLine(block);
    ASSERT_STREQ(line3, (char *)"");
    block_deinit(block);
    EXPECT_EQ(pikaMemNow(), 0);
}

TEST(block, assert1)
{
    PikaObj *block = block_init(NULL);
    {
        block_setAssert(block, (char *)"1");
        int res = block_checkAssert(block);
        ASSERT_EQ(1, res);
    }

    {
        block_setAssert(block, (char *)"0");
        int res = block_checkAssert(block);
        ASSERT_EQ(0, res);
    }
    block_deinit(block);
}

TEST(block, if1)
{
    PikaObj *block = block_init(NULL);
    if_setAssert(block, (char *)"if 1 :");
    if_pushLine(block, (char *)"    print('hello')");
    if_pushLine(block, (char *)"    print('hello2')");
    char *assert = block_getAssert(block);
    ASSERT_STREQ((char *)"1 ", assert);
    int res = block_checkAssert(block);
    char *line1 = block_popLine(block);
    ASSERT_STREQ((char *)"print('hello')", line1);
    char *line2 = block_popLine(block);
    ASSERT_STREQ((char *)"print('hello2')", line2);
    ASSERT_EQ(1, res);
    block_deinit(block);
}

TEST(block, if2)
{
    PikaObj *obj = New_TinyObj(NULL);
    obj_run(obj, (char *)"if 1 :");
    obj_deinit(obj);
}

TEST(block, if3)
{
    PikaObj *obj = New_TinyObj(NULL);
    obj_run(obj, (char *)"if 1 :");
    obj_run(obj, (char *)"    print('hello')");
    obj_run(obj, (char *)"    print('hello2')");
    PikaObj *block = obj_getObj(obj, (char *)"_block", 0);
    char *mode = block_getMode(block);
    ASSERT_STREQ((char *)"if", mode);
    char *assert = block_getAssert(block);
    ASSERT_STREQ((char *)"1 ", assert);
    char *line1 = block_popLine(block);
    ASSERT_STREQ((char *)"print('hello')", line1);
    char *line2 = block_popLine(block);
    ASSERT_STREQ((char *)"print('hello2')", line2);
    obj_deinit(obj);
}

void block_pushLine(PikaObj *self, char *line);

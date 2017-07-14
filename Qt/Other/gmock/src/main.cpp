/*********************************************************************************
**                                                                              **
**     Copyright (C) 2017                                                       **
**                                                                              **
**     This program is free software: you can redistribute it and/or modify     **
**     it under the terms of the GNU General Public License as published by     **
**     the Free Software Foundation, either version 3 of the License, or        **
**     (at your option) any later version.                                      **
**                                                                              **
**     This program is distributed in the hope that it will be useful,          **
**     but WITHOUT ANY WARRANTY; without even the implied warranty of           **
**     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            **
**     GNU General Public License for more details.                             **
**                                                                              **
**     You should have received a copy of the GNU General Public License        **
**     along with this program.  If not, see http://www.gnu.org/licenses/.      **
**                                                                              **
**********************************************************************************
**                   Author: Bikbao Rinat Zinorovich                            **
**********************************************************************************/
#include </usr/include/gmock/gmock.h>
// using namespace testing;
//--------------------------------------------------------------------------------
class Foo
{
public:
    Foo() : i(0)
    {
        std::cout << "CONSTRUCTED" << std::endl;
    }
    ~Foo()
    {
        std::cout << "DESTRUCTED" << std::endl;
    }
    int i;
};
//--------------------------------------------------------------------------------
class TestFoo : public ::testing::Test
{
protected:
    void SetUp()
    {
        foo = new Foo;
        foo->i = 5;
    }
    void TearDown()
    {
        delete foo;
    }
    Foo *foo;
};
//--------------------------------------------------------------------------------
TEST_F(TestFoo, test1)
{
    ASSERT_EQ(foo->i, 5);
    foo->i = 10;
}
//--------------------------------------------------------------------------------
TEST_F(TestFoo, test2)
{
    ASSERT_EQ(foo->i, 5);
}
//--------------------------------------------------------------------------------
int main(int argc, char** argv)
{
    testing::InitGoogleMock(&argc, argv);
    return RUN_ALL_TESTS();
}
//--------------------------------------------------------------------------------

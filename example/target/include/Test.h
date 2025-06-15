//
// Created by weichuanqi on 2025/5/19.
//

#ifndef CLUCID_TEST_H
#define CLUCID_TEST_H


#include "CLucidDefines.h"

#include <algorithm>
#include <string>


FUNCTION()
static void globalFunction();

namespace TestNamespace {
    /// -----------------  全局函数  ---------------------
    FUNCTION()
    static void globalFunctionInNameSpace();

    FUNCTION()
    static void globalFunctionInNameSpaceParam1(int a);

    FUNCTION(globalFunc)
    void globalFunctionMarked();

    FUNCTION(globalFunc)
    void globalFunctionMarkedParam1(int a);

    /// -----------------  枚举  ---------------------
    enum ENUM() EItemType{
        EItemType_1,
        EItemType_2,
        EItemType_3,
    };

    /// -----------------  结构  ---------------------
    struct CLASS() Item{
        PROPERTY()
        int itemProperty;
        PROPERTY()
        EItemType type;
    };

    /// -----------------  类  ---------------------
    class CLASS(AnnoName1 = aaa ,AnnoName2 = bbb )
    Test {

        Test(int a){

        }

        Test(const Test& t){

        }

        Test(Test&& t){

        }

        virtual ~Test(){

        }

        METHOD()
        static int staticFunction(int bb);

        PROPERTY(wantProp=aaa)
        int prop1;

        PROPERTY(wantProp=bbb)
        Item item1;

        PROPERTY()
        Item item2;

        METHOD()
        void testVoid();

        METHOD()
        void testReturnVoid();

        METHOD(wantMethod)
        int testReturnInt();
    public:
        METHOD()
        int* testReturnPoint();

        METHOD()
        int& testReturnRef();

        METHOD()
        Item* testReturnItemPoint();

        METHOD()
        Item& testReturnItemRef();

        METHOD()
        int testParam1(int a);

        METHOD()
        int testParam2(int a, std::string bb);

        METHOD()
        int testParamPoint(int* a, std::string bb);

        METHOD()
        int testParamRef(int* a, std::string& bb);

        METHOD()
        int testParamRight(int* a, std::string& bb, Item&& it);

        METHOD(wantMethod=thisismethod, nomatchAnno=kkkkk)
        const Item* testParamComplex(int* a, std::string& bb, Item&& it, const Item* const * & c);
    };


}


#endif //CLUCID_TEST_H

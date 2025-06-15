//
// Created by weichuanqi on 2025/5/19.
//
#include "Test.h"

using namespace TestNamespace;
//namespace TestNamespace{
//    void globalFunctionMarked() {
//
//    }
//
//    void Test::testVoid() {
//
//    }
//}
void TestNamespace::globalFunctionMarked() {

}

void TestNamespace::globalFunctionMarkedParam1(int a) {

}

void TestNamespace::globalFunctionInNameSpaceParam1(int a) {

}

void staticInNameSpace() {

}

void Test::testVoid() {

}

void Test::testReturnVoid() {

}

int Test::testReturnInt() {
    return 0;
}

int Test::testParam1(int a) {
    return 0;
}

int Test::testParam2(int a, std::string bb) {
    return 0;
}

int Test::testParamPoint(int *a, std::string bb) {
    return 0;
}

int Test::testParamRef(int *a, std::string &bb) {
    return 0;
}

int * Test::testReturnPoint() {
    return nullptr;
}

int & Test::testReturnRef() {
    return prop1;
}

Item * Test::testReturnItemPoint() {
    return &item1;
}

Item & Test::testReturnItemRef() {
    return item1;
}

int Test::testParamRight(int *a, std::string &bb, Item &&it) {
    return 0;
}

int Test::staticFunction(int bb) {
    return 0;
}

const Item*  Test::testParamComplex(int* a, std::string& bb, Item&& it, const Item* const * & c) {
    return &item1;
}

void globalFunction() {
}

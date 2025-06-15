//
// Created by weichuanqi on 2025/6/10.
//

#include "ReflectSystem.h"
#include <cstdio>
#include <iostream>

#include "Actor.h"

#include <iostream>
#include <cstddef>  // 用于 offsetof 宏

//
//class MyClass;
//
//class OffsetCalculator {
//public:
//    template<typename T>
//    static std::ptrdiff_t getMemberOffset(T MyClass::*member);
//};
//
//class MyClass {
//private:
//    int privateVar;
//
//public:
//    MyClass(int val) : privateVar(val) {}
//
//    // 声明 OffsetCalculator 为友元类
//    friend class OffsetCalculator;
//};
//
//template<typename T>
//std::ptrdiff_t OffsetCalculator::getMemberOffset(T MyClass::*member) {
//    MyClass obj(0, 0.0);  // 创建一个对象
//    std::ptrdiff_t offset = reinterpret_cast<char*>(&(obj.*member)) - reinterpret_cast<char*>(&obj);
//    return offset;
//}
//
//std::ptrdiff_t getOffset(const char* memberName) {
//    // 获取成员变量的偏移量
//    return ((std::ptrdiff_t)&reinterpret_cast<char const volatile&>((((MyClass*)0)->*memberName)));
//}
//
//template<typename T>
//std::ptrdiff_t getMemberOffset(T MyClass::*member) {
//    MyClass obj(0, 0.0);  // 创建一个对象
//    std::ptrdiff_t offset = reinterpret_cast<char*>(&(obj.*member)) - reinterpret_cast<char*>(&obj);
//    return offset;
//}
//
//int main() {
//    std::ptrdiff_t offset1 = OffsetCalculator::getMemberOffset(&MyClass::privateVar1);
//
//    // 使用 offsetof 宏来计算偏移量
//    auto member = &MyClass::privateVar;
//    auto* p = (MyClass*) nullptr;
//    std::ptrdiff_t offset = ((::size_t)&reinterpret_cast<char const volatile&>((((MyClass*)0)->privateVar)));
//
//    std::cout << "Offset of privateVar: " << offset << std::endl;
//
//    return 0;
//}
//
//
//ReflectSystem rs = ReflectSystem();
//
//class MyClass {
//public:
//    void memberFunction(int x) {
//        std::cout << "Member function called with " << x << std::endl;
//    }
//
//    int getValue() const {
//        return value_;
//    }
//
//    static void staticFunction() {
//        std::cout << "Static function called" << std::endl;
//    }
//
//private:
//    int value_ = 42;
//};
//
//// 获取成员函数指针的语法：返回类型 (类名::*指针名)(参数类型)
//void (MyClass::*memberFuncPtr)(int) = &MyClass::memberFunction;
//int (MyClass::*constMemberFuncPtr)() const = &MyClass::getValue;

ReflectSystem::ReflectSystem(){
//    char memArray[2048];
//
//    char* mem = memArray;
//    Actor* actor = (Actor*)mem;
//    size_t offseta = (char*)&actor->a - (char*)actor;
//
//    size_t offsetb = (char*)&actor->b - (char*)actor;
//
//    auto a = &Actor::ActorBaseMethod2;
//
////    MyClass obj;
////
////    // 方法1：使用 .* 操作符（对象）
////    (obj.*memberFuncPtr)(10);
////    int value = (obj.*constMemberFuncPtr)();
////
////    // 方法2：使用 ->* 操作符（指针）
////    MyClass* objPtr = &obj;
////    (objPtr->*memberFuncPtr)(20);
////    value = (objPtr->*constMemberFuncPtr)();
//
//    std::printf("offset:: %lld", offseta);
//    std::printf("offset:: %lld", offsetb);
//
////    std::ptrdiff_t offset = offsetof(Actor, a);
//
////    int Actor::* aa =&Actor::a;
////    auto ii = *actor.*aa;
////    std::ptrdiff_t off = reinterpret_cast<std::ptrdiff_t>(&(actor.*aa)) - (char*)actor;

}
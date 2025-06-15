//
// Created by weichuanqi on 2025/6/11.
//

#include "test/AActor.h"
#include <any>
#include <atomic>
#include <unordered_map>

void visitor_reflect_for_AActor() {
    std::unordered_map<std::string, size_t> map;
//    AActor* obj = (AActor*) nullptr;  // 创建一个对象
    AActor obj1;  // 创建一个对象
    AActor* obj = &obj1;
    size_t of = (char*)&obj->a - (char*)obj;
    map.emplace("a",of);
    ptrdiff_t offsetA = reinterpret_cast<ptrdiff_t>(&((*obj).AActor::a)) - reinterpret_cast<ptrdiff_t>(obj);
    ptrdiff_t offsetActor2 = reinterpret_cast<ptrdiff_t>(&((*obj).AActor::actor2)) - reinterpret_cast<ptrdiff_t>(obj);


    auto test1 = &AActor::test1;
    // 使用 reinterpret_cast 替代 dynamic_cast
    std::unordered_map<std::string, std::any> map1;
    map1.emplace("test1", test1);


    auto result = map1.find("test1");
    using Test1FuncPtr = int (AActor::*)(int, std::string) const;
    auto r = std::any_cast<Test1FuncPtr>(result->second);
    (*obj.*r)(1, "2");
//    auto r = dynamic_cast<int (AActor::*)(int, std::string) const>(result->second);
//    auto rr = (int (AActor::*)(int, std::string) const) *result->second;
//    auto rrr = (*obj.**rr)(1, "2");
//    ptrdiff_t offsetTest1 = reinterpret_cast<ptrdiff_t>(&((*obj).*test1)) - reinterpret_cast<ptrdiff_t>(obj);

}

//
// Created by weichuanqi on 2025/6/11.
//

#include "test/AActor2.h"

void visitor_reflect_for_AActor2() {
    AActor2* obj = (AActor2*) nullptr;  // 创建一个对象
    size_t of = (char*)&obj->b - (char*)obj;
    ptrdiff_t offset = reinterpret_cast<ptrdiff_t>(&((*obj).AActor2::b)) - reinterpret_cast<ptrdiff_t>(obj);
}

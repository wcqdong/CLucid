//
// Created by weichuanqi on 2025/5/30.
//

#include "Actor.h"

int Actor::test1(int a, std::string bb){
    return 0;
}

//const Item * const * const Actor::test2(const int* a, const Item const * const * const && it1, Item& it2, Item*&  it3, const Item&& it4, std::string bb) const{
//    return reinterpret_cast<const Item *const *>(&item);
//}

const Item *const *const Actor::test2( const int* a,  const ItemNamespace::Item* const * const && it1, ItemNamespace::Item& it2, ItemNamespace::Item*& it3,  const ItemNamespace::Item&& it4, basic_string<char, char_traits<char>, allocator<char>> bb) const{
    return reinterpret_cast<const Item *const *>(&item);
}







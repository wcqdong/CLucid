//
// Created by weichuanqi on 2025/5/30.
//

#ifndef GENERATORS_ACTOR_H
#define GENERATORS_ACTOR_H


#include "ActorBase.h"
#include "CLucidDefines.h"
#include <string>


enum class ENUM() EItemClassType{
    EItemClassType_1,
    EItemClassType_2,
    EItemClassType_3,
};

enum ENUM() EItemType{
    EItemType_1,
    EItemType_2,
    EItemType_3,
};

namespace ItemNamespace{
    class Item{
    public:
        Item(int a){

        }
    };
}


using namespace std;
using namespace ItemNamespace;

using constItem = const Item;
class CLASS() Actor : public ActorBase, public ActorBase1{

    METHOD()
    Actor(int aaa, const Item * const * & item1): a(aaa), b(7), c("jjj"), item1(item1){

    }

    ~Actor() override{

    }
    PROPERTY(serializable)
    int a;
    PROPERTY()
    int b;
    PROPERTY(serializable)
    std::string c = "kkk";
    PROPERTY(serializable)
    Item item = Item(99);

    PROPERTY(serializable)
    const Item* const* & item1;

    METHOD()
    int test1(int a, std::string bb);

    METHOD()
    const Item * const * const test2(const int* a, const Item * const * const && it1, Item& it2, Item*& it3, const Item&& it4, std::string bb) const;

public:

    METHOD()
    long ActorBaseMethod2(int a) const override  {
        return 0;
    }

    METHOD()
    bool operator==(const Actor& actor) {
        return true;
    }
    METHOD()
    Actor& operator=(const Actor& actor) {
        return *this;
    }

    METHOD()
    Actor& operator=(Actor&& actor) noexcept{
        return *this;
    }

    METHOD()
    int operator++(int)
    {
        return 2;
    }

};


#endif //GENERATORS_ACTOR_H

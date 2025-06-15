//
// Created by weichuanqi on 2025/5/30.
//

#ifndef GENERATORS_ACTOR_H
#define GENERATORS_ACTOR_H


#include "ActorBase.h"
#include "CLucidDefines.h"
#include <string>
#include <vector>


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

class Item{
public:
    Item(int a){

    }
};

class CLASS() Actor : public ActorBase, public ActorBase1{
    METHOD()
    Actor(int aaa): a(aaa), b(7), c("jjj"){

    }
private:
    PROPERTY(serializable)
    int a;
    PROPERTY()
    int b = 1;
    PROPERTY(serializable)
    std::string c = "kkk";
    PROPERTY()
    Item d = Item(77);


    [[nodiscard]]
    METHOD()
    int test1(int a, std::string bb) const;

//    METHOD()
//    int test2(int a, std::string bb, ItemNamepsace::Item1 it) const;

public:

    METHOD()
    long ActorBaseMethod2(int a) override {
        return 0;
    }
};


#endif //GENERATORS_ACTOR_H

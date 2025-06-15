//
// Created by weichuanqi on 2025/6/2.
//

#ifndef CLUCID_ACTORBASE_H
#define CLUCID_ACTORBASE_H

#include "CLucidDefines.h"

class CLASS() ActorBase{
    METHOD()
    virtual long ActorBaseMethod1(int a){
         return 0;
    }

    METHOD()
    virtual long ActorBaseMethod2(int a) = 0;
};

class CLASS() ActorBase1{

};
#endif //CLUCID_ACTORBASE_H

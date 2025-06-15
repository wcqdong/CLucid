//
// Created by weichuanqi on 2025/6/11.
//

#ifndef GENERATORS_AACTOR_H
#define GENERATORS_AACTOR_H

#include "AActor2.h"
#include "MiniCore.h"
#include <string>

#include "AActor.generated.h"

class CLASS() AActor {
    GENERATED_BODY()

private:
    PROPERTY()
    int a;

    PROPERTY()
    AActor2 actor2;

    METHOD()
    int test1(int a, std::string bb) const
    {
        return a + bb.size();
    }
};


#endif //GENERATORS_AACTOR_H

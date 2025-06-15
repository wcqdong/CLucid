//
// Created by weichuanqi on 2025/6/10.
//

#ifndef GENERATORS_REFLECTSYSTEM_H
#define GENERATORS_REFLECTSYSTEM_H


class ReflectSystem {
public:
    ReflectSystem();

    template<typename R, typename T>
    R invoke(T* obj, const char* methodName, ...){
        return R();
    }
};

extern ReflectSystem rs;

#endif //GENERATORS_REFLECTSYSTEM_H

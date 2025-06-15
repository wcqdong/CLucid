//
// Created by weichuanqi on 2025/5/19.
//

#ifndef CLUCID_DEFINE_H
#define CLUCID_DEFINE_H

#define ANO_CLASS "anno-class;"
#define ANO_METHOD "anno-method;"
#define ANO_PROPERTY "anno-property;"
#define ANO_FUNCTION "anno-function;"
#define ANO_PARAM "anno-param;"
#define ANO_ENUM "anno-enum;"

#ifdef __CLUCID_PARSER__
#define CLASS(...) __attribute__((annotate(ANO_CLASS #__VA_ARGS__)))
#define METHOD(...) __attribute__((annotate(ANO_METHOD #__VA_ARGS__)))
#define FUNCTION(...) __attribute__((annotate(ANO_FUNCTION #__VA_ARGS__)))
#define PROPERTY(...) __attribute__((annotate(ANO_PROPERTY #__VA_ARGS__)))
#define PARAM(...) __attribute__((annotate(ANO_PARAM #__VA_ARGS__)))
#define ENUM(...) __attribute__((annotate(ANO_ENUM #__VA_ARGS__)))
#else
#define CLASS(...)
#define METHOD(...)
#define PROPERTY(...)
#define FUNCTION(...)
#define PARAM(...)
#define ENUM(...)
#endif

#endif //CLUCID_DEFINE_H

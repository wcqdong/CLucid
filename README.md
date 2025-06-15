# CLucid

## 原理
可查看我的知乎

https://zhuanlan.zhihu.com/p/1913356610983663401

## 目的
实现类似UE的UCLASS / UFUNCTION / UPROPERTY / UENUM / USTRUCT的宏功能，通过libclang解析被注解的类、方法、属性、枚举、结构体，获得元数据信息。

CLucid语法解析和反射的用途类似，作用都是获取c++代码自身结构。

通过解析出的元数据可以开发一些列的代码生成工具如

- 序列化代码生成工具
- rpc代理生成工具
- 消息自动注册生成工具
- 事件监听注册生成工具
- ……

## 工程介绍

- CLucid：c++语法解析库，通过CLASS/METHOD/FUNCTION/ENUM/FUNCTION等宏扩展c++信息，并获得c++元数据信息
- example/demo：CLucid的使用实例，把c++源文件解析为语法书，并按照规则匹配自定义的宏信息
- example/target：被demo进行语法解析的工程
- example/Generators：代码生成工具，实例代码，如何使用CLucid语法解析和inja模板引擎库制作代码生成工具。
- example/TestProject：测试工程，演示如何接入Generators实现代码自动生成。

## 使用方法

# 仅解析c++语法

接入和使用CLucid可查看demo工程的CMakeLists.txt和main.cpp

demo工程演示了如果扩展宏信息、匹配宏信息、获得想要的c++结构。

# 制作代码生成工具

查看Generators工程和TestProject工程

有简单的实例演示如何利用CLucid语法解析生成代码，并接入到项目工程中实现自动生成代码。






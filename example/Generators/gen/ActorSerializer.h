#ifndef GENERATORS_ACTOR_GENERATED_H
#define GENERATORS_ACTOR_GENERATED_H


class ActorSerializer {
public:
    // 只演示生成过程，并没有真正实现序列化，所以注释掉
    void read(/* Stream& stream, Actor& obj */){
    
        // obj.a = stream.readint();
    
        // obj.c = stream.readbasic_string<char, char_traits<char>, allocator<char>>();
    
    }

    void write(/* Stream& stream, Actor& obj */){
    
        // stream.writeint(obj.a);
    
        // stream.writebasic_string<char, char_traits<char>, allocator<char>>(obj.c);
    
    }

};


#endif //GENERATORS_ACTOR_GENERATED_H
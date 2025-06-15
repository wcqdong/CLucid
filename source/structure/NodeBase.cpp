//
// Created by weichuanqi on 2025/5/20.
//

#include "structure/NodeBase.h"

using namespace CLucid;

NodeBase::NodeBase(const CXCursor& cursor, const std::string& annotation)
    : cursor(cursor)
    , annotation(annotation)
    , parent(nullptr)
{
    init();
}

void NodeBase::init() {
    parseAnnotation();
}

void NodeBase::parseAnnotation() {
    if (annotation.empty()){
        return;
    }
    std::string annotationStr = Utils::trim(annotation);
    std::vector<std::string> splitAnnotation = Utils::split(annotationStr, ',');
    for (const auto& item : splitAnnotation) {
        std::vector<std::string> sp = Utils::split(item, '=');
        if (sp.size() > 2) {
            GLog.error("Annotation format error: %s", name.c_str());
            exit(-1);
        }
        annotationContainer.addAnnotation(sp[0], sp.size() >= 2 ? sp[1] : "");
    }
}
void NodeBase::setParent(NodeBase* inParent) {
    parent = inParent;
}
const std::string& NodeBase::getName() const{
    return name;
}

void NodeBase::setName(const char* inName) {
    name = inName;
}
const AnnotationContainer& NodeBase::getAnnotationContainer() const {
    return annotationContainer;
}

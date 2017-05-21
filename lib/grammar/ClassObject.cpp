//
// Created by BraxtonN on 1/31/2017.
//

#include "ClassObject.h"
#include "Param.h"
#include "runtime.h"

size_t ClassObject::constructorCount() {
    return constructors.size();
}

bool ClassObject::addChildClass(ClassObject klass) {
    if(getChildClass(klass.name) != NULL) {
        if(this->getChildClass(klass.name) || this->name == klass.name)
            return false;
        return false;
    }

    klass.vaddr = runtime::classUID++;
    childClasses.push_back(klass);
    return true;
}

Method* ClassObject::getConstructor(int p) {
    return &constructors.get(p);
}

Method *ClassObject::getConstructor(List<Param>& params) {
    for(unsigned int i = 0; i < constructors.size(); i++) {
        Method& function = constructors.get(i);
        if(Param::match(*function.getParams(), params))
            return &function;
    }

    if(base != NULL)
        return base->getConstructor(params);

    return NULL;
}

bool ClassObject::addConstructor(Method constr) {
    if(getConstructor(*constr.getParams()) != NULL)
        return false;

    constructors.push_back(constr);
    return true;
}

size_t ClassObject::functionCount() {
    return functions.size();
}

Method* ClassObject::getFunction(int p) {
    return &functions.get(p);
}

Method *ClassObject::getFunction(string name, List<Param>& params) {
    for(unsigned int i = 0; i < functions.size(); i++) {
        Method& function = functions.get(i);
        if(Param::match(*function.getParams(), params) && name == function.getName())
            return &function;
    }

    return NULL;
}

Method *ClassObject::getFunction(string name, int64_t _offset) {
    for(unsigned int i = 0; i < functions.size(); i++) {
        Method& function = functions.get(i);
        if(name == function.getName()) {
            if(_offset == 0)
                return &function;
            else
                _offset--;
        }
    }

    return NULL;
}

bool ClassObject::addFunction(Method function) {
    if(getFunction(function.getName(), *function.getParams()) != NULL)
        return false;

    functions.push_back(function);
    return true;
}

size_t ClassObject::fieldCount() {
    return fields.size();
}

Field* ClassObject::getField(int p) {
    return &fields.get(p);
}

Field* ClassObject::getField(string name) {
    for(unsigned int i = 0; i < fields.size(); i++) {
        Field& field = fields.get(i);
        if(field.name == name)
            return &field;
    }

    return NULL;
}


long ClassObject::getFieldIndex(string name) {
    long iter = 0;
    for(unsigned int i = 0; i < fields.size(); i++) {
        Field& field = fields.get(i);
        if(field.name == name)
            return iter;
        iter++;
    }

    return iter;
}

bool ClassObject::addField(Field field) {
    if(getField(field.name) != NULL)
        return false;

    field.vaddr = this->getTotalFieldCount()==0?0:this->getTotalFieldCount()-1;
    field.fullName = this->fullName + "." + name;
    fields.push_back(field);
    return true;
}

size_t ClassObject::childClassCount() {
    return childClasses.size();
}

ClassObject* ClassObject::getChildClass(int p) {
    return &childClasses.get(p);
}

ClassObject* ClassObject::getChildClass(string name) {
    for(unsigned int i = 0; i < childClasses.size(); i++) {
        ClassObject& klass = childClasses.get(i);
        if(klass.name == name)
            return &klass;
    }

    return NULL;
}

void ClassObject::free() {

}

size_t ClassObject::overloadCount() {
    return overloads.size();
}

OperatorOverload *ClassObject::getOverload(size_t p) {
    return &overloads.get(p);
}

OperatorOverload *ClassObject::getOverload(_operator op, List<Param> &params) {
    for(unsigned int i = 0; i < overloads.size(); i++) {
        OperatorOverload& oper = overloads.get(i);
        if(Param::match(*oper.getParams(), params) && op == oper.getOperator())
            return &oper;
    }

    return NULL;
}

OperatorOverload *ClassObject::getOverload(_operator op, int64_t _offset) {
    for(unsigned int i = 0; i < overloads.size(); i++) {
        OperatorOverload& oper = overloads.get(i);
        if(op == oper.getOperator()) {
            if(_offset == 0)
                return &oper;
            else
                _offset--;
        }
    }

    return NULL;
}

bool ClassObject::addOperatorOverload(OperatorOverload overload) {
    if(getOverload(overload.getOperator(), *overload.getParams()) != NULL)
        return false;

    overloads.push_back(overload);
    return true;
}

size_t ClassObject::macrosCount() {
    return macros.size();
}

Method *ClassObject::getMacros(int p) {
    return &macros.get(p);
}

Method *ClassObject::getMacros(string name, List<Param> &params) {
    for(unsigned int i = 0; i < macros.size(); i++) {
        Method& macro = macros.get(i);
        if(Param::match(*macro.getParams(), params) && name == macro.getName())
            return &macro;
    }

    return NULL;
}

Method *ClassObject::getMacros(string name, int64_t _offset) {
    for(unsigned int i = 0; i < macros.size(); i++) {
        Method& macro = macros.get(i);
        if(name == macro.getName()) {
            if(_offset == 0)
                return &macro;
            else
                _offset--;
        }
    }

    return NULL;
}

bool ClassObject::addMacros(Method macro) {
    if(getMacros(macro.getName(), *macro.getParams()) != NULL)
        return false;

    macros.push_back(macro);
    return true;
}

int cSuper = 1;
bool ClassObject::curcular(ClassObject *pObject) {
    cSuper--;

    if(pObject == NULL)
        return false;
    if(cSuper == 0 && super != NULL) {
        cSuper++;
        return super->curcular(pObject);
    }

    for(unsigned int i = 0; i < childClasses.size(); i++) {
        ClassObject& klass = this->childClasses.get(i);
        if(klass.match(pObject) || klass.curcular(pObject)) {
            cSuper = 0;
            return true;
        }
    }

    cSuper = 0;
    return false;
}

bool ClassObject::matchBase(ClassObject *pObject) {
    return base != NULL && pObject != NULL && pObject->base != NULL
           && base->match(pObject->base);
}

bool ClassObject::hasBaseClass(ClassObject *pObject) {
    if(base == NULL) return true;
    ClassObject* k, *_klass = this;

    for(;;) {
        k = _klass->getBaseClass();

        if(k == NULL)
            return false;

        if(k->match(pObject)) {
            return true;
        }else
            _klass = k;
    }
}

int ClassObject::baseClassDepth(ClassObject *pObject) {
    if(base == NULL) return 0;
    ClassObject* k, *_klass = this;
    int depth=0;

    for(;;) {
        depth++;
        k = _klass->getBaseClass();

        if(k == NULL)
            return depth;

        if(k->match(pObject)) {
            return depth;
        }else
            _klass = k;
    }
}

bool ClassObject::hasOverload(_operator op) {
    for(unsigned int i = 0; i < overloads.size(); i++) {
        OperatorOverload& oper = overloads.get(i);
        if(op == oper.getOperator())
            return true;
    }

    return false;
}

OperatorOverload *ClassObject::getPostIncOverload() {
    for(unsigned int i = 0; i < overloads.size(); i++) {
        OperatorOverload& oper = overloads.get(i);
        if(oper_INC == oper.getOperator()) {
            if(oper.getParams()->size() > 0 && oper.getParams()->get(oper.getParams()->size()-1).field.nativeInt()) {
                return &oper;
            }
        }
    }

    return NULL;
}

OperatorOverload *ClassObject::getPostDecOverload() {
    for(unsigned int i = 0; i < overloads.size(); i++) {
        OperatorOverload& oper = overloads.get(i);
        if(oper_DEC == oper.getOperator()) {
            if(oper.getParams()->size() > 0 && oper.getParams()->get(oper.getParams()->size()-1).field.nativeInt()) {
                return &oper;
            }
        }
    }

    return NULL;
}

OperatorOverload *ClassObject::getPreIncOverload() {
    for(unsigned int i = 0; i < overloads.size(); i++) {
        OperatorOverload& oper = overloads.get(i);
        if(oper_INC == oper.getOperator()) {
            if(oper.getParams()->size() == 0 || !oper.getParams()->get(oper.getParams()->size()-1).field.nativeInt()) {
                return &oper;
            }
        }
    }

    return NULL;
}

OperatorOverload *ClassObject::getPreDecOverload() {
    for(unsigned int i = 0; i < overloads.size(); i++) {
        OperatorOverload& oper = overloads.get(i);
        if(oper_DEC == oper.getOperator()) {
            if(oper.getParams()->size() == 0 || !oper.getParams()->get(oper.getParams()->size()-1).field.nativeInt()) {
                return &oper;
            }
        }
    }

    return NULL;
}

long ClassObject::getTotalFieldCount() {
    if(base == NULL) return fieldCount();
    ClassObject* k, *_klass = this;
    long fields=fieldCount();

    for(;;) {
        k = _klass->getBaseClass();

        if(k == NULL)
            return fields;

        fields+=k->fieldCount();
        _klass = k;
    }
}

long ClassObject::getTotalFunctionCount() {
    if(base == NULL) return TOTAL_FUNCS(this);
    ClassObject* k, *_klass = this;
    long total=TOTAL_FUNCS(this);

    for(;;) {
        k = _klass->getBaseClass();

        if(k == NULL)
            return total;

        total+=TOTAL_FUNCS(k);
        _klass = k;
    }
}

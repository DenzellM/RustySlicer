#include "table.h"

void Table::update(std::string i, double v)
{
    dict_[i].value = v;
}

void Table::update(std::string i, Variable v)
{
    if(dict_.find(i) == dict_.end()){
        keys_.push_back(i);
        dict_.insert_or_assign(i, v);
        return;
    }

    if(dict_[i].mut){  
        if(v.type == Type::Int) v.value = floor(v.value);
        dict_.insert_or_assign(i, v);
        return;
    }
    //the variable wasnt assigned a value at declaration
    if(dict_[i].type == Type::Unknown || !dict_[i].assigned){  
        if(v.type == Type::Int) v.value = floor(v.value);
        dict_.insert_or_assign(i, v);
        return;
    }
    
    std::cerr << "attempted to edit non-mutable variable" << std::endl;
    throw new std::exception();
}

void Table::insert(std::string i, Variable v)
{
    if(dict_.find(i) == dict_.end())    keys_.push_back(i);

    if(v.type == Type::Int) v.value = floor(v.value);

    dict_.insert_or_assign(i, v);
    return;
}
std::string Table::getNewKey(){
    return std::to_string(keys_.size());
}
int Table::getIndex(std::string key)
{
    for (size_t i = 0; i < keys_.size(); i++)
    {
       if(keys_[i] == key){
        return i;
       }
    }
    std::cerr << "couldnt find requested key" << std::endl;
    throw new std::exception();
}

std::string Table::getKey(int index)
{

    if(index >= keys_.size()){
        std::cerr << "segfault" << std::endl;
        throw new std::exception();
    }
    return keys_[index];
}

std::shared_ptr<Table> Table::clone(){
    std::shared_ptr<Table> clone = std::make_shared<Table>();
    clone->dict_ = dict_;
    clone->funcs_ = funcs_;
    clone->keys_ = keys_;

    clone->loopCtrl = loopCtrl;
    clone->ret = ret;
    clone->returnVal = returnVal;
    return clone;
}

std::shared_ptr<Table> Table::shallowClone(){
    std::shared_ptr<Table> clone = std::make_shared<Table>();
    // clone->dict_ = dict_;
    clone->funcs_ = funcs_;
    // clone->keys_ = keys_;

    clone->loopCtrl = loopCtrl;
    clone->ret = ret;
    clone->returnVal = returnVal;
    return clone;
}

void Table::updateScope(std::shared_ptr<Table> scope){
    returnVal = scope->returnVal;
    ret = scope->ret;
    loopCtrl = scope->loopCtrl;
    for(std::map<std::string,Variable>::iterator it = scope->dict_.begin(); it != scope->dict_.end(); ++it) {
        if(dict_.find(it->first) != dict_.end()){
            dict_[it->first] = it->second;
        }
    }
}
void Table::shallowUpdate(std::shared_ptr<Table> scope){
    for (size_t i = 0; i < scope->keys_.size(); i++)
    {
        if(scope->getVariable(scope->keys_[i]).type == Type::Ref){
            std::string key =  scope->getKey(scope->getVariable(scope->keys_[i]).value);
            Variable var = scope->getVariable(key);
            if(var.type == Type::Ref)   continue;
            if(dict_.find(key) == dict_.end())  continue;

            dict_[key] = var;
        }
    }
}

Variable Table::getVariable(std::string i)
{
    if(dict_.find(i) == dict_.end()){
        std::cerr << "couldnt acces requested variable" << std::endl;
        throw new std::exception();
    }
    return dict_[i];
}


void Table::setRet(ReturnType retType, double retVal){
    ret = retType;
    returnVal = retVal;
}

void Table::resetRet(){
    ret = ReturnType::NoReturn;
    returnVal = 0.0;
}

ReturnType Table::getReturnType(){
    return ret;
}

double Table::getReturnValue(){
    return returnVal;
}

void Table::update(std::string i, Stm_ * e){
    if(funcs_.find(i) == funcs_.end()){
        funcs_.insert_or_assign(i, e);
        return;
    }

    throw new std::exception();
}

Stm_ * Table::getFunc(std::string i){
    return funcs_[i];
}

// Type Table::getFuncType(std::string i){
//     return funcs_[i]->getType();
// }
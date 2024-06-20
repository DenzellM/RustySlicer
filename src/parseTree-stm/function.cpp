#include "function.h"

funcStm::funcStm(std::string n, funcParamList *p, std::string  ret, Exp b) : name(n), body(b){

    if(ret == "None") returnType = Type::None;
    else if(ret == "i32") returnType = Type::Int;
    else if(ret == "float") returnType = Type::Double;
    else if(ret == "bool") returnType = Type::Bool;
    funcParamList *tail = p->tail;
    funcParam *head = p->head;

    if(head)
        params.push_back(head);

    while(tail){
        params.push_back(tail->head);
        tail = tail->tail;
    }

    //std::cout << "init funcStm: "<< n << ", " << params.size() << " vars, ret: " << ret << std::endl;

}

double funcStm::interp(std::shared_ptr<Table> table){
    //std::cout << "interp funcStm, added: "<< name << " to function table"<< std::endl;
    table->update(name, this);
    return std::nan("1");
}

double funcStm::interp(std::shared_ptr<Table> table, std::vector<Exp> exps){

    std::shared_ptr<Table> scope = table->clone();

    for (size_t i = 0; i < params.size(); i++)
    {
        Type type = params[i]->type;
        Variable var = Variable{.value = exps[i]->interp(table), .type = type, .mut = params[i]->mut};
        if(params[i]->borrowCount > 0){
            std::string id = exps[i]->getId();

            if(id == ""){
                id = scope->getNewKey();
                scope->update(id, var);
            }
            int index = table->getIndex(id);
            var = Variable{.value = (double) index, .type = Type::Ref, .mut = params[i]->mut};
            for (size_t j = 1; j < params[i]->borrowCount; j++)
            {
                id = scope->getNewKey();
                scope->update(id, var);
                index = table->getIndex(id);
                var = Variable{.value = (double) index, .type = Type::Ref, .mut = params[i]->mut};
            }
        }

        scope->update(params[i]->name, var);
    }
    double val =  body->interp(scope);
    std::cout << "val: "<< val << std::endl;
    std::cout << "ret: "<< scope->getReturnValue() << std::endl;
    table->shallowUpdate(scope);
    if(scope->getReturnType() == ReturnType::ReturVal){
        //std::cout << name <<" returned: " << table->getReturnValue() << std::endl;
        return scope->getReturnValue();
    } 
    //std::cout << name <<" returned: " << val << std::endl;
    return val;
}


Type funcStm::typeCheck(std::shared_ptr<Table> table){
    std::shared_ptr<Table> scope = table->shallowClone();
    for (size_t i = 0; i < params.size(); i++)
    {
       Type type = params[i]->type;
        Variable var = Variable{.value = 0, .type = type, .mut = params[i]->mut};
        if(params[i]->borrowCount > 0){
            std::string id = "";

            if(id == ""){
                id = scope->getNewKey();
                scope->update(id, var);
            }
            int index = scope->getIndex(id);
            var = Variable{.value = (double) index, .type = Type::Ref, .mut = params[i]->mut};
            for (size_t j = 1; j < params[i]->borrowCount; j++)
            {
                id = scope->getNewKey();
                scope->update(id, var);
                index = scope->getIndex(id);
                var = Variable{.value = (double) index, .type = Type::Ref, .mut = params[i]->mut};
            }
        }

        scope->update(params[i]->name, var);
        return Type::None;
    }

    Type exptype = body->typeCheck(scope);
    if(returnType == Type::None){
        returnType = exptype;
        return Type::None;
    }

    if(exptype != returnType){
        std::cerr << "return type was not what was expected" << std::endl;
        throw new std::exception();
    }

    return Type::None;
}   

void funcStm::checkParam(std::shared_ptr<Table> table, std::vector<Exp> exps){
    if(exps.size() != params.size()) {
        std::cerr << name <<": " <<  params.size() << " parameters were expected but " << exps.size() << " were provided" <<std::endl;

        throw new std::exception();
    }

    for (size_t i = 0; i < params.size(); i++)
    {
        Type type =  exps[i]->typeCheck(table);
        if(params[i]->type != type && !(type == Type::Ref && params[i]->borrowCount > 0)){
            std::cerr <<"type mismatch: " << type << " and " << params[i]->type<<std::endl;
            throw new std::exception();
        }    
    }
}  
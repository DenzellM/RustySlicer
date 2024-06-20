#include "statementStructs.h"
//==================================================================
ItemStm::ItemStm(Stm s1, Stm s2) : stm1(s1), stm2(s2) {}

double ItemStm::interp(std::shared_ptr<Table> table)
{
    if(table->getReturnType() != ReturnType::NoReturn) return std::nan("1");
    if(table->getLoopVar() != LoopCtrl::NoCtrl) return std::nan("1");

    stm1->interp(table);
    
    if(stm2) stm2->interp(table);
    return std::nan("1");
}

Type ItemStm::typeCheck(std::shared_ptr<Table> table)
{
    stm1->typeCheck(table);
    if(stm2) stm2->typeCheck(table);
    //std::cout << "typeCheck ItemStm" << std::endl;
    return Type::None;

}

//==================================================================
CompStm::CompStm(Stm s1, Stm s2) : stm1(s1), stm2(s2) 
{
    //std::cout << "init CompStm" << std::endl;
}

double CompStm::interp(std::shared_ptr<Table> table)
{
    if(table->getReturnType() != ReturnType::NoReturn) return std::nan("1");

    if(table->getLoopVar() != LoopCtrl::NoCtrl) return std::nan("1");
    stm1->interp(table);
    if(table->getLoopVar() != LoopCtrl::NoCtrl) return std::nan("1");
    if(stm2) stm2->interp(table);

    return std::nan("1");
}

Type CompStm::typeCheck(std::shared_ptr<Table> table)
{
    stm1->typeCheck(table);
    if(stm2) stm2->typeCheck(table);
    //std::cout << "typeCheck CompStm" << std::endl;

    return Type::None;
}

//==================================================================
LetStm::LetStm(bool mut, std::string i, std::string type, Exp e) : id(i), exp(e){

    var.mut = mut;
    var.assigned = true;

    if(type == "i32"){
        var.type = Type::Int;
    }else if (type == "float"){
        var.type = Type::Double;
    } else if (type == "bool"){
        var.type = Type::Bool;
    } else {
        var.type = Type::Unknown;
    }

    //std::cout << "init LetStm: " << id << ": " << type << std::endl;
    
}

double LetStm::interp(std::shared_ptr<Table> table){
    if(table->getReturnType() != ReturnType::NoReturn) return std::nan("1");
    if(table->getLoopVar() != LoopCtrl::NoCtrl) return std::nan("1");

    var.assigned = false;

    if(exp){
        var.value = exp->interp(table);
        var.assigned = true;
    }

    table->insert(id, var);
    // if(exp)
    //std::cout << "interp LetStm: " << id << " = " << var.value << std::endl;
    // else
    //std::cout << "interp LetStm: " << id << " (initialised)"<< std::endl;
    return std::nan("1");
}

Type LetStm::typeCheck(std::shared_ptr<Table> table){
    Type expType = Type::Unknown;
    if(exp) expType =  exp->typeCheck(table);
    else var.assigned = false;

    if(var.type == Type::Unknown){
        var.type = expType;
    }

    else if (expType != Type::Unknown && var.type != expType)
    {
        throw new std::exception();
    }
    
    table->insert(id, var);
    //std::cout << "typecheck LetStm: " << id << ": " << var.type << std::endl;
    return Type::None;
}

//==================================================================
ReturnStm::ReturnStm(Exp e) : exp(e)
{
    //std::cout << "init ReturnStm" << std::endl;
}

double ReturnStm::interp(std::shared_ptr<Table> table){
    if(table->getReturnType() != ReturnType::NoReturn) return std::nan("1");
    if(table->getLoopVar() != LoopCtrl::NoCtrl) return std::nan("1");

    if(exp){
        table->setRet(ReturnType::ReturVal, exp->interp(table));

    }else{
        table->setRet(ReturnType::ReturnNoVal);
    }
    return std::nan("1");
}

Type ReturnStm::typeCheck(std::shared_ptr<Table> table){exp->typeCheck(table); return Type::None;};

//==================================================================
expStm::expStm(Exp e) : exp(e) {}

double expStm::interp(std::shared_ptr<Table> table){
    if(table->getReturnType() != ReturnType::NoReturn) return std::nan("1");
    if(table->getLoopVar() != LoopCtrl::NoCtrl) return std::nan("1");

    return exp->interp(table);
}

Type expStm::typeCheck(std::shared_ptr<Table> table){
    return exp->typeCheck(table);
}

//==================================================================
PrintVarStm::PrintVarStm(std::string i){
    input = i.erase(0, 11);
    input = input.erase(input.length() - 3, 3);
    //std::cout << "init PrintVarStm" << std::endl;
}

double PrintVarStm::interp(std::shared_ptr<Table> table){
    if(table->getReturnType() != ReturnType::NoReturn) return std::nan("1");
    if(table->getLoopVar() != LoopCtrl::NoCtrl) return std::nan("1");


    Variable var = table->getVariable(input);
    if(var.type == Type::None){
        std::cerr << "cant print uninitialised variable"<<std::endl;
        throw new std::exception();
    }

    if(var.type == Type::Bool){
        if (var.value == -1)
        {
           std::cout << "Print: " << "false" << std::endl;
        }else if(var.value == 1){
           std::cout << "Print: " << "true" << std::endl;
        }else{
           std::cout << "Print: " << "maybe" << std::endl;
        }
    }else if(var.type <= Type::Double){
        std::cout << "Print: " << var.value << std::endl;
    }else{
        std::cerr << "type cant be printed"<<std::endl;
        throw new std::exception();
    }

    return std::nan("1");
}

Type PrintVarStm::typeCheck(std::shared_ptr<Table> table){
    return Type::None;
}

//==================================================================
PrintStringStm::PrintStringStm(std::string i){
    input = i.erase(0, 10);
    input = input.erase(input.length() - 2, 2);
    //std::cout << "init PrintStringStm" << std::endl;
}
double PrintStringStm::interp(std::shared_ptr<Table> table){
    if(table->getReturnType() != ReturnType::NoReturn) return std::nan("1");
    if(table->getLoopVar() != LoopCtrl::NoCtrl) return std::nan("1");


    std::cout << "Print: " << input << std::endl;
    return std::nan("1");
}
Type PrintStringStm::typeCheck(std::shared_ptr<Table> table){
    return Type::None;
}

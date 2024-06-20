#include "statementStructs.h"
//==================================================================
ItemExp::ItemExp(Exp s1, Exp s2) : Exp1(s1), Exp2(s2) {}

double ItemExp::interp(std::shared_ptr<Table> table)
{
    if(table->getReturnType() != ReturnType::NoReturn) return std::nan("1");
    if(table->getLoopVar() != LoopCtrl::NoCtrl) return std::nan("1");

    Exp1->interp(table);
    
    if(Exp2) Exp2->interp(table);
    return std::nan("1");
}

Type ItemExp::typeCheck(std::shared_ptr<Table> table)
{
    Exp1->typeCheck(table);
    if(Exp2) Exp2->typeCheck(table);
    //std::cout << "typeCheck ItemExp" << std::endl;
    return Type::None;

}

//==================================================================
CompExp::CompExp(Exp s1, Exp s2) : Exp1(s1), Exp2(s2) 
{
    //std::cout << "init CompExp" << std::endl;
}

double CompExp::interp(std::shared_ptr<Table> table)
{
    if(table->getReturnType() != ReturnType::NoReturn) return std::nan("1");

    if(table->getLoopVar() != LoopCtrl::NoCtrl) return std::nan("1");
    Exp1->interp(table);
    if(table->getLoopVar() != LoopCtrl::NoCtrl) return std::nan("1");
    if(Exp2) Exp2->interp(table);

    return std::nan("1");
}

Type CompExp::typeCheck(std::shared_ptr<Table> table)
{
    Exp1->typeCheck(table);
    if(Exp2) Exp2->typeCheck(table);
    //std::cout << "typeCheck CompExp" << std::endl;

    return Type::None;
}

//==================================================================
LetExp::LetExp(bool mut, std::string i, std::string type, Exp e) : id(i), exp(e){

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

    //std::cout << "init LetExp: " << id << ": " << type << std::endl;
    
}

double LetExp::interp(std::shared_ptr<Table> table){
    if(table->getReturnType() != ReturnType::NoReturn) return std::nan("1");
    if(table->getLoopVar() != LoopCtrl::NoCtrl) return std::nan("1");

    var.assigned = false;

    if(exp){
        var.value = exp->interp(table);
        var.assigned = true;
    }

    table->insert(id, var);
    // if(exp)
    //std::cout << "interp LetExp: " << id << " = " << var.value << std::endl;
    // else
    //std::cout << "interp LetExp: " << id << " (initialised)"<< std::endl;
    return std::nan("1");
}

Type LetExp::typeCheck(std::shared_ptr<Table> table){
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
    //std::cout << "typecheck LetExp: " << id << ": " << var.type << std::endl;
    return Type::None;
}

//==================================================================
ReturnExp::ReturnExp(Exp e) : exp(e)
{
    //std::cout << "init ReturnExp" << std::endl;
}

double ReturnExp::interp(std::shared_ptr<Table> table){
    if(table->getReturnType() != ReturnType::NoReturn) return std::nan("1");
    if(table->getLoopVar() != LoopCtrl::NoCtrl) return std::nan("1");

    if(exp){
        table->setRet(ReturnType::ReturVal, exp->interp(table));

    }else{
        table->setRet(ReturnType::ReturnNoVal);
    }
    return std::nan("1");
}

Type ReturnExp::typeCheck(std::shared_ptr<Table> table){exp->typeCheck(table); return Type::None;};

//==================================================================
expExp::expExp(Exp e) : exp(e) {}

double expExp::interp(std::shared_ptr<Table> table){
    if(table->getReturnType() != ReturnType::NoReturn) return std::nan("1");
    if(table->getLoopVar() != LoopCtrl::NoCtrl) return std::nan("1");

    return exp->interp(table);
}

Type expExp::typeCheck(std::shared_ptr<Table> table){
    return exp->typeCheck(table);
}

//==================================================================
PrintVarExp::PrintVarExp(std::string i){
    input = i.erase(0, 11);
    input = input.erase(input.length() - 3, 3);
    //std::cout << "init PrintVarExp" << std::endl;
}

double PrintVarExp::interp(std::shared_ptr<Table> table){
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

Type PrintVarExp::typeCheck(std::shared_ptr<Table> table){
    return Type::None;
}

//==================================================================
PrintStringExp::PrintStringExp(std::string i){
    input = i.erase(0, 10);
    input = input.erase(input.length() - 2, 2);
    //std::cout << "init PrintStringExp" << std::endl;
}
double PrintStringExp::interp(std::shared_ptr<Table> table){
    if(table->getReturnType() != ReturnType::NoReturn) return std::nan("1");
    if(table->getLoopVar() != LoopCtrl::NoCtrl) return std::nan("1");


    std::cout << "Print: " << input << std::endl;
    return std::nan("1");
}
Type PrintStringExp::typeCheck(std::shared_ptr<Table> table){
    return Type::None;
}

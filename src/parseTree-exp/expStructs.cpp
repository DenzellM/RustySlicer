#include "expStructs.h"

//==================================================================
IdExp::IdExp(std::string i){
    //std::cout << "init idExp: " << i << std::endl;

    id =  i;
    mutabl = false;
}

IdExp::IdExp(std::string i, bool mut){
    //std::cout << "init idExp: " << i << " (" << mut << ")"<< std::endl;

    id =  i;
    mutabl = mut;
}

double IdExp::interp(std::shared_ptr<Table> table)
{
    if(table->getReturnType() != ReturnType::NoReturn) return std::nan("1");
    if(table->getLoopVar() != LoopCtrl::NoCtrl) return std::nan("1");

    double val =  table->getVariable(id).value;
    //std::cout << "interp idExp: " << id << " --> " << val << std::endl;
    return val;
}

std::string IdExp::getId(){
    return id;
}

bool IdExp::getMut(){
    return mutabl;
}
       

Type IdExp::typeCheck(std::shared_ptr<Table> table){
    Type type = table->getVariable(id).type;
    //std::cout << "typecheck idExp " << type << std::endl;
    return type;
}
//==================================================================

NumExp::NumExp(double n){
    //std::cout << "init numExp: " << n << std::endl;

    num = n;
}


double NumExp::interp(std::shared_ptr<Table> table)
{
    if(table->getReturnType() != ReturnType::NoReturn) return std::nan("1");
    if(table->getLoopVar() != LoopCtrl::NoCtrl) return std::nan("1");

    //std::cout << "interp numExp: " << num << std::endl;
    return num;
}

Type NumExp::typeCheck(std::shared_ptr<Table> table){
    //std::cout << "typecheck numExp: " << Type::Double << std::endl;
    if(fmod(num, 1) == 0) {
        return Type::Int;
    }
    return Type::Double;
}
//==================================================================

BoolExp::BoolExp(bool n){
    if(n){
        val = 1;
    }else{
        val = -1;
    }
}

double BoolExp::interp(std::shared_ptr<Table> table){
    if(table->getReturnType() != ReturnType::NoReturn) return std::nan("1");
    if(table->getLoopVar() != LoopCtrl::NoCtrl) return std::nan("1");

    return val;
}

Type BoolExp::typeCheck(std::shared_ptr<Table> table){
    return Type::Bool;
}


//==================================================================
NegExp::NegExp(Exp expr, Type typ){
    exp = expr;
    type = typ;
}

double NegExp::interp(std::shared_ptr<Table> table){
    if(table->getReturnType() != ReturnType::NoReturn) return std::nan("1");
    if(table->getLoopVar() != LoopCtrl::NoCtrl) return std::nan("1");

    double val = exp->interp(table);
    Type expType = exp->typeCheck(table);
    //! and bool
    if(type == Type::Bool && expType == Type::Bool)
        return -val;

    //! and double (idk why it is like this)
    if(type <= Type::Double && expType == Type::Bool)
        return -val - 1;
    
    //- and double (all else would have failed type check)
    return -val;
}

Type NegExp::typeCheck(std::shared_ptr<Table> table){
    Type expType = exp->typeCheck(table);
    if (expType <= Type::Double)
        return Type::Double;
    if (expType == Type::Bool && type == Type::Bool)
        return Type::Bool;
    std::cerr << "cant do neg expression on given type" << std::endl;
    throw new std::exception(); 
}
//==================================================================

BoolOpExp::BoolOpExp(Exp l, Boolop op, Exp r){
    left = l;
    oper = op;
    right = r;
}

double BoolOpExp::interp(std::shared_ptr<Table> table){
    if(table->getReturnType() != ReturnType::NoReturn) return std::nan("1");
    if(table->getLoopVar() != LoopCtrl::NoCtrl) return std::nan("1");

    double l = left->interp(table);
    //lazy: if l is false and is always false
    if(l < 0 && oper == Boolop::And){
        return -1;
    }

    //lazy: if l is true or is always true
    if(l > 0 && oper == Boolop::Or){
        return 1;
    }
    //for and l wil now always be true
    //for or l wil now always be false
    double r = right->interp(table);


    if (r > 0)
    {
        return 1;
    }
     
    return -1;
    }

    Type BoolOpExp::typeCheck(std::shared_ptr<Table> table){
        if(left->typeCheck(table) != Type::Bool || right->typeCheck(table)!= Type::Bool){
            throw new std::exception();
        }
        return Type::Bool;
    }
//==================================================================
OpExp::OpExp(Exp l, Binop op, Exp r)
{
    left = l;
    oper = op;
    right = r;

    //std::cout << "init OpExp: " << op << std::endl;
}


double OpExp::interp(std::shared_ptr<Table> table)
{
    if(table->getReturnType() != ReturnType::NoReturn) return std::nan("1");
    if(table->getLoopVar() != LoopCtrl::NoCtrl) return std::nan("1");

    double l = left->interp(table);
    double r = right->interp(table);
    //std::cout << "interp OpExp: " << l << " (" << oper << ") "<< r << " --> ";

    switch (oper)
    {
    case Plus:
        //std::cout << l + r << std::endl;
        return l + r;
    case Minus:
        //std::cout << l - r << std::endl;
        return l - r;
    case Times:
        //std::cout << l * r << std::endl;
        return l*r;
    case Div:
        if(r == 0){
            std::cerr << "attempted to devide by 0" <<std::endl;
            throw new std::exception();
        } 
        //std::cout << l / r << std::endl;
        if(right->typeCheck(table) == Type::Double || left->typeCheck(table) == Type::Double)
            return l / r;
        return floor(l / r);
    default:
            std::cerr << "unknown operation" <<std::endl;
            throw new std::exception();
        return 0;
    }
}

Type OpExp::typeCheck(std::shared_ptr<Table> table){
    Type ltype = left->typeCheck(table);
    Type rtype = right->typeCheck(table);
    if(ltype > Type::Double || rtype > Type::Double){
        std::cerr << "can only perform aritmetic operations on doubles" << std::endl;
        throw new std::exception();
    }
    if(ltype == Type::Double || rtype == Type::Double) return Type::Double;
    return Type::Int;
}

//==================================================================

AssExp::AssExp(Exp i, Exp e) : id(i), exp(e) {
    //std::cout << "init AssExp: " << id->getId() << std::endl;
}

double AssExp::interp(std::shared_ptr<Table> table)
{
    if(table->getReturnType() != ReturnType::NoReturn) return std::nan("1");
    if(table->getLoopVar() != LoopCtrl::NoCtrl) return std::nan("1");

    id->interp(table);
    std::string i = id->getId(); 

    if(i == ""){
        std::cerr << "cant assign to this expresion" << std::endl;
        throw new std::exception();
    }
    double val = exp->interp(table);
    table->update(i, val);
    //std::cout << "interp AssExp: " << i << " = " << val << std::endl;

    return 0;
}

Type AssExp::typeCheck(std::shared_ptr<Table> table){
    Type expType =  exp->typeCheck(table);
    id->typeCheck(table);
    std::string i = id->getId(); 

    Variable var = table->getVariable(i);

    if(var.type == Type::Unknown){
        var.type = expType;
    }

    else if (var.type != expType)
    {
        std::cerr << var.type << " cant be assigned to " << expType <<std::endl;
        throw new std::exception();
    }
    
    table->update(i, var);

    //std::cout << "typecheck AssExp: " << i << ": " << var.type << std::endl;
    return Type::None;
}
//==================================================================


CompAssExp::CompAssExp(Exp e1, Exp e2, CompOp opperator) : exp1(e1), exp2(e2), opp(opperator)
{
    //std::cout << "init CompAssExp: " << e1->getId() << " " << opperator << std::endl;
}

Type CompAssExp::typeCheck(std::shared_ptr<Table> table){
    if(exp1->getId() == ""){
        std::cerr << "first expresion is not an id" <<std::endl;
        throw new std::exception();
    } 
    Type type = exp1->typeCheck(table);
    if( type > Type::Double ||
        exp2->typeCheck(table) > Type::Double){
        std::cerr << "both sides of compound statment must have double or int type" << std::endl;
        throw new std::exception();
    }
    if(!table->getVariable(exp1->getId()).mut){
        std::cerr << "left side of a Compound assign must be mutable" << std::endl;
        throw new std::exception();
    }
    //std::cout << "typecheck CompAssExp: " << opp << " -> " << type << std::endl;
    return Type::None;
}

double CompAssExp::interp(std::shared_ptr<Table> table){
    if(table->getReturnType() != ReturnType::NoReturn) return std::nan("1");
    if(table->getLoopVar() != LoopCtrl::NoCtrl) return std::nan("1");

    double val1 = exp1->interp(table);
    double val2 = exp2->interp(table);
    // std::cout << "interp CompAssExp: " << exp1->getId() << ": " << val1 << 
    // " (" << opp << ") "<< val2;
    switch (opp)
    {
    case CompOp::PlusEq:
        val1 += val2;
        break;

    case CompOp::MinEq:
        val1 -= val2;
        break;

    case CompOp::DivEq:
        if(val2 == 0) throw new std::exception();
        val1 /= val2;
        break;

    case CompOp::TimeEq:
        val1 *= val2;
        break;

    default:
        throw new std::exception();
        break;
    }
    Type outType = exp1->typeCheck(table);
    if(exp2->typeCheck(table) == Type::Double) outType = Type::Double;

    Variable out{.value = val1, .type= outType, .mut = true, .assigned = true};
    table->update(exp1->getId(), out);

    // std::cout << " --> " << val1 << std::endl;
    return 0;
}

//==================================================================
compOpExp::compOpExp(Exp l, Compop op, Exp r)
{
    left = l;
    oper = op;
    right = r;

    //std::cout << "init CompOpExp: " << op << std::endl;

}

double compOpExp::interp(std::shared_ptr<Table> table)
{
    if(table->getReturnType() != ReturnType::NoReturn) return std::nan("1");
    if(table->getLoopVar() != LoopCtrl::NoCtrl) return std::nan("1");

    double l = left->interp(table);
    double r = right->interp(table);
    //std::cout << "interp CompOpExp: " << l << " (" << oper <<") " << r << std::endl;

    switch (oper)
    {
    case Compop::Eq:
        return (l == r) ? 1 : -1;
    case Compop::Ne:
        return (l != r) ? 1 : -1;
    case Compop::Gt:
        return (l > r) ? 1 : -1;
    case Compop::Lt:
        return (l < r) ? 1 : -1;
    case Compop::Ge:
        return (l >= r) ? 1 : -1;
    case Compop::Le:
        return (l <= r) ? 1 : -1;

    default:
        throw new std::exception();
    }
}

Type compOpExp::typeCheck(std::shared_ptr<Table> table){
    Type ltype = left->typeCheck(table);
    Type rtype = right->typeCheck(table);
    if( ltype > Type::Double || rtype > Type::Double){
        std::cerr << "type combination "<< ltype << " and " << rtype << " are incompatible for comparison" << std::endl;
        throw new std::exception();
    }
    //std::cout << "typeCheck CompOpExp: double, double --> bool" << std::endl;

    return Type::Bool;
}
//==================================================================
BlockExp::BlockExp(Exp s, Exp e) : exp(s), tail(e) 
{
    //std::cout << "init BlockExp" << std::endl;
}

double BlockExp::interp(std::shared_ptr<Table> table){
    if(table->getReturnType() != ReturnType::NoReturn) return std::nan("1");
    if(table->getLoopVar() != LoopCtrl::NoCtrl) return std::nan("1");

    std::shared_ptr<Table> scope = table->clone();
    if(exp) exp->interp(scope);
    if(tail){
        if(table->getReturnType() != ReturnType::NoReturn) return std::nan("1");
        double val = tail->interp(scope);
        table->updateScope(scope);
        return val;
    }

    table->updateScope(scope);
    return 0.0;
}

Type BlockExp::typeCheck(std::shared_ptr<Table> table){
    std::shared_ptr<Table> scope = table->clone();

    if(exp) exp->typeCheck(scope);

    if(tail){
        Type type = tail->typeCheck(scope);
        //std::cout << "typeCheck BlockExp: "<< type << std::endl;

        return type;
    }
    //std::cout << "typeCheck BlockExp: None" << std::endl;
    return Type::None;
}
//==================================================================
whileExp::whileExp(Exp p, Exp l): pred(p), loop(l) {}
double whileExp::interp(std::shared_ptr<Table> table) {
    if(table->getReturnType() != ReturnType::NoReturn) return std::nan("1");
    if(table->getLoopVar() != LoopCtrl::NoCtrl) return std::nan("1");

    double val;
    while (pred->interp(table) == 1)
    {
        val = loop->interp(table);

        if(table->getLoopVar() == LoopCtrl::Break) break;
        table->resetLoopVar();
    }

    table->resetLoopVar();
    return val;
}
Type whileExp::typeCheck(std::shared_ptr<Table> table) {
    if(pred->typeCheck(table) != Type::Bool){
        throw new std::exception();
    }

    return loop->typeCheck(table);
}
//==================================================================
ifExp::ifExp(Exp p, Exp ex, Exp e){
    pred = p;
    exp = ex;
    elseExp = e;
    // if(e)   //std::cout << "init ifExp: if --> else" << std::endl;
    // else //std::cout << "init ifExp: if" << std::endl;

}

Type ifExp::typeCheck(std::shared_ptr<Table> table){
    if(pred->typeCheck(table) != Type::Bool){
        std::cerr << "the predicate of an if statment should reter a boolean" << std::endl;
        throw new std::exception();
    }
    if(elseExp) elseExp->typeCheck(table);
    Type ret = exp->typeCheck(table);

    //std::cout << "typeCheck ifExp: if(bool) --> "<< ret << std::endl;
    return ret;
}

double ifExp::interp(std::shared_ptr<Table> table){
    if(table->getReturnType() != ReturnType::NoReturn) return std::nan("1");
    if(table->getLoopVar() != LoopCtrl::NoCtrl) return std::nan("1");

    if(pred->interp(table) < 0){
        //std::cout << "interp ifExp: (false)" << std::endl;
        if (elseExp) return elseExp->interp(table);
        else return std::nan("1");
    }else

    //std::cout << "interp ifExp: (true)" << std::endl;
    return exp->interp(table);
}
//==================================================================

funCallExp::funCallExp(Exp f, ExpList *p) : func(f){
    if(f->getId() == ""){
        std::cerr << "function call doesnt have a name"<<std::endl;
        throw new std::exception();
    }

    if(p->head)  params.push_back(p->head);
    ExpList *rest = p->tail;
    while (rest)
    {
        params.push_back(rest->head);
        rest = rest->tail;
    }
    //std::cout << "init funCallExp: " << func->getId() << ", " << params.size()<< " args" << std::endl;
}

double funCallExp::interp(std::shared_ptr<Table> table){
    if(table->getReturnType() != ReturnType::NoReturn) return std::nan("1");
    if(table->getLoopVar() != LoopCtrl::NoCtrl) return std::nan("1");

    funcExp* fun = (funcExp*) table->getFunc(func->getId());

    double val = fun->interp(table, params);

    //std::cout << "interp funCallExp: " << func->getId() << " --> " << val << std::endl;
    table->resetRet();
    return val;
}

Type funCallExp::typeCheck(std::shared_ptr<Table> table){
    funcExp* fun = (funcExp*) table->getFunc(func->getId());
    fun->checkParam(table, params);

    Type type = fun->getType();
    //std::cout << "typeCheck funCallExp: " << func->getId() << " --> " << type << std::endl;

    return type;
}
//==================================================================
borrowExp::borrowExp(Exp e, bool m) : exp(e), mut(m) {
//std::cout << "init borrowExp: " << mut << std::endl;

}

double borrowExp::interp(std::shared_ptr<Table> table){
    if(table->getReturnType() != ReturnType::NoReturn) return std::nan("1");
    if(table->getLoopVar() != LoopCtrl::NoCtrl) return std::nan("1");

    std::string id = exp->getId();
    double val =  exp->interp(table);
    Type type = exp->typeCheck(table);
    if(id == ""){
        id = table->getNewKey();

        Variable var = {.value = val, .type = type};
        table->update(id, var);
    }

    int index = table->getIndex(id);

    //std::cout << "interp borrowExp: " << id << 
    // " (" << val << " : " << type << ") --> " << index << std::endl;
    return index;
}

Type borrowExp::typeCheck(std::shared_ptr<Table> table){
    //std::cout << "/* message */" << std::endl;
    origType = exp->typeCheck(table);
    //std::cout << "typeCheck borrowExp: Ref" << std::endl;
    return Type::Ref;
}

Type borrowExp::getOrigType(){
    return origType;
}
        
//==================================================================
derefExp::derefExp(Exp e) : exp(e){
//std::cout << "init derefExp: " << std::endl;
}

double derefExp::interp(std::shared_ptr<Table> table){
    if(table->getReturnType() != ReturnType::NoReturn) return std::nan("1");
    if(table->getLoopVar() != LoopCtrl::NoCtrl) return std::nan("1");
    
    double index = table->getVariable(table->getKey(exp->interp(table))).value;
    // key = table->getKey(index);
    // double val = table->getVariable(key).value;
    key = table->getKey(index);
    double val =  table->getVariable(key).value;
    //std::cout << "interp derefExp: " << key << " --> " << val << std::endl;
    return val;
}

Type derefExp::typeCheck(std::shared_ptr<Table> table){
    if(exp->typeCheck(table) != Type::Ref){
        std::cerr<<"attempted to dereference a non-reference"<<std::endl;
        throw new std::exception();
    }

    key = table->getKey(table->getVariable(exp->getId()).value);
    Type type =  table->getVariable(key).type;
    //std::cout << "typeCheck derefExp: " << type << std::endl;

    return type;
}

std::string derefExp::getId(){
    return key;
}
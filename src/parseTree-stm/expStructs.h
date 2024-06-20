#ifndef EXPSTRUCTSH
#define EXPSTRUCTSH

#include "function.h"

typedef Exp_* Exp;

struct IdExp : public Exp_
{
    private:
        std::string id;
        bool mutabl;
        
    public:
        IdExp(std::string i);
        IdExp(std::string i, bool mut);
        std::string getId() override;
        bool getMut() override;
        double interp(std::shared_ptr<Table> table);
        Type typeCheck(std::shared_ptr<Table> table);
};


struct NumExp : public Exp_
{
    private:
        double num;
    public:
        NumExp(double n);
        double interp(std::shared_ptr<Table> table);
        Type typeCheck(std::shared_ptr<Table> table);
};

struct BoolExp : public Exp_
{
    private:
        double val;
    public:
        BoolExp(bool n);
        double interp(std::shared_ptr<Table> table);
        Type typeCheck(std::shared_ptr<Table> table);
};

struct NegExp : public Exp_
{
    private:
        Exp exp;
        Type type;
        
    public:
        NegExp(Exp expr, Type typ);
        double interp(std::shared_ptr<Table> table);
        Type typeCheck(std::shared_ptr<Table> table);
};

struct BoolOpExp : public Exp_
{
    private:
        Exp left, right;
        Boolop oper;
        
    public:
        BoolOpExp(Exp l, Boolop op, Exp r);
        double interp(std::shared_ptr<Table> table);
        Type typeCheck(std::shared_ptr<Table> table);
};

struct OpExp : public Exp_
{
    private:
        Exp left, right;
        Binop oper;

    public:
        OpExp(Exp l, Binop op, Exp r);
        double interp(std::shared_ptr<Table> table);
        Type typeCheck(std::shared_ptr<Table> table);
};

struct AssExp : public Exp_
{
    private:
        Exp id;
        Exp exp;
    public:
        AssExp(Exp i, Exp e);
        double interp(std::shared_ptr<Table> table);
        Type typeCheck(std::shared_ptr<Table> table);
};

struct CompAssExp : public Exp_
{
    private:
        Exp exp1;
        Exp exp2;
        CompOp opp;
    public:
        CompAssExp(Exp e1, Exp e2, CompOp opperator);
        double interp(std::shared_ptr<Table> table);
        Type typeCheck(std::shared_ptr<Table> table);
};
struct compOpExp : public Exp_
{
    private:
        Exp left, right;
        Compop oper;
        
    public:
        compOpExp(Exp l, Compop op, Exp r);
        double interp(std::shared_ptr<Table> table);
        Type typeCheck(std::shared_ptr<Table> table);
};

struct BlockExp : public Exp_
{
    private:
        Stm stm;
        Exp tail;
        
    public:
        BlockExp(Stm s, Exp e = nullptr);
        double interp(std::shared_ptr<Table> table);
        Type typeCheck(std::shared_ptr<Table> table);
};

struct whileExp : public Exp_
{
    private:
        Exp pred;
        Exp loop;

    public:
        whileExp(Exp p, Exp l);
        double interp(std::shared_ptr<Table> table);
        Type typeCheck(std::shared_ptr<Table> table);
};

struct ifExp : public Exp_
{
    private:
        Exp pred;
        Exp exp;

        Exp elseExp;

    public:
        ifExp(Exp p, Exp ex, Exp e = nullptr);
        double interp(std::shared_ptr<Table> table);
        Type typeCheck(std::shared_ptr<Table> table);
};


struct funCallExp : public Exp_
{
    private:
        std::vector<Exp> params;
        Exp func;
    public:
        funCallExp(Exp f, ExpList *p);
    
        double interp(std::shared_ptr<Table> table);
        Type typeCheck(std::shared_ptr<Table> table);
};

struct borrowExp : public Exp_
{
    private:
        std::string id;
        Type origType;
        Exp exp;
        bool mut;

    public:
        borrowExp(Exp e, bool m);
    
        double interp(std::shared_ptr<Table> table);
        Type typeCheck(std::shared_ptr<Table> table);
        Type getOrigType() override;
};

struct derefExp : public Exp_
{
    private:
        Exp exp;
        std::string key;

    public:
        derefExp(Exp e);
    
        double interp(std::shared_ptr<Table> table);
        Type typeCheck(std::shared_ptr<Table> table);
        std::string getId() override;

};


#endif // !EXPSTRUCTSH

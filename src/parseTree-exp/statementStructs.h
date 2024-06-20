#ifndef C749B994_C447_4B9D_9B72_50DDE5E62D8D
#define C749B994_C447_4B9D_9B72_50DDE5E62D8D

// #include"statement.h"
#include"expresion.h"

#include <iostream>

struct ItemExp : public Exp_
{
    private:
        Exp Exp1, Exp2;
    public:
        ItemExp(Exp s1, Exp s2);
        double interp(std::shared_ptr<Table> table);
        Type typeCheck(std::shared_ptr<Table> table);
};

struct CompExp : public Exp_
{
    private:
        Exp Exp1, Exp2;
    public:
        CompExp(Exp s1, Exp s2);
        double interp(std::shared_ptr<Table> table);
        Type typeCheck(std::shared_ptr<Table> table);
};

struct LetExp : public Exp_
{
    private:
        std::string id;
        Exp exp;
        Variable var;
        
    public:
        // LetExp(std::string i, Exp e, bool mut = false, Type type = Type::None);
        LetExp(bool mut, std::string i, std::string type, Exp e);
        double interp(std::shared_ptr<Table> table);
        Type typeCheck(std::shared_ptr<Table> table);
};



struct ContExp : public Exp_
{
    private:

    public:
        ContExp(){};
        double interp(std::shared_ptr<Table> table){table->setLoopCtrl(LoopCtrl::Continue); return std::nan("1");};
        Type typeCheck(std::shared_ptr<Table> table){return Type::None;};
};

struct BreakExp : public Exp_
{
    private:

    public:
        BreakExp(){};
        double interp(std::shared_ptr<Table> table){table->setLoopCtrl(LoopCtrl::Break); return std::nan("1");};
        Type typeCheck(std::shared_ptr<Table> table){return Type::None;};
};

struct ReturnExp : public Exp_
{
    private:
        Exp exp;
    public:
        ReturnExp(Exp e);
        double interp(std::shared_ptr<Table> table);
        Type typeCheck(std::shared_ptr<Table> table);
};


struct expExp : public Exp_
{
    private:
        Exp exp;
    public:
        expExp(Exp e);
    
        double interp(std::shared_ptr<Table> table);
        Type typeCheck(std::shared_ptr<Table> table);
};

struct PrintVarExp : public Exp_
{
    private:
        std::string input;
    public:
        PrintVarExp(std::string i);
        double interp(std::shared_ptr<Table> table);
        Type typeCheck(std::shared_ptr<Table> table);
};

struct PrintStringExp : public Exp_
{
    private:
        std::string input;
    public:
        PrintStringExp(std::string i);
        double interp(std::shared_ptr<Table> table);
        Type typeCheck(std::shared_ptr<Table> table);
};
// struct PrintExp : public Exp_
// {
//     private:
//         ExpList exps;
//     public:
//         PrintExp(ExpList es);
//         int count();
//         double interp(std::shared_ptr<Table> table);
// };



#endif /* C749B994_C447_4B9D_9B72_50DDE5E62D8D */

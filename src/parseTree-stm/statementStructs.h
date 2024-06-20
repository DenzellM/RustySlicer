#ifndef C749B994_C447_4B9D_9B72_50DDE5E62D8D
#define C749B994_C447_4B9D_9B72_50DDE5E62D8D

#include"statement.h"
#include"expresion.h"

#include <iostream>

struct ItemStm : public Stm_
{
    private:
        Stm stm1, stm2;
    public:
        ItemStm(Stm s1, Stm s2);
        double interp(std::shared_ptr<Table> table);
        Type typeCheck(std::shared_ptr<Table> table);
};

struct CompStm : public Stm_
{
    private:
        Stm stm1, stm2;
    public:
        CompStm(Stm s1, Stm s2);
        double interp(std::shared_ptr<Table> table);
        Type typeCheck(std::shared_ptr<Table> table);
};

struct LetStm : public Stm_
{
    private:
        std::string id;
        Exp exp;
        Variable var;
        
    public:
        // LetStm(std::string i, Exp e, bool mut = false, Type type = Type::None);
        LetStm(bool mut, std::string i, std::string type, Exp e);
        double interp(std::shared_ptr<Table> table);
        Type typeCheck(std::shared_ptr<Table> table);
};



struct ContStm : public Stm_
{
    private:

    public:
        ContStm(){};
        double interp(std::shared_ptr<Table> table){table->setLoopCtrl(LoopCtrl::Continue); return std::nan("1");};
        Type typeCheck(std::shared_ptr<Table> table){return Type::None;};
};

struct BreakStm : public Stm_
{
    private:

    public:
        BreakStm(){};
        double interp(std::shared_ptr<Table> table){table->setLoopCtrl(LoopCtrl::Break); return std::nan("1");};
        Type typeCheck(std::shared_ptr<Table> table){return Type::None;};
};

struct ReturnStm : public Stm_
{
    private:
        Exp exp;
    public:
        ReturnStm(Exp e);
        double interp(std::shared_ptr<Table> table);
        Type typeCheck(std::shared_ptr<Table> table);
};


struct expStm : public Stm_
{
    private:
        Exp exp;
    public:
        expStm(Exp e);
    
        double interp(std::shared_ptr<Table> table);
        Type typeCheck(std::shared_ptr<Table> table);
};

struct PrintVarStm : public Stm_
{
    private:
        std::string input;
    public:
        PrintVarStm(std::string i);
        double interp(std::shared_ptr<Table> table);
        Type typeCheck(std::shared_ptr<Table> table);
};

struct PrintStringStm : public Stm_
{
    private:
        std::string input;
    public:
        PrintStringStm(std::string i);
        double interp(std::shared_ptr<Table> table);
        Type typeCheck(std::shared_ptr<Table> table);
};
// struct PrintStm : public Stm_
// {
//     private:
//         ExpList exps;
//     public:
//         PrintStm(ExpList es);
//         int count();
//         double interp(std::shared_ptr<Table> table);
// };



#endif /* C749B994_C447_4B9D_9B72_50DDE5E62D8D */

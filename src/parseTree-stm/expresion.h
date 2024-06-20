#ifndef A17A2BFA_ACDE_404C_B44C_B1EFF99B3B8A
#define A17A2BFA_ACDE_404C_B44C_B1EFF99B3B8A

#include "table.h"

class Exp_
{
    private:
    public:
        // virtual double maxargs() = 0;
        virtual double interp(std::shared_ptr<Table> table) = 0;
        virtual Type typeCheck(std::shared_ptr<Table> table) = 0;

        virtual std::string getId(){return "";};
        virtual bool getMut(){return false;};
        virtual Type getOrigType(){return Type::None;};

        ~Exp_(){free(this);};
};

typedef Exp_* Exp;



struct ExpList
{
    public:
    Exp head;
    ExpList *tail;
    ExpList(Exp e, ExpList *t) : head(e), tail(t) {};
    ExpList(Exp e) : head(e), tail(nullptr) {};
    ExpList() : head(nullptr), tail(nullptr) {};

};


#endif /* A17A2BFA_ACDE_404C_B44C_B1EFF99B3B8A */

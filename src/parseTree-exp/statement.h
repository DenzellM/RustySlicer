#ifndef EAA5EAE1_708E_41AB_8F12_25AA692074C5
#define EAA5EAE1_708E_41AB_8F12_25AA692074C5

#include "table.h"
#include "expresion.h"


class Stm_ : public Exp_
{
    private:
    public:
        virtual double interp(std::shared_ptr<Table> table) = 0;
        // virtual int maxargs() = 0;
        virtual Type typeCheck(std::shared_ptr<Table> table) = 0;
        ~Stm_(){free(this);};
};

typedef Stm_* Stm;

#endif /* EAA5EAE1_708E_41AB_8F12_25AA692074C5 */

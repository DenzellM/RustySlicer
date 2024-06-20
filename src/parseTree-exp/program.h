#ifndef F1028275_DECB_487F_AAE5_F8FC61643FA0
#define F1028275_DECB_487F_AAE5_F8FC61643FA0

#include "statementStructs.h"
#include "function.h"
class Program
{
    private:
        ItemExp *items;
    public:
        Program(ItemExp *i): items(i) {};

        void execute(){
            std::shared_ptr<Table> check = std::make_shared<Table>();
            items->interp(check);
            items->typeCheck(check);
            std::cout << "succesfully typechecked" << std::endl;
            
            std::shared_ptr<Table> table = std::make_shared<Table>();
            items->interp(table);
            std::cout << "succesfully initialised program" << std::endl;
            ((funcExp *) table->getFunc("main"))->interp(table, std::vector<Exp>());
        }
};

#endif /* F1028275_DECB_487F_AAE5_F8FC61643FA0 */

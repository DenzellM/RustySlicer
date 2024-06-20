#ifndef D2956C44_4D70_4FBA_B45E_0B1D8A92A112
#define D2956C44_4D70_4FBA_B45E_0B1D8A92A112

#include "statement.h"
#include "expresion.h"
struct funcParam
{
    public:
        std::string name;
        Type type;
        int borrowCount = 0;
        bool mut = false;
        funcParam(Exp e, std::string t){
            bool changed = true;
            while (changed){
                changed = false;
                if(t[0] == '&'){
                    borrowCount++;
                    t = t.erase(0, 1);
                    changed = true;
                }

                if(t.find("mut") == 0){
                    mut = true;
                    t = t.erase(0, 3);
                    changed = true;
                }
            }

            if(borrowCount == 0) mut = true;
            if(e->getId() == "") throw new std::exception();

            name = e->getId();
                       if(t == "i32"){
                type = Type::Int;
            }else if (t == "bool"){
                type = Type::Bool;
                
            }else if (t == "float"){
                type = Type::Double;
                
            }else{
                std::cerr <<"unrecognised type"<<std::endl;
                throw new std::exception();
            }

            // std::cout << "init funcParam: " << name << "-->" << t << std::endl;
        };
};

struct funcParamList
{
    public:
        funcParam *head;
        funcParamList *tail;

        funcParamList(funcParam *h, funcParamList *t): head(h), tail(t) {};
        funcParamList(funcParam *h): head(h), tail(nullptr) {};
        funcParamList(): head(nullptr), tail(nullptr) {};
};

struct funcStm : public Stm_
{
    private:
        std::vector<funcParam *> params;
        std::string name;
        Type returnType;
        Exp body;

    public:
        funcStm(std::string n, funcParamList *p, std::string ret, Exp b);
    
        double interp(std::shared_ptr<Table> table);
        double interp(std::shared_ptr<Table> table, std::vector<Exp> exps);
        Type typeCheck(std::shared_ptr<Table> table);
        void checkParam(std::shared_ptr<Table> table, std::vector<Exp> exps);

        Type getType() {return returnType;}
};




 
#endif /* D2956C44_4D70_4FBA_B45E_0B1D8A92A112 */

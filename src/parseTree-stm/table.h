#ifndef ABB9CEE5_CDCE_4CD6_8232_4A066ADD0213
#define ABB9CEE5_CDCE_4CD6_8232_4A066ADD0213

#include <string>
#include <map>
#include <memory>
#include <vector>

#include "defines.h"

class Stm_;


class Table
{
private:
    std::map<std::string, Variable> dict_;
    std::map<std::string, Stm_ *> funcs_;
    std::vector<std::string> keys_;
    LoopCtrl loopCtrl = NoCtrl;
    ReturnType ret = ReturnType::NoReturn;
    double returnVal;
public:
    std::shared_ptr<Table> clone();
    std::shared_ptr<Table> shallowClone();
    
    void updateScope(std::shared_ptr<Table>);
    void shallowUpdate(std::shared_ptr<Table>);
    
    void setLoopCtrl(LoopCtrl ctrl) {loopCtrl = ctrl;};
    void resetLoopVar() {loopCtrl = NoCtrl;};
    LoopCtrl getLoopVar() {return loopCtrl;};

    void setRet(ReturnType retType = ReturnType::NoReturn, double retVal = 0.0);
    void resetRet();

    ReturnType getReturnType();
    double getReturnValue();

    void update(std::string i, double v);
    void update(std::string i, Variable v);
    void insert(std::string i, Variable v);
    int getIndex(std::string i);
    std::string getKey(int index);
    std::string getNewKey();
    Variable getVariable(std::string i);

    void update(std::string i, Stm_ * e);
    Stm_ * getFunc(std::string i);
    Type getFuncType(std::string i);
};


#endif /* ABB9CEE5_CDCE_4CD6_8232_4A066ADD0213 */

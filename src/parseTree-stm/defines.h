#ifndef DEFINESH
#define DEFINESH

#include <iostream>
#include <vector>
#include <cmath>


typedef enum {PlusEq, MinEq, TimeEq, DivEq} CompOp;

typedef enum {Plus, Minus, Times, Div} Binop;
typedef enum {And, Or} Boolop;
typedef enum {Eq, Ne, Gt, Lt, Ge, Le} Compop;

typedef enum{Int, Double, Bool,Unknown, Function, None, Ref} Type;
typedef enum{ReturnNoVal, ReturVal, NoReturn} ReturnType;
typedef enum{Break, Continue, NoCtrl} LoopCtrl;

typedef struct{
    double value;
    Type type;
    bool mut;
    bool assigned;
} Variable;

#endif // !DEFINESH

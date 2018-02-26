%{

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "declaration.c"    

extern FILE* yyin;
extern char* yytext;
extern int yylineno;
extern int yylex();

#define MAX_STRUCTS 1000
#define MAX_NR_IDENTIFIERS 1000
#define MAX_NR_FUNCTIONS 1000
#define MAX_NR_STACKS 255



struct declaration *identifierStruct[MAX_NR_STACKS][MAX_NR_IDENTIFIERS],
                   *functionStruct[MAX_NR_FUNCTIONS];


int yyerror(char *s){
   printf("The Program is bad defined!!! Error: %s at line:%d\n", s, yylineno);
}

void handleError(int code) {
    
    if(code == DIFFERENT_TYPES) {
        yyerror("The variables have not the same type!");
    }
    else if (code == DIV_BY_ZERO) {
        yyerror("Warning! Division by zero!");
    }
    else if (code == DST_NOT_INITIALIZED) {
        yyerror("Destination variable is not initialized!");
    }
    else if (code == SRC_NOT_INITIALIZED) {
        yyerror("Source variable is not initialized!");
    }
    else if (code == OPERATION_NOT_SUPPORTED_ON_TYPE) {
        yyerror("Operation is not supported on this type of variable!");
    }
    if(code != 0) {
        exit(code);
    }
}


int nrStacks;
int nrIdentifiers[MAX_NR_STACKS];

void increaseStack() {
  nrStacks++;
}

void decreaseStacks() {
  for(int i=0; i<nrIdentifiers[nrStacks-1]; i++){
      free (identifierStruct[nrStacks-1][i]);
      free (identifierStruct[nrStacks-1][i]);
      identifierStruct[nrStacks-1][i] = NULL;
  }
  nrStacks--;
}

struct declaration* find(struct declaration** vector, int numberElements, char *name) {
    for(int i = 0; i < numberElements; i++) {
        if(compare(vector[i]->name, name) == 0){
            return vector[i];
        }
    }
    
    return NULL;
}

struct declaration* findInList(struct list* theList, char *name) {
    if(theList == NULL) {
        return NULL;
    }
    
    for(struct list *item = theList->head; item != NULL; item = item->next) {
        if(compare(item->value->name, name) == 0){
            return item->value;
        }
    }
   
    return NULL;
}

struct declaration* getMember(struct declaration* var, struct declaration *idTypeForms) {
    struct declaration *member = findInList(var->vars, idTypeForms->name);
    
    if (member == NULL) {
        yyerror("Member not found!");
        exit(1);
    }
    
    return member;
}


// # ## IDENTIFIERS ## #

struct declaration* findIdentifier(char *name) {
    struct declaration *elem = find(identifierStruct[nrStacks-1], nrIdentifiers[nrStacks-1], name);
    if(elem == NULL) {
        yyerror("Variable is not defined!");
        exit(0);
    }
    return elem;
}

void copyValue(char *dstName, char *srcName){
    
    struct declaration *dst = findIdentifier(dstName);
    struct declaration *src = findIdentifier(srcName);
    handleError(copyTo(dst, src));
}

// # ## END ## #


struct list* cloneVars(struct list *theList) {
    
    if(theList == NULL){
        return NULL;
    }
    
    
    struct list *newLst = newList(cloneVariable(theList->head->value));
    for(struct list *item = newLst->head->next; item != NULL; item = item->next) {         
        newLst = pushBack(newLst, cloneVariable(item->value));
    }
    return newLst;
}


// # ## STRUCTS ## # 

struct declaration *structs[MAX_STRUCTS];
int nrStructs;

struct declaration* findStruct (char *name) {
    struct declaration *elem = find(structs, nrStructs, name);
    if(elem == NULL) {
        yyerror("Struct is not defined!");
        exit(0);
    }
    return elem;
}

void addStructToStack(struct declaration *var) {
    struct declaration *stackStruct = find(structs, nrStructs, var->name);
    if(stackStruct != NULL) {
        yyerror("Struct already defined!");
        exit(0);
    }
    
    structs[nrStructs] = var;
    nrStructs++;
}


struct declaration* addNewStruct(char *structName, struct list *members) {
    struct declaration *strct;
    strct = newDeclaration(NULL, structName, false);
    strct->vars = cloneVars(members);
    
    addStructToStack(strct);
    return strct;
}

void setStructSingle(struct declaration *strct, struct declaration *var) {
    var->type = strdup(strct->name);
    var->vars = cloneVars(strct->vars);
}

void setStruct(struct declaration *strct, struct list *vars) {
    for(struct list *item = vars->head; item != NULL; item = item->next) {
        setStructSingle(strct, item->value);
    }
}

// # ## END ## #

void addToStack(struct declaration *var) {
    struct declaration *stackVar = find(identifierStruct[nrStacks-1], nrIdentifiers[nrStacks-1], var->name);

    if(stackVar != NULL) {
        if(compare(var->type, stackVar->type) != 0) {
            yyerror("Variable already defined with another type!");
            exit(0);
        }
        else {
            yyerror("Variable already defined!");
            exit(0);
        }
    }
    identifierStruct[nrStacks-1][nrIdentifiers[nrStacks-1]] = var;
    nrIdentifiers[nrStacks-1]++;
}

void addListToStack(struct list *vars) {
    for(struct list *item = vars->head; item != NULL; item = item->next){
        addToStack(item->value);
    }
}

// # ## FUNCTION ## # 

int nrFunctions;

void addFuncToStack(struct declaration *func) {
    struct declaration *stackFunc = find(functionStruct, nrFunctions, func->name);
    if(stackFunc != NULL) {
        yyerror("Function already defined!");
        exit(0);
    }
    
    functionStruct[nrFunctions] = func;
    nrFunctions++;
}

struct declaration* addNewFunction(char *functionType, char *functionName, struct list *params) {
    struct declaration *func;
    func = newDeclaration(functionType, functionName, false);
    func->vars = cloneVars(params);
    
    addFuncToStack(func);
    return func;
}

struct declaration* findFunction (char *name) {
    struct declaration *elem = find(functionStruct, nrFunctions, name);
    if(elem == NULL) {
        yyerror("Function is not defined!");
        exit(0);
    }
    return elem;
}



// # ## END ## #



%}

%union { float floatNumberValue; int intNumberValue; char *StringValue; struct declaration *var; struct list *vars; } 


%type <vars> VarDeclList
%type <vars> DeclParameters
%type <vars> VariableDeclaration
%type <vars> DataTypeBlock
%type <vars> StructStmt
%type <vars> StructVariableDeclaration
%type <vars> CallParameters
%type <var> IdentTypes 
%type <var> AExp
%type <var> BExp
%type <var> FunctionPrototype
%type <var> ValueTypes
%type <var> FunctionCall
%type <var> AtomicValues
%type <var> AssignementDeclID
%token <StringValue> ID
%token <StringValue> VAR_TYPE 
%token <intNumberValue> INTEGER
%token <floatNumberValue> FLOAT
%token CONST VOID_TYPE PRINT STRUCT  RETURN TEOF
%token ASSIGN_PLUS ASSIGN_MINUS ASSIGN_PROD ASSIGN_DIV ASSIGN_MOD ASSIGN
%token MINUS PLUS DIVISION MULTIPLY MODULO
%token CHAR STRING

%token TRUE FALSE BOOL_OP
%token FOR WHILE DO IF ELSE


%nonassoc "simple_if"
%nonassoc ELSE


%left BOOL_OP
%left PLUS MINUS
%left MULTIPLY DIVISION MODULO
%start RunProgram

%%

RunProgram : CodeProgram TEOF { printf("The Program is Well Defined! \n"); exit(0); }
           ;

CodeProgram : /* EMPTY */
            | ProgramMainBody
            ;
ProgramMainBody : Declarations
            | ProgramMainBody Declarations
            ;
Declarations : VariableDeclaration ';'
             | FunctionDecl
             | DataTypeDecl ';'
             | ';'
             ;


VariableDeclaration : VAR_TYPE VarDeclList {
        for(struct list *elem = $2->head ; elem !=NULL; elem=elem->next) {
            elem->value->type = strdup($1);
            addToStack(elem->value);
        }
}
             | CONST VAR_TYPE VarDeclList {
                 
        for(struct list *elem = $3->head ; elem !=NULL; elem=elem->next) {
            elem->value->type = strdup($2);
            addToStack(elem->value);
        }   
    }

             ;

IdentTypes : ID  { $$ = newDeclaration(NULL, $1, false); }
            | ID '[' AExp ']' { $$ = newDeclaration(NULL, $1, true); }
            | ID '[' AExp ']' '[' AExp ']' { $$ = newDeclaration(NULL, $1, true); }
            ;
            

VarDeclList : IdentTypes   { $$ = newList($1); }
            | VarDeclList ',' IdentTypes  { $$ = pushBack($1, $3);}
            | AssignementDeclID    { $$ = newList($1); }
            | VarDeclList ',' AssignementDeclID  { $$ = pushBack($1, $3);}
            ;

DataTypeDecl : STRUCT ID DataTypeBlock                      
             { addNewStruct($2, $3); }
             | STRUCT ID DataTypeBlock VarDeclList          
             { 
                struct declaration *strct = addNewStruct($2, $3);
                setStruct(strct, $4); //set the type for structs
                addListToStack($4); //add the variables in stack
             } 
             | ID VarDeclList                               
             { 
                struct declaration *strct = findStruct($1); 
                setStruct(strct, $2);
                addListToStack($2);
             }
             | CONST ID VarDeclList                         
             {  
                struct declaration *strct = findStruct($2); 
                setStruct(strct, $3);
                addListToStack($3);
             }
             ;
DataTypeBlock : '{' '}' { $$ = NULL; }
              | '{' StructStmt '}' { $$ = $2; }
              ;
StructStmt : StructVariableDeclaration ';' { $$ = $1; }
           | StructStmt StructVariableDeclaration ';' { $$ = extendBack($1, $2); }
           ;

StructVariableDeclaration : VAR_TYPE VarDeclList {
   
        for(struct list *elem = $2->head ; elem !=NULL; elem=elem->next) {
            
            elem->value->type = strdup($1);
 
        }
        $$ = $2;
}
             | CONST VAR_TYPE VarDeclList {
                 
        for(struct list *elem = $3->head ; elem !=NULL; elem=elem->next) {
            elem->value->type = strdup($2);
        }   
        
        $$ = $3;
    }

             ;

FunctionPrototype : VAR_TYPE ID '(' DeclParameters ')' {
                    $$ = addNewFunction($1, $2, $4);
                  }
                  | VAR_TYPE ID '(' ')' {
                    $$ = addNewFunction($1, $2, NULL);
                  }
                  | VOID_TYPE ID '(' DeclParameters ')' {
                    $$ = addNewFunction(NULL, $2, $4);
                  }
                  | VOID_TYPE ID '(' ')'  {
                    $$ = addNewFunction(NULL, $2, NULL);
                  }
                  ;
                  
DeclParameters : VAR_TYPE IdentTypes {
                $2->type = $1;
                $$ = newList($2); //lista de declaratii
              }
              | DeclParameters ',' VAR_TYPE IdentTypes {
                $4->type = $3;
                $$ = pushBack($1, $4); //lista de declaratii
              }
              | CONST VAR_TYPE IdentTypes {
                $3->type = $2;
                $$ = newList($3); //lista de declaratii
              }
              | DeclParameters ',' CONST VAR_TYPE IdentTypes
              {
                $5->type = $4;
                $$ = pushBack($1, $5); //lista de declaratii
              }
              
              | ID IdentTypes                              {
                    struct declaration *strct = findStruct($1);                    
                    setStructSingle(strct, $2);
                    $$ = newList($2); //lista de declaratii
                }
              
              | DeclParameters ',' ID IdentTypes            { 
                  struct declaration *strct = findStruct($3);
                  setStructSingle(strct, $4);
                  $$ = pushBack($1, $4); //lista de declaratii
                }
              
              | CONST ID IdentTypes                        { 
                  struct declaration *strct = findStruct($2);
                  setStructSingle(strct, $3);
                  $$ = newList($3); //lista de declaratii
              }
              
              | DeclParameters ',' CONST ID IdentTypes      { 
                  struct declaration *strct = findStruct($4); 
                  setStructSingle(strct, $5);
                  $$ = pushBack($1, $5); //lista de declaratii  
              }
              ;

FunctionDecl : FunctionPrototype ';'
             | FunctionPrototype BlockStatement
             ;


printFunction : PRINT '(' AExp ')' ';' {
                if(compare($3->type, "int") == 0) {
                    printf("%d \n", $3->intValue);
                }
                if(compare($3->type, "float") == 0) {
                    printf("%f \n", $3->floatValue);
                }
                
                /*if(compare($3->type, "char") == 0 || compare($3->type, "string") == 0) {
                    yyerror("The types of the print parameters are not numeric!!!");
                }*/
}
                ;
                
BlockStatement : '{' '}'
             | '{' Statements '}'
             ;
Statements : SimpleStmt
           | Statements SimpleStmt
           ;
SimpleStmt : SingleStatement ';'
           | BlockStatement
           | ControlStmt
           | printFunction
           | RETURN ValueTypes ';'
           | RETURN ';'
           | ';'
           ;

AtomicValues : INTEGER {
                $$ = newDeclaration("int", NULL, false);
                $$->isInitialized = true;
                $$->intValue = $1;
                
             }
             | FLOAT {
                $$ = newDeclaration("float", NULL, false);
                $$->isInitialized = true;
                $$->floatValue = $1;
             }
             | CHAR {
                $$ = newDeclaration("char", NULL, false);
                $$->isInitialized = true;
             }
             | STRING {
                $$ = newDeclaration("string", NULL, false);
                $$->isInitialized = true;
             }
             ;

AExp : IdentTypes     { $$ = findIdentifier($1->name); } 
     | IdentTypes '.' IdentTypes { $$ = getMember(findIdentifier($1->name), $3); }
     | AtomicValues    { $$ = $1; }
     | FunctionCall    { $$ = $1; } 
     | AExp PLUS AExp  {
        $$ = cloneVariable($1);
        handleError(addOperation($$, $3));
     }
     | AExp MINUS AExp{ 
        $$ = cloneVariable($1);
        handleError(subOperation($$, $3));
     }
     | AExp MULTIPLY AExp { 
        $$ = cloneVariable($1);
        handleError(mulOperation($$, $3));
     }
     | AExp DIVISION AExp { 
        $$ = cloneVariable($1);
        handleError(divOperation($$, $3));
     }
     | AExp MODULO AExp { 
        $$ = cloneVariable($1);
        handleError(modOperation($$, $3));
     }
     | '(' AExp ')'      { $$ = $2; }
     ;
     
AssignementDeclID : IdentTypes ASSIGN ValueTypes { $$ = findIdentifier($1->name);
                                       handleError(copyTo($$, $3));
                                     }
                    | IdentTypes '.' IdentTypes ASSIGN ValueTypes { $$ = getMember(findIdentifier($1->name), $3);
                                       handleError(copyTo($$, $5));
                                     }    
                  ;

IdAssignement : ID ASSIGN_PLUS AExp {
                handleError(addOperation(findIdentifier($1), $3));
              }
              | ID ASSIGN_MINUS AExp {
                handleError(subOperation(findIdentifier($1), $3));
              }
              | ID ASSIGN_PROD AExp {
                handleError(mulOperation(findIdentifier($1), $3));
              }
              | ID ASSIGN_DIV AExp {
                handleError(divOperation(findIdentifier($1), $3));  
              }
              | ID ASSIGN_MOD AExp {
                handleError(addOperation(findIdentifier($1), $3));
              }       
              ;
              
Assignement : AssignementDeclID
            | IdAssignement
            ;



CallParameters : ValueTypes { $$ = newList($1); }
              | CallParameters ',' ValueTypes { $$ = pushBack($1, $3); }
              
              ;
              
FunctionCall : ID '(' ')' { $$ = findFunction($1); }
             | ID '(' CallParameters ')' {  //here we check if the functionCallParameters have the same type with the declFunctionParameters
                $$ = findFunction($1);
                struct list *first, *second;
                for(first = $$->vars->head, second =$3->head; first != NULL && second != NULL; first = first->next, second = second->next) {
                    if(compare($$->vars->value->type, $3->value->type) != 0) {
                        yyerror("Different parameters type!!!");
                    }     
                }
                
                /*if (first != second) {
                        yyerror("Too many parameters!");
                }*/
                
                }
             ;
SingleStatement : VariableDeclaration
                | DataTypeDecl
                | CondExp
                ;

BExp : TRUE {
            $$ = newDeclaration("bool", NULL, false);
            $$->isInitialized = true;
            $$->intValue = 1;
        }
     | FALSE {
            $$ = newDeclaration("bool", NULL, false);
            $$->isInitialized = true;
            $$->intValue = 0;
        }
     | '(' BExp ')' {$$ = $2;}
     | ValueTypes BOOL_OP ValueTypes {$$ = $1;}
     ;
ValueTypes : BExp { $$ = $1; }
       | AExp { $$ = $1; }
       ;
CondExp : ValueTypes
        | Assignement
        | '(' Assignement ')'
        ;
IfStatement : IF '(' CondExp ')' SimpleStmt                        %prec "simple_if"
       | IF '(' CondExp ')' SimpleStmt ELSE SimpleStmt
       ;
IfCond : CondExp '?' CondExp ':' CondExp


StepFor1 : /* EMPTY */
         | VariableDeclaration
         | CondExp
         ;
StepFor2_3 : /* EMPTY */
           | CondExp
           ;
ForStatement : FOR '(' StepFor1 ';' StepFor2_3 ';' StepFor2_3 ')' SimpleStmt
        ;


WhileStatement : WHILE '(' CondExp ')' SimpleStmt
          ;


ControlStmt : IfStatement
            | IfCond ';'
            | ForStatement
            | WhileStatement
            ;

%%

int main(int argc, char** argv){
  yyin=fopen(argv[1],"r");
  yyparse();
}

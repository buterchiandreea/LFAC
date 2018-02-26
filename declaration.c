#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define OPERATION_NOT_SUPPORTED_ON_TYPE 5
#define DIV_BY_ZERO 4
#define DST_NOT_INITIALIZED 3
#define SRC_NOT_INITIALIZED 2
#define DIFFERENT_TYPES 1


int compare(char *a, char *b) {
    if(a == NULL || b == NULL) {
        return -1;
    }
    return strcmp(a, b);
}

struct list;

struct declaration{
    struct list *vars;
    bool isArray;
    bool isInitialized;
    int  intValue;
    float floatValue;
    char *type;
    char *name;
}; 

struct list {
    struct list *next;
    struct list *head;
    struct declaration *value;
};

struct list* newList(struct declaration *var) {
     struct list *newList = (struct list*) malloc (sizeof(struct list));
     newList->value = var;
     newList->head = newList;
     newList->next = NULL;
}


struct list* pushBack(struct list *self, struct declaration *var) {
    self->next = newList(var);
    self->next->head = self->head;
    return self->next;
}

struct list* extendBack(struct list *dst, struct list *src) {
    dst->next = src->head;
    for(struct list *item = src->head; item != NULL; item = item->next) {
        src->head = dst->head;
    }
    return src;
}

struct declaration* newDeclaration(char* type, char* name, bool isArray) {
   
    struct declaration *newDeclaration;
    newDeclaration = (struct declaration*) malloc (sizeof(struct declaration));
    newDeclaration->isArray = isArray;    
    newDeclaration->vars = NULL;    
    
    newDeclaration->isInitialized = false;
   
    if(type != NULL) {
        newDeclaration->type = strdup(type);
    }
    else {
        newDeclaration->type=NULL;
    }
    
    if(name != NULL) {
        newDeclaration->name = strdup(name);
    }
    else {
        newDeclaration->name=NULL;
    }
  
    
    return newDeclaration;
} 



int copyTo(struct declaration *dst, struct declaration *src) {
         
    if(compare(dst->type, src->type) != 0) {
        return DIFFERENT_TYPES;
    }
    if(!src->isInitialized){
        return SRC_NOT_INITIALIZED;
    }
   
    if(compare(dst->type, "int") == 0) { 
        dst->intValue = src->intValue;
    }
    else if(compare(dst->type, "float") == 0) { 
        dst->floatValue = src->floatValue;
    }
    else if(compare(dst->type, "bool") == 0) { 
        dst->intValue = src->intValue;
    }
    dst->isInitialized = true;
 
    return 0;
}

int addOperation(struct declaration *dst, struct declaration *src) {
    if(compare(dst->type, src->type) != 0) {
        return DIFFERENT_TYPES;
    }
    if(!src->isInitialized){
        return SRC_NOT_INITIALIZED;
    }
    if(!dst->isInitialized){
        return DST_NOT_INITIALIZED;
    }
    if(compare(dst->type, "int") == 0) { 
        dst->intValue += src->intValue;
    }
    else if(compare(dst->type, "float") == 0) { 
        dst->floatValue += src->floatValue;
    }
    else if(compare(dst->type, "bool") == 0) { 
        dst->intValue |= src->intValue;
    }
    return 0;
}

int subOperation(struct declaration *dst, struct declaration *src) {
    if(compare(dst->type, src->type) != 0) {
        return DIFFERENT_TYPES;
    }
    if(!src->isInitialized){
        return SRC_NOT_INITIALIZED;
    }
    if(!dst->isInitialized){
        return DST_NOT_INITIALIZED;
    }
    if(compare(dst->type, "int") == 0) { 
        dst->intValue -= src->intValue;
    }
    else if(compare(dst->type, "float") == 0) { 
        dst->floatValue -= src->floatValue;
    }
    else if(compare(dst->type, "bool") == 0) { 
        dst->intValue |=! src->intValue;
    }
    return 0;
}



int mulOperation(struct declaration *dst, struct declaration *src) {
    
    
    if(compare(dst->type, src->type) != 0) {
        return DIFFERENT_TYPES;
    }
    if(!src->isInitialized){
        return SRC_NOT_INITIALIZED;
    }
    if(!dst->isInitialized){
        return DST_NOT_INITIALIZED;
    }
    if(compare(dst->type, "int") == 0) { 
        dst->intValue *= src->intValue;
    }
    else if(compare(dst->type, "float") == 0) { 
        dst->floatValue *= src->floatValue;
    }
    else if(compare(dst->type, "bool") == 0) { 
        dst->intValue &= src->intValue;
    }
    return 0;
}

int divOperation(struct declaration *dst, struct declaration *src) {
    
    
    if(compare(dst->type, src->type) != 0) {
        return DIFFERENT_TYPES;
    }
    if(!src->isInitialized){
        return SRC_NOT_INITIALIZED;
    }
    if(!dst->isInitialized){
        return DST_NOT_INITIALIZED;
    }
    if(compare(dst->type, "int") == 0) {
        if(src->intValue == 0) {
            return DIV_BY_ZERO;
        }            
        dst->intValue /= src->intValue;
    }
    else if(compare(dst->type, "float") == 0) { 
        
        if(src->floatValue == 0) {
            return DIV_BY_ZERO;
        }  
        dst->floatValue /= src->floatValue;
    }
    else if(compare(dst->type, "bool") == 0) { 
        dst->intValue ^= src->intValue;
    }
    
    return 0;
}

int invOperation(struct declaration *dst) {
    
    if(!dst->isInitialized){
        return DST_NOT_INITIALIZED;
    }
    if(compare(dst->type, "int") == 0) { 
        dst->intValue = -dst->intValue;
    }
    else if(compare(dst->type, "float") == 0) { 
        dst->floatValue = -dst->floatValue;
    }
    else if(compare(dst->type, "bool") == 0) { 
        dst->intValue = ! dst->intValue;
    }
    return 0;
}

int modOperation(struct declaration *dst, struct declaration *src) {
    
    if(!dst->isInitialized){
        return DST_NOT_INITIALIZED;
    }
    if(!src->isInitialized){
        return SRC_NOT_INITIALIZED;
    }
    if(compare(src->type, "int") != 0) { 
        return OPERATION_NOT_SUPPORTED_ON_TYPE;
    }
    if(compare(dst->type, "int") != 0) { 
        return OPERATION_NOT_SUPPORTED_ON_TYPE;
    }
    dst->intValue %= src->intValue;
    return 0;
}




struct declaration* cloneVariable(struct declaration *var) {
    
    struct declaration *newVar;
    newVar = newDeclaration(var->type, var->name, var->isArray);
    newVar->isInitialized = var->isInitialized;
    newVar->intValue = var->intValue;
    newVar->floatValue = var->floatValue;
    return newVar;
}




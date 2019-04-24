#pragma once
#include <map>
#include <string>
#include <list>

typedef void (*TVirtualMethod)( void* );

// virtual class without base
#define VIRTUAL_CLASS( className ) \
std::list<std::pair<std::string, TVirtualMethod>> initMethods##className; \
struct className { \
    std::string Name = #className; \
    std::map<std::string, TVirtualMethod> VirtualTable; \
    className() { \
        for (auto item : initMethods##className ) { \
            VirtualTable[item.first] = item.second; \
        } \
    }

#define END_CLASS( className ) \
};

// define virtual method of class that prints its full name
#define VIRTUAL_METHOD( className, methodName ) \
void className##methodName( void* self_ ) { \
    className* self = reinterpret_cast<className*>(self_); \
    std::cout << #className "::" #methodName "\n";

#define END_METHOD( className, methodName ) \
} \
/* insert method in init list */ \
int insert##className##methodName = []()->int { \
    initMethods##className.push_back( std::pair<std::string, TVirtualMethod>( #methodName, className##methodName ) ); \
    return 0; \
}();


// virtual class derived from base
#define VIRTUAL_CLASS_DERIVED( className, baseClassName ) \
std::list<std::pair<std::string, TVirtualMethod>> initMethods##className; \
struct className : baseClassName { \
    className() { \
        for (auto item : initMethods##className ) { \
            VirtualTable[item.first] = item.second; \
        } \
    }

// call virtual method
#define VIRTUAL_CALL( self, methodName ) \
(self)->VirtualTable[#methodName]( self );
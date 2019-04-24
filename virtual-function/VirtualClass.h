#pragma once
#include <map>
#include <string>

typedef void (*TVirtualMethod)( void* );

std::map<std::string, std::map<std::string, TVirtualMethod>> classMethods;

// virtual class without base
#define VIRTUAL_CLASS( className ) \
/* insert new class into map */ \
int insertClass##className = []()->int { \
    classMethods[#className] = std::map<std::string, TVirtualMethod>(); \
    return 0; \
}(); \
struct className { \
    std::string Name = #className; \
    std::map<std::string, TVirtualMethod> VirtualTable; \
    className() { \
        for (auto item : classMethods[#className] ) { \
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
    classMethods[#className][#methodName] = className##methodName; \
    return 0; \
}();

// virtual class derived from base
#define VIRTUAL_CLASS_DERIVED( className, baseClassName ) \
/* insert new class into map */ \
int insertClass##className = []()->int { \
    classMethods[#className] = std::map<std::string, TVirtualMethod>(); \
    return 0; \
}(); \
struct className : baseClassName { \
    std::string Name = #className; \
    className() { \
        for (auto item : classMethods[#className] ) { \
            VirtualTable[item.first] = item.second; \
        } \
    }

// call virtual method
#define VIRTUAL_CALL( self, methodName ) \
if ( classMethods[(self)->Name].find( #methodName ) != classMethods[(self)->Name].end() ) { \
    (self)->VirtualTable[#methodName]( self ); \
} else { \
    throw std::runtime_error( #self " has no method " #methodName ); \
}
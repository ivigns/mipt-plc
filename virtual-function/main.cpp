#include <iostream>

#include "VirtualClass.h"

VIRTUAL_CLASS( CBase )
    int a;
END_CLASS( CBase )

VIRTUAL_METHOD( CBase, Both )
    std::cout << "a = " << self->a << "\n";
END_METHOD( CBase, Both )
VIRTUAL_METHOD( CBase, OnlyBase )
    std::cout << "a = " << self->a << "\n";
END_METHOD( CBase, OnlyBase )

VIRTUAL_CLASS_DERIVED( CDerived, CBase )
    int b;
END_CLASS( CDerived )

VIRTUAL_METHOD( CDerived, Both )
    std::cout << "b = " << self->b << "\n";
END_METHOD( CDerived, Both )
VIRTUAL_METHOD( CDerived, OnlyDerived )
    std::cout << "b = " << self->b << "\n";
END_METHOD( CDerived, OnlyDerived )

int main()
{
    CBase base;
    base.a = 0;
    CDerived derived;
    derived.a = 1;
    derived.b = 1;
    CBase* reallyDerived = reinterpret_cast<CBase*>(&derived);

    try {
        VIRTUAL_CALL( &base, Both );
        VIRTUAL_CALL( reallyDerived, Both );
        VIRTUAL_CALL( reallyDerived, OnlyBase );
        VIRTUAL_CALL( reallyDerived, OnlyDerived );
    } catch( std::runtime_error ex ) {
        std::cout << ex.what() << "\n";
    }
    /* Should be printed:
     * CBase::Both
     * a = 0
     * CDerived::Both
     * b = 1
     * CBase::OnlyBase
     * a = 1
     * reallyDerived has no method OnlyDerived
    */

    system( "pause" );
    return 0;
}
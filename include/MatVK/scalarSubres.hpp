#pragma once

#include "matvk.hpp"


namespace matvk
{

    class ScalarSubres
    {
    public:
        ScalarSubres(ElemType type);
        ~ScalarSubres();
        ElemType type();
        const char* value();

    private:
        ElemType _type;
        const char* _val;
    };

};

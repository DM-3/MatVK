#pragma once

#include "matvk.hpp"


namespace matvk
{

    class ScalarSubres
    {
    public:
        ScalarSubres(ElemType type);
        ElemType type();

    private:
        ElemType _type;
    };

};

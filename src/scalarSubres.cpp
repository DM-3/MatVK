#include "MatVK/scalarSubres.hpp"


namespace matvk
{

    ScalarSubres::ScalarSubres(ElemType type) :
        _type(type)
    {}

    ElemType ScalarSubres::type()
    {
        return _type;
    }

};

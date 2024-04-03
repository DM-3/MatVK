#include "MatVK/scalarSubres.hpp"


namespace matvk
{

    ScalarSubres::ScalarSubres(ElemType type) :
        _type(type)
    {
        _val = new char[sizeofType(type)];
    }

    ScalarSubres::~ScalarSubres()
    {
        delete[] _val;
    }

    ElemType ScalarSubres::type()
    {
        return _type;
    }

    const char* ScalarSubres::value()
    {
        return _val;
    }

};

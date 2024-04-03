#include "MatVK/matvk.hpp"
#include "MatVK/scalarSubres.hpp"
#include "MatVK/shader.hpp"


namespace matvk
{

    ScalarBase::ScalarBase(ElemType elemType) : ExpressionBase({}),
        _sub(std::make_shared<ScalarSubres>(elemType)) 
    {}

    void* ScalarBase::value()
    {
        return (void*)_sub->value();
    }

    void ScalarBase::record(Shader& shader) const 
    {
        shader.addScalar(_sub);
    }
    
};

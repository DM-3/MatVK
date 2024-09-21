#include "computeTest.hpp"

using namespace matvk::tests;


#define COMPUTE_TEST_SINGLE_OPERATOR(Op) \
    this->_queue << (this->_vC = this->_vA Op this->_vB); \
    this->_queue.execute(); \
    this->_eC.array() = this->_eA.array() Op this->_eB.array(); \
    compareMatrices(this->_vC, this->_eC, 1e-6);


#define COMPUTE_TEST_MULTIPLE_OPERATORS() \
    this->_queue << (this->_vC = \
        this->_vA * (this->_vA + this->_vB) - this->_vA / this->_vB); \
    this->_queue.execute(); \
    this->_eC.array() = this->_eA.array() * (this->_eA + this->_eB).array() \
        - this->_eA.array() / this->_eB.array(); \
    compareMatrices(this->_vC, this->_eC, 1e-6);


#define COMPUTE_TEST_MULTIPLE_ASSIGNMENTS() \
    this->_queue = { \
        this->_vC = this->_vA * this->_vB + this->_vB, \
        this->_vB = this->_vC / this->_vA, \
        this->_vA = this->_vC - this->_vB }; \
    this->_queue.execute(); \
    this->_eC = (this->_eA.array() * this->_eB.array()).matrix() + this->_eB; \
    this->_eB.array() = this->_eC.array() / this->_eA.array(); \
    this->_eA = this->_eC - this->_eB; \
    compareMatrices(this->_vA, this->_eA, 3e-5); \
    compareMatrices(this->_vB, this->_eB, 3e-5); \
    compareMatrices(this->_vC, this->_eC, 3e-5);



// type tests

TYPED_TEST_SUITE_P(ComputeTestT);

TYPED_TEST_P(ComputeTestT, Addition)
{
    COMPUTE_TEST_SINGLE_OPERATOR(+)
}

TYPED_TEST_P(ComputeTestT, Subtraction)
{
    COMPUTE_TEST_SINGLE_OPERATOR(-)
}

TYPED_TEST_P(ComputeTestT, Multiplication)
{
    COMPUTE_TEST_SINGLE_OPERATOR(*)
}

TYPED_TEST_P(ComputeTestT, Division)
{
    COMPUTE_TEST_SINGLE_OPERATOR(/)
}

TYPED_TEST_P(ComputeTestT, MultipleOperators)
{
    COMPUTE_TEST_MULTIPLE_OPERATORS()
}

TYPED_TEST_P(ComputeTestT, MultipleAssignments)
{
    COMPUTE_TEST_MULTIPLE_ASSIGNMENTS()
}

REGISTER_TYPED_TEST_SUITE_P(ComputeTestT,
    Addition, 
    Subtraction, 
    Multiplication, 
    Division,
    MultipleOperators,
    MultipleAssignments);

using types = testing::Types<float, int>;
INSTANTIATE_TYPED_TEST_SUITE_P(Typed, ComputeTestT, types);



// param tests

TEST_P(ComputeTestP, Addition)
{
    COMPUTE_TEST_SINGLE_OPERATOR(+)
}

TEST_P(ComputeTestP, Subtraction)
{
    COMPUTE_TEST_SINGLE_OPERATOR(-)
}

TEST_P(ComputeTestP, Multiplication)
{
    COMPUTE_TEST_SINGLE_OPERATOR(*)
}

TEST_P(ComputeTestP, Division)
{
    COMPUTE_TEST_SINGLE_OPERATOR(/)
}

TEST_P(ComputeTestP, MultipleOperators)
{
    COMPUTE_TEST_MULTIPLE_OPERATORS()
}

TEST_P(ComputeTestP, MultipleAssignments)
{
    COMPUTE_TEST_MULTIPLE_ASSIGNMENTS()
}

INSTANTIATE_TEST_SUITE_P(Parametrized, ComputeTestP,
    testing::Combine(
        testing::Values(1, 17, 512), 
        testing::Values(1, 42)));

#include "MatVK/matvk.hpp"

namespace matvk
{

    Queue::Queue() {}

    Queue::Queue(std::initializer_list<Assignment> assignments) {}

    Queue& Queue::operator<<(Assignment assignment) { return *this; }

    Queue& Queue::operator<<(std::initializer_list<Assignment> assignments) { return *this; }

    void Queue::endRecording() {}

    void Queue::execute(bool immediateWait) {}

    void Queue::waitFinished() {}
    
};

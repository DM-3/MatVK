#include "MatVK/queueBase.hpp"

namespace matvk
{

    Queue::Queue() :
        Queue::Queue({})
    {}

    Queue::Queue(std::initializer_list<Assignment> assignments) :
        _recording(true), _base(std::make_shared<QueueBase>())
    {
        for (auto& a : assignments)
            *this << a;
    }

    Queue& Queue::operator<<(std::initializer_list<Assignment> assignments) 
    { 
        for (auto& a : assignments)
            *this << a;

        return *this; 
    }

    Queue& Queue::operator<<(Assignment assignment) 
    { 
        if (_recording)
            _base->addAssignment(assignment);
        else
            throw std::logic_error("Queue no longer open for recording");
        
        return *this; 
    }

    bool Queue::isRecording()
    {
        return _recording;
    }

    void Queue::endRecording() 
    {
        _base->compile();
        _recording = false;
    }

    void Queue::execute(bool immediateWait) 
    {
        if (_recording)
            endRecording();

        _base->execute();
        
        if (immediateWait)
            _base->waitFinished();
    }

    void Queue::waitFinished() 
    {
        if (_recording) return;        
        _base->waitFinished();
    }
    
};

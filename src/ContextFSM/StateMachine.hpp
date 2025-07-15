#pragma once

namespace ContextFSM
{

template <typename CTX, typename T>
struct StateMachine
{
    /**
    This is the base State that all derived states MUST inherit
    */
    struct State
    {
        virtual void OnEntry(StateMachine<CTX, T> &fsm) = 0;
        virtual void OnExit(StateMachine<CTX, T> &fsm) = 0;
    };

    /**
    We will store a singleton for each State used by the StateMachine.
    The drawback with this is that the state CANNOT store any local variables because they will be shared with ALL instances of the StateMachine.
    But the state CAN use the Context for the StateMachine
    */
    template <typename STATE>
    static STATE &GetStateInstance()
    {
        static STATE instance;
        return instance;
    }

    /**
    Internal: Pointer to the current state
    */
    State *state = nullptr;

    /**
    Each instance of the StateMachine will have it's own context
    */
    CTX ctx;

    /**
    Set an initial state and make the StateMachine ready for usage
    */
    template <typename STATE>
    void Start()
    {
        this->ctx = {};
        this->state = &GetStateInstance<STATE>();
        if (this->state != nullptr)
        {
            this->state->OnEntry(*this);
        }
    }

    /**
    Call OnExit on the current state and clear the StateMachine
    */
    void Stop()
    {
        if (this->state != nullptr)
        {
            this->state->OnExit(*this);
        }
        this->state = nullptr;
        this->ctx = {};
    }

    /**
    Change the StateMachine into a new State
    */
    template <typename STATE>
    void Transit()
    {
        if (this->state != nullptr)
        {
            this->state->OnExit(*this);
        }
        this->state = &GetStateInstance<STATE>();
        if (this->state != nullptr)
        {
            this->state->OnEntry(*this);
        }
    }

    /**
    Dispatch an Event to the current state
     */
    template <typename EVENT>
    void Dispatch(EVENT &event)
    {
        if (this->state != nullptr)
        {
            static_cast<T *>(this->state)->OnEvent(*this, event);
        }
    }

    template <typename STATE>
    bool IsState()
    {
        return this->state == &GetStateInstance<STATE>();
    }
};
} // namespace ContextFSM
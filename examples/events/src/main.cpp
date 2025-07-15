#include <ContextFSM/StateMachine.hpp>
#include <cstddef>
#include <spdlog/spdlog.h>

struct ExampleContext
{
    std::uint16_t handle = 0;
};

struct ExampleBaseState;
typedef ContextFSM::StateMachine<ExampleContext, ExampleBaseState> ExampleFSM;

struct EventConnect
{
    std::uint16_t handle;
};

struct EventDisconnect
{
};

struct EventTick
{
};

struct ExampleBaseState : ExampleFSM::State
{

    virtual void OnEvent(ExampleFSM &fsm, EventConnect &event)
    {
    }

    virtual void OnEvent(ExampleFSM &fsm, EventDisconnect &event)
    {
    }

    virtual void OnEvent(ExampleFSM &fsm, EventTick &event)
    {
    }
};

struct StateIdle;
struct StateConnected;

struct StateIdle : ExampleBaseState
{
    void OnEntry(ExampleFSM &fsm) override
    {
        spdlog::debug("StateIdle : OnEntry : handle = {}", fsm.ctx.handle);
    }

    void OnExit(ExampleFSM &fsm) override
    {
        spdlog::debug("StateIdle : OnExit : handle = {}", fsm.ctx.handle);
    }

    void OnEvent(ExampleFSM &fsm, EventConnect &event) override
    {
        spdlog::debug("StateIdle : EventConnect : handle = {}", event.handle);
        fsm.ctx.handle = event.handle;
        fsm.Transit<StateConnected>();
    }
};

struct StateConnected : ExampleBaseState
{
    void OnEntry(ExampleFSM &fsm) override
    {
        spdlog::debug("StateConnected : OnEntry : handle = {}", fsm.ctx.handle);
    }

    void OnExit(ExampleFSM &fsm) override
    {
        spdlog::debug("StateConnected : OnExit : handle = {}", fsm.ctx.handle);
        fsm.ctx.handle = 0;
    }

    void OnEvent(ExampleFSM &fsm, EventTick &event) override
    {
        spdlog::debug("StateConnected : EventTick : Should handle data exchange with connected client");
    }

    void OnEvent(ExampleFSM &fsm, EventDisconnect &event) override
    {
        spdlog::debug("StateConnected : EventDisconnect");
        fsm.Transit<StateIdle>();
    }
};

ExampleFSM fsm;
EventConnect eventConnect;
EventDisconnect eventDisconnect;
EventTick eventTick;

int main()
{
    spdlog::set_level(spdlog::level::debug);
    fsm.Start<StateIdle>();
    spdlog::debug("IsState<StateIdle> = {}", fsm.IsState<StateIdle>());
    spdlog::debug("IsState<StateConnected> = {}", fsm.IsState<StateConnected>());
    //
    // Simulate idle time
    for (int i = 0; i < 3; i++)
    {
        fsm.Dispatch(eventTick);
    }
    //
    // Simulate a new connection
    eventConnect.handle = 0x1234;
    fsm.Dispatch(eventConnect);
    spdlog::debug("IsState<StateIdle> = {}", fsm.IsState<StateIdle>());
    spdlog::debug("IsState<StateConnected> = {}", fsm.IsState<StateConnected>());
    //
    // Simulate time handling the established connection
    for (int i = 0; i < 3; i++)
    {
        fsm.Dispatch(eventTick);
    }
    //
    // Simulate a disconnect
    fsm.Dispatch(eventDisconnect);
    //
    // Cleanup
    fsm.Stop();
}
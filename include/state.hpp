#ifndef state_hpp
#define state_hpp

#include "basix.hpp"
#include "maze.hpp"
#include "hero.hpp"

using StateID = string;

class StateQueue;

class State
{
public:
    struct Context
    {
        Context(sf::RenderWindow &win, Maze *m, Hero *h, PathComputer *pc);

        sf::RenderWindow *window;
        Maze *maze;
        Hero *hero;
        PathComputer *computer;
    };

    State(StateQueue &st_stack, State::Context ctxt);
    virtual void process_event(Event event) = 0;
    virtual void check_input() = 0;
    virtual void update(Time delta) = 0;
    virtual void draw() = 0;
    StateQueue *state_stack;
    StateID current_state;
    State::Context context;
};

class StateQueue
{
public:
    void register_state(StateID id, State *state);
    StateID current_state_ID() const;
    State *current_state();
    void set_current_state(StateID id);
    void process_event(Event event);
    void check_input();
    void update(Time delta);
    void draw();
    ~StateQueue();

private:
    map<StateID, State *> states_;
    StateID current_state_;
};

class DesignState : public State
{
public:
    DesignState(StateQueue &st_stack, State::Context ctxt);
    void process_event(Event event) override;
    void check_input() override;
    void update(Time delta) override;
    void draw() override;

private:
    bool clicked_ = false;
    Sprite *dragging_ = nullptr;
    Sprite *pick_draggable(Vector2f pointer);
};

class PathState : public State
{
public:
    PathState(StateQueue &st_stack, State::Context ctxt);
    void process_event(Event event) override;
    void check_input() override;
    void update(Time delta) override;
    void draw() override;
};

class RunState : public State
{
public:
    RunState(StateQueue &st_stack, State::Context ctxt);
    void process_event(Event event) override;
    void check_input() override;
    void update(Time delta) override;
    void draw() override;
};

#endif
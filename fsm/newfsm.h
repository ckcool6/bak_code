#pragma once

#define __MAKE_ID__(n) EVENT##n
#define __MAKE_HANDLE__(state) state##_handle, state

typedef enum
{
    light_attack,
    heavy_attack,
    remote_attack,
    jump_attack,
    wait,
} State;

typedef enum
{
    EVENT1 = 1,
    EVENT2,
    EVENT3,
} Event;

typedef struct
{
    /* data */
    Event event;
    State curr_state;
    void (*event_handler)();
    State next_state;
} StateTable;

typedef struct
{
    /* data */
    StateTable *state_table;
    State current_state;
} FSM;

void light_attack_handle();
void heavy_attack_handle();
void remote_attack_handle();
void jump_attack_handle();
void wait_handle();

void init_fsm(FSM *_fsm);
void FSM_process(FSM *_fsm, int event);
void FSM_state_transfer(FSM *_fsm, int state);



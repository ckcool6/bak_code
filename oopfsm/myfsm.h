#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>


struct FSM;

typedef struct State {
  char *name;
  void (*handle)(struct FSM *, int, void **);
  struct State *next;
} State;

typedef struct FSM {
  State *fsm_base;
  State *fsm_current_state;
  char *fsm_cur_state_name;
  int event_id;
  void **event;
} FSM;

//
int fsm_init(FSM *_fsm);

int fsm_main(FSM *_fsm);

int fsm_next_state(FSM *_fsm);

int fsm_add(FSM *_fsm, char *state, void (*handle)(FSM *, int, void **));

int fsm_default(FSM *_fsm, void (*handle)(FSM *, int, void **));

int fsm_remove(FSM *_fsm, char *state);

int fsm_transfer_state(FSM *_fsm, char *state, int event_id, void **event);

void fsm_terminate(FSM *_fsm);

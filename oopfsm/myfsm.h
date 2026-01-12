#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <stdbool.h>

#define _FSM_SEND_MESSAGE_(method, ...) \
    fsm_##method(__VA_ARGS__)         


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
int fsm_init(FSM *this);

int fsm_start(FSM *this);

bool fsm_next_state(FSM *this);

int fsm_add(FSM *this, char *state, void (*handle)(FSM *, int, void **));

int fsm_default(FSM *this, void (*handle)(FSM *, int, void **));

bool fsm_remove(FSM *this, char *state);

bool fsm_transfer_state(FSM *this, char *state, int event_id, void **event);

void fsm_terminate(FSM *this);

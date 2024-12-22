#include "newfsm.h"
#include <stdbool.h>
#include <stdio.h>
#include <windows.h>


StateTable monster[] = {
    {__MAKE_ID__(1), wait, __MAKE_HANDLE__(light_attack)},
    {__MAKE_ID__(2), light_attack, __MAKE_HANDLE__(heavy_attack)},
    {__MAKE_ID__(3), heavy_attack, __MAKE_HANDLE__(remote_attack)},
    {__MAKE_ID__(4), remote_attack, __MAKE_HANDLE__(jump_attack)},
    {__MAKE_ID__(5), jump_attack, __MAKE_HANDLE__(wait)},
};

int g_max_num;

int main(int argc, char const *argv[]) {
  /* init fsm */
  FSM fsm;
  init_fsm(&fsm);

  // init event
  int event = EVENT1;

  //
  while (true) {
    printf("event %d has happened: \n", event);
    FSM_process(&fsm, event);
    printf("current state is %d\n", fsm.current_state);

    printf("\n");
    printf("\n");

    if (event == 5) {
      event = 1;
    } else {
      event++;
    }

    Sleep(2000);
  }

  return 0;
}

void init_fsm(FSM *_fsm) {
  g_max_num = sizeof(monster) / sizeof(StateTable); // get numbers of member
  _fsm->current_state = wait;
  _fsm->state_table = monster;
}

void FSM_process(FSM *_fsm, int event) {
  StateTable *handle_table = _fsm->state_table;
  void (*event_handle)() = NULL;
  int next_state;
  int cur_state = _fsm->current_state;

  for (int i = 0; i < g_max_num; i++) {
    if (event == handle_table[i].event &&
        cur_state == handle_table[i].curr_state) {
      event_handle = handle_table[i].event_handler;
      next_state = handle_table[i].next_state;
      if (event_handle) {
        /* code */
        event_handle();
      }
      //
      FSM_state_transfer(_fsm, next_state);
    } else {
      // nothing to do
    }
  }
}

void FSM_state_transfer(FSM *_fsm, int state) { _fsm->current_state = state; }

void light_attack_handle() { printf("========lose little heath======\n"); }

void heavy_attack_handle() { printf("========lost many of heath=======\n"); }

void remote_attack_handle() { printf("=========fell bad========\n"); }

void jump_attack_handle() { printf("========fell angry=========\n"); }

void wait_handle() { printf("========goto bed=======\n"); }

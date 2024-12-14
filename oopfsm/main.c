#include "myfsm.h"

void state1(FSM *_fsm, int event_id, void **event) {
  printf("%d\n", event_id);
  printf("%s\n", _fsm->fsm_cur_state_name);
  fsm_transfer_state(_fsm, "eat", 1, NULL);
}

void state2(FSM *_fsm, int event_id, void **event) {
  printf("%d\n", event_id);
  printf("%s\n", _fsm->fsm_cur_state_name);
  fsm_transfer_state(_fsm, "default", 1, NULL);
}

void state3(FSM *_fsm, int event_id, void **event) {
  printf("%d\n", event_id);
  printf("%s\n", _fsm->fsm_cur_state_name);
  fsm_transfer_state(_fsm, "walk", 1, NULL);
}

int main() {
  FSM _fsm;
  fsm_init(&_fsm);

  fsm_default(&_fsm, state1); // 设置默认状态

  fsm_add(&_fsm, "walk", state2);
  fsm_add(&_fsm, "eat", state3);

  fsm_main(&_fsm);
  return 0;
}

int fsm_main(FSM *obj) {
  while (!fsm_next_state(obj))
    ;
  return 0;
}
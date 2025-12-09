#include "myfsm.h"

int g_num = 1;

int Event1(int n)
{
  return g_num = n * 1;
}

int Event2(int n)
{
  return g_num = n * 2;
}

int Event3(int n)
{
  return g_num = n * 3;
}

void event_handle_1(FSM *this, int event_id, void **event)
{
  Event1(2);
  printf("id is %d\n", event_id);
  printf("state is %s\n", this->fsm_cur_state_name);
  printf("g_num is %d\n", g_num);
  printf("--------------\n");
  fsm_transfer_state(this, "light_attack", 1, (void *)Event1);
}

void event_handle_2(FSM *this, int event_id, void **event)
{
  Event3(2);
  printf("id is %d\n", event_id);
  printf("state is %s\n", this->fsm_cur_state_name);
  printf("g_num is %d\n", g_num);
  printf("--------------\n");
  fsm_transfer_state(this, "remote_attack", 2, (void*)Event3);
}

void event_handle_3(FSM *this, int event_id, void **event)
{
  Event2(2);
  printf("id is %d\n", event_id);
  printf("state is %s\n", this->fsm_cur_state_name);
  printf("g_num is %d\n", g_num);
  printf("--------------\n");
  fsm_transfer_state(this, "default", 3, (void*)Event2);
}


int main()
{
  FSM this;
  fsm_init(&this);

  fsm_default(&this, event_handle_1); // 设置默认状态

  fsm_add(&this, "remote_attack", event_handle_3);
  fsm_add(&this, "light_attack", event_handle_2);

  fsm_main(&this);
  return 0;
}

int fsm_main(FSM *obj)
{
  while (!fsm_next_state(obj))
    ;
  return 0;
}
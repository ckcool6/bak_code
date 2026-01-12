#include "myfsm.h"

int fsm_init(FSM *this)
{
  this->fsm_base = NULL;
  this->fsm_cur_state_name = NULL;
  this->event_id = 0;
  this->event = NULL;
  return 0;
}


int fsm_default(FSM *this,
                void (*func)(FSM *this, int event_id, void **event))
{

  this->fsm_base = malloc(sizeof(State));
  this->fsm_base->name = "default";
  this->fsm_base->handle = func;
  this->fsm_base->next = NULL;

  //
  this->fsm_current_state = this->fsm_base;
  this->fsm_cur_state_name = this->fsm_base->name;

  return 0;
}


bool fsm_transfer_state(FSM *this, char *state, int event_id, void **event)
{
  State *tmp = this->fsm_base;

  while ((tmp != NULL) && (strcmp(tmp->name, state)))
  {
    tmp = tmp->next;
  }

  if (tmp != NULL)
  {
    this->fsm_current_state = tmp;
    this->fsm_cur_state_name = tmp->name;
    this->event_id = event_id;
    this->event = event;
  }
  else
  {
    return false;
  }

  return true;
}


int fsm_add(FSM *this, char *state, void (*func)(FSM *, int, void **))
{
  State *tmp = this->fsm_base;
  State *new_state = malloc(sizeof(State));
  // 走到最后
  while (tmp->next)
  {
    tmp = tmp->next;
  }
  // 后插
  new_state->name = state;
  new_state->handle = func;
  new_state->next = NULL;
  tmp->next = new_state;
  return 0;
}


bool fsm_next_state(FSM *this)
{
  State *tmp = this->fsm_base;
  if ((this->fsm_base != NULL) || (this->fsm_current_state != NULL))
  {
    while ((tmp->name != this->fsm_cur_state_name) && (tmp != NULL))
    {
      tmp = tmp->next;
    }
  }
  else
  {
    return false;
  }

  Sleep(2000);

  if (tmp != NULL)
  {
    tmp->handle(this, this->event_id, this->event);
  }
  else
  {
    return false;
  }

  return true;
}


bool fsm_remove(FSM *this, char *state)
{
  State *to_del;
  State *tmp = this->fsm_base;

  if (strcmp(state, "default"))
  {
    while ((tmp->next != NULL) && (strcmp(tmp->next->name, state)))
    {
      tmp = tmp->next;
    }
  }
  else
  {
    return false;
  }

  if (tmp != NULL)
  {
    to_del = tmp->next;
    tmp->next = tmp->next->next;
    free(to_del);

    return true;
  }
  else
  {
    return false;
  }
}


void fsm_terminate(FSM *obj)
{
  // delete all states to prevent memory leek
  State *tmp = obj->fsm_base;
  State *to_del = tmp;
  while (tmp)
  {
    to_del = tmp;
    tmp = tmp->next;
    free(to_del);
  }
  // reset FSM base to NULL causes while loop in fsm_main to quit
  // terminating the program
  obj->fsm_current_state = NULL;
  obj->fsm_cur_state_name = NULL;
  obj->fsm_base = NULL;
}

int fsm_start(FSM *obj)
{
  while (fsm_next_state(obj))
    ;
  return 0;
}
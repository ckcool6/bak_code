#include "myfsm.h"

/**
 * @brief
 * 初始化状态机
 * @param this
 * @return int
 */
int fsm_init(FSM *this)
{
  this->fsm_base = NULL;
  this->fsm_cur_state_name = NULL;
  this->event_id = 0;
  this->event = NULL;
  return 0;
}

/**
 * @brief
 * 设置默认状态
 * @param this
 * @param func
 * @return int
 */
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

/**
 * @brief
 * 转换状态
 * @param this
 * @param state
 * @param event_id
 * @param event
 * @return int
 */
int fsm_transfer_state(FSM *this, char *state, int event_id, void **event)
{
  State *tmp = this->fsm_base;
  while ((tmp != NULL) && (strcmp(tmp->name, state)))
  {
    tmp = tmp->next;
  }
  if (tmp == NULL)
  {
    return -1;
  }
  this->fsm_current_state = tmp;
  this->fsm_cur_state_name = tmp->name;
  this->event_id = event_id;
  this->event = event;

  return 0;
}

/**
 * @brief
 * 添加新状态
 * @param this
 * @param state
 * @param handle
 * @return int
 */
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

/**
 * @brief
 * 执行下一个状态
 * @param this
 * @return int
 */
int fsm_next_state(FSM *this)
{
  State *tmp = this->fsm_base;
  if ((this->fsm_base == NULL) || (this->fsm_current_state == NULL))
  {
    return -1;
  }

  while ((tmp->name != this->fsm_cur_state_name) && (tmp != NULL))
  {
    tmp = tmp->next;
  }
  Sleep(2000);
  if (tmp == NULL)
  {
    return -1;
  }
  tmp->handle(this, this->event_id, this->event);
  return 0;
}

/**
 * @brief
 * 删除状态
 * @param this
 * @param state
 * @return int
 */
int fsm_remove(FSM *this, char *state)
{
  if (!strcmp(state, "default"))
  {
    return -1;
  }
  State *to_del;
  State *tmp = this->fsm_base;
  while ((tmp->next != NULL) && (strcmp(tmp->next->name, state)))
    tmp = tmp->next;
  if (tmp == NULL)
    return -1;
  to_del = tmp->next;
  tmp->next = tmp->next->next;
  free(to_del);
  return 0;
}

/**
 * @brief
 * 终止状态机
 * @param obj
 */
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
  while (!fsm_next_state(obj))
    ;
  return 0;
}
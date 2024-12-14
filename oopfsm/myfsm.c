#include "myfsm.h"

/**
 * @brief
 * 初始化状态机
 * @param _fsm
 * @return int
 */
int fsm_init(FSM *_fsm) {
  _fsm->fsm_base = NULL;
  _fsm->fsm_cur_state_name = NULL;
  _fsm->event_id = 0;
  _fsm->event = NULL;
  return 0;
}

/**
 * @brief
 * 设置默认状态
 * @param _fsm
 * @param func
 * @return int
 */
int fsm_default(FSM *_fsm,
                void (*func)(FSM *_fsm, int event_id, void **event)) {

  _fsm->fsm_base = malloc(sizeof(State));
  _fsm->fsm_base->name = "default";
  _fsm->fsm_base->handle = func;
  _fsm->fsm_base->next = NULL;

  //
  _fsm->fsm_current_state = _fsm->fsm_base;
  _fsm->fsm_cur_state_name = _fsm->fsm_base->name;

  return 0;
}

/**
 * @brief
 * 转换状态
 * @param _fsm
 * @param state
 * @param event_id
 * @param event
 * @return int
 */
int fsm_transfer_state(FSM *_fsm, char *state, int event_id, void **event) {
  State *tmp = _fsm->fsm_base;
  while ((tmp != NULL) && (strcmp(tmp->name, state))) {
    tmp = tmp->next;
  }
  if (tmp == NULL) {
    return -1;
  }
  _fsm->fsm_current_state = tmp;
  _fsm->fsm_cur_state_name = tmp->name;
  _fsm->event_id = event_id;
  _fsm->event = event;

  return 0;
}

/**
 * @brief
 * 添加新状态
 * @param _fsm
 * @param state
 * @param handle
 * @return int
 */
int fsm_add(FSM *_fsm, char *state, void (*func)(FSM *, int, void **)) {
  State *tmp = _fsm->fsm_base;
  State *new_state = malloc(sizeof(State));
  // 走到最后
  while (tmp->next) {
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
 * @param _fsm
 * @return int
 */
int fsm_next_state(FSM *_fsm) {
  State *tmp = _fsm->fsm_base;
  if ((_fsm->fsm_base == NULL) || (_fsm->fsm_current_state == NULL)) {
    return -1;
  }

  while ((tmp->name != _fsm->fsm_cur_state_name) && (tmp != NULL)) {
    tmp = tmp->next;
  }
  Sleep(2000);
  if (tmp == NULL) {
    return -1;
  }
  tmp->handle(_fsm, _fsm->event_id, _fsm->event);
  return 0;
}

/**
 * @brief
 * 删除状态
 * @param _fsm
 * @param state
 * @return int
 */
int fsm_remove(FSM *_fsm, char *state) {
  if (!strcmp(state, "default")) {
    return -1;
  }
  State *to_del;
  State *tmp = _fsm->fsm_base;
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
void fsm_terminate(FSM *obj) {
  // delete all states to prevent memory leek
  State *tmp = obj->fsm_base;
  State *to_del = tmp;
  while (tmp) {
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
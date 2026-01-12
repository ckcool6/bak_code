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

/**
 * @brief 初始化状态机
 * 
 * @param this
 * @return int
 */
int fsm_init(FSM *this);

/**
 * @brief 执行下一个状态
 * 
 * @param this
 * @return int
 */
bool fsm_next_state(FSM *this);

/**
 * @brief 添加新状态
 * 
 * @param this
 * @param state
 * @param handle
 * @return int
 */
int fsm_add(FSM *this, char *state, void (*handle)(FSM *, int, void **));

/**
 * @brief 设置默认状态
 * 
 * @param this
 * @param func
 * @return int
 */
int fsm_default(FSM *this, void (*handle)(FSM *, int, void **));

/**
 * @brief 删除状态
 * 
 * @param this
 * @param state
 * @return int
 */
bool fsm_remove(FSM *this, char *state);

/**
 * @brief 切换状态
 * 
 * @param this
 * @param state
 * @param event_id
 * @param event
 * @return int
 */
bool fsm_transfer_state(FSM *this, char *state, int event_id, void **event);

/**
 * @brief 终止状态机
 * 
 * @param obj
 */
void fsm_terminate(FSM *this);

/**
 * @brief 启动状态机
 * 
 * @param this 
 * @return int 
 */
int fsm_start(FSM *this);
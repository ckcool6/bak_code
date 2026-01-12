#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <stdbool.h>

#define _FSM_SEND_MESSAGE_(method, ...) \
  fsm_##method(__VA_ARGS__)

struct FSM;

typedef struct State
{
  char *name;
  void (*handle)(struct FSM *, int, void **);
  struct State *next;
} State;

typedef struct FSM
{
  State *fsm_base;
  State *fsm_current_state;
  char *fsm_cur_state_name;
  int event_id;
  void **event;
} FSM;

/**
 * 初始化状态机
 */
int fsm_init(FSM *);

/**
 * 执行下一个状态
 */
bool fsm_next_state(FSM *);

/**
 * 添加新状态
 */
int fsm_add(FSM *, char *, void (*handle)(FSM *, int, void **));

/**
 * 设置默认状态
 */
int fsm_default(FSM *, void (*handle)(FSM *, int, void **));

/**
 * 删除状态
 */
bool fsm_remove(FSM *, char *);

/**
 * 切换状态
 */
bool fsm_transfer_state(FSM *, char *, int, void **);

/**
 * 终止状态机
 */
void fsm_terminate(FSM *);

/**
 * 启动状态机
 */
int fsm_start(FSM *);
#include <setjmp.h>
#include <stdbool.h>
#include <stdio.h>
#include <time.h>

#define YIELD(func, n)                                                         \
  if (!setjmp(func##_gen_jmp)) {                                               \
    func##_ret = n;                                                            \
    longjmp(func##_caller_jmp, 1);                                             \
  }

#define GENERATOR(ret, func, argt, argv)                                       \
  static jmp_buf func##_caller_jmp;                                            \
  static jmp_buf func##_gen_jmp;                                               \
  static bool func##_continue = false;                                         \
  static ret func##_ret;                                                       \
                                                                               \
  void func##__real(argt argv);                                                \
                                                                               \
  ret func(argt argv) {                                                        \
    if (!func##_continue) {                                                    \
      func##_continue = true;                                                  \
      if (!setjmp(func##_caller_jmp)) {                                        \
        func##__real(argv);                                                    \
      } else {                                                                 \
        return func##_ret;                                                     \
      }                                                                        \
    } else {                                                                   \
      longjmp(func##_gen_jmp, 1);                                              \
    }                                                                          \
    return 0;                                                                  \
  }                                                                            \
  void func##__real(argt argv)

GENERATOR(int, getNext, int, n) {
  static int counter;

  counter = n;
  while (true) {
    counter = counter + 1;
    YIELD(getNext, counter);
  }
}

GENERATOR(int, pow_two, int, n) {
  static int val;

  val = n;
  while (true) {
    val = val * 2;
    YIELD(pow_two, val);
  }
}

GENERATOR(int, fib, int, n) {
  static int val;

  static int a = 0;
  static int b = 1;
  val = n;
  while (true) {
    YIELD(fib, val);
    val = a + b;
    a = b;
    b = val;
  }
}

// map impl
typedef int (*Gen)(int);
typedef int *(*func)(int);

int map(func func, Gen gen_func, int n) {
  int res = *func(gen_func(n));
  return res;
}

int *times2(int n) {
  int *res = NULL;
  n = n * 2;
  res = &n;
  return res;
}

int main() {
  // map (x2) 整数流
  for (int i = 0; i < 10; i++) {
    int res = map(&times2, &getNext, i);
    printf("%d, ", res);
  }
  printf("\n");

  // 2的幂流
  for (size_t i = 0; i < 10; i++) {
    printf("%d, ", pow_two(1));
  }
  printf("\n");

  // fibnacci流
  for (size_t i = 0; i < 10; i++) {
    printf("%d, ", fib(1));
  }
  printf("\n");
}

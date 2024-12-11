#ifndef MPFS_TEST_H
#define MPFS_TEST_H

#define TEST_PATH "/mnt/MPFS/append_test.txt"
#define TIMER_START(INDEX)                                                     \
  struct timespec tv_start##INDEX, tv_end##INDEX;                              \
  uint64_t time_start##INDEX, time_end##INDEX;                                 \
  clock_gettime(CLOCK_REALTIME, &tv_start##INDEX)
#define TIMER_END(INDEX)
#define TIMER_PRINT(INDEX, MSG)                                                \
  clock_gettime(CLOCK_REALTIME, &tv_end##INDEX);                               \
  time_start##INDEX = (uint64_t)tv_start##INDEX.tv_sec * 1000000000UL +        \
                      (uint64_t)tv_start##INDEX.tv_nsec;                       \
  time_end##INDEX = (uint64_t)tv_end##INDEX.tv_sec * 1000000000UL +            \
                    (uint64_t)tv_end##INDEX.tv_nsec;                           \
  printf(MSG " time: %lu ns\n", time_end##INDEX - time_start##INDEX)

char data[] =
  "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"
  "ucKs55VFkAp3DsJMLrcpWsqU\n";

#endif
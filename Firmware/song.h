#pragma once
#include <Arduino.h>

struct Note {
  int8_t  s1;     
  int8_t  s2;     
  uint8_t units;
  char    dir;
  uint8_t bar;
};

const Note PART[] PROGMEM = {
  // bar 1
  {4, 5, 1, 'D', 1}, {7, 5, 1, 'U', 1}, {7, 5, 1, 'D', 1}, {7, 5, 1, 'U', 1},
  // bar 2
  {7, 5, 1, 'D', 2}, {4, 5, 1, 'U', 2}, {5, 5, 2, 'D', 2},
  // bar 3
  {5, 5, 1, 'D', 3}, {9, 7, 1, 'U', 3}, {9, 7, 1, 'D', 3}, {9, 7, 1, 'U', 3},
  // bar 4
  {9, 7, 1, 'D', 4}, {5, 7, 1, 'U', 4}, {7, 7, 2, 'D', 4},
  // bar 5
  {4, 5, 1, 'D', 5}, {7, 5, 1, 'U', 5}, {7, 5, 1, 'D', 5}, {7, 5, 1, 'U', 5},
  // bar 6
  {7, 5, 1, 'D', 6}, {4, 5, 1, 'U', 6}, {5, 5, 2, 'D', 6},
  // bar 7
  {5, 5, 1, 'D', 7}, {7, 5, 1, 'U', 7}, {4, 5, 1, 'D', 7}, {4, 5, 1, 'U', 7},
  // bar 8
  {2, 0, 1, 'D', 8}, {2, 0, 1, 'U', 8}, {0, 5, 2, 'D', 8},
};

const uint16_t PART_LEN        = sizeof(PART) / sizeof(PART[0]);   
const uint8_t  PART_BARS       = 8;
const uint8_t  SECTION_REPEATS = 2;                                

inline uint16_t songLength() { return PART_LEN * SECTION_REPEATS; }

inline Note noteAt(uint16_t i) {
  Note n;
  memcpy_P(&n, &PART[i % PART_LEN], sizeof(Note));
  n.bar += (i / PART_LEN) * PART_BARS;   
  return n;
}

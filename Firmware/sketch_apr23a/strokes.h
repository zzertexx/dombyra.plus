#ifndef STROKES_H
#define STROKES_H

struct Stroke {
  int upper;
  int bottom;
  bool isUp;
};

struct Kui {
  Stroke strokes[300];
  int totalStrokes;
  char name[20];
};

Kui kuis[] = {
  {
    {5, 4, false}, {5, 7, true}, {5, 7, false}, {5, 7, true}, {5, 7, false}, {5, 4, true}, {5, 5, false}, {5, 9, false}, {7, 9, true}, {7, 9, false}, {7, 9, true}, {7, 5, false},
    {7, 7, true}, {0, 2, false}, {0, 2, true}, {5, 0, false}, {5, 4, false}, {5, 7, true}, {5, 7, false}, {5, 7, true}, {5, 7, false}, {5, 4, true}, {5, 5, false}, {5, 7, false}, 
    {5, 4, true}, {5, 4, false}, {5, 4, true}, {5, 9, false}, {7, 9, true}, {7, 9, false}, {7, 9, true}, {7, 5, false}, {7, 7, true}, {0, 2, false}, {0, 2, true}, {5, 0, false},
    36
    "Erkem Ai"
  },
  { 
    {5, 4, false}, {5, 4, false}, {5, 4, false}, {5, 4, true},
    {5, 4, false}, {5, 4, false}, {5, 4, false}, {0, 2, true},
    {5, 4, false}, {5, 5, true}, {5, 5, true}, {5, 5, false}, {5, 5, true},
    {5, 5, false}, {5, 5, true}, {5, 5, false}, {0, 2, true},
    {5, 4, false}, {5, 4, false}, {5, 4, false}, {5, 4, true},
    {5, 4, false}, {5, 4, false}, {5, 4, false}, {0, 2, true},
    {5, 4, false}, {5, 5, true}, {5, 5, true}, {5, 7, false}, {5, 7, true},
    {5, 7, false}, {5, 7, true}, {5, 7, false},
    {0, 12, false}, {0, 12, true}, {0, 12, false}, {0, 12, true},
    {0, 11, false}, {0, 12, true}, {0, 14, true}, {0, 11, false}, {0, 11, true},
    {0, 9, false}, {0, 11, true}, {0, 12, true}, {0, 9, false}, {0, 9, true},
    {0, 7, false}, {0, 7, true}, {0, 7, false},
    {0, 12, false}, {0, 12, true}, {0, 12, false}, {0, 12, true},
    {0, 11, false}, {0, 12, true}, {0, 14, true}, {0, 11, false}, {0, 11, true},
    {0, 9, false}, {0, 11, true}, {0, 12, true}, {0, 9, false}, {0, 9, true},
    {0, 7, false}, {0, 7, true}, {0, 7, false},
    {5, 4, false}, {5, 4, false}, {5, 4, false}, {0, 0, true},
    {0, 2, false}, {0, 2, false}, {0, 2, false}, {5, 4, true},
    {5, 7, false}, {5, 7, true}, {5, 7, true}, {5, 7, false}, {7, 9, true}, {9, 11, true},
    {10, 12, false}, {10, 12, false}, {10, 12, false},
    {5, 4, false}, {5, 4, false}, {5, 4, false}, {0, 0, true},
    {0, 2, false}, {0, 2, false}, {0, 2, false}, {5, 4, true},
    {5, 7, false}, {5, 7, true}, {5, 7, true}, {5, 7, false}, {5, 4, true}, {0, 2, true},
    {0, 0, false}, {0, 0, false}, {0, 0, false},
    {5, 4, false}, {5, 4, false}, {5, 4, false}, {0, 0, true},
    {0, 2, false}, {0, 2, false}, {0, 2, false}, {5, 4, true},
    {5, 7, false}, {5, 7, true}, {5, 7, true}, {5, 7, false}, {7, 9, true}, {9, 11, true},
    {10, 12, false}, {10, 12, false}, {10, 12, false},
    {5, 4, false}, {5, 4, false}, {5, 4, false}, {0, 0, true},
    {0, 2, false}, {0, 2, false}, {0, 2, false}, {5, 4, true},
    {5, 7, false}, {5, 7, true}, {5, 7, true}, {5, 7, false}, {5, 4, true}, {0, 2, true},
    {0, 0, false}, {0, 0, false}, {0, 0, false},
    {0, 12, false}, {0, 14, true}, {0, 16, false}, {0, 16, true},
    {0, 16, false}, {0, 16, true}, {0, 16, false}, {0, 16, true},
    {0, 14, false}, {0, 16, true}, {0, 17, true}, {0, 16, false}, {0, 17, true}, {0, 16, true},
    {0, 14, false}, {0, 14, false}, {0, 14, false}, {0, 14, true},
    {0, 12, false}, {0, 14, true}, {0, 16, true}, {0, 14, false}, {0, 16, true}, {0, 14, true},
    {0, 12, false}, {0, 12, false}, {0, 12, false}, {0, 0, true},
    {0, 12, false}, {0, 14, true}, {0, 16, false}, {0, 16, true},
    {0, 16, false}, {0, 16, true}, {0, 16, false}, {0, 16, true},
    {0, 14, false}, {0, 16, true}, {0, 17, true}, {0, 16, false}, {0, 17, true}, {0, 16, true},
    {0, 14, false}, {0, 14, false}, {0, 14, false}, {0, 14, true},
    {0, 12, false}, {0, 14, true}, {0, 16, true}, {0, 14, false}, {0, 16, true}, {0, 14, true},
    {0, 12, false}, {0, 12, false}, {0, 12, false}, {0, 12, true},
    {0, 11, false}, {0, 12, true}, {0, 14, true}, {0, 12, false}, {0, 14, true}, {0, 12, true},
    {0, 11, false}, {0, 11, false}, {0, 11, false}, {0, 11, true},
    {0, 9, false}, {0, 9, false}, {0, 14, false}, {0, 14, true},
    {0, 7, false}, {0, 7, false}, {0, 7, false},
    {5, 4, false}, {5, 4, false}, {5, 4, false}, {0, 0, true},
    {0, 2, false}, {0, 2, false}, {0, 2, false}, {5, 4, true},
    {5, 7, false}, {5, 7, true}, {5, 7, true}, {5, 7, false}, {7, 9, true}, {9, 11, true},
    {10, 12, false}, {10, 12, false}, {10, 12, false},
    {5, 4, false}, {5, 4, false}, {5, 4, false}, {0, 0, true},
    {0, 2, false}, {0, 2, false}, {0, 2, false}, {5, 4, true},
    {5, 7, false}, {5, 7, true}, {5, 7, true}, {5, 7, false}, {5, 4, true}, {0, 2, true},
    {0, 0, false}, {0, 0, false}, {0, 0, false},
    {5, 4, false}, {5, 4, false}, {5, 4, false}, {0, 0, true},
    {0, 2, false}, {0, 2, false}, {0, 2, false}, {5, 4, true},
    {5, 7, false}, {5, 7, true}, {5, 7, true}, {5, 7, false}, {7, 9, true}, {9, 11, true},
    {10, 12, false}, {10, 12, false}, {10, 12, false},
    {5, 4, false}, {5, 4, false}, {5, 4, false}, {0, 0, true},
    {0, 2, false}, {0, 2, false}, {0, 2, false}, {5, 4, true},
    {5, 7, false}, {5, 7, true}, {5, 7, true}, {5, 7, false}, {5, 4, true}, {0, 2, true},
    {0, 0, false}, {0, 0, false}, {0, 0, false},
    276
    "Kelinshek"
  },
};

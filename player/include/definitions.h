#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#include <algorithm>
#include <array>
#include <cstdint>
#include <iostream>
#include <stdint.h>
#include <stdlib.h>
#include <vector>

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <sys/sem.h>
#include <errno.h>

#define KEY_PATH "/mnt/e/UnB/SO/Trabalho 2/parallel-programming-game/board/src/game.cpp"
#define KEY_ID 2807
#define BOARD_SIZE 8

typedef u_int8_t UINT8;
typedef std::vector<std::pair<int, int>> COORD_VECTOR;

#endif
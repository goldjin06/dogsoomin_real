#ifndef DEFINE_H_INCLUDED
#define DEFINE_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <time.h>
#include "Start.h"
#include "name.h"
#include "title.h"
//#include "ImageLayer.h"

struct information {
    char name[15];
    char gender;
    char difficultyInformation;
	int nowDifficulty; // 0 : easy, 1 : normal, 2 : hard
};

#define MAX_X 135
//console width
#define MAX_Y 40
//console height

HWND WINDOW_HANDLE;

#define INACTIVE_BLOCK 3

#define MAP_WIDTH 11
#define MAP_HEIGHT 23

#define MARGIN_X 2
#define MARGIN_Y 2

#define LEFT 75
#define RIGHT 77
#define DOWN 80
#define UP 72
#define SPACE 32
#define ENTER 13

#define WINDOW 119
#define PERFUME 112
#define LOCKDOOR 100
#define FANBUTTON 102

#define EAT 101
#define HIDE 104


#endif // DEFINE_H_INCLUDED

#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include <conio.h>

#define GAME_X 12
#define GAME_Y 22
#define MAX_HEIGHT 16
#define GAME_POS_X 0
#define GAME_POS_Y 0
#define NEXT_POS_X 14
#define NEXT_POS_Y 3
#define SCORE_POS_X 15
#define SCORE_POS_Y 14
#define COMBO_POS_X 15
#define COMBO_POS_Y 16

#define LEFT 97
#define RIGHT 100
#define DOWN 115
#define ROTATION 119
#define ESC 27
#define SPACE_BAR 32

#define EMPTY 0
#define BLOCK_ACT 1
#define BLOCK_INA 2
#define LINE 3
#define WALL 4

extern game_table[GAME_Y][GAME_X];
extern game_table_copy[GAME_Y][GAME_X];
extern blocks[7][4][4][4];
extern new_block_flag;
extern current_block;
extern next_block;
extern game_running;
extern b_pos_x;
extern b_pos_y;
extern b_rotation;
extern game_speed;
extern loop;
extern key;
extern score;
extern sysloop;
extern paused;
extern esc_paused;
extern floop;
extern gameover_flag;

void gotoxy(int, int);
void init(void);
void reset_table(void);
void draw_table(void);
void new_block(void);
void draw_next_block(void);
void draw_ui(void);
void place_block(void);
void block_down(void);
void block_stack(void);
void check_gameover(void);
void draw_gameover(void);
void move_block(int);
int check_crash(int);
void check_key(void);
void rotate_block(void);
void erase_block(void);
void check_line(void);
void draw_score(void);
void draw_combo(int, int);
void remove_blank(void);
void drop_blocks(int);
void pause_game(void);
void escape_game(void);
void hard_drop(void);
void draw_title(void);

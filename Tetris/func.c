#include "tetris.h"




void init(void) // 초기화
{
    system("mode con cols=46 lines=22 | title TETRIS"); // 콘솔크기및 창 이름

    HANDLE console_handle = GetStdHandle(STD_OUTPUT_HANDLE); // 커서 숨김
    CONSOLE_CURSOR_INFO console_cursor;
    console_cursor.bVisible = 0;
    console_cursor.dwSize = 1;
    SetConsoleCursorInfo(console_handle, &console_cursor);

    srand((unsigned)time(NULL)); // 난수표 생성

    next_block = rand() % 7;
    new_block_flag = 1;
    game_running = 0;
}

void gotoxy(int x, int y) // 콘솔 XY위치로 이동
{
    COORD Pos;
    Pos.X = x*2;
    Pos.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Pos);
}

void draw_title(void)
{
    system("cls");

    gotoxy(1,3); printf("■■■  ■■■  ■■■  ■■■  ■  ■■■"); // 테트리스 문자 출력
    gotoxy(1,4); printf("  ■    ■        ■    ■  ■  ■  ■");
    gotoxy(1,5); printf("  ■    ■■■    ■    ■■    ■  ■■■");
    gotoxy(1,6); printf("  ■    ■        ■    ■  ■  ■      ■");
    gotoxy(1,7); printf("  ■    ■■■    ■    ■  ■  ■  ■■■");
    gotoxy(8,11); printf("Move:  W A S D");
    gotoxy(8,12); printf("Drop:  SpaceBar");
    gotoxy(8,13); printf("Pause: P");
    gotoxy(8,14); printf("Quit:  ESC");
    gotoxy(6,17); printf("Press SpaceBar to start");
}

void reset_table(void) // 게임판 초기화
{
    int i, j;

    for (i = 0; i < GAME_Y ; i++) // 게임판을 빈 공간으로 채움
    {
        for (j = 0; j < GAME_X ; j++)
        {
            game_table[i][j] = EMPTY;
            game_table_copy[i][j] = 10;
        }
    }

    for (i = 0; i < GAME_X ; i++) // 테두리에 벽 생성
    {
        game_table[0][i] = WALL;
        game_table[GAME_Y - 1][i] = WALL;
    }
    for (i = 0; i < GAME_Y ; i++)
    {
        game_table[i][GAME_X - 1] = WALL;
        game_table[i][0] = WALL;
    }
}

void draw_table(void) // 게임판 출력
{
    int i, j;

    for (j = 1; j < GAME_X - 1; j++)
    {
        if (game_table[GAME_Y - MAX_HEIGHT - 1][j] == EMPTY)
        {
            gotoxy(j,GAME_Y - MAX_HEIGHT - 1);
            printf("__");
        }
    }


    for (i = 0; i < GAME_Y; i++)
    {
        for (j = 0; j < GAME_X; j++)
        {
            if (game_table[i][j] != game_table_copy[i][j]) // 다른 블럭만 변경해서 출력하기
            {
                gotoxy(GAME_POS_X + j, GAME_POS_Y + i); // 해당 좌표로 이동
                switch(game_table[i][j]) // 해당하는 블럭 출력
                {
                case EMPTY:
                    printf("  ");
                    break;
                case BLOCK_ACT:
                    printf("□");
                    break;
                case BLOCK_INA:
                    printf("▣");
                    break;
                case LINE:
                    printf("_");
                    break;
                case WALL:
                    printf("■");
                    break;
                }

                game_table_copy[i][j] = game_table[i][j]; // 다 출력하면 복사해줌
            }
        }
    }


}

void new_block(void) // 다음  블럭 설정
{
    if (new_block_flag != 1)
        return;

    current_block = next_block;
    next_block = rand() % 7;

    b_pos_x = GAME_X / 2 - 2;
    b_pos_y = 1;
    b_rotation = 0;

    draw_next_block();

    loop = 0;
    new_block_flag = 0;
}

void draw_next_block(void) // 다음 블럭 출력
{
    int i, j;

    gotoxy(NEXT_POS_X, NEXT_POS_Y + 2); printf("            ");
    gotoxy(NEXT_POS_X, NEXT_POS_Y + 3); printf("            ");
    gotoxy(NEXT_POS_X, NEXT_POS_Y + 4); printf("            ");
    gotoxy(NEXT_POS_X, NEXT_POS_Y + 5); printf("            ");

    for (i = 0; i < 4; i++)
    {
        for (j = 0; j < 4; j++)
        {
            if (blocks[next_block][0][i][j] == BLOCK_ACT)
            {
                gotoxy(NEXT_POS_X + j, NEXT_POS_Y + 2 + i);
                printf("□");
            }
        }
    }
}

void draw_ui(void) // 다음블럭, 점수판 등 출력
{

    gotoxy(NEXT_POS_X + 1, NEXT_POS_Y); printf("NEXT");           // 다음 블럭

    gotoxy(SCORE_POS_X, SCORE_POS_Y); printf("SCORE :"); // 점수
}

void place_block(void) // 블럭을 게임판에 대입
{
    int i, j;

    for (i = 0; i < 4; i++)
    {
        for (j = 0; j < 4; j++)
        {
            if (blocks[current_block][b_rotation][i][j] == 1)
            {
                game_table[b_pos_y + i][b_pos_x + j] = BLOCK_ACT;
            }
        }
    }
}

void block_down(void) // 블럭 내려가게 만드는 함수
{
    int i, j;

    for (i = 0; i < 4; i++)
    {
        for (j = 0; j < 4; j++)
        {
            if (blocks[current_block][b_rotation][i][j] == 1)
            {
                if (check_crash(DOWN) == 1)
                {
                    block_stack(); // 바닥에 벽이 있을 경우 블럭 쌓임
                    new_block_flag = 1; // 새 블럭 요청
                    return;  // 함수 탈출
                }
            }
        }
    }

    erase_block();

    b_pos_y++; // 블럭의 충돌이 없을 경우
    for (i = 0; i < 4; i++) // 블럭 이동 후 위치 출력
    {
        for (j = 0; j < 4; j++)
        {
            if(blocks[current_block][b_rotation][i][j] == 1)
                game_table[i + b_pos_y][j + b_pos_x] = BLOCK_ACT;
        }
    }
}

void block_stack(void) // 블럭을 쌓이게 해주는 함수
{
    int i, j;
    for (i = 0; i < 4; i++)
    {
        for (j = 0; j < 4; j++)
        {
            if (blocks[current_block][b_rotation][i][j] == 1)
            {
                game_table[i + b_pos_y][j + b_pos_x] = BLOCK_INA;
            }
        }
    }
}

void check_gameover(void) // 게임오버 판정
{
    int i, j;

    for (i = 0; i < GAME_Y - MAX_HEIGHT; i++)
    {
        for (j = 0; j < GAME_X; j++)
        {
            if (game_table[i][j] == BLOCK_INA)
            {
                gameover_flag = 1;
            }
        }
    }

    if (gameover_flag == 1)
        draw_gameover();
}

void draw_gameover(void) // 게임오버 화면 출력
{
    game_running = 0;

    system("cls");
    gotoxy(11,9); printf("GAME OVER");
    gotoxy(11,11); printf("SCORE: %d", score);

    while(!kbhit())
        Sleep(33);

    while(kbhit())
        getch();

    draw_title();
    gameover_flag = 0;
}

void move_block(int dir) // 블럭 이동
{
    int i, j;

    switch(dir)
    {
    case LEFT:
        erase_block();
        b_pos_x--;
        place_block();
        break;

    case RIGHT:
        erase_block();
        b_pos_x++;
        place_block();
        break;

    case DOWN:
        erase_block();
        b_pos_y++;
        place_block();
        loop = 0;
        break;
    }
}

int check_crash(int dir) // 충돌 검사
{
    int i, j;
    int status = 0;
    int r_temp;

    switch(dir)
    {
        case LEFT:
            for (i = 0; i < 4; i++)
            {
                for (j = 0; j < 4; j++)
                {
                    if (blocks[current_block][b_rotation][i][j] == 1)
                    {
                        if (game_table[i + b_pos_y][j + b_pos_x - 1] == WALL || game_table[i + b_pos_y][j + b_pos_x - 1] == BLOCK_INA)
                        {
                            status = 1;
                        }
                    }
                }
            }
            break;

        case RIGHT:
            for (i = 0; i < 4; i++)
            {
                for (j = 0; j < 4; j++)
                {
                    if (blocks[current_block][b_rotation][i][j] == 1)
                    {
                        if (game_table[i + b_pos_y][j + b_pos_x + 1] == WALL || game_table[i + b_pos_y][j + b_pos_x + 1] == BLOCK_INA)
                        {
                            status = 1;
                        }
                    }
                }
            }
            break;

        case DOWN:
            for (i = 0; i < 4; i++)
            {
                for (j = 0; j < 4; j++)
                {
                    if (blocks[current_block][b_rotation][i][j] == 1)
                    {
                        if (game_table[i + b_pos_y + 1][j + b_pos_x] == WALL || game_table[i + b_pos_y + 1][j + b_pos_x] == BLOCK_INA)
                        {
                            status = 1;
                        }
                    }
                }
            }
            break;

        case ROTATION:
            if (b_rotation == 3) // 회전후의 값 임시 저장
                r_temp = 0;
            else
                r_temp = b_rotation + 1;

            for (i = 0; i < 4; i++)
            {
                for (j = 0; j < 4; j++)
                {
                    if (blocks[current_block][r_temp][i][j] == 1)
                    {
                        if (game_table[i + b_pos_y][j + b_pos_x] == WALL || game_table[i + b_pos_y][j + b_pos_x] == BLOCK_INA)
                            status = 1;
                    }
                }
            }

            break;
    }

    return status;
}

void check_key(void) // 키입력 받고 커맨드 수행
{
    if (kbhit())
    {
        key = getch();

        switch (key)
        {
        case LEFT:
            if (check_crash(LEFT) == 0)
                move_block(LEFT);
            break;
        case RIGHT:
            if (check_crash(RIGHT) == 0)
                move_block(RIGHT);
            break;
        case DOWN:
            if (check_crash(DOWN) == 0)
                move_block(DOWN);
            else
            {
                block_stack();
                new_block_flag = 1;
            }
            break;
        case ROTATION:
            if (check_crash(ROTATION) == 0)
                rotate_block();
            break;

        case 121: // ESC 누른 후 y를 눌렀을 때 종료
            if (esc_paused == 1)
            {
                if(game_running == 1)
                {
                    game_running = 0;
                    draw_title();
                }
                else
                    sysloop = 0;
                floop = 0;
                esc_paused = 0;
            }
            break;

        case 110: // ESC를 누른 후 n을 눌렀을 때 복귀
            if (esc_paused == 1)
            {
                unpause();
                esc_paused = 0;
            }
            break;

        case SPACE_BAR:
            if (game_running == 0)
                game_running = 1;
            else
                hard_drop();
            break;

        case ESC:
            escape_game();
            break;

        case 112:
            if (game_running == 1)
            {
                if (paused == 1)
                {
                    unpause();
                    paused = 0;
                }
                else
                    pause_game();
            }
            break;


        }
    }

}

void rotate_block(void) // 블럭 회전
{
    int i, j;

    erase_block();

    if (b_rotation == 3)
        b_rotation = 0;
    else
        b_rotation++;

    place_block();
}

void erase_block(void) // 대입한 블럭 제거
{
    int i, j;

    for (i = 0; i < 4; i++)
    {
        for (j = 0; j < 4; j++)
        {
            if (blocks[current_block][b_rotation][i][j])
            {
                game_table[i + b_pos_y][j + b_pos_x] = EMPTY;
            }
        }
    }
}

void check_line(void) // 라인 확인, 제거, 점수 획득
{
    int i, j, k, l;
    int blocks;
    int lines = 0;
    int income = 0;

    for (i = 0; i < GAME_Y; i++) // 조건을 만족하는 총 라인 갯수 계산
    {
        blocks = 0;
        for(j = 0; j < GAME_X; j++)
        {
            if (game_table[i][j] == BLOCK_INA)
            {
                blocks++;
            }
        }
        if (blocks == GAME_X - 2)
        {
            lines++;
            for (j = 1; j < GAME_X - 1; j++) // 라인달성시 그 줄 제거
            {
                game_table[i][j] = EMPTY;
                draw_table();
            }
        }
    }

    if (lines >= 1)
    {
        income = lines * (GAME_X - 2) * lines * 10; // 총 블럭 갯수 * 10 * 콤보
        score += income;

        draw_combo(lines, income); // 콤보 출력
    }
}

void draw_score(void) // 점수 출력
{
    gotoxy(SCORE_POS_X + 4, SCORE_POS_Y); printf("%d", score);
}

void draw_combo(int combo, int income) // 콤보 출력
{
    gotoxy(COMBO_POS_X, COMBO_POS_Y); printf("%d COMBO!", combo);
    gotoxy(COMBO_POS_X, COMBO_POS_Y + 1); printf("+%d Points", income);

    Sleep(600); // 대기 후 메시지 제거

    remove_blank(); // 생긴 공백 제거

    gotoxy(COMBO_POS_X, COMBO_POS_Y); printf("         ");
    gotoxy(COMBO_POS_X, COMBO_POS_Y + 1); printf("             ");
}

void remove_blank(void) // 라인 제거 후 공백 제거
{
    int i, j, k;
    int blanks;
    int flag = 0;
    int line;

    for (i = 0; i < GAME_Y; i++)
    {
        blanks = 0;
        flag = 0;
        for (j = 0; j < GAME_X; j++) // 한 줄의 총 공백 개수 계산
        {
            if (game_table[i][j] == EMPTY)
                blanks++;
        }

        if (blanks == GAME_X - 2) // 한줄이 전부 공백일 경우
        {
            for (k = i; k > 1; k--)
            {
                for (j = 1; j < GAME_X - 1; j++)
                {
                    if (game_table[k][j] == BLOCK_INA) // 그 윗쪽에 블럭이 있으면
                        flag = 1;
                }
            }
        }

        if (flag == 1) // 블럭들을 가지고 내려옴
        {
            drop_blocks(i);
        }
    }
}

void drop_blocks(int line) // 비어있는 줄 윗 블럭들을 내려줌
{
    int i, j;

    for (i = 0; i < GAME_Y; i++)  // 일단 복사
    {
        for (j = 0; j < GAME_X; j++)
        {
            game_table_copy[i][j] = game_table[i][j];
        }
    }

    for (i = 2; i <= line; i++) // 지정된 줄 기준 윗 블럭들을 아래로 가져옴
    {
        for (j = 0; j < GAME_X; j++)
        {
            game_table[i][j] = game_table_copy[i - 1][j];
        }
    }

    draw_table();
}

void pause_game(void)
{
    floop = 1;
    paused = 1;

    gotoxy(7,6);  printf("******************");
    gotoxy(7,7);  printf("*     PAUSED     *");
    gotoxy(7,8);  printf("******************");
    gotoxy(7,9);  printf("* MOVE: A,D      *");
    gotoxy(7,10); printf("* SOFT DROP: S   *");
    gotoxy(7,11); printf("* HARD DROP: SPC *");
    gotoxy(7,12); printf("* QUIT: ESC      *");
    gotoxy(7,13); printf("* ROTATE: W      *");
    gotoxy(7,14); printf("* PAUSE: P       *");
    gotoxy(7,15); printf("* MADE BY COSYAN *");
    gotoxy(7,16); printf("******************");

    while (floop)
    {
        check_key();
        Sleep(33);
    }
}

void escape_game(void)
{
    floop = 1;
    esc_paused = 1;

    gotoxy(8,10); printf("***************");
    gotoxy(8,11); printf("* EXIT? (y/n) *");
    gotoxy(8,12); printf("***************");

    while(floop)
    {
        check_key();
        Sleep(33);
    }
}

void hard_drop(void)
{
    int i, j;
    loop = 0;
    while(!check_crash(DOWN))
    {
        move_block(DOWN);
    }
    block_stack();
    new_block_flag = 1;
}

void unpause(void)
{
    int i, j;

    system("cls");

    if (game_running == 1)
    {
        for (i = 0; i < GAME_Y; i++)
        {
            for (j = 0; j < GAME_X; j++)
            {
                game_table_copy[i][j] = 10;
            }
        }
        draw_ui();
        draw_next_block();
        draw_score();
    }
    else
        draw_title();

    floop = 0;
}

#include "tetris.h"

unsigned short b_color;
unsigned short b_color_next = RED;
unsigned short block_color[GAME_Y][GAME_X];
unsigned short block_color_copy[GAME_Y][GAME_X];
int total_lines;
int game_level;
int line_need;

void init(void) // �ʱ�ȭ
{
    system("mode con cols=46 lines=22 | title TETRIS"); // �ܼ�ũ��� â �̸�

    HANDLE console_handle = GetStdHandle(STD_OUTPUT_HANDLE); // Ŀ�� ����
    CONSOLE_CURSOR_INFO console_cursor;
    console_cursor.bVisible = 0;
    console_cursor.dwSize = 1;
    SetConsoleCursorInfo(console_handle, &console_cursor);

    srand((unsigned)time(NULL)); // ����ǥ ����

    next_block = rand() % 7;
    new_block_flag = 1;
    game_running = 0;
}

void gotoxy(int x, int y) // �ܼ� XY��ġ�� �̵�
{
    COORD Pos;
    Pos.X = x*2;
    Pos.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Pos);
}

void draw_title(void)
{
    text_color(BACKGROUND,WHITE);
    system("cls");

    text_color(BLACK,RED);
    gotoxy(1,3); printf("�ââ�"); // T
    gotoxy(1,4); printf("  ��");
    gotoxy(1,5); printf("  ��");
    gotoxy(1,6); printf("  ��");
    gotoxy(1,7); printf("  ��");

    text_color(BLACK,ORANGE);
    gotoxy(5,3); printf("�ââ�"); // E
    gotoxy(5,4); printf("��");
    gotoxy(5,5); printf("�ââ�");
    gotoxy(5,6); printf("��");
    gotoxy(5,7); printf("�ââ�");

    text_color(BLACK,GREEN);
    gotoxy(9,3); printf("�ââ�"); // T
    gotoxy(9,4); printf("  ��");
    gotoxy(9,5); printf("  ��");
    gotoxy(9,6); printf("  ��");
    gotoxy(9,7); printf("  ��");

    text_color(BLACK,BLUE);
    gotoxy(13,3); printf("�ââ�"); // R
    gotoxy(13,4); printf("��  ��");
    gotoxy(13,5); printf("�â�");
    gotoxy(13,6); printf("��  ��");
    gotoxy(13,7); printf("��  ��");

    text_color(BLACK,DARK_BLUE);
    gotoxy(17,3); printf("��"); // I
    gotoxy(17,4); printf("��");
    gotoxy(17,5); printf("��");
    gotoxy(17,6); printf("��");
    gotoxy(17,7); printf("��");

    text_color(BLACK,PURPLE);
    gotoxy(19,3); printf("�ââ�"); // S
    gotoxy(19,4); printf("��");
    gotoxy(19,5); printf("�ââ�");
    gotoxy(19,6); printf("    ��");
    gotoxy(19,7); printf("�ââ�");

    text_color(BLACK,WHITE);
    gotoxy(8,11); printf("Move:  W A S D");
    gotoxy(8,12); printf("Drop:  SpaceBar");
    gotoxy(8,13); printf("Pause: P");
    gotoxy(8,14); printf("Quit:  ESC");
    gotoxy(6,17); printf("Press SpaceBar to start");
}

void reset_table(void) // ������ �ʱ�ȭ
{
    int i, j;

    for (i = 0; i < GAME_Y ; i++) // �������� �� �������� ä��
    {
        for (j = 0; j < GAME_X ; j++)
        {
            game_table[i][j] = EMPTY;
            game_table_copy[i][j] = 10;
            block_color[i][j] = BACKGROUND;
        }
    }

    for (i = 0; i < GAME_X ; i++) // �׵θ��� �� ����
    {
        game_table[0][i] = WALL;
        game_table[GAME_Y - 1][i] = WALL;
        block_color[0][i] = WHITE;
        block_color[GAME_Y - 1][i] = WHITE;
    }
    for (i = 0; i < GAME_Y ; i++)
    {
        game_table[i][GAME_X - 1] = WALL;
        game_table[i][0] = WALL;
        block_color[i][GAME_X - 1] = WHITE;
        block_color[i][0] = WHITE;
    }
}

void draw_table(void) // ������ ���
{
    int i, j;

    text_color(BACKGROUND,WHITE);
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
            if (game_table[i][j] != game_table_copy[i][j] || block_color[i][j] != block_color_copy[i][j]) // �ٸ� ���� �����ؼ� ����ϱ�
            {
                gotoxy(GAME_POS_X + j, GAME_POS_Y + i); // �ش� ��ǥ�� �̵�
                text_color(BACKGROUND,block_color[i][j]);
                switch(game_table[i][j]) // �ش��ϴ� �� ���
                {
                case EMPTY:
                    printf("  ");
                    break;
                case BLOCK_ACT:
                    printf("��");
                    break;
                case BLOCK_INA:
                    printf("��");
                    break;
                case LINE:
                    printf("_");
                    break;
                case WALL:
                    text_color(BACKGROUND,WHITE);
                    printf("��");
                    break;
                }

                game_table_copy[i][j] = game_table[i][j]; // �� ����ϸ� ��������
                block_color_copy[i][j] = block_color[i][j];
            }
        }
    }
}

void new_block(void) // ����  �� ����
{
    if (new_block_flag != 1)
        return;

    current_block = next_block;
    next_block = rand() % 7;

    b_pos_x = GAME_X / 2 - 2;
    b_pos_y = 1;
    b_rotation = 0;

    random_color();
    place_block();

    draw_next_block();

    loop = 0;
    new_block_flag = 0;
}

void draw_next_block(void) // ���� �� ���
{
    int i, j;

    text_color(BACKGROUND,WHITE);
    gotoxy(UI_POS_X, UI_POS_Y + 2); printf("            ");
    gotoxy(UI_POS_X, UI_POS_Y + 3); printf("            ");
    gotoxy(UI_POS_X, UI_POS_Y + 4); printf("            ");
    gotoxy(UI_POS_X, UI_POS_Y + 5); printf("            ");

    text_color(BACKGROUND,b_color_next);
    for (i = 0; i < 4; i++)
    {
        for (j = 0; j < 4; j++)
        {
            if (blocks[next_block][0][i][j] == BLOCK_ACT)
            {
                gotoxy(UI_POS_X + j, UI_POS_Y + 2 + i);
                printf("��");
            }
        }
    }
}

void draw_ui(void) // ������, ������ �� ���
{
    text_color(BACKGROUND,WHITE);
    gotoxy(UI_POS_X + 1, UI_POS_Y); printf("NEXT");           // ���� ��

    gotoxy(UI_POS_X, UI_POS_Y + 9); printf("SCORE : %-6d", score); // ����

    gotoxy(UI_POS_X, UI_POS_Y + 11); printf("LEVEL : %-3d", game_level); // ���� ����

    gotoxy(UI_POS_X - 1, UI_POS_Y + 12); printf("NEXT LV.: %-2d", line_need);
}

void place_block(void) // ���� �����ǿ� ����
{
    int i, j;

    for (i = 0; i < 4; i++)
    {
        for (j = 0; j < 4; j++)
        {
            if (blocks[current_block][b_rotation][i][j] == 1)
            {
                game_table[b_pos_y + i][b_pos_x + j] = BLOCK_ACT;
                block_color[b_pos_y + i][b_pos_x + j] = b_color;
            }
        }
    }
}

void block_down(void) // �� �������� ����� �Լ�
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
                    block_stack(); // �ٴڿ� ���� ���� ��� �� ����
                    new_block_flag = 1; // �� �� ��û
                    return;  // �Լ� Ż��
                }
            }
        }
    }

    erase_block();

    b_pos_y++; // ���� �浹�� ���� ���

    place_block();
}

void block_stack(void) // ���� ���̰� ���ִ� �Լ�
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

void check_gameover(void) // ���ӿ��� ����
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

void draw_gameover(void) // ���ӿ��� ȭ�� ���
{
    game_running = 0;

    system("cls");

    text_color(GRAY,WHITE);
    gotoxy(8,8);  printf("               ");
    gotoxy(8,9);  printf(" | GAME OVER | ");
    gotoxy(8,10); printf("               ");
    gotoxy(8,11); printf(" SCORE: %-6d ", score);
    gotoxy(8,12); printf(" LEVEL: %-6d ", game_level);
    gotoxy(8,13); printf(" LINES: %-6d ", total_lines);
    gotoxy(8,14); printf("               ");

    Sleep(1000);

    while(kbhit())
        getch();

    while(!kbhit())
        Sleep(33);

    while(kbhit())
        getch();

    draw_title();
    gameover_flag = 0;
}

void move_block(int dir) // �� �̵�
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

int check_crash(int dir) // �浹 �˻�
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
            if (b_rotation == 3) // ȸ������ �� �ӽ� ����
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

void check_key(void) // Ű�Է� �ް� Ŀ�ǵ� ����
{
    if (kbhit())
    {
        key = getch();

        switch (key)
        {
        case LEFT:
            if (paused == 1 || game_running == 0)
                break;
            if (check_crash(LEFT) == 0)
                move_block(LEFT);
            break;
        case RIGHT:
            if (paused == 1 || game_running == 0)
                break;
            if (check_crash(RIGHT) == 0)
                move_block(RIGHT);
            break;
        case DOWN:
            if (paused == 1 || game_running == 0)
                break;
            if (check_crash(DOWN) == 0)
                move_block(DOWN);
            else
            {
                block_stack();
                new_block_flag = 1;
            }
            break;
        case ROTATION:
            if (paused == 1 || game_running == 0)
                break;
            if (check_crash(ROTATION) == 0)
                rotate_block();
            break;

        case 121: // ESC ���� �� y�� ������ �� ����
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

        case 110: // ESC�� ���� �� n�� ������ �� ����
            if (esc_paused == 1)
            {
                unpause();
                esc_paused = 0;
                paused = 0;
            }
            break;

        case SPACE_BAR:
            if (paused == 1)
                break;
            else if (game_running == 0)
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

void rotate_block(void) // �� ȸ��
{
    int i, j;

    erase_block();

    if (b_rotation == 3)
        b_rotation = 0;
    else
        b_rotation++;

    place_block();
}

void erase_block(void) // ������ �� ����
{
    int i, j;

    for (i = 0; i < 4; i++)
    {
        for (j = 0; j < 4; j++)
        {
            if (blocks[current_block][b_rotation][i][j])
            {
                game_table[i + b_pos_y][j + b_pos_x] = EMPTY;
                block_color[i + b_pos_y][j + b_pos_x] = BACKGROUND;
            }
        }
    }
}

void check_line(void) // ���� Ȯ��, ����, ���� ȹ��
{
    int i, j, k, l;
    int blocks;
    int lines = 0;
    int income = 0;

    for (i = 0; i < GAME_Y; i++) // ������ �����ϴ� �� ���� ���� ���
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
            total_lines++;
            line_need--;
            for (j = 1; j < GAME_X - 1; j++) // ���δ޼��� �� �� ����
            {
                game_table[i][j] = EMPTY;
                block_color[i][j] = BACKGROUND;
                draw_table();
            }
        }
    }

    if (lines >= 1)
    {
        income = lines * (GAME_X - 2) * lines * 10; // �� �� ���� * 10 * �޺�
        score += income;

        draw_combo(lines, income); // �޺� ���
    }
}

void draw_combo(int combo, int income) // �޺� ���
{
    text_color(BACKGROUND,WHITE);
    gotoxy(UI_POS_X, UI_POS_Y + 14); printf("%d COMBO!", combo);
    gotoxy(UI_POS_X, UI_POS_Y +15); printf("+%d Points", income);

    Sleep(600); // ��� �� �޽��� ����

    remove_blank(); // ���� ���� ����

    gotoxy(UI_POS_X, UI_POS_Y + 14); printf("         ");
    gotoxy(UI_POS_X, UI_POS_Y +15); printf("             ");
}

void remove_blank(void) // ���� ���� �� ���� ����
{
    int i, j, k;
    int blanks;
    int flag = 0;
    int line;

    for (i = 0; i < GAME_Y; i++)
    {
        blanks = 0;
        flag = 0;
        for (j = 0; j < GAME_X; j++) // �� ���� �� ���� ���� ���
        {
            if (game_table[i][j] == EMPTY)
                blanks++;
        }

        if (blanks == GAME_X - 2) // ������ ���� ������ ���
        {
            for (k = i; k > 1; k--)
            {
                for (j = 1; j < GAME_X - 1; j++)
                {
                    if (game_table[k][j] == BLOCK_INA) // �� ���ʿ� ���� ������
                        flag = 1;
                }
            }
        }

        if (flag == 1) // ������ ������ ������
        {
            drop_blocks(i);
        }
    }
}

void drop_blocks(int line) // ����ִ� �� �� ������ ������
{
    int i, j;

    for (i = 0; i < GAME_Y; i++)  // �ϴ� ����
    {
        for (j = 0; j < GAME_X; j++)
        {
            game_table_copy[i][j] = game_table[i][j];
            block_color_copy[i][j] = block_color[i][j];
        }
    }

    for (i = 2; i <= line; i++) // ������ �� ���� �� ������ �Ʒ��� ������
    {
        for (j = 0; j < GAME_X; j++)
        {
            game_table[i][j] = game_table_copy[i - 1][j];
            block_color[i][j] = block_color_copy[i - 1][j];
        }
    }

    draw_table();
}

void pause_game(void)
{
    floop = 1;
    paused = 1;

    text_color(GRAY,WHITE);
    gotoxy(7,6);  printf("                  ");
    gotoxy(7,7);  printf("    | PAUSED |    ");
    gotoxy(7,8);  printf("                  ");
    gotoxy(7,9);  printf("  MOVE: A,D       ");
    gotoxy(7,10); printf("  SOFT DROP: S    ");
    gotoxy(7,11); printf("  HARD DROP: SPC  ");
    gotoxy(7,12); printf("  QUIT: ESC       ");
    gotoxy(7,13); printf("  ROTATE: W       ");
    gotoxy(7,14); printf("  PAUSE: P        ");
    gotoxy(7,15); printf("                  ");
    gotoxy(7,16); printf("  MADE BY COSYAN  ");
    gotoxy(7,17); printf("                  ");

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

    text_color(BRIGHT_WHITE,BLACK);
    gotoxy(8,10); printf("               ");
    gotoxy(8,11); printf("  QUIT? (y/n)  ");
    gotoxy(8,12); printf("               ");

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

    text_color(BACKGROUND,WHITE);
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
    }
    else
        draw_title();

    floop = 0;
}

void text_color(unsigned short bg, unsigned short fg)
{
    SetConsoleTextAttribute( GetStdHandle( STD_OUTPUT_HANDLE ), (bg << 4) | fg);
}

void random_color(void)
{
    unsigned short r;

    b_color = b_color_next;

    r = rand() % 7;

    switch(r)
    {
    case 0:
        b_color_next = RED;
        break;
    case 1:
        b_color_next = ORANGE;
        break;
    case 2:
        b_color_next = GREEN;
        break;
    case 3:
        b_color_next = BLUE;
        break;
    case 4:
        b_color_next = PURPLE;
        break;
    case 5:
        b_color_next = DARK_BLUE;
        break;
    case 6:
        b_color_next = YELLOW;
        break;
    }


}

void check_level(void)
{
    if (line_need <= 0)
    {
        text_color(GRAY,WHITE);
        gotoxy(GAME_POS_X + (GAME_X / 2) - 3, GAME_POS_Y + (GAME_Y - MAX_HEIGHT - 2) - 1);
        printf("             ");
        gotoxy(GAME_POS_X + (GAME_X / 2) - 3, GAME_POS_Y + (GAME_Y - MAX_HEIGHT - 2));
        printf(" SPEED UP!!! ");
        gotoxy(GAME_POS_X + (GAME_X / 2) - 3, GAME_POS_Y + (GAME_Y - MAX_HEIGHT - 2) + 1);
        printf("             ");

        Sleep(2000);
        text_color(BACKGROUND,WHITE);
        gotoxy(GAME_POS_X + (GAME_X / 2) - 3, GAME_POS_Y + (GAME_Y - MAX_HEIGHT - 2) - 1);
        printf("             ");
        gotoxy(GAME_POS_X + (GAME_X / 2) - 3, GAME_POS_Y + (GAME_Y - MAX_HEIGHT - 2));
        printf("             ");
        gotoxy(GAME_POS_X + (GAME_X / 2) - 3, GAME_POS_Y + (GAME_Y - MAX_HEIGHT - 2) + 1);
        printf("             ");

        game_level++;

        if (game_speed > 0)
            game_speed--;

        line_need = 4 + game_level;
    }
}



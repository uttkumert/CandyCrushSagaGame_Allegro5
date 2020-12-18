#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include <stdlib.h>
#include <math.h>
#include <windows.h>
#include <time.h>


#define PLAYS_ 5 
#define NUM_TYPES 4
#define N_YATAY 9
#define N_DIKEY 6

#define UCGEN 1
#define DORTGEN 2
#define DAIRE 3
#define Y_DORTGEN 4

#define SCREEN_W 480
#define SCREEN_H 640
#define INFO_H 64
#define FPS 10
#define MARGIN 5

typedef struct Candy
{
	int type;
	int offset_lin;
	int offset_col;
	int active;
	ALLEGRO_COLOR cor;
} Candy;

Candy M[N_YATAY][N_DIKEY];
ALLEGRO_COLOR colors[NUM_TYPES + 1];

ALLEGRO_DISPLAY *display = NULL;
ALLEGRO_EVENT_QUEUE *event_queue = NULL;
ALLEGRO_TIMER *timer = NULL;

const float CELL_W = (float)SCREEN_W / N_DIKEY;
const float CELL_H = (float)(SCREEN_H - INFO_H) / N_YATAY;

int score = 0, plays = PLAYS_;
char PLYR_POINT[100], PLYR_PLAYS[100];

ALLEGRO_FONT *size_f;


int generateRandomCandy()
{
	return rand() % NUM_TYPES + 1;
}

void matrixYazici()
{
	printf("\n");
	int i, j;
	for (i = 0; i < N_YATAY; i++)
	{
		for (j = 0; j < N_DIKEY; j++)
		{
			printf("%d (%d,%d) ", M[i][j].type, M[i][j].offset_lin, M[i][j].active); 
		}
		printf("\n");
	}
}

void matrixTamamlayici()
{
	int i, j;
	for (i = 0; i < N_YATAY; i++)
	{
		for (j = 0; j < N_DIKEY; j++)
		{
			if (M[i][j].type == 0)
			{
				M[i][j].type = generateRandomCandy();
				M[i][j].offset_col = 0;
				M[i][j].offset_lin = 0;
				M[i][j].active = 1;
				M[i][j].cor = colors[M[i][j].type];
			}
		}
	}
}

void startGame()
{
	int i, j;
	for (i = 0; i < N_YATAY; i++)
	{
		for (j = 0; j < N_DIKEY; j++)
		{
			M[i][j].type = generateRandomCandy();
			M[i][j].offset_col = 0;
			M[i][j].offset_lin = 0;
			M[i][j].active = 1;
			M[i][j].cor = colors[M[i][j].type];
			printf("%d ", M[i][j].type);
		}
		printf("\n");
	}
}

void pause_()
{
	al_stop_timer(timer);
	al_rest(3);
	al_start_timer(timer);
}

void draw_candy(int lin, int col)
{

	int cell_x = CELL_W * col;
	int cell_y = INFO_H + CELL_H * lin;

	if (M[lin][col].type == UCGEN)
	{
		al_draw_filled_triangle(cell_x + MARGIN, cell_y + CELL_H - MARGIN,
			cell_x + CELL_W - MARGIN, cell_y + CELL_H - MARGIN,
			cell_x + CELL_W / 2, cell_y + MARGIN,
			M[lin][col].cor);
	}
	else if (M[lin][col].type == DORTGEN)
	{
		al_draw_filled_rectangle(cell_x + 2 * MARGIN, cell_y + 2 * MARGIN,
			cell_x - 2 * MARGIN + CELL_W, cell_y - 2 * MARGIN + CELL_H,
			M[lin][col].cor);
	}
	else if (M[lin][col].type == Y_DORTGEN)
	{
		al_draw_filled_rounded_rectangle(cell_x + MARGIN, cell_y + MARGIN,
			cell_x - MARGIN + CELL_W, cell_y - MARGIN + CELL_H,
			CELL_W / 3, CELL_H / 3, M[lin][col].cor);
	}
	else if (M[lin][col].type == DAIRE)
	{
		al_draw_filled_ellipse(cell_x + CELL_W / 2, cell_y + CELL_H / 2,
			CELL_W / 2 - MARGIN, CELL_H / 2 - MARGIN,
			M[lin][col].cor);
	}
}

void anaEkran()
{

	ALLEGRO_COLOR BKG_COLOR = al_map_rgb(0, 0, 0);
	al_set_target_bitmap(al_get_backbuffer(display));
	al_clear_to_color(BKG_COLOR);

	sprintf(PLYR_POINT, "Puan: %d", score);
	al_draw_text(size_f, al_map_rgb(255, 255, 255), SCREEN_W - 200, INFO_H / 4, 0, PLYR_POINT);
	sprintf(PLYR_PLAYS, "Kalan: %d", plays);
	al_draw_text(size_f, al_map_rgb(255, 255, 255), 10, INFO_H / 4, 0, PLYR_PLAYS);

	int i, j;
	for (i = 0; i < N_YATAY; i++)
	{
		for (j = 0; j < N_DIKEY; j++)
		{
			draw_candy(i, j);
		}
	}
}

int clear_(int li, int lf, int ci, int cf)
{
	int i, j, count = 0;
	for (i = li; i <= lf; i++)
	{
		for (j = ci; j <= cf; j++)
		{
			count++;
			M[i][j].active = 0;
			M[i][j].cor = colors[0];
		}
	}
	return count;
}

int processMatrix()
{
	int i, j, k, count = 0;
	int current, seq, u;

	for (i = 0; i < N_YATAY; i++)
	{
		current = M[i][0].type;
		seq = 1;
		for (j = 1; j < N_DIKEY; j++)
		{
			if (current == M[i][j].type && current > 0)
			{
				seq++;
				if (j == N_DIKEY - 1 && seq >= 3)
					count += clear_(i, i, j - seq + 1, N_DIKEY - 1);
			}
			else
			{
				if (seq >= 3)
				{
					count += clear_(i, i, j - seq, j - 1);
				}
				seq = 1;
				current = M[i][j].type;
			}
		}
	}

	for (j = 0; j < N_DIKEY; j++)
	{
		current = M[0][j].type;
		seq = 1;
		for (i = 1; i < N_YATAY; i++)
		{
			if (current == M[i][j].type && current > 0)
			{
				seq++;
				if (i == N_YATAY - 1 && seq >= 3)
					count += clear_(i - seq + 1, N_YATAY - 1, j, j);
			}
			else
			{
				if (seq >= 3)
				{
					count += clear_(i - seq, i - 1, j, j);
				}

				seq = 1;
				current = M[i][j].type;
			}
		}
	}

	return count;
}

void offset_Update()
{
	matrixYazici();
	int i, j, offset;

	for (j = 0; j < N_DIKEY; j++)
	{
		offset = 0;
		for (i = N_YATAY - 1; i >= 0; i--)
		{
			M[i][j].offset_lin = offset;
			if (M[i][j].active == 0)
			{
				M[i][j].type = 0;
				offset++;
			}
		}
	}
}

void matrix_Update()
{
	int i, j, offset;

	for (j = 0; j < N_DIKEY; j++)
	{
		for (i = N_YATAY - 1; i >= 0; i--)
		{
			offset = M[i][j].offset_lin;
			if (offset > 0)
			{
				M[i + offset][j].type = M[i][j].type;
				M[i + offset][j].active = M[i][j].active;
				M[i + offset][j].cor = M[i][j].cor;
				M[i][j].type = 0;
				M[i][j].active = 0;
				M[i][j].offset_lin = 0;
			}
		}
	}
	matrixTamamlayici();
}

void getCell(int x, int y, int *lin, int *col)
{
	*lin = (y - INFO_H) / CELL_H;
	*col = x / CELL_W;
}

int distance(int lin1, int col1, int lin2, int col2)
{
	return sqrt(pow(lin1 - lin2, 2) + pow(col1 - col2, 2));
}

void sign_(int lin1, int col1, int lin2, int col2)
{
	Candy aux = M[lin1][col1];
	M[lin1][col1] = M[lin2][col2];
	M[lin2][col2] = aux;
	plays--;
}

int controlUnit() {
	if (!al_init())
	{
		fprintf(stderr, "allegro failed\n");
		return -1;
	}

	if (!al_init_primitives_addon())
	{
		fprintf(stderr, "primitives failed\n");
		return -1;
	}

	timer = al_create_timer(1.0 / FPS);
	if (!timer)
	{
		fprintf(stderr, "timer failed\n");
		return -1;
	}

	display = al_create_display(SCREEN_W, SCREEN_H);
	if (!display)
	{
		fprintf(stderr, "display failed\n");
		al_destroy_timer(timer);
		return -1;
	}

	if (!al_install_mouse())
		fprintf(stderr, "mouse failed\n");

	al_init_font_addon();
	al_init_ttf_addon();

	size_f = al_load_font("arial.ttf", 32, 1);

	event_queue = al_create_event_queue();
	if (!event_queue)
	{
		fprintf(stderr, "event_queue failed\n");
		al_destroy_display(display);
		al_destroy_timer(timer);
		return -1;
	}
	al_install_keyboard();
	al_register_event_source(event_queue, al_get_display_event_source(display));
	al_register_event_source(event_queue, al_get_timer_event_source(timer));
	al_register_event_source(event_queue, al_get_keyboard_event_source());
	al_register_event_source(event_queue, al_get_mouse_event_source());
	al_start_timer(timer);
}

int main(int argc, char **argv)
{

	controlUnit();

	colors[0] = al_map_rgb(0, 248, 0);
	colors[UCGEN] = al_map_rgb(255, 0, 0);
	colors[DORTGEN] = al_map_rgb(255, 255, 0);
	colors[DAIRE] = al_map_rgb(0, 0, 255);
	colors[Y_DORTGEN] = al_map_rgb(128, 128, 128);

	srand(time(NULL));
	startGame();
	int n_zeros = processMatrix();
	while (n_zeros > 0)
	{
		do
		{
			offset_Update();
			matrix_Update();
		} while (processMatrix());
		matrixTamamlayici();
		n_zeros = processMatrix();
	}

	anaEkran();
	al_flip_display();
	int pointss, playing = 1, col_src, lin_src, col_dst, lin_dst, flag_animation = 0;
	while (playing)
	{
		ALLEGRO_EVENT ev;
		al_wait_for_event(event_queue, &ev);

		if (ev.type == ALLEGRO_EVENT_KEY_DOWN)
		{
			if (ev.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
			{
				playing = 0;
			}
		}
		else if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN && !flag_animation)
		{
			getCell(ev.mouse.x, ev.mouse.y, &lin_src, &col_src);
		}
		else if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP && !flag_animation)
		{
			getCell(ev.mouse.x, ev.mouse.y, &lin_dst, &col_dst);
			if (distance(lin_src, col_src, lin_dst, col_dst) == 1 && M[lin_src][col_src].type && M[lin_dst][col_dst].type)
			{
				sign_(lin_src, col_src, lin_dst, col_dst);
				flag_animation = 1;
			}
		}
		else if (ev.type == ALLEGRO_EVENT_TIMER)
		{
			pointss = processMatrix();
			while (pointss > 0)
			{
				anaEkran();
				al_flip_display();
				pause_();
				offset_Update();
				matrix_Update();
				score += pow(2, pointss);
				pointss = processMatrix();
			}

			anaEkran();
			al_flip_display();

			if (plays == 0)
				playing = 0;
			flag_animation = 0;
		}
		else if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
		{
			playing = 0;
		}
	}

	al_rest(1);

	al_clear_to_color(al_map_rgb(230, 240, 250));
	sprintf(PLYR_POINT, "Puan: %d", score);
	al_draw_text(size_f, al_map_rgb(200, 0, 30), SCREEN_W / 3, SCREEN_H / 2, 0, PLYR_POINT);

	al_flip_display();
	al_rest(2);

	al_destroy_timer(timer);
	al_destroy_display(display);
	al_destroy_event_queue(event_queue);

	return 0;
}
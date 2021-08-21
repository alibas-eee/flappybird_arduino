#include "Que.h"

char sp = ' ';
char bird = 'O';
char CHAR_PIPE = '#';
int bird_alt = 10;
boolean game_over = false;
long game_counter = 0;

#define MAX_BIRD_ALT 16
#define TABLE_WIDTH	16
#define TABLE_HEIGTH 16
#define PIPE_PERIODE 7

char flap[] = {'F', 'L', 'A', 'P'};
char brd[] = {'B', 'I', 'R', 'D'};

Que scene_que(TABLE_WIDTH + 2);

void setup() {
  Serial.begin(500000);
  delay(100);
  fill_space();
  print_scene();
}

void loop() {
  Game();
}

void(* resetFunc) (void) = 0; //

int availableMemory() {
  // Use 1024 with ATmega168
  int size = 2048;
  byte *buf;
  while ((buf = (byte *) malloc(--size)) == NULL);
  free(buf);
  return size;
}

void Game() {

  if (game_over) {
    print_gameover();
    while (!Serial.available());
    //restart jump 0
    resetFunc();  //call reset
  } else {
    clear_screen();
    read_line();
    play_game();
    Serial.println(game_counter);
    delay(200);
  }
}

void play_game() {
  game_counter++;

  if (game_counter % PIPE_PERIODE == 0) {
    int g = random(3) + 3;
    int a = random(3) + 5;
    generate_pipe(g, a);
  } else {
    generate_space();
  }
  delete_col();
  print_scene();
}

void clear_screen() {
  Serial.write(27);       // ESC command
  Serial.print("[2J");    // clear screen command
  Serial.write(27);
  Serial.print("[H");     // cursor to home command
}

void bird_flap() {
  bird_alt += 2;
  if (bird_alt > MAX_BIRD_ALT)
    bird_alt = MAX_BIRD_ALT;
}

void bird_free_fall() {

  bird_alt--;
  if (bird_alt == 0)
    game_over = true;

}


void generate_pipe(int gap, int gap_alt) {
  Column col;
  for (int i = 0; i < TABLE_HEIGTH ; i++) {
    if ( i < gap_alt || i > gap + gap_alt) {
      col.elements[i] = CHAR_PIPE;
    } else {
      col.elements[i] = ' ';
    }
  }
  scene_que.enqueue(&col);
}

void generate_space() {
  Column col;
  for (int i = 0; i < TABLE_HEIGTH ; i++) {
    col.elements[i] = ' ';

  }
  scene_que.enqueue(&col);
}

void delete_col() {
  scene_que.dequeue();
}

void fill_space() {
  for (int i = 0; i < TABLE_WIDTH; i++) {
    Column col;
    for (int j = 0; j < 16 ; j++) {
      if ( j == 9 && i > 8 && i < 13) {
        col.elements[j] = flap[i - 9];
      }
      else if ( j == 8 && i > 8 && i < 13) {
        col.elements[j] = brd[i - 9];
      }
      else {
        col.elements[j] = ' ';
      }
    }
    scene_que.enqueue(&col);
  }
}

void read_line() {
  if (Serial.available()) {
    char c = Serial.read();
    if ( c == 'w') {
      bird_flap();
    } else {
      bird_free_fall();
    }
  } else {
    bird_free_fall();
  }
  Serial.flush();

}
void print_scene() {
  int i, j;
  for ( i = 0; i < 32 ; i++) {
    Serial.print('_');
  }
  Serial.println(' ');

  for ( i = 15; i >= 0 ; i--) {		//y
    for ( j = 0; j < 16 ; j++) {	//x
      Column *col = scene_que.look_at(j);
      if (j == 2 && i == bird_alt) {
        if (col->elements[i] == CHAR_PIPE)
          game_over = true;
        Serial.print('O');
        Serial.print('o');
      } else {
        Serial.print(col->elements[i]);
        Serial.print(' ');
      }

    }
    Serial.println(' ');
  }

  for ( i = 0; i < 32 ; i++) {
    Serial.print('_');
  }
}


void print_gameover() {
  int i;
  for (i = 0; i < 16 ; i++) {
    Serial.print('_');
  }
  Serial.println(' ');

  Serial.println("####  GAME OVER ####");
  Serial.println("\tPOINT");
  Serial.print("\t");
  Serial.println(game_counter / PIPE_PERIODE);
  Serial.print("Press key to RESTART!");

  for ( i = 0; i < 16 ; i++) {
    Serial.print('_');
  }
  Serial.println(' ');
}

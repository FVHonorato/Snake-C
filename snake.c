#include <SDL2/SDL_events.h>
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_video.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <stdlib.h>

//variables global
const int window_size[] = {800,600};
int gameRunning = 0;

SDL_Window* window = NULL;
SDL_Renderer* renderer;

struct snake{
  int x;
  int y;
  int w;
  int h;
}snake;

struct apple{
  int x;
  int y;
  int w;
  int h;
}apple;

int direction;
int steps;
int speed;
int snake_size[2][1000];
int snake_count;


// functions
int Init_Window(void){
  if(SDL_Init(SDL_INIT_EVERYTHING) !=0){
    fprintf(stderr, "ERRO \n");
    return 0;
  };
  
  window = SDL_CreateWindow(NULL, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,window_size[0],window_size[1], SDL_WINDOW_BORDERLESS);

  if(!window){
    fprintf(stderr, "ERRO WINDOW\n");
    return 0;
  }

  renderer = SDL_CreateRenderer(window,-1, 0);

  if(!renderer){
    fprintf(stderr,"ERRO RENDER\n");
    return 0;
  }

  

  return 1;
}


void process_input(){
  SDL_Event event;
  SDL_PollEvent(&event);

  switch (event.type) {
    case SDL_QUIT:
      gameRunning = 0;
      break;

    case SDL_KEYDOWN:
      if(event.key.keysym.sym == SDLK_UP){
        if(direction != 3)
          direction = 1; 
      }
      
      if(event.key.keysym.sym == SDLK_DOWN){
        if(direction != 1)
          direction = 3; 
      }
            
      if(event.key.keysym.sym == SDLK_LEFT){
        if(direction != 4)
          direction = 2;
      }

      if(event.key.keysym.sym == SDLK_RIGHT){
        if(direction != 2)
          direction = 4;
      }
      
      if(event.key.keysym.sym == SDLK_s){
        if(steps == 20)
          steps = 10;
      }

      if(event.key.keysym.sym == SDLK_f){
        if(steps == 10)
          steps = 20;
      }

      if(event.key.keysym.sym == SDLK_ESCAPE)
        gameRunning = 0;
      break;
  }
}

//fuction with the configurations
void setup(){
  snake_count = 100; 
  steps = 10; 
  direction = 4;
  speed = 50;

  snake.x = 400;
  snake.y = 300;
  snake.w = 10;
  snake.h = 10;

  apple.x = 350;
  apple.y = 250;
  apple.w = 10;
  apple.h = 10;

  srand(10);

}


void update(){
  if(direction == 1){
    snake.y -= steps; 
  }
  if(direction == 3){
    snake.y += steps;
  }

  if(direction == 2){
    snake.x -= steps;
  }
  if(direction == 4){
    snake.x += steps;
  }
  
  //save position od snake
  for(int i=0;i<=snake_count;i++){ 
    if(i > 0){
      snake_size[0][i-1] = snake_size[0][i];
      snake_size[1][i-1] = snake_size[1][i];
    }
    snake_size[0][i] = snake.x;
    snake_size[1][i] = snake.y;  
  }

  for(int i=0;i<snake_count;i++){
    if(snake.x == snake_size[0][i] && snake.y == snake_size[1][i])
      snake_count = 6;

  }
  
  //apple position
  if(snake.x == apple.x && snake.y == apple.y){
    snake_count += 1; 
    apple.x = rand()%window_size[0];
    do {
      apple.x +=1;
         }
    while(apple.x%10 != 0);

    apple.y = rand()%window_size[1];
    do{
      apple.y +=1; 
    }while(apple.y%10 != 0);
  }

  //to no exit of window
  if(snake.x > window_size[0])
    snake.x = 0;
  
  if(snake.y > window_size[1])
      snake.y = 0;

  if(snake.x < 0)
    snake.x = window_size[0]-10;

  if(snake.y < 0)
    snake.y = window_size[1]-10;

  SDL_Delay(speed);
}


void render(){
  //render window
  SDL_SetRenderDrawColor(renderer, 10,100,10,255);
  SDL_RenderClear(renderer);
  
  //render fild
  SDL_SetRenderDrawColor(renderer, 10,50,10,255);
  SDL_Rect fild_rect_UP = {0,0,window_size[0],30};
  SDL_RenderFillRect(renderer, &fild_rect_UP);
  SDL_Rect fild_rect_DOWN = {0,window_size[1]-30,window_size[0],30};
  SDL_RenderFillRect(renderer, &fild_rect_DOWN);

  //render apple
  SDL_SetRenderDrawColor(renderer, 255,0,0,255);
  SDL_Rect apple_rect = {apple.x, apple.y, apple.w, apple.h};
  SDL_RenderFillRect(renderer, &apple_rect);

  //draw snake  
  for(int i=0;i<=snake_count;i++){
    if(snake_count >10){
      if(i%2 == 0){
        SDL_SetRenderDrawColor(renderer, 150,250,150,255);
      }else{
        SDL_SetRenderDrawColor(renderer, 50,250,50,255);
      }
    }else{
      SDL_SetRenderDrawColor(renderer, 0,0,0,255);
    }

    SDL_Rect snake_rect = {snake_size[0][i], snake_size[1][i], snake.w, snake.h};
    SDL_RenderFillRect(renderer, &snake_rect);
    
  }
  SDL_RenderPresent(renderer);

}


void destroy_window(){
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
}


int main(){
  gameRunning = Init_Window(); 
  setup();
  while(gameRunning){
    render();
    process_input();
    update();
  }
  destroy_window();
  return 0;
} 

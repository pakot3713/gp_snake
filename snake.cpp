#include "snake.h"
#define SZ 20
#define START_LEN 3
#define DEATH_TIME 200
using namespace std;

int vx[4] = {-1,0,1,0};
int vy[4] = {0,-1,0,1};

Snake::Snake(){
  mt19937 mt(rd());
  uniform_int_distribution<int> rd_d(1,SZ);
  for(int i=0;i<SZ+2;i++){
    for(int j=0;j<SZ+2;j++){
      if(i==0||j==0||i==SZ+1||j==SZ+1){
	board[i][j] = -1;
      }else{
	board[i][j] = 0;
      }
    }
  }
  for(int i=0;i<START_LEN;i++){
    board[SZ/2+1+i][SZ/2+1] = START_LEN-i;
  }
  head_pos.x = SZ/2+1; head_pos.y = SZ/2+1;
  tail_pos.x = SZ/2+START_LEN; tail_pos.y = SZ/2+1;
  do{
    food_pos.x = rd_d(mt)+1;
    food_pos.y = rd_d(mt)+1;
  }while(board[food_pos.x][food_pos.y]!=0);
  len = START_LEN;
  vec = 0;
  last_eat = 0;
  return;
}

int Snake::get_time(){
  return board[head_pos.x][head_pos.y]-START_LEN;
}

int Snake::get_len(){
  return len;
}

void Snake::print_b(){
  printf("\033[2J");
  cout << "time : " << get_time();
  cout << " len  : " << get_len();
  cout << " head : " << head_pos.x << "," <<head_pos.y;
  cout << " tail : " << tail_pos.x << "," <<tail_pos.y << endl;
  for(int i=0;i<SZ+2;i++){
    for(int j=0;j<SZ+2;j++){
       if(food_pos.x==j&&food_pos.y==i){
	cout << "f";
      }else if(board[j][i]==-1){
	cout << "#";
      }else if(board[j][i]==0){
	cout << ".";
      }else{
	cout << "o";
      }
    }
    cout << endl;
  }
  return;
}

bool Snake::move(mov_v mov){
  mt19937 mt(rd());
  uniform_int_distribution<int> rd_d(1,SZ);
  if(mov == left){
    vec = (vec+3)%4;
  }else if(mov == right){
    vec = (vec+1)%4;
  }
  int x = head_pos.x + vx[vec];
  int y = head_pos.y + vy[vec];
  if((board[x][y]!=0&&(!(x==tail_pos.x&&y==tail_pos.y)))||(last_eat +DEATH_TIME < get_time())){//障害物にぶつかるorDEATH_TIMEの間餌を食べられない時死ぬ
    return false;
  }else{
    if(food_pos.x == x && food_pos.y == y){
      //餌を食べると体長が1伸びる
      last_eat = get_time();
      len++;
      do{
	food_pos.x = rd_d(mt)+1;
	food_pos.y = rd_d(mt)+1;
      }while(board[food_pos.x][food_pos.y]!=0);
    }else{
      for(int i=0;i<4;i++){
	if(board[tail_pos.x+vx[i]][tail_pos.y+vy[i]]==board[tail_pos.x][tail_pos.y]+1){
	  board[tail_pos.x][tail_pos.y] = 0;
	  tail_pos.x = tail_pos.x+vx[i];
	  tail_pos.y = tail_pos.y+vy[i];
	  break;
	}
      }
    }
    board[x][y] = board[head_pos.x][head_pos.y] + 1;
    head_pos.x = x; head_pos.y = y;
    return true;
  }
  return true;
}

bool Snake::get_l_snake(){
  int v = (vec+3)%4;
  return board[head_pos.x+vx[v]][head_pos.y+vx[v]]!=0; 
}
bool Snake::get_f_snake(){
  int v = vec;
  return board[head_pos.x+vx[v]][head_pos.y+vx[v]]!=0; 
}
bool Snake::get_r_snake(){
  int v = (vec+1)%4;
  return board[head_pos.x+vx[v]][head_pos.y+vx[v]]!=0; 
}
bool Snake::get_l_wall(){
  int v = (vec+3)%4;
  return board[head_pos.x+vx[v]][head_pos.y+vx[v]]<0; 
}
bool Snake::get_f_wall(){
  int v = vec;
  return board[head_pos.x+vx[v]][head_pos.y+vx[v]]<0; 
}
bool Snake::get_r_wall(){
  int v = (vec+1)%4;
  return board[head_pos.x+vx[v]][head_pos.y+vx[v]]<0; 
}
bool Snake::get_l_food(){
  int v = (vec+3)%4;
  return food_pos.x == head_pos.x+vx[v] && food_pos.y == head_pos.y+vx[v]; 
}
bool Snake::get_f_food(){
  int v = vec;
  return food_pos.x == head_pos.x+vx[v] && food_pos.y == head_pos.y+vx[v]; 
}
bool Snake::get_r_food(){
  int v = (vec+1)%4;
  return food_pos.x == head_pos.x+vx[v] && food_pos.y == head_pos.y+vx[v]; 
}
bool Snake::get_food_front(){
  int v = vec;
  switch(v){
  case 0:
    return food_pos.x < head_pos.x;
  case 1:
    return food_pos.y < head_pos.y;
  case 2:
    return food_pos.x > head_pos.x;
  case 3:
    return food_pos.y > head_pos.y;
  default:
    return false;
  }
}
bool Snake::get_food_left_side(){
  int v = (vec+3)%4;
  switch(v){
  case 0:
    return food_pos.x < head_pos.x;
  case 1:
    return food_pos.y < head_pos.y;
  case 2:
    return food_pos.x > head_pos.x;
  case 3:
    return food_pos.y > head_pos.y;
  default:
    return false;
  }
}
bool Snake::get_food_right_side(){
  int v = (vec+1)%4;
  switch(v){
  case 0:
    return food_pos.x < head_pos.x;
  case 1:
    return food_pos.y < head_pos.y;
  case 2:
    return food_pos.x > head_pos.x;
  case 3:
    return food_pos.y > head_pos.y;
  default:
    return false;
  }
}

#include <vector>
#include <algorithm>
#include <iostream>
#include <cstdio>
#include <cmath>
#include <cstdlib>
#include <string>
#include <random>
using namespace std;

#define SZ 20

typedef struct{
  int x;
  int y;
}Pos;

class Snake{
 private:
  int board[SZ+2][SZ+2];
  int len;
  int vec; //ヘビの向き 0~3
  int last_eat; //最後に食べた時間
  random_device rd;
  Pos head_pos; //ヘビの頭の位置
  Pos tail_pos; //蛇のしっぽの位置
  Pos food_pos; //餌の位置
 public:
  enum mov_v{left = 0,front = 1,right = 2};
  Snake();
  bool move(mov_v mov); //入力された方向に動く
  void print_b(); 
  int get_time();
  int get_len();

  //障害物(蛇の体・壁)が1マス隣にあるかチェック
  bool get_l_snake(); //if-0 
  bool get_f_snake(); //if-1
  bool get_r_snake(); //if-2
  //餌が1マス隣にあるかチェック
  bool get_l_food(); //if-3
  bool get_f_food(); //if-4
  bool get_r_food(); //if-5
  //餌のおおまかな位置を知る
  bool get_food_front(); //if-6
  bool get_food_left_side(); //if-7
  bool get_food_right_side(); //if-8

  //使用しない関数
  bool get_l_wall();
  bool get_f_wall();
  bool get_r_wall();
};


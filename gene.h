#include <vector>
#include <algorithm>
#include <iostream>
#include <cstdio>
#include <cmath>
#include <cstdlib>
#include <string>
#include <random>
#include <stack>
#include <unistd.h>
#include <sys/time.h>
#include "snake.h"
using namespace std;


//if_f は if文で分岐を行う関数を持つノード
//proc は 2つの動作連続して行うよう指定するノード
enum node{l = 0,front = 1,r = 2,if_f = 3,proc = 4};

//ヘビの動くプログラムの遺伝子を表す木
struct Gene_tree{
  node mov;
  int n_func; //if文の種類
  bool (Snake::*p)(); //if_fで分岐を行う関数
  struct Gene_tree * lt;
  struct Gene_tree * rt;
};
typedef struct Gene_tree gene_t;
typedef gene_t * gene_t_p;

class Gene{
private:
  random_device rd;
  gene_t_p t_tr;
  double len;
  double time;
  stack<gene_t_p> t_stack;
  vector<gene_t_p *> node_list;
public:
  Gene();
  Gene(gene_t_p g);//遺伝子の選択用
  Gene(string s);
  ~Gene();
  gene_t_p mk_gene_from_string(string s,int sn, int en);
  gene_t_p get_root_t(); //遺伝子の木のルートを取得
  gene_t_p cp_tree(gene_t_p t); //木のコピー
  void free_tree(gene_t_p t); 
  Snake::mov_v get_move(Snake * s, gene_t_p t);
  void print_tree(gene_t_p t); //遺伝子木をS式風スタイルで出力
  void print_score(); 
  void do_sim();
  void shorten_gene(gene_t_p g);//縮約
  double get_fitness();//適応度計算 REP_S回の試行を行い、生存時間と体長の平均から算出
  gene_t_p get_random_tree();//ランダムな部分木を得る
  void mutation();//突然変異
  void get_node_list(gene_t_p t);
  gene_t_p * get_random_node();
  void crossover(gene_t_p * node); //交叉
};

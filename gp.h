#include <vector>
#include <algorithm>
#include <iostream>
#include <cstdio>
#include <cmath>
#include <cstdlib>
#include <string>
#include <random>
#include "gene.h"
using namespace std;

#define G_NUM 60

class GP{
private:
  random_device rd;
  Gene * g[G_NUM];
  Gene * new_g[G_NUM];
  double score[G_NUM];
public:
  GP();
  ~GP();
  void calc_score();//適応度計算
  void select_gene();//遺伝子の選択
  void cross_over();//遺伝子の交叉
  void mutation();//遺伝子の突然変異
  void view_info();//遺伝子情報の出力
};

#include "gp.h"
using namespace std;

#define G_NUM 60
#define CO_P 0.5
#define MU_P 0.3

GP::GP(){
  for(int i=0;i<G_NUM;i++)
    g[i] = new Gene();
  return;
}
GP::~GP(){
  for(int i=0;i<G_NUM;i++)
    delete(g[i]);
  return;
}

void GP::calc_score(){
  double sum_score = 0.0;
  //シミュレーションを並列処理で行う
  #pragma omp parallel for
  for(int i=0;i<G_NUM;i++){
    g[i]->shorten_gene(g[i]->get_root_t());
    score[i] = g[i]->get_fitness();
    sum_score += score[i];
  }/*
  for(int i=0;i<G_NUM;i++){
    cout << i <<"th : "; 
    cout << " score : " << score[i];
    g[i]->print_score();
    cout << "  gene :   ";
    g[i]->print_tree(g[i]->get_root_t());
    cout << endl;
  }
  */
  for(int i=0;i<G_NUM;i++){
    score[i] /= sum_score;
    score[i] += i==0?0.0:score[i-1];
  }
  return;
}

void GP::select_gene(){
  random_device rd;
  mt19937 mt(rd());  
  uniform_real_distribution<double> rd_double(0.0,1.0);
  for(int i=0;i<G_NUM;i++){
    double s = rd_double(mt);
    for(int j=0;j<G_NUM;j++){
      if(s<score[j]){
	new_g[i] = new Gene(g[j]->get_root_t());
	break;
      }
    }
  }
  for(int i=0;i<G_NUM;i++){
    delete(g[i]);
    g[i] = new_g[i];
  }
  return;
}

void GP::cross_over(){
  random_device rd;
  mt19937 mt(rd());  
  uniform_real_distribution<double> rd_double(0.0,1.0);
  uniform_int_distribution<int> rd_int(0,G_NUM-2);
  for(int i=0;i<G_NUM;i++){
    if(rd_double(mt)<CO_P){
      int j = rd_int(mt);
      if(i<=j)j++;
      g[i]->crossover(g[j]->get_random_node());
      break;
    }
  }
  return;
}

void GP::mutation(){
  random_device rd;
  mt19937 mt(rd());  
  uniform_real_distribution<double> rd_double(0.0,1.0);
  for(int i=0;i<G_NUM;i++){
    if(rd_double(mt)<MU_P){
      g[i]->mutation();
    }
  }
  return;
}

void GP::view_info(){
  double mx = -100000.0;
  int p;
  for(int i=0;i<G_NUM;i++){
    double s = i==0?score[i]:(score[i]-score[i-1]);
    if(mx < s){
      mx = s;
      p = i;
    }
  }
  cout << " Max score gene is " << p << "\t";
  g[p]->print_score();
  g[p]->print_tree(g[p]->get_root_t());
  return;
}

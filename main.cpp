#include <vector>
#include <algorithm>
#include <iostream>
#include <cstdio>
#include <cmath>
#include <cstdlib>
#include <string>
#include "gp.h"
using namespace std;

#define LOOP 100000

int main(int argv,char ** argc){
  if(argv >= 2 && !strcmp(argc[1],"-s")){
    cout << "Gene sim mode : input gene data." << endl;
    string s;
    getline(cin, s);
    Gene g(s);
    g.print_tree(g.get_root_t());
    g.do_sim();
    return 0;
  }
  GP g;
  for(int lp=0;lp<LOOP;lp++){
    g.calc_score();
    cout << lp << "th Gen" ;
    g.view_info();
    cout << endl;
    g.select_gene();
    g.cross_over();
    g.mutation();
  }
  return 0;
}

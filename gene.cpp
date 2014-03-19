#include "gene.h"

//ランダムな木の生成時の各確率
#define RAND_FRONT 0.25
#define RAND_LR 0.125
#define RAND_IF 0.4
#define START_LEN 3
#define NUM_IF 9 //if文の種類の数
#define RAND_PROC 1.0 - RAND_FRONT - RAND_LR*2.0 - RAND_IF
//適応度計算時の重み付け変数
//算出式 : pow(W_POW,<生存時間>/<ヘビの長さ-初期長さ>*W_TIME + <ヘビの長さ>*W_LEN) + CON
#define W_LEN 1.0 
#define W_TIME -0.22
#define W_POW 1.23
#define CON 0.00
//シミュレーションの繰り返し回数
#define REP_S 100

gene_t_p Gene::get_random_tree(){
  //ランダムな木を再帰的に生成する
  gene_t_p tr = new gene_t;
  mt19937 mt(rd());
  uniform_real_distribution<double> rd_double(0.0,1.0);
  if(rd_double(mt)<RAND_FRONT){
    tr->mov = front;
  }else if(rd_double(mt) < RAND_FRONT + RAND_LR){
    tr->mov = l;
  }else if(rd_double(mt) < RAND_FRONT + RAND_LR*2.0){
    tr->mov = r;
  }else if(rd_double(mt) < RAND_FRONT + RAND_LR*2.0+RAND_IF){
    tr->mov = if_f;
    do{
    tr->lt = get_random_tree();
    tr->rt = get_random_tree();
    }while(tr->lt->mov == tr->rt->mov && tr->lt->mov != if_f && tr->lt->mov != proc);
    uniform_int_distribution<int> rd_int(0,NUM_IF-1);
    tr->n_func = rd_int(mt);
    switch(tr->n_func){
    case 0:
      tr->p = &Snake::get_l_snake;
      break;
    case 1:
      tr->p = &Snake::get_f_snake;
      break;
    case 2:
      tr->p = &Snake::get_r_snake;
      break;
    case 3:
      tr->p = &Snake::get_l_food;
      break;
    case 4:
      tr->p = &Snake::get_f_food;
      break;
    case 5:
      tr->p = &Snake::get_r_food;
      break;
    case 6:
      tr->p = &Snake::get_food_front;
      break;
    case 7:
      tr->p = &Snake::get_food_left_side;
      break;
    case 8:
      tr->p = &Snake::get_food_right_side;
      break;
     default:
       printf("Error if function\n");
       exit(1); 
    }
    
  }else{
    tr->mov = proc;
    do{
      tr->lt = get_random_tree();
      tr->rt = get_random_tree();
    }while(tr->lt->mov == tr->rt->mov && tr->lt->mov != if_f && tr->lt->mov != proc);
    
  }
  return tr;
}

Gene::Gene(){
  t_tr = get_random_tree();
  return;
}

Gene::Gene(gene_t_p g){
  t_tr = cp_tree(g);
  return;
}

Gene::Gene(string s){
  int t = s.size()-1;
  while(s[t] != ')' && !(s[t] >= 'a' && s[t] <= 'z')){
    t--;
  }
  t_tr = mk_gene_from_string(s,0,t);
  return;
}

Gene::~Gene(){
  free_tree(t_tr);
  return;
}

gene_t_p Gene::mk_gene_from_string(string s,int sn, int en){
  gene_t_p n = new gene_t();
  if(sn==en){
    n -> mov = s[sn] == 'l' ? l:(s[sn]=='f'?front:r);
  }else{
    sn++;
    en--;
    if(s[sn]=='i'){
      n -> mov = if_f;
      n -> n_func = s[sn+3] - '0';
      switch(s[sn+3]-'0'){
      case 0:
	n->p = &Snake::get_l_snake;
	break;
      case 1:
	n->p = &Snake::get_f_snake;
	break;
      case 2:
	n->p = &Snake::get_r_snake;
	break;
      case 3:
	n->p = &Snake::get_l_food;
	break;
      case 4:
	n->p = &Snake::get_f_food;
	break;
      case 5:
	n->p = &Snake::get_r_food;
	break;
      case 6:
	n->p = &Snake::get_food_front;
	break;
      case 7:
	n->p = &Snake::get_food_left_side;
	break;
      case 8:
	n->p = &Snake::get_food_right_side;
	break; 
      }
    }else{
      n -> mov = proc;
    }
    sn += 5;
    int sp = 0;
    int f = 0;
    for(int i=en;i>=sn;i--){
      if(f == 0 && s[i] == ' '){
	sp = i;break;
      }else if(s[i]==')'){
	f++;
      }else if(s[i]=='('){
	f--;
      }
    }
    n->lt = mk_gene_from_string(s,sn,sp-1);
    n->rt = mk_gene_from_string(s,sp+1,en);   
  }
  return n;
}

gene_t_p Gene::get_root_t(){
  return t_tr;
}

gene_t_p Gene::cp_tree(gene_t_p t){
  gene_t_p nt = new gene_t;
  nt -> mov = t-> mov;
  if(t->mov == if_f || t->mov == proc){
    nt->p = t->p;
    nt->n_func = t->n_func;
    nt->lt = cp_tree(t->lt);
    nt->rt = cp_tree(t->rt);
  }
  return nt;
}

void Gene::free_tree(gene_t_p t){
  if(t->mov == if_f || t->mov == proc){
    free_tree(t->rt);
    free_tree(t->lt);
  }
  delete(t);
  return;
}

void Gene::print_tree(gene_t_p t){
  switch(t->mov){
  case l:
    cout << "l";
    break;
  case front:
    cout << "f";
    break;
  case r:
    cout << "r";
    break;
  case if_f:
    cout << "(if-"<<t->n_func<<" ";
    print_tree(t->lt);
    cout <<" ";
    print_tree(t->rt);
    cout<<")";
    break;
  case proc:
    cout << "(proc ";
    print_tree(t->lt);
    cout <<" ";
    print_tree(t->rt);
    cout<<")";
    break;
  }
  return;
}

void Gene::print_score(){
  cout << " len : " << len << "\ttime : " << time << "\tgene : ";
  return;
}

void Gene::do_sim(){
  Snake s;
  Snake::mov_v v;
  do{
    if(t_stack.empty()){
      v = get_move(&s,t_tr);
    }else{
      v = get_move(&s,t_stack.top());
      t_stack.pop();
    }
    s.print_b();
    usleep(50000);
  }while(s.move(v));
}

double Gene::get_fitness(){
  len = time = 0.0;
  for(int i=0;i<REP_S;i++){
    Snake s;
    Snake::mov_v v;
    do{
      if(t_stack.empty()){
	v = get_move(&s,t_tr);
      }else{
	v = get_move(&s,t_stack.top());
	t_stack.pop();
      }
      //s.print_b();
    }while(s.move(v));
    len += (double)s.get_len();
    time += (double)s.get_time();
  }
  len /= (double)REP_S;
  time /= (double)REP_S;
  return pow(W_POW,time/(len-START_LEN+1.0)*W_TIME + len*W_LEN) + CON;
}

Snake::mov_v Gene::get_move(Snake * s, gene_t_p t){
  switch(t->mov){
  case l:
  case front:
  case r:
    return (Snake::mov_v)t->mov;
  case if_f:
    if((s->*(t->p))()){
      return get_move(s,t->lt);
    }else{
      return get_move(s,t->rt);
    }
  case proc:
    t_stack.push(t->rt);
    return get_move(s,t->lt);
  }
}

void Gene::mutation(){
  gene_t_p * t = get_random_node();
  free_tree(*t);
  *t = get_random_tree();
  return;
}

void Gene::get_node_list(gene_t_p t){
  if(t->mov==if_f||t->mov==proc){
    node_list.push_back(&(t->lt));
    node_list.push_back(&(t->rt));
    get_node_list(t->lt);
    get_node_list(t->rt);
  }
  return;
}


void Gene::shorten_gene(gene_t_p t){
  if(t->mov==if_f && t->lt->mov == t->rt->mov && t->lt->mov!=proc  && t->lt->mov!=if_f){
    t->mov = t->lt->mov;
    free(t->lt);
    free(t->rt);
  }else if(t->mov == if_f || t->mov == proc){
    shorten_gene(t->lt);
    shorten_gene(t->rt);
  }
  return;
}


gene_t_p * Gene::get_random_node(){
  node_list.clear();
  node_list.push_back(&t_tr);
  get_node_list(t_tr);
  mt19937 mt(rd());
  uniform_int_distribution<int> rd_int(0,node_list.size()-1);
  return node_list[rd_int(mt)];
}

void Gene::crossover(gene_t_p * n){
  gene_t_p * t = get_random_node();
  gene_t_p sp = *t;
  *t = *n;
  *n = sp;
  return;
}

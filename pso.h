#ifndef PSO_H_INCLUDED
#define PSO_H_INCLUDED
#include "utils.h"
#include "solver.h"
#include "randomTSP.h"
#include "argparser.h"

#include <vector>
#include <map>
#include <limits>
#include <cmath>
#include <iostream>
#include <fstream>

class ParticleSwarm {
 public:

   ParticleSwarm(Graph* g,RandomTSP* sol1)
        : G{g},solver1{sol1}
        {

        }




     vector< Solution* > my_solutions;
     vector< Solution* > my_solutions_temp;
     Solution* x_best;
    Solution* x_best_temp;
     vector< Solution* > my_solutions_comp;
          vector< Solution* > my_solutions_sorted;


  void GenerateParticles(int num){
    for( int i = 0; i < num; i++ )
  {
      solver1->solve();
      my_solutions.push_back( solver1->get_solution() );
  }

  }

vector< Solution* > SortParticles(vector< Solution* > my_solutions){

    my_solutions_sorted=my_solutions;
    int y, j, minim;
    Solution* temp;

   for (y = 0; y < my_solutions_sorted.size(); y++) {
      minim = y;
      for (j = y + 1; j < my_solutions_sorted.size(); j++)
      if (my_solutions_sorted[j]->get_objective() < my_solutions_sorted[minim]->get_objective())
      minim = j;
      temp = my_solutions_sorted[y];
      my_solutions_sorted[y] = my_solutions_sorted[minim];
      my_solutions_sorted[minim] = temp;
   }

   return my_solutions_sorted;
  }

  void solve_pso(int itterations){
      GenerateParticles(itterations);
      int k=0;
            my_solutions_temp=SortParticles(my_solutions);
            x_best_temp=my_solutions_temp[0];
            for (int p=1; p<my_solutions_temp.size(); p++) {my_solutions_comp.push_back(my_solutions_temp[p]);}
      while(k<itterations){
            for(int i=0;i<my_solutions_comp.size()-1;i++){
                my_solutions_comp[i]=update_rule(my_solutions_comp[i],x_best_temp);

            }
            for(int j=0;j<my_solutions_comp.size()-1;j++){
                if(my_solutions_comp[j]->get_objective()<x_best_temp->get_objective()){
                    x_best_temp=my_solutions_comp[j];
                }
            }
                    my_solutions_comp.clear();
                    my_solutions_comp.push_back(x_best_temp);
                    for (int l=0; l<my_solutions_temp.size(); l++) {my_solutions_comp.push_back(my_solutions_temp[l]);}
                    my_solutions_comp=SortParticles(my_solutions_comp);

        k++;

      }

      G->print_vertices(x_best_temp->get_solution());
      cout<<std::fixed<<"distance :"<<x_best_temp->get_objective()<<endl;

  }

  //applies update rule for a solution
  Solution *update_rule(Solution *x,Solution *x_b){
    double alpha=0.5;
    int index;
    int numswaps=0;

    vector<const Vertex *> V_best=G->save_vertices(x_b->get_solution());
    vector<const Vertex *> V_temp=G->save_vertices(x->get_solution());


    vector<const Vertex *> V_best1=G->save_vertices(x_b->get_solution());
    vector<const Vertex *> V_temp1=G->save_vertices(x->get_solution());
    int numswaps1=0;
    int numswaps2;

    Solution * x_temp = new Solution(x->get_solution(),G);
    Solution * x_best_temp = new Solution(x_b->get_solution(),G);

for(int u=0;u<V_temp.size()-1;u++){
    if(V_temp[u]->id!=V_best[u]->id){
        for (int e=0;e<V_temp.size()-1;e++){
            if (V_best[u]->id == V_temp[e]->id){

                index=e;
                x_temp->swap_vertices(u,index);
                V_temp=G->save_vertices(x_temp->get_solution());
                numswaps++;

            goto jump;
            }
        }
    }
jump:;

}
numswaps2=round(numswaps*alpha);

    for(int u=0;u<V_temp1.size()-1;u++){
    if(V_temp1[u]->id!=V_best1[u]->id){
        for (int e=0;e<V_temp1.size()-1;e++){
            if (V_best1[u]->id == V_temp1[e]->id){
                if(numswaps1==numswaps2){

                    goto jump2;
                }
                index=e;
                x_best_temp->swap_vertices(u,index);
                V_temp1=G->save_vertices(x_best_temp->get_solution());
                numswaps1++;
            goto jump1;
            }
        }
    }
jump1:;
}
jump2:;



  return x_best_temp;
  }

 private:

Graph* G;
RandomTSP* solver1;




};


#endif // PSO_H_INCLUDED

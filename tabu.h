#ifndef TABU_H_INCLUDED
#define TABU_H_INCLUDED

#include "utils.h"
#include "solver.h"
#include "randomTSP.h"
#include "argparser.h"

#include <vector>
#include <map>
#include <limits>
#include <cmath>
#include <iostream>
#include <algorithm>

class TabuSearch{
public:
    vector<const Vertex *> vertex_list;
       TabuSearch(Graph* g,RandomTSP* sol3)
        : G{g},solver3{sol3}
        {
        }

    vector< Solution* > generate_neighbourhood(Solution* sol){

        vertex_list=G->save_vertices(sol->get_solution());

        Solution *sol_temp=sol;
        vector< Solution* > neigh_solutions;

        for(int i=0;i<vertex_list.size()-1;i++){
                sol_temp=sol;
            for(int j=0;j<vertex_list.size()-1;j++){
                sol_temp->swap_vertices(i,j);
                if(sol_temp->is_feasible()==1){
                    neigh_solutions.push_back(sol_temp);
                }
            }
        }

    return neigh_solutions;

    }

    vector< Solution* > SortVector(vector< Solution* > my_solutions){

    vector<Solution *>my_solutions_sorted=my_solutions;
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

    void solve_Tabu(int itterations)
    {
    Solution*  init_sol;
    solver3->solve();
    init_sol=solver3->get_solution() ;
    vector<Solution*> neighbourhood_solutions=generate_neighbourhood(init_sol);
    vector<Solution*> TabuList;
    vector<Solution*> difference;

int i=0;


        while(i<itterations){
            neighbourhood_solutions=SortVector(neighbourhood_solutions);
            TabuList=SortVector(TabuList);

            difference.clear();

            set_difference(
            neighbourhood_solutions.begin(), neighbourhood_solutions.end(),
            TabuList.begin(), TabuList.end(),
            back_inserter( difference )
            );


            if (difference.size()==0){
                goto jump;
            }

            for(int i=0;i<difference.size()-1;i++){
                TabuList.insert (TabuList.begin(),init_sol);
                if(difference[i]->get_objective()<init_sol->get_objective()){
                    init_sol=difference[i];
                    TabuList.pop_back();
                }
            }
            neighbourhood_solutions=generate_neighbourhood(init_sol);
            i++;
        }
jump:
    G->print_vertices(init_sol->get_solution());
    cout<<std::fixed<<"distance :"<<init_sol->get_objective()<<endl;
;
    }


private:
Graph* G;
RandomTSP* solver3;
};


#endif // TABU_H_INCLUDED

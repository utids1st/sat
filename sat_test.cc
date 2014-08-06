#include <iostream>
#include <string>
#include <vector>

#include "global.h"
#include "solver.h"


using namespace std;

void usage(){
  cout << "usage:    ./solver input.cnf output.txt" << endl;
  return;
}

int main(int argc, char const *argv[])
{
	int N_clause, N_var; //number of Clause, variable
	
  if( argc != 3 ){
    usage();
    exit(1);
  }

	Constraint constraint = init_cons_stdin(N_clause, N_var, argv);
	// print_cons(constraint);


	//割り当て済み変数の管理
	// bitset<N_var> assigned; // なんかテンプレート引数に変数は使えないっぽい
	// std::vector<bool> assigned(N_var,false);

	// print_cons(constraint);

	bool sat_flag = false;
	Candidates candidates;
	for (int i = 0; i < N_var; ++i){
		std::vector<bool> v;
		v.push_back(true);
		v.push_back(false);
		candidates.push_back(v);
	}

	// print_candidates(candidates);

	Candidates answer = solve(constraint, candidates, sat_flag);

	if(sat_flag)
	{
		cout << "sat: " << endl;
		// print_ans(answer);
		print_ans_to_file(answer, argv[2]);
	}
	else
	{
		cout << "unsat." << endl;
	}
	return 0;
}

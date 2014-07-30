#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <cassert>
#include <bitset>
#include <algorithm>

typedef int Literal;
typedef std::vector<Literal> Clause;
typedef std::vector<Clause> Constraint;
typedef std::vector< std::vector<bool> >  Candidates;

using namespace std;

//lineから要素をいただく。各業はClauseを構成するLiteralで，０が来たら終了。
Clause get_line(string line){	
	stringstream strs(line);
	Clause clause;

	Literal literal;
	while(strs >> literal){
		if (literal==0){
			break;
		}
		clause.push_back(literal);
	}

	return clause;
}

void print_cons(const Constraint &constraint){
	int count = 0;
	for (Constraint::const_iterator i = constraint.begin(); i != constraint.end(); ++i) {
		++count;
		cout << "Constraint#" << count << ": ";
		for (Clause::const_iterator j = (*i).begin(); j != (*i).end(); ++j){
			cout << *j << " ";
		}
		cout << endl;
	}
}

Constraint init_cons(int &N_clause, int &N_var){
	cout << "name of input file: ";
	string fname;
	cin >> fname;
	ifstream ifs(fname.c_str());
	
	if(ifs.fail()) {
		cerr << "File do not exist.\n";
		exit(0);
	}

	string line;

	string hoge, format;

	//ここから入力ファイルの読み込み（コメント行と空行が冒頭にあって，
	//p行以降はコメント行や空行を含まず，制約が並んでいるコトを前提としている点は改善の余地あり）

	while(getline(ifs, line)) {
		if (line == ""){
			continue;
		}
		else if (line.at(0) == 'c'){
			continue;
		}
		else if (line.at(0) == 'p'){
			stringstream strs(line);
			strs >> hoge >> format >> N_clause >> N_var;
			break;
		}
	}	
	assert(hoge == "p");
	assert(format == "cnf");

	Constraint constraint;

	while(getline(ifs,line)){
		Clause clause;
		clause = get_line(line);

		constraint.push_back(clause);
	}
	
	return constraint;
}

int get_candidate(const Candidates candidates){
	int candidate = -1;
	int count = 0;
	for (Candidates::const_iterator i = candidates.begin(); i != candidates.end(); ++i) {
		if ((*i).size() == 2){
			candidate = count;
			break;
		}
	}

	return candidate;
}

bool is_include(std::vector<bool> v, bool val){
	std::vector<bool>::iterator i;
	i = find(v.begin(), v.end(), val);
	return i!=v.end() ? true : false;
}

bool check_constraint(const Constraint &constraint, const Candidates candidates){
	for (Constraint::const_iterator i = constraint.begin(); i != constraint.end(); ++i) {
		for (Clause::const_iterator j = (*i).begin(); j != (*i).end(); ++j) {
			//clause の中に ひとつはsatなものがあればよい
			if ( (*j)>=0 ){
				if(is_include(candidates.at(*j), true)){
					break;
				}
			}
			else{
				if(is_include(candidates.at(*j), false)){
					break;
				}
			}
		}
		return false; //unsat
	}
	return true; // every clauses are satisfied
}

bool set_candidate(const Constraint &constraint, Candidates &candidates, int candidate, bool val){
	assert(candidates.at(candidate).size() == 2);

	candidates.at(candidate).clear();
	candidates.at(candidate).push_back(val);

	return check_constraint(constraint, candidates);
}

Candidates dpll_solve(const Constraint &constraint, Candidates &candidates, int depth, bool sat_flag){
	if (sat_flag){
		return candidates;
	}

	int candidate = get_candidate(candidates);
	if (candidate==-1) { // find sat_answer!
		sat_flag = true;
		return candidates;
	}

	Candidates candidates1(candidates);
	if (set_candidate(constraint, candidates1, candidate, true)){
		Candidates ans1 = dpll_solve(constraint, candidates1, depth+1, sat_flag);
		if (sat_flag){ return ans1; }
	}


	Candidates candidates2(candidates);
	if (set_candidate(constraint, candidates2, candidate, false)){
		Candidates ans2 = dpll_solve(constraint, candidates2, depth+1, sat_flag);
		if (sat_flag){ return ans2; }
	}

	Candidates fail;
	return fail; //ゴミ
}

Candidates solve(const Constraint &constraint, Candidates &candidates, bool sat_flag){
	return dpll_solve(constraint, candidates, 0, sat_flag); // 0 is depth of root
}


int main(int argc, char const *argv[])
{
	int N_clause, N_var; //number of Clause, variable

	Constraint constraint = init_cons(N_clause, N_var);
	
	//割り当て済み変数の管理
	// bitset<N_var> assigned; // なんかテンプレート引数に変数は使えないっぽい
	std::vector<bool> assigned(N_var,false);

	// print_cons(constraint);

	bool sat_flag = false;
	Candidates candidates;
	for (int i = 0; i < N_var; ++i){
		std::vector<bool> v;
		v.push_back(true);
		v.push_back(false);
		candidates.push_back(v);
	}

	Candidates answer = solve(constraint, candidates, sat_flag);

	if(sat_flag)
	{
		cout << "sat: " << endl;
		// print_ans(answer);
	}
	else
	{
		cout << "unsat." << endl;
	}
	return 0;
}
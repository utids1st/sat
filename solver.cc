#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <cassert>
#include <bitset>
#include <algorithm>
#include <iomanip>    // manipulator setw()

//constraint,clauseに入っている変数は１スタートであることに注意（constraintで-を使って否定を表しているので0スタートだと不便）

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
			strs >> hoge >> format >> N_var >> N_clause;
			break;
		}
	}	
	assert(hoge == "p");
	assert(format == "cnf");

	Constraint constraint;

	for(int i = 0; i < N_clause; ++i){ 
		getline(ifs,line);
		Clause clause;
		clause = get_line(line);

		constraint.push_back(clause);
	}
	
	return constraint;
}

Constraint init_cons_stdin(int &N_clause, int &N_var, char const *argv[] ){
	cout << "input file is ... " << argv[1] << endl;
	string fname = argv[1];
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
			strs >> hoge >> format >> N_var >> N_clause;
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
		count++;
	}

	return candidate;
}

bool is_include(const std::vector<bool> v, bool val){
	std::vector<bool>::const_iterator i;
	i = find(v.begin(), v.end(), val);
	return i!=v.end() ? true : false;
}

void print_candidates(const Candidates v, const int depth){
	cout << "now in depth : " << depth << "\n";

	int count = 0;
	for (Candidates::const_iterator i = v.begin(); i != v.end(); ++i){
		++count;
		cout << "var#" << count << " ";
		for (std::vector<bool>::const_iterator j = (*i).begin(); j != (*i).end(); ++j){
			cout << *j << ":";
		}
		cout << "\n";
	}
	cout << "\n";
}

bool check_constraint(const Constraint &constraint, const Candidates candidates){
	// print_cons(constraint);
	// print_candidates(candidates, 0);
	for (Constraint::const_iterator i = constraint.begin(); i != constraint.end(); ++i) {
		for (Clause::const_iterator j = (*i).begin(); j != (*i).end(); ++j) {
			//clause の中に ひとつはsatなものがあればよい
			// cout << "var # " << *j << endl;

			if ( (*j)>=0 ){
				if(is_include(candidates.at((*j)-1), true)){
					// cout << "break1!\n";
					goto NEXT_CLAUSE;
				}
			}
			else{
				if(is_include(candidates.at(-(*j)-1), false)){
					// cout << "break2!\n";
					goto NEXT_CLAUSE;
				}
			}
			// cout << "ret false!\n\n";
		}
		return false; //unsat

		NEXT_CLAUSE: //this clause is satisfied
		;
	}
	// cout << "ret true!\n";
	return true; // every clauses are satisfied
}

bool set_candidate(const Constraint &constraint, Candidates &candidates, int candidate, bool val){
	assert(candidates.at(candidate).size() == 2);

	candidates.at(candidate).clear();
	candidates.at(candidate).push_back(val);
	// cout << val << endl;
	// print_candidates(candidates, 1);

	return check_constraint(constraint, candidates);
}

Candidates dpll_solve(const Constraint &constraint, Candidates &candidates, int depth, bool &sat_flag){
	// cout << "what's happens?\n";
	// print_candidates(candidates,depth);

	if (sat_flag){
		return candidates;
	}

	int candidate = get_candidate(candidates);
	// cout << candidate << endl;
	if (candidate==-1) { // find sat_answer!
		// cout << "sat!!!!!" << endl;
		sat_flag = true;
		return candidates;
	}

	Candidates candidates1(candidates);
	if (set_candidate(constraint, candidates1, candidate, true)){
		// print_candidates(candidates1, depth);
		// cout << "set!\n";
		Candidates ans1 = dpll_solve(constraint, candidates1, depth+1, sat_flag);
		if (sat_flag){ return ans1; }
	}


	Candidates candidates2(candidates);
	if (set_candidate(constraint, candidates2, candidate, false)){
		// print_candidates(candidates2 , depth);
		Candidates ans2 = dpll_solve(constraint, candidates2, depth+1, sat_flag);
		if (sat_flag){ return ans2; }
	}

	Candidates fail;
	return fail; //ゴミ
}

Candidates solve(const Constraint &constraint, Candidates &candidates, bool &sat_flag){
	return dpll_solve(constraint, candidates, 0, sat_flag); // 0 is depth of root
}

void print_ans(const Candidates answer){
	int count = 0;
	for (Candidates::const_iterator i = answer.begin(); i != answer.end(); ++i){
		++count;
		assert((*i).size() == 1);
		cout << "var #" << count << " is " << (*i).at(0) << endl;
	}
}

void print_ans_to_file(const Candidates answer, const char * filename ){
	int count = 0;
	ofstream fout( filename );
	for (Candidates::const_iterator i = answer.begin(); i != answer.end(); ++i){
		++count;
		assert((*i).size() == 1);
		fout <<  (*i).at(0) << " (var# " << setw(6) /*fixed width*/ << count << " )" << endl;
	}
	cout << "   written in ... " << filename << endl;
	return ;
}

void usage(){
  cout << "usage:    ./solver input.cnf output.txt" << endl;
  return ;
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
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <cassert>

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

void print_cons(const Constraint constraint){
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

Constraint dpll_solve(const Constraint constraint, Candidates candidates, int depth, bool sat_flag){
	if (sat_flag){ return constraint; }


}

Constraint solve(const Constraint constraint, Candidates candidates, bool sat_flag){
	return dpll_solve(constraint, candidates, 0, sat_flag); // 0 is depth
}


int main(int argc, char const *argv[])
{
	int N_clause, N_var; //number of Clause, variable

	Constraint constraint = init_cons(N_clause, N_var);

	// print_cons(constraint);
	
	bool sat_flag = false;
	Candidates candidates;

	// Constraint ans = solve(constraint, candidates, sat_flag);

	if(sat_flag)
	{
		cout << "sat: " << endl
			 << "ans" << endl;
	}
	else
	{
		cout << "unsat." << endl;
	}
	return 0;
}
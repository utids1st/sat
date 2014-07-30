#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <cassert>

typedef int Literal;
typedef std::vector<Literal> Clause;
typedef std::vector<Clause> Constraint;

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

void print_cons(Constraint constraint){
	int count = 0;
	for (Constraint::iterator i = constraint.begin(); i != constraint.end(); ++i) {
		++count;
		cout << "Constraint#" << count << ": ";
		for (Clause::iterator j = (*i).begin(); j != (*i).end(); ++j){
			cout << *j << " ";
		}
		cout << endl;
	}
}

Constraint init_cons(){
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
	int N_clause, N_var; //number of Clause, variable


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

int main(int argc, char const *argv[])
{
	Constraint constraint = init_cons();

	// デバッグ用
	// print_cons(constraint);

	if(1)//solved 
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
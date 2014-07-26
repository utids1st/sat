#include <iostream>
#include <string>
#include <fstream>

using namespace std;

int main(int argc, char const *argv[])
{
	string fname;
	cin >> fname;
	ifstream ifs(fname.c_str());

	string line;
	while(getline(ifs, line)){
		cout << line << endl;
	}
	return 0;
}
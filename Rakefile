require 'rake/clean'

CC="clang++"

SRCS = FileList["**/*.cc"]
OBJS = SRCS.ext('o')

CLEAN.include(OBJS)

task :test => "solver_test"

file "solver_test" => ["solver_test.o"] do
	sh "#{CC} -o solver_test solver_test.o"
end

file "solver_test.o" => "solver_test.cc" do
	sh "#{CC} -c solver_test.cc"
end

task :default => "solver"

file "solver" => ["solver.o"] do
	sh "#{CC} -o solver solver.o"
end

file "solver.o" => "solver.cc" do
	sh "#{CC} -c solver.cc"
end
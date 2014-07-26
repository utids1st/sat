require 'rake/clean'

CC="clang++"

SRCS = FileList["**/*.cc"]
OBJS = SRCS.ext('o')

CLEAN.include(OBJS)

task :default => "solver"

file "solver" => ["solver.o"] do
	sh "#{CC} -o solver solver.o"
end

file "solver.o" => "solver.cc" do
	sh "#{CC} -c solver.cc"
end
sat solver

rake && ./solver
を実行して，
DIMACSフォーマットの.cnfファイルの名前を入力したら解いてくれます。
(サンプルとしてsample.cnfが入っています)

http://www.cs.ubc.ca/~hoos/SATLIB/benchm.html
にベンチマーク用のDIMACSフォーマットの問題がたくさんあります。

見本
minisat
grasp

DIMACS CNF
http://www.pied-piper.net/note/note.cgi?10

minisat on mac os
http://www.nishanttotla.com/researchblog/2013/10/29/running-minisat-on-mac-osx/
https://groups.google.com/forum/#!topic/minisat/0mGR5vei9Sc

dpll法
backtrack -> back-jump(このためにはスタック管理をして自分で再帰の実装をする必要がある)
constraint propagation

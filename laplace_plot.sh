#!/bin/bash
#Usage:
#./laplace_plot.sh 200 200 .05 9 1 1 1
NX=${1:-330}
NY=${2:-70}
EPS=${3:-0.05}
FX=${4:-30}
FY=${5:-55}
FG=${6:-30}
./bin/laplace -nx $NX -ny $NY -eps $EPS -fx $FX -fy $FY -fg $FG > "./out/output.csv"
R --no-save << EOT
A = scan(file="./out/output.csv",sep=",")
A = matrix(A,ncol=($NX*$NY),nrow=3)
A = t(A)
par(pty="s")
palette(rainbow(384,end=.75))
plot(A[,2],A[,1],col=A[,3],xlab="X",ylab="Y",main="Laplace Project",pch=" ")
points(A[,2],A[,1],col=A[,3])
quit("no")
EOT

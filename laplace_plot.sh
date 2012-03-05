#!/bin/bash
#Usage:
#./laplace_plot.sh 200 200 .05 9 1 1 1
NX=${1:-200}
NY=${2:-200}
EPS=${3:-.05}
FX=${4:-9}
FY=${5:-1}
FG=${6:-1}
BH=${7:-1}
./bin/laplace -nx $NX -ny $NY -eps $EPS -fx $FX -fy $FY -fg $FG -bh $BH > "./out/output.csv"
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

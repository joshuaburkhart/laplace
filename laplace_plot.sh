#!/bin/bash
#Usage:
#./laplace_plot.sh 200 200 .05 9 1 1 1

./bin/laplace -nx $1 -ny $2 -eps $3 -fx $4 -fy $5 -fg $6 -bh $7 > "./out/output.csv"
R --no-save << EOT
A = scan(file="./out/output.csv",sep=",")
A = matrix(A,ncol=($1*$2),nrow=3)
A = t(A)
par(pty="s")
palette(rainbow(384,end=.75))
plot(A[,2],A[,1],col=A[,3],xlab="X",ylab="Y",main="Laplace Project",pch=" ")
points(A[,2],A[,1],col=A[,3])
quit("no")
EOT

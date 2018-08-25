#include<stdio.h>
#include<stdlib.h>
int main(){
int AX,BX,CX,DX,EX,FX,GX,HX;
int B;int A;int C[4];int D;const int E=0;
scanf("%d",&AX);
scanf("%d",&BX);
A=AX;
B=BX;
CX=AX+BX;
X:
scanf("%d",&AX);
DX=AX-BX;
printf("%d\t",DX);
printf("%d\t",CX);
if(CX!=DX){
C[0]=CX;
printf("%d\t",C[0]);
}
else{
C[1]=DX;
printf("%d\t",C[1]);
goto X;
}
printf("%d\t",E);
getchar();
return 0;
}

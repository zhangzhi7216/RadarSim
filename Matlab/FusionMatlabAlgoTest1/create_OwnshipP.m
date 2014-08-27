%产生本机的真值
function [OwnshipPNew]=create_OwnshipP(T,OwnshipP)
% 赋本机前状态矩阵

F=[1 T 0.5*T^2 0 0 0 0 0 0;
   0 1 T 0 0 0 0 0 0;
   0 0 1 0 0 0 0 0 0;
   0 0 0 1 T 0.5*T^2 0 0 0;
   0 0 0 0 1 T 0 0 0;
   0 0 0 0 0 1 0 0 0;
   0 0 0 0 0 0 1 T 0.5*T^2;
   0 0 0 0 0 0 0 1 T;
   0 0 0 0 0 0 0 0 1];

    OwnshipPNew=F*OwnshipP(:,end);
    
end
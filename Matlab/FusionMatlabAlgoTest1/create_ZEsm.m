%产生观测值

function [Z]=create_ZEsm(X,R,Ownship)

%产生观测值
    V=[normrnd(0,R(1,1),1);normrnd(0,R(2,2),1);normrnd(0,R(3,3),1)];
    x=X(1,end);
    y=X(4,end);
    xo1=Ownship(1).P(1,end);
    yo1=Ownship(1).P(4,end);
    xo2=Ownship(2).P(1,end);
    yo2=Ownship(2).P(4,end);
    xo3=Ownship(3).P(1,end);
    yo3=Ownship(3).P(4,end);
    RAB = [atan((y-yo1)/(x-xo1));atan((y-yo2)/(x-xo2));atan((y-yo3)/(x-xo3))];
    Z(:,1) = RAB + V;
end
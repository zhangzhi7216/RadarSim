%产生观测值

function [Z]=create_ZInf(X,R,Ownship)

%产生观测值
    %产生噪声
   % V=mvnrnd(zeros(1,3),R,1)';
    V=[normrnd(0,R(1,1),1);normrnd(0,R(2,2),1);normrnd(0,R(1,1),1);normrnd(0,R(2,2),1);normrnd(0,R(1,1),1);normrnd(0,R(2,2),1)];
    x=X(1,end);
    y=X(4,end);
    z=X(7,end);
    xo1=Ownship(1).P(1,end);
    yo1=Ownship(1).P(4,end);
    zo1=Ownship(1).P(7,end);
    xo2=Ownship(2).P(1,end);
    yo2=Ownship(2).P(4,end);
    zo2=Ownship(2).P(7,end);
    xo3=Ownship(3).P(1,end);
    yo3=Ownship(3).P(4,end);
    zo3=Ownship(3).P(7,end);
    RAB = [atan((y-yo1)/(x-xo1));atan((z-zo1)/sqrt(((x-xo1)^2+(y-yo1)^2)));atan((y-yo2)/(x-xo2));atan((z-zo2)/sqrt(((x-xo2)^2+(y-yo2)^2)));atan((y-yo3)/(x-xo3));atan((z-zo3)/sqrt(((x-xo3)^2+(y-yo3)^2)))];
    Z(:,1) = RAB + V;
end

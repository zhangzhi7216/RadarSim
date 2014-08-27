function [Z]=create_ZInfEquivalent(X,R,Ownship,mode)

%循环产生观测值
Size=size(X);
for i=1:Size(2)
    %产生噪声
   % V=mvnrnd(zeros(1,3),R,1)';
    x=X(1,i);
    y=X(2,i);
    z=X(3,i);
    xo1=Ownship(1).P(1,i);
    yo1=Ownship(1).P(4,i);
    zo1=Ownship(1).P(7,i);
    RAB = [atan((y-yo1)/(x-xo1));atan((z-zo1)/sqrt(((x-xo1)^2+(y-yo1)^2)))];
    Z(:,i) = RAB;
end;

end
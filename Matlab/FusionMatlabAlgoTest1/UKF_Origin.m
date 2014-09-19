%UKF (无迹卡尔曼滤波器）

function [X_eNew,P_New]=UKF_Origin(T,Q,R,X_e,Z,Ownship,P)

%赋初值
L=15;      %维数
alpha=1e-4;
kappa=0;
beta=2;
lambda=alpha^2*(L+kappa)-L;

w(1,1)=lambda /(L+lambda);
v(1,1)=lambda /(L+lambda)+1-alpha^2+beta;
for j=2:(2*L+1)
    w(1,j) = 1 / ( 2 * ( L + lambda));
    v(1,j) = 1 / ( 2 * ( L + lambda));
end


%状态转移矩阵
F=[1 T 0.5*T^2 0 0 0 0 0 0;
   0 1 T 0 0 0 0 0 0;
   0 0 1 0 0 0 0 0 0;
   0 0 0 1 T 0.5*T^2 0 0 0;
   0 0 0 0 1 T 0 0 0;
   0 0 0 0 0 1 0 0 0;
   0 0 0 0 0 0 1 T 0.5*T^2;
   0 0 0 0 0 0 0 1 T;
   0 0 0 0 0 0 0 0 1];
G=[0.5*T^2 0 0;
    T 0 0;
    1 0 0;
    0 0.5*T^2 0;
    0 T 0;
    0 1 0;
    0 0 0.5*T^2;
    0 0 T;
    0 0 1];

Q=eye(3)*Q;
%滤波
    X_extend=[X_e;zeros(6,1)];
    P_extend=[P,zeros(9,6);zeros(3,9),Q,zeros(3,3);zeros(3,12),R];
    
    %计算sigma点
    P_sqrt=schol(P_extend);
    X_sigma(:,1)=X_extend;
    for j=1:L
        X_sigma(:,j+1)=X_extend+(L+lambda)^0.5*P_sqrt(:,j);
        X_sigma(:,j+L+1)=X_extend-(L+lambda)^0.5*P_sqrt(:,j);
    end
    %计算转移后的sigma点
    for j=1:(2*L+1)
        Xi_sigma(:,j)=F*X_sigma(1:9,j)+G*X_sigma(10:12,j);
    end
    %计算状态先验估计值
    X_eNew(:,1)=Xi_sigma*w';
    
    %计算状态预测协方差
    P_x=zeros(9,9);
    for j=1:(2*L+1)
        P_x=P_x+v(1,j)*(Xi_sigma(:,j)-X_eNew(:,1))*(Xi_sigma(:,j)-X_eNew(:,1))';
    end
    
        xo1=Ownship(1).P(1,1);
        yo1=Ownship(1).P(4,1);
        xo2=Ownship(2).P(1,1);
        yo2=Ownship(2).P(4,1);
        xo3=Ownship(3).P(1,1);
        yo3=Ownship(3).P(4,1);
    
    %计算观测的sigma点
    for j=1:(2*L+1)
        x=Xi_sigma(1,j);y=Xi_sigma(4,j);
        Z_sigma(:,j)=[atan((y-yo1)/(x-xo1));atan((y-yo2)/(x-xo2));atan((y-yo3)/(x-xo3))]+X_sigma(13:15,j);%此处需修改
    end
    
   %计算观测估计值
   Z_eNew(:,1)=Z_sigma*w';
   
   %计算观测的方差
   P_z=zeros(3,3);
   for j=1:(2*L+1)
       P_z=P_z+v(1,j)*(Z_sigma(:,j)-Z_eNew(:,1))*(Z_sigma(:,j)-Z_eNew(:,1))';
   end
   
   %计算X与Z的交叉方差
   P_xz=zeros(9,3);
   for j=1:(2*L+1)
       P_xz=P_xz+v(1,j)*(Xi_sigma(:,j)-X_eNew(:,1))*(Z_sigma(:,j)-Z_eNew(:,1))';
   end
   %测量更新（修正）
   Residual=Z-Z_eNew(:,1);
   K=P_xz*inv(P_z);
   X_eNew(:,1)=X_eNew(:,1)+K*Residual;
   P_New=P_x-K*P_z*K';
%    msgbox(num2str(K),'K');

end

function [ fusionDatas, filterDatas, controlDatas, globalVarsOutput ] = FusionMatlabAlgoTest1( planeTrueDatas, targetNoiseDatas, globalVars, interval, infraredMaxDis )

% set(msg123,'Position',[1000 100 400 300]);

T = interval;
% sigma = 0.05;                        %过程噪声均方差
% Q = sigma.^2;                   %过程噪声方差
% Q = diag([40000,200,0.1,40000,200,0.1,40000,200,0.1]); 
G = [T^2/2 0 0;T 0 0;1 0 0;0 T^2/2 0;0 T 0;0 1 0;0 0 T^2/2;0 0 T;0 0 1];
Onum = size(planeTrueDatas,1);                      %我机个数
Tnum = size(targetNoiseDatas,1)/Onum;                          %敌机个数

globalVarsMatrix = zeros(Tnum+Onum,25,25);
for k = 1:Tnum+Onum
    for i = 1:25
        globalVarsMatrix(k,:,i) = permute(globalVars(k,(25*i-24):25*i),[3 2 1]);         
    end
end
% globalVarsMatrixOutput = globalVarsMatrix;
globalVarsMatrixOutput = zeros(Tnum+Onum,25,25);

targetNoiseDatas(:,4:5) = targetNoiseDatas(:,4:5)/57.2957795130823208768;

for k = 1:Onum
    planePitch = atan(planeTrueDatas(k,7)/planeTrueDatas(k,6));
    planeYaw = atan(planeTrueDatas(k,8)/sqrt(planeTrueDatas(k,7)^2+planeTrueDatas(k,6)^2));
    targetNoiseDatas((4*k-3):4*k,4) = targetNoiseDatas((4*k-3):4*k,4) + permute([planePitch;planePitch;planePitch;planePitch],[1 3 2]);
    targetNoiseDatas((4*k-3):4*k,5) = targetNoiseDatas((4*k-3):4*k,5) + permute([planeYaw;planeYaw;planeYaw;planeYaw],[1 3 2]);
end

targetNoiseDatasTrans = zeros(Onum,Tnum,8);
for k = 1:Onum
    for i = 1:Tnum
        targetNoiseDatasTrans(k,i,:) = permute(targetNoiseDatas(4*k-4+i,:),[1 3 2]); 
    end    
end
VarREsm = diag([(sqrt(targetNoiseDatasTrans(1,1,7))/180*pi)^2,(sqrt(targetNoiseDatasTrans(2,1,7))/180*pi)^2,(sqrt(targetNoiseDatasTrans(3,1,7))/180*pi)^2]);
VarRInf = diag([(sqrt(targetNoiseDatasTrans(1,1,7))/180*pi)^2,(sqrt(targetNoiseDatasTrans(1,1,8))/180*pi)^2,(sqrt(targetNoiseDatasTrans(2,1,7))/180*pi)^2,(sqrt(targetNoiseDatasTrans(2,1,8))/180*pi)^2,(sqrt(targetNoiseDatasTrans(3,1,7))/180*pi)^2,(sqrt(targetNoiseDatasTrans(3,1,8))/180*pi)^2]);
Association = zeros(10,1);%初始化目标关联矩阵


if(planeTrueDatas(1,1)==0)
        globalVarsMatrix(1,1,1:11) = permute([0;100;400000+20000;120500+6000;20100;-340-30;5;0;0;0;0],[3 2 1]);
        globalVarsMatrix(2,1,1:11) = permute([0;101;400000+18000;165000-5000;19000;-340-28;0;0;0;0;0],[3 2 1]);
        globalVarsMatrix(3,1,1:11) = permute([0;102;400000+19000;150000+6000;20000;-340-29;0;0;0;0;0],[3 2 1]);
        globalVarsMatrix(4,1,1:11) = permute([0;103;400000+20000;135000+5000;21000;-340-30;0;0;0.05;0;0],[3 2 1]);
end
% msg10 = msgbox(num2str([globalVarsMatrix(1,1,1:11);globalVarsMatrix(2,1,1:11);globalVarsMatrix(3,1,1:11);globalVarsMatrix(4,1,1:11)]),'globalstate');
% set(msg10,'Position',[1000 100 300 150]);
%msgbox(num2str(targetNoiseDatasTrans(1:1, 2:2)));
%msgbox(num2str(targetNoiseDatasTrans(2:2, 2:2)));
%msgbox(num2str(targetNoiseDatasTrans(3:3, 2:2)));

%计算我机与敌机最大距离，辨别是否离开Esm滤波区域
for k = 1:Tnum
    for i = 1:Onum
%        distanceEsm(k,i) = ((globalVarsMatrix(k,1,3)-planeTrueDatas(i,3))^2+(globalVarsMatrix(k,1,4)-planeTrueDatas(i,4))^2+(globalVarsMatrix(k,1,5)-planeTrueDatas(i,5))^2)^0.5;
       distanceEsm(k,i) = ((globalVarsMatrix(k,1,3)-planeTrueDatas(i,3))^2+(globalVarsMatrix(k,1,4)-planeTrueDatas(i,4))^2)^0.5;
    end
end
DistanceMax = max(max(distanceEsm));
% msgbox(num2str(DistanceMax));
if(DistanceMax>3.5*infraredMaxDis)
   Q = G*diag([2e-1,2e-1,1e-4])*G';                   %过程噪声方差
elseif(DistanceMax>3*infraredMaxDis && DistanceMax<=3.5*infraredMaxDis)
   Q = G*diag([4e-4,4e-4,1e-4])*G';
else
   Q = G*diag([4e-4,4e-4,1e-4])*G'; 
end
%Esm探测范围内滤波
if(DistanceMax>infraredMaxDis)
% if(DistanceMax<3e5)
 if(planeTrueDatas(1,1)~=0)
    %赋初值
   for k=1:Tnum
      Target(k).ZEsm = zeros(Onum,1);
      Target(k).P_UKFEsm = permute(globalVarsMatrix(k,1:9,12:20),[3,2,1]);
   end
   
   %根据ID号对Esm段滤波进行关联
   for k=1:Onum
       for i=1:Tnum
            TargetID = targetNoiseDatasTrans(k,i,2)+1-100;
            Target(TargetID).ZEsm(k,1) = targetNoiseDatasTrans(k,i,4); 
       end
   end
   %输入我机的位置
   for k=1:Onum
       Ownship(k).P = (planeTrueDatas(k,3:11))';
       Ownship(k).P = [Ownship(k).P(1,1);Ownship(k).P(4,1);Ownship(k).P(7,1);Ownship(k).P(2,1);Ownship(k).P(5,1);Ownship(k).P(8,1);Ownship(k).P(3,1);Ownship(k).P(6,1);Ownship(k).P(9,1)];
   end

   %输入敌机的位置
   for k=1:Tnum
       Target(k).X_e_UKFEsm = permute(globalVarsMatrix(k,1,3:11),[3,2,1]);
       Target(k).X_e_UKFEsm = [Target(k).X_e_UKFEsm(1,1);Target(k).X_e_UKFEsm(4,1);Target(k).X_e_UKFEsm(7,1);Target(k).X_e_UKFEsm(2,1);Target(k).X_e_UKFEsm(5,1);Target(k).X_e_UKFEsm(8,1);Target(k).X_e_UKFEsm(3,1);Target(k).X_e_UKFEsm(6,1);Target(k).X_e_UKFEsm(9,1)];
   end
%     msg11 = msgbox(num2str([Ownship(1).P,Ownship(2).P,Ownship(3).P]),'Ownship');
%     set(msg11,'Position',[1000 500 300 150]);
%     msgbox(num2str([Target(1).ZEsm,Target(2).ZEsm,Target(3).ZEsm,Target(4).ZEsm]),'ZEsm');
%     msg123 = msgbox(num2str([Target(1).X_e_UKFEsm,Target(2).X_e_UKFEsm,Target(3).X_e_UKFEsm,Target(4).X_e_UKFEsm]),'xukfesm');
%     set(msg123,'Position',[500 100 300 150]);
%     msg1 = msgbox(num2str(Target(3).P_UKFEsm),'PUKFEsmafter');
%     set(msg1,'Position',[100 300 400 400]);
   %ESM-UKF滤波
    for k = 1:Tnum
       [Target(k).X_e_UKFEsmNew,Target(k).P_UKFEsmNew]=UKF_Origin(T,Q,VarREsm,Target(k).X_e_UKFEsm,Target(k).ZEsm,Ownship,Target(k).P_UKFEsm);
       Target(k).X_e_UKFEsm = [Target(k).X_e_UKFEsmNew(1,1);Target(k).X_e_UKFEsmNew(4,1);Target(k).X_e_UKFEsmNew(7,1);Target(k).X_e_UKFEsmNew(2,1);Target(k).X_e_UKFEsmNew(5,1);Target(k).X_e_UKFEsmNew(8,1);Target(k).X_e_UKFEsmNew(3,1);Target(k).X_e_UKFEsmNew(6,1);Target(k).X_e_UKFEsmNew(9,1)];
       globalVarsMatrixOutput(k,1,1) = planeTrueDatas(1,1);
       globalVarsMatrixOutput(k,1,2) = 100+k-1;
       globalVarsMatrixOutput(k,1,3:11) = permute(Target(k).X_e_UKFEsm,[3,2,1]);
       Target(k).P_UKFEsm = Target(k).P_UKFEsmNew;
       globalVarsMatrixOutput(k,1:9,12:20) = permute(Target(k).P_UKFEsm,[3,2,1]);
    end
%     h = msgbox(num2str([Target(1).X_e_UKFEsm,Target(2).X_e_UKFEsm,Target(3).X_e_UKFEsm,Target(4).X_e_UKFEsm]),'XEUKFafter');
%     set(h,'Position',[100 100 300 300]);
       globalVarsOutput = zeros(Tnum+Onum,625);
       for k = 1:Tnum+Onum
           for i = 1:25
               globalVarsOutput(k,(25*i-24):25*i) = permute(globalVarsMatrixOutput(k,:,i),[3 2 1]);         
           end
       end
    %输出融合值
    fusionDatasOutput = zeros(Tnum,11);
    filterDatasOutput = zeros(Tnum,11);
    controlDatas = zeros(Onum,3);
    for k = 1:Tnum
    fusionDatasOutput(k,1) = planeTrueDatas(1,1);
    fusionDatasOutput(k,2) = 100+k-1;
    fusionDatasOutput(k,3:11) = Target(k).X_e_UKFEsm';
    filterDatasOutput(k,1) = planeTrueDatas(1,1);
    filterDatasOutput(k,2) = 100+k-1;
    filterDatasOutput(k,3:11) = Target(k).X_e_UKFEsm';
    end
    fusionDatas = fusionDatasOutput;
    filterDatas = filterDatasOutput;
%     msg2 = msgbox(num2str(globalVarsOutput),'globalOut');
%     set(msg2,'Position',[1000 1000 400 300]);
%     msgbox(num2str(size(fusionDatas)),'fusion');
%     msgbox(num2str(size(filterDatas)),'filter');
%     msgbox('Message 1');
 else
   for k=1:Tnum
       Target(k).X_e_UKFEsm = permute(globalVarsMatrix(k,1,3:11),[3,2,1]);
       Target(k).P_UKFEsm = diag([4e-4,4e-4,4e-4,4e-4,4e-4,4e-4,4e-4,4e-4,4e-4]);
       globalVarsMatrixOutput(k,1,1) = planeTrueDatas(1,1);
       globalVarsMatrixOutput(k,1,2) = 100+k-1;
       globalVarsMatrixOutput(k,1,3:11) = permute(Target(k).X_e_UKFEsm,[3,2,1]);
       globalVarsMatrixOutput(k,1:9,12:20) = permute(Target(k).P_UKFEsm,[3,2,1]);
    end
       globalVarsOutput = zeros(Tnum+Onum,625);
       for k = 1:Tnum+Onum
           for i = 1:25
               globalVarsOutput(k,(25*i-24):25*i) = permute(globalVarsMatrixOutput(k,:,i),[3 2 1]);         
           end
       end
    fusionDatasOutput = zeros(Tnum,11);
    filterDatasOutput = zeros(Tnum,11);
    controlDatas = zeros(Onum,3);
    for k = 1:Tnum
       fusionDatasOutput(k,:) = (permute(globalVarsMatrix(k,1,1:11),[3 2 1]))';
       filterDatasOutput(k,:) = (permute(globalVarsMatrix(k,1,1:11),[3 2 1]))';
    end
    fusionDatas = fusionDatasOutput;
    filterDatas = filterDatasOutput;
 end
% elseif(DistanceMax<=1e5)
else
% elseif(DistanceMax>3e5)
Q = G*diag([4e-4,4e-4,1e-4])*G';                   %过程噪声方差
if(any(any(targetNoiseDatasTrans(:,:,4)))==1)
   %判断未被击落的敌机目标
   TnumExist = 0;
   for k=1:Tnum
       if(any(targetNoiseDatasTrans(:,k,4))==1)
           TnumExist = TnumExist+1;
           TargetExist(TnumExist,1) = k;
       end
   end
   %输入我机的位置
   for k=1:Onum
       Ownship(k).P = (planeTrueDatas(k,3:11))';
       Ownship(k).P = [Ownship(k).P(1,1);Ownship(k).P(4,1);Ownship(k).P(7,1);Ownship(k).P(2,1);Ownship(k).P(5,1);Ownship(k).P(8,1);Ownship(k).P(3,1);Ownship(k).P(6,1);Ownship(k).P(9,1)];
   end
    %赋初值
   for k=1:Tnum
      Target(k).ZInf = zeros(Onum*2,1);
      if(globalVarsMatrix(1,10,1:11)==permute(zeros(11,1),[3 2 1]))
%             Target(k).P_UKFInf = diag([1,0.01,0.0001,1,0.01,0.0001,1,0.01,0.0001]);
            Target(k).P_UKFInf = permute(globalVarsMatrix(k,1:9,12:20),[3 2 1]);
%             Target(k).P_UKFInf = diag([4e-4,4e-4,4e-4,4e-4,4e-4,4e-4,4e-4,4e-4,4e-4]);
            Target(k).X_e_UKFInf = permute(globalVarsMatrix(k,1,3:11),[3 2 1]);
            Target(k).X_e_UKFInf = [Target(k).X_e_UKFInf(1,1);Target(k).X_e_UKFInf(4,1);Target(k).X_e_UKFInf(7,1);Target(k).X_e_UKFInf(2,1);Target(k).X_e_UKFInf(5,1);Target(k).X_e_UKFInf(8,1);Target(k).X_e_UKFInf(3,1);Target(k).X_e_UKFInf(6,1);Target(k).X_e_UKFInf(9,1)];
      else
            Target(k).P_UKFInf = permute(globalVarsMatrix(k,10:18,12:20),[3 2 1]);
            Target(k).X_e_UKFInf = permute(globalVarsMatrix(k,10,3:11),[3 2 1]);
            Target(k).X_e_UKFInf = [Target(k).X_e_UKFInf(1,1);Target(k).X_e_UKFInf(4,1);Target(k).X_e_UKFInf(7,1);Target(k).X_e_UKFInf(2,1);Target(k).X_e_UKFInf(5,1);Target(k).X_e_UKFInf(8,1);Target(k).X_e_UKFInf(3,1);Target(k).X_e_UKFInf(6,1);Target(k).X_e_UKFInf(9,1)];
      end      
   end
   targetNoiseDatasTransVerse = permute(targetNoiseDatasTrans,[3 2 1]);%这里做三维数组的转置是为了方便处理数据
   for i = 1:Onum
       for k = 1:Tnum
           Target(k).ZInf(2*i-1:2*i,1) = targetNoiseDatasTransVerse(4:5,k,i); 
       end
   end
   TargetDisorder(:,1) = Target(TargetExist(1,1)).ZInf(:,1);
   if(TnumExist>1)
       for k = 2:TnumExist
          TargetDisorder = cat(2,TargetDisorder,Target(TargetExist(k,1)).ZInf(:,1));
       end
   end
%    TargetDisorder = [Target(1).ZInf(:,1),Target(2).ZInf(:,1),Target(3).ZInf(:,1),Target(4).ZInf(:,1)];
   AssociationCount = 0;
%    msgbox('Message 6');
%    msgbox(num2str(TargetDisorder),'TargetDisorder');
   %Inf目标关联
     for a = 1:TnumExist
       for b = 1:TnumExist
           for c = 1:TnumExist
            TargetTheta = [TargetDisorder(1,a);TargetDisorder(3,b);TargetDisorder(5,c)];
            TargetPhi = [TargetDisorder(2,a);TargetDisorder(4,b);TargetDisorder(6,c)];
            OwnShipX = [Ownship(1).P(1,1);Ownship(2).P(1,1);Ownship(3).P(1,1)];
            OwnShipY = [Ownship(1).P(4,1);Ownship(2).P(4,1);Ownship(3).P(4,1)];
            OwnShipZ = [Ownship(1).P(7,1);Ownship(2).P(7,1);Ownship(3).P(7,1)];
            [X0,Y0,Z0,r,theta,phi] = Target_Association(OwnShipX,OwnShipY,OwnShipZ,TargetPhi,TargetTheta);
            FalsePoint = [X0;Y0;Z0];
            globalVarsMatrixOutput(a,19:21,(b-1)*Tnum+c) = permute(FalsePoint',[3 2 1]);
            if(r<3e3)
                Centre = [X0;Y0;Z0;r;theta;phi];
                Association = cat(2,Association,Centre);
                if(Association(:,1)==zeros(10,1))
                Association = Association(:,2:end); 
                end
                AssociationCount = AssociationCount+1;
            end
           end
       end
     end
%      msgbox(num2str(Association),'Datas');
%      msgbox('Message 2');
     globalVarsMatrixOutput(1,22:24,1:AssociationCount) = permute(Association(1:3,:)',[3 2 1]);
%      msgbox('Message 2');
   %目标排序
   AssociationInOrder = zeros(10,1);
       for k = 1:TnumExist
           for l = 1:size(Association,2)
               distance(l,1) = ((Target(TargetExist(k,1)).X_e_UKFInf(1,end)-Association(1,l))^2 + (Target(TargetExist(k,1)).X_e_UKFInf(4,end)-Association(2,l))^2)^0.5;           
           end
           [DistanceMax,DistanceMaxID] = min(distance);
           AssociationInOrder(:,k) = Association(:,DistanceMaxID);
       end   
   %以关联后目标作为测量值，配对测量值与初始状态
   Comparemin = 1;
   MapVector = zeros(TnumExist,1);
   for k = 1:TnumExist
       Target(TargetExist(k,1)).ZInfComp = atan((Target(TargetExist(k,1)).X_e_UKFInf(4,1)-Ownship(1).P(4,end))/(Target(TargetExist(k,1)).X_e_UKFInf(1,1)-Ownship(1).P(1,end))); 
       ZInfEquivalent(:,1,k) = [AssociationInOrder(5,k);AssociationInOrder(8,k);AssociationInOrder(6,k);AssociationInOrder(9,k);AssociationInOrder(7,k);AssociationInOrder(10,k)];
   end
   for k = 1:TnumExist
       for l = 1:TnumExist
           Compare = abs(Target(TargetExist(k,1)).ZInfComp - ZInfEquivalent(1,1,l));
           if(Compare<Comparemin)
               Comparemin = Compare;
               MapVector(k,1) = l;
           end
       end
       Comparemin = 1;
       Target(TargetExist(k,1)).ZInfEquivalent = ZInfEquivalent(:,:,MapVector(k,1)); 
   end
end
   %Inf-UKF滤波
   for k = 1:Tnum
        if(targetNoiseDatasTrans(:,k,4) == zeros(Onum,1))
             globalVarsMatrixOutput(k,1,1) = planeTrueDatas(1,1);
             globalVarsMatrixOutput(k,1,2) = 100+k-1;
             globalVarsMatrixOutput(k,1,3:11) = globalVarsMatrix(k,10,3:11);
             globalVarsMatrixOutput(k,10,1) = planeTrueDatas(1,1);
             globalVarsMatrixOutput(k,10,2) = 100+k-1;
             globalVarsMatrixOutput(k,10,3:11) = globalVarsMatrix(k,10,3:11);
             globalVarsMatrixOutput(k,10:18,12:20) = globalVarsMatrix(k,10:18,12:20);
        else
            [Target(k).X_e_UKFInfNew,Target(k).P_UKFInfNew]=UKF_Origin1(T,Q,VarRInf,Target(k).X_e_UKFInf,Target(k).ZInfEquivalent,Ownship,Target(k).P_UKFInf); %这里的初始状态等分距离部分调通了修改为Esm的末状态
             Target(k).X_e_UKFInf = [Target(k).X_e_UKFInfNew(1,1);Target(k).X_e_UKFInfNew(4,1);Target(k).X_e_UKFInfNew(7,1);Target(k).X_e_UKFInfNew(2,1);Target(k).X_e_UKFInfNew(5,1);Target(k).X_e_UKFInfNew(8,1);Target(k).X_e_UKFInfNew(3,1);Target(k).X_e_UKFInfNew(6,1);Target(k).X_e_UKFInfNew(9,1)];
             globalVarsMatrixOutput(k,1,1) = planeTrueDatas(1,1);
             globalVarsMatrixOutput(k,1,2) = 100+k-1;
             globalVarsMatrixOutput(k,1,3:11) = permute(Target(k).X_e_UKFInf,[3 2 1]);
             globalVarsMatrixOutput(k,10,1) = planeTrueDatas(1,1);
             globalVarsMatrixOutput(k,10,2) = 100+k-1;
             globalVarsMatrixOutput(k,10,3:11) = permute(Target(k).X_e_UKFInf,[3 2 1]);
             Target(k).P_UKFInf = Target(k).P_UKFInfNew;
             globalVarsMatrixOutput(k,10:18,12:20) = permute(Target(k).P_UKFInf,[3 2 1]);
        end
   end
   globalVarsOutput = zeros(Tnum+Onum,625);
   for k = 1:Tnum+Onum
       for i = 1:25
           globalVarsOutput(k,(25*i-24):25*i) = permute(globalVarsMatrixOutput(k,:,i),[3 2 1]);         
       end
   end
    %输出融合值
    fusionDatasOutput = zeros(Tnum,11);
    filterDatasOutput = zeros(Tnum,11);
    controlDatas = zeros(Onum,3);
    for k = 1:Tnum
        if(targetNoiseDatasTrans(:,k,4) == zeros(Onum,1))
%            msgbox('message1');
           fusionDatasOutput(k,1) = planeTrueDatas(1,1);
           fusionDatasOutput(k,2) = 100+k-1;
           fusionDatasOutput(k,3:11) = permute(globalVarsMatrix(k,10,3:11),[1 3 2]);
%            fusionDatasOutput(k,3:11) = zeros(1,9);
           filterDatasOutput(k,1) = planeTrueDatas(1,1);
           filterDatasOutput(k,2) = 100+k-1;
           filterDatasOutput(k,3:11) = permute(globalVarsMatrix(k,10,3:11),[1 3 2]);
%            filterDatasOutput(k,3:11) = zeros(1,9);
%            msgbox(num2str(fusionDatasOutput(k,:)),'Test2');
        else
           fusionDatasOutput(k,1) = planeTrueDatas(1,1);
           fusionDatasOutput(k,2) = 100+k-1;
           fusionDatasOutput(k,3:11) = Target(k).X_e_UKFInf';
           filterDatasOutput(k,1) = planeTrueDatas(1,1);
           filterDatasOutput(k,2) = 100+k-1;
           filterDatasOutput(k,3:11) = Target(k).X_e_UKFInf';
        end
    end
    fusionDatas = fusionDatasOutput;
    filterDatas = filterDatasOutput;
%     msgbox('Message 4');
end


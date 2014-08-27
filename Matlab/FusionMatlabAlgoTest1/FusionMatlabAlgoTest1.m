function [ fusionDatas, filterDatas, controlDatas, globalVarsOutput ] = FusionMatlabAlgoTest1( planeTrueDatas, targetNoiseDatas, globalVars, interval )

msgbox(num2str(0))

T = interval;
Q = 0;
Tnum = size(targetNoiseDatas,2);                          %敌机个数
Onum = size(planeTrueDatas,1);  
msgbox(num2str(0.1))%我机个数
REsm = [targetNoiseDatas(1,1,6),targetNoiseDatas(2,1,6),targetNoiseDatas(3,1,6)];
RInf = [targetNoiseDatas(1,1,6),targetNoiseDatas(1,1,7),targetNoiseDatas(2,1,6),targetNoiseDatas(2,1,7),targetNoiseDatas(3,1,6),targetNoiseDatas(3,1,7)];
msgbox(num2str(0.2))
VarREsm = REsm' * REsm;
VarRInf = RInf' * RInf;
Association = zeros(Tnum,1);%初始化目标关联矩阵

msgbox(num2str(1))

globalVarsMatrix = zeros(Tnum+Onum,25,25);
for k = 1:Tnum+Onum
    for i = 1:25
        globalVarsMatrix(k,:,i) = permute(globalVars(k,(25*i-24):25*i),[3 2 1]);         
    end
end
globalVarsMatrixOutput = globalVarsMatrix;
%msgbox(num2str(targetNoiseDatas(1:1, 2:2)));
%msgbox(num2str(targetNoiseDatas(2:2, 2:2)));
%msgbox(num2str(targetNoiseDatas(3:3, 2:2)));

msgbox(num2str(2))

%计算我机与敌机最大距离，辨别是否离开Esm滤波区域
for k = 1:Tnum
    for i = 1:Onum
       distanceEsm(k,i) = ((globalVarsMatrix(k,1,3)-planeTrueDatas(i,3))^2+(globalVarsMatrix(k,1,4)-planeTrueDatas(i,4))^2+(globalVarsMatrix(k,1,5)-planeTrueDatas(i,5))^2)^0.5;
    end
end
DistanceMin = min(min(distanceEsm));

msgbox(num2str(3))

%Esm探测范围内滤波
if(DistanceMin>2e5)
    %赋初值
   for k=1:Tnum
      Target(k).ZEsm = zeros(Onum,1);
      if(planeTrueDatas(1,1)==0)
           Target(k).P_UKFEsm = diag([1,0.01,0.0001,1,0.01,0.0001,1,0.01,0.0001]);
      end
   end
   
msgbox(num2str(4))

   %根据ID号对Esm段滤波进行关联
   for k=1:Onum
       for i=1:Tnum
            TargetID = targetNoiseDatas(k,i,2)+1;
            Target(TargetID).ZEsm(k,1) = targetNoiseDatas(k,i,4); 
       end
   end

msgbox(num2str(5))

   %输入我机的位置
   for k=1:Onum
       Ownship(k).P = (planeTrueDatas(k,3:11))';
       Ownship(k).P = [Ownship(k).P(1,1);Ownship(k).P(4,1);Ownship(k).P(7,1);Ownship(k).P(2,1);Ownship(k).P(5,1);Ownship(k).P(8,1);Ownship(k).P(3,1);Ownship(k).P(6,1);Ownship(k).P(9,1)];
   end

msgbox(num2str(6))

   %输入敌机的位置
   for k=1:Tnum
       Target(k).X_e_UKFEsm = permute(globalVarsMatrix(k,1,3:11),[3,2,1]);
       Target(k).X_e_UKFEsm = [Target(k).X_e_UKFEsm(1,1);Target(k).X_e_UKFEsm(4,1);Target(k).X_e_UKFEsm(7,1);Target(k).X_e_UKFEsm(2,1);Target(k).X_e_UKFEsm(5,1);Target(k).X_e_UKFEsm(8,1);Target(k).X_e_UKFEsm(3,1);Target(k).X_e_UKFEsm(6,1);Target(k).X_e_UKFEsm(9,1)];
   end
   
msgbox(num2str(7))

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
    
msgbox(num2str(8))

       globalVarsOutput = zeros(Tnum+Onum,625);
       for k = 1:Tnum+Onum
           for i = 1:25
               globalVarsOutput(k,(25*i-24):25*i) = permute(globalVarsMatrixOutput(k,:,i),[3 2 1]);         
           end
       end
    %输出融合值
    fusionDatas = zeros(Tnum,9);
    filterDatas = zeros(Tnum,9);
    for k = 1:Tnum
       fusionDatas(k,:) = Target(k).X_e_UKFEsm';
       filterDatas(k,:) = Target(k).X_e_UKFEsm';
       controlDatas = planeTrueDatas;
    end
    
msgbox(num2str(9))

elseif(DistanceMin<2e5 && DistanceMin>2e4)
   %输入我机的位置
   for k=1:Onum
       Ownship(k).P = (planeTrueDatas(k,3:11))';
       Ownship(k).P = [Ownship(k).P(1,1);Ownship(k).P(4,1);Ownship(k).P(7,1);Ownship(k).P(2,1);Ownship(k).P(5,1);Ownship(k).P(8,1);Ownship(k).P(3,1);Ownship(k).P(6,1);Ownship(k).P(9,1)];
   end
   
msgbox(num2str(10))

    %赋初值
   for k=1:Tnum
      Target(k).ZInf = zeros(Onum*2,1);
      if(globalVarsMatrix(1,10,1:11)==permute(zeros(11,1),[3 2 1]))
            Target(k).P_UKFInf = diag([1,0.01,0.0001,1,0.01,0.0001,1,0.01,0.0001]);
            Target(k).X_e_UKFInf = permute(globalVarsMatrix(k,1,3:11),[3 2 1]);
            Target(k).X_e_UKFInf = [Target(k).X_e_UKFInf(1,1);Target(k).X_e_UKFInf(4,1);Target(k).X_e_UKFInf(7,1);Target(k).X_e_UKFInf(2,1);Target(k).X_e_UKFInf(5,1);Target(k).X_e_UKFInf(8,1);Target(k).X_e_UKFInf(3,1);Target(k).X_e_UKFInf(6,1);Target(k).X_e_UKFInf(9,1)];
      else
            Target(k).P_UKFInf = permute(globalVarsMatrix(k,10:18,12:20),[3 2 1]);
            Target(k).X_e_UKFInf = permute(globalVarsMatrix(k,10,3:11),[3 2 1]);
            Target(k).X_e_UKFInf = [Target(k).X_e_UKFInf(1,1);Target(k).X_e_UKFInf(4,1);Target(k).X_e_UKFInf(7,1);Target(k).X_e_UKFInf(2,1);Target(k).X_e_UKFInf(5,1);Target(k).X_e_UKFInf(8,1);Target(k).X_e_UKFInf(3,1);Target(k).X_e_UKFInf(6,1);Target(k).X_e_UKFInf(9,1)];
      end      
   end
   
msgbox(num2str(11))

   targetNoiseDatasTrans = permute(targetNoiseDatas,[3 2 1]);%这里做三维数组的转置是为了方便处理数据
   for i = 1:Onum
       for k = 1:Tnum
           Target(k).ZInf(2*i-1:2*i,1) = targetNoiseDatasTrans(4:5,k,i); 
       end
   end
   
msgbox(num2str(12))

   TargetDisorder(:,:,1) = [Target(1).ZInf(:,1),Target(2).ZInf(:,1),Target(3).ZInf(:,1),Target(4).ZInf(:,1)];
   AssociationCount = 0;
   %Inf目标关联
     for a = 1:Tnum
       for b = 1:Tnum
           for c = 1:Tnum
            TargetTheta = [TargetDisorder(1,a,1);TargetDisorder(3,b,1);TargetDisorder(5,c,1)];
            TargetPhi = [TargetDisorder(2,a,1);TargetDisorder(4,b,1);TargetDisorder(6,c,1)];
            OwnShipX = [Ownship(1).P(1,end);Ownship(2).P(1,end);Ownship(3).P(1,end)];
            OwnShipY = [Ownship(1).P(4,end);Ownship(2).P(4,end);Ownship(3).P(4,end)];
            OwnShipZ = [Ownship(1).P(7,end);Ownship(2).P(7,end);Ownship(3).P(7,end)];
            [X0,Y0,Z0,r] = Target_Association(OwnShipX,OwnShipY,OwnShipZ,TargetPhi,TargetTheta);
            FalsePoint = [X0;Y0;Z0];
            globalVarsMatrixOutput(a,19:21,(b-1)*Tnum+c) = permute(FalsePoint',[3 2 1]);
            if(r<3e3)
                Centre = [X0;Y0;Z0;r];
                Association = cat(2,Association,Centre);
                if(Association(:,1)==zeros(4,1))
                Association = Association(:,2:end); 
                end
                AssociationCount = AssociationCount+1;
            end
           end
       end
     end
msgbox(num2str(13))

     globalVarsMatrixOutput(1,22:24,1:AssociationCount) = permute(Association(1:3,:)',[3 2 1]);
   %目标排序
   AssociationInOrder = zeros(Tnum,1);
       for k = 1:Tnum
           for l = 1:size(Association,2)
               distance(l,1) = ((Target(k).X_e_UKFInf(1,end)-Association(1,l))^2 + (Target(k).X_e_UKFInf(4,end)-Association(2,l))^2)^0.5;           
           end
           [distancemin,distanceminID] = min(distance);
           AssociationInOrder(:,k) = Association(:,distanceminID);
       end
      
msgbox(num2str(14))

   %以关联后目标作为测量值，配对测量值与初始状态
   Comparemin = 1;
   MapVector = zeros(Tnum,1);
   for k = 1:Tnum
       Target(k).ZInfComp = atan((Target(k).X_e_UKFInf(4,1)-Ownship(1).P(4,end))/(Target(k).X_e_UKFInf(1,1)-Ownship(1).P(1,end))); 
       ZInfEquivalent(:,1,k) = [atan((AssociationInOrder(2,k)-Ownship(1).P(4,end))/(AssociationInOrder(1,k)-Ownship(1).P(1,end)));atan((AssociationInOrder(3,k)-Ownship(1).P(7,end))/((AssociationInOrder(2,k)-Ownship(1).P(4,end))^2+(AssociationInOrder(1,k)-Ownship(1).P(1,end))^2)^0.5); ... ,
       atan((AssociationInOrder(2,k)-Ownship(2).P(4,end))/(AssociationInOrder(1,k)-Ownship(2).P(1,end)));atan((AssociationInOrder(3,k)-Ownship(2).P(7,end))/((AssociationInOrder(2,k)-Ownship(2).P(4,end))^2+(AssociationInOrder(1,k)-Ownship(2).P(1,end))^2)^0.5); ... ,
       atan((AssociationInOrder(2,k)-Ownship(3).P(4,end))/(AssociationInOrder(1,k)-Ownship(3).P(1,end)));atan((AssociationInOrder(3,k)-Ownship(3).P(7,end))/((AssociationInOrder(2,k)-Ownship(3).P(4,end))^2+(AssociationInOrder(1,k)-Ownship(3).P(1,end))^2)^0.5)];    
   end
   for k = 1:Tnum
       for l = 1:Tnum
           Compare = abs(Target(k).ZInfComp - ZInfEquivalent(1,1,l));
           if(Compare<Comparemin)
               Comparemin = Compare;
               MapVector(k,1) = l;
           end
       end
       Comparemin = 1;
       Target(k).ZInfEquivalent = ZInfEquivalent(:,:,MapVector(k,1));
   end
   
msgbox(num2str(15))

   %Inf-UKF滤波
   for k = 1:Tnum
        [Target(k).X_e_UKFInfNew,Target(k).P_UKFInfNew]=UKF_Origin1(T,Q,VarRInf,Target(k).X_e_UKFInf,Target(k).ZInfEquivalent,Ownship,Target(k).P_UKFInf); %这里的初始状态等分距离部分调通了修改为Esm的末状态
        Target(k).X_e_UKFInf = [Target(k).X_e_UKFInfNew(1,1);Target(k).X_e_UKFInfNew(4,1);Target(k).X_e_UKFInfNew(7,1);Target(k).X_e_UKFInfNew(2,1);Target(k).X_e_UKFInfNew(5,1);Target(k).X_e_UKFInfNew(8,1);Target(k).X_e_UKFInfNew(3,1);Target(k).X_e_UKFInfNew(6,1);Target(k).X_e_UKFInfNew(9,1)];
        globalVarsMatrixOutput(k,10,1) = planeTrueDatas(1,1);
        globalVarsMatrixOutput(k,10,2) = 100+k-1;
        globalVarsMatrixOutput(k,10,3:11) = permute(Target(k).X_e_UKFInf,[3 2 1]);
        Target(k).P_UKFInf = Target(k).P_UKFInfNew;
        globalVarsMatrixOutput(k,10:18,12:20) = permute(Target(k).P_UKFInf,[3 2 1]);
   end
   
msgbox(num2str(16))

   globalVarsOutput = zeros(Tnum+Onum,625);
   for k = 1:Tnum+Onum
       for i = 1:25
           globalVarsOutput(k,(25*i-24):25*i) = permute(globalVarsMatrixOutput(k,:,i),[3 2 1]);         
       end
   end
msgbox(num2str(17))

    %输出融合值
    fusionDatas = zeros(Tnum,9);
    filterDatas = zeros(Tnum,9);
    for k = 1:Tnum
       fusionDatas(k,:) = Target(k).X_e_UKFInf';
       filterDatas(k,:) = Target(k).X_e_UKFInf';
       controlDatas = planeTrueDatas;
    end
end


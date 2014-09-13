%function [] = onetime_matlab_dialog()
close all;
fidin1 = fopen('..\Fusion.dat'); % 打开test.txt文件，这里是同级并列的文件夹，这里是标准的格式
fidin2 = fopen('..\TargetTrue.dat');
Hostile = cell2mat(textscan(fidin1,'%f',1,'HeaderLines',0));
Time = cell2mat(textscan(fidin1,'%f',1,'HeaderLines',1));

Datafusion = cell2mat(textscan(fidin1,'%*f %*f %f %f %f %f %f %f %f %f %f %*f',Time,'HeaderLines',1));
Datatrue = cell2mat(textscan(fidin2,'%*f %*f %f %f %f %f %f %f %f %f %f %*f',Time+1,'HeaderLines',3));
DataFusionDistance = (Datafusion(:,1).^2 + Datafusion(:,2).^2 + Datafusion(:,3).^2).^0.5;
DataFusionPhi = atan(Datafusion(:,3)./(Datafusion(:,1).^2 + Datafusion(:,2).^2).^0.5);
DataFusionTheta = atan(Datafusion(:,2).* Datafusion(:,1).^(-1));
DataFusionTransform = cat(2,DataFusionDistance,DataFusionTheta,DataFusionPhi);
DataTrueDistance = (Datatrue(:,1).^2 + Datatrue(:,2).^2 + Datatrue(:,3).^2).^0.5;
DataTruePhi = atan(Datatrue(:,3)./(Datatrue(:,1).^2 + Datatrue(:,2).^2).^0.5);
DataTrueTheta = atan(Datatrue(:,2).* Datatrue(:,1).^(-1));
DataTrueTransform = cat(2,DataTrueDistance,DataTrueTheta,DataTruePhi);

while ~feof(fidin1)                                      % 判断是否为文件末尾
       TargetEnd1 = cell2mat(textscan(fidin1,'%f',1,'HeaderLines',1));
       if(TargetEnd1 ~= 0)
           Data1 = cell2mat(textscan(fidin1,'%*f %*f %f %f %f %f %f %f %f %f %f %*f',Time,'HeaderLines',1)); 
           Datafusion = cat(2,Datafusion,Data1);    
       end
end 
fclose(fidin1);

while ~feof(fidin2)                                      % 判断是否为文件末尾
       TargetEnd2 = cell2mat(textscan(fidin2,'%f',1,'HeaderLines',1));
       if(TargetEnd2 ~= 0)
           Data2 = cell2mat(textscan(fidin2,'%*f %*f %f %f %f %f %f %f %f %f %f %*f',Time+1,'HeaderLines',1)); 
           Datatrue = cat(2,Datatrue,Data2); 
       end
end 
fclose(fidin2);

if Hostile ~= 1
for k=2:Hostile
DataFusionDistance = (Datafusion(:,9*k-8).^2 + Datafusion(:,9*k-7).^2 + Datafusion(:,9*k-6).^2).^0.5;
DataFusionPhi = atan(Datafusion(:,9*k-6)./(Datafusion(:,9*k-8).^2 + Datafusion(:,9*k-7).^2).^0.5);
DataFusionTheta = atan(Datafusion(:,9*k-7).* Datafusion(:,9*k-8).^(-1));
DataFusionTransform = cat(2,DataFusionTransform,DataFusionDistance,DataFusionTheta,DataFusionPhi);
DataTrueDistance = (Datatrue(:,9*k-8).^2 + Datatrue(:,9*k-7).^2 + Datatrue(:,9*k-6).^2).^0.5;
DataTruePhi = atan(Datatrue(:,9*k-6)./(Datatrue(:,9*k-8).^2 + Datatrue(:,9*k-7).^2).^0.5);
DataTrueTheta = atan(Datatrue(:,9*k-7).* Datatrue(:,9*k-8).^(-1));
DataTrueTransform = cat(2,DataTrueTransform,DataTrueDistance,DataTrueTheta,DataTruePhi);
end
end

DataCompare = (DataTrueTransform(1:end-1,:).^-1).*(DataFusionTransform - DataTrueTransform(1:end-1,:));
for i=1:Hostile
    figure(i);
    set(figure(i),'outerposition',[672 100 500 950]);
for j=1:3
       subplot(3,1,j);
       plot(DataCompare(:,j+3*i-3),'linewidth',1.5);hold on;
       grid on;
       xlabel('t/s');
       switch j
           case 1
           title(sprintf('第%d架敌机距离融合值与真值对比图',i));
           ylabel('distance/km');
           case 2
           title(sprintf('第%d架敌机方向角融合值与真值对比图',i));
           ylabel('phi/radian');
           case 3
           title(sprintf('第%d架敌机俯仰角融合值与真值对比图',i));
           ylabel('theta/radian');
       end
       legend('相对误差');       
end
end
%end


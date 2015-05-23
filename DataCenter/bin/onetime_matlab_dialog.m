function [] = onetime_matlab_dialog(round)
msgbox(num2str(round))
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
flag = 0;
ErrorEnd = 0;
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

for k = 1:Time+1
    if(abs(DataTrue(k,1))<1||abs(DataTrue(k,10))<1||abs(DataTrue(k,19))<1||abs(DataTrue(k,1))<28)
        if(flag==0)
            ErrorEnd = k-1;
            flag = 1;
        end
    end
end
if(ErrorEnd==0)
    ErrorEnd = size(DataTrueTransform,1);
end
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
DataTrueTransform = DataTrueTransform(1:ErrorEnd,:);
DataCompare = (DataTrueTransform(1:ErrorEnd,:).^-1).*(DataFusionTransform(1:ErrorEnd,:) - DataTrueTransform(1:ErrorEnd,:));
for i=1:Hostile
    figure(i+1);
    set(figure(i+1),'outerposition',[300 100 500 900]);
for j=1:2
       subplot(2,1,j);
       switch j
           case 1
           plot(DataCompare(:,3*i-2),'linewidth',1.5);hold on;
           grid on;
           title(sprintf('第%d架敌机距离融合值与真值对比图',i));
           xlabel('t/s');
           ylabel('误差');
           legend('相对误差');
           case 2
           plot(DataTrueTransform(:,3*i-2),DataCompare(:,3*i-2),'linewidth',1.5);hold on;grid on;xlabel('true distance/m');
           grid on;
           axis tight;
           axis 'auto y';
           set(gca,'XDir','reverse');
           title(sprintf('第%d架敌机距离融合值与真值对比图（横坐标真实距离）',i));ylabel('误差');
           legend('相对误差');
       end       
end
end
end


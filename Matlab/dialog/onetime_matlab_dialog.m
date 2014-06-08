function [] = onetime_matlab_dialog()
fidin1 = fopen('..\Fusion.dat'); % 打开test.txt文件，这里是同级并列的文件夹，这里是标准的格式
fidin2 = fopen('..\TargetTrue.dat');
Hostile = cell2mat(textscan(fidin1,'%f',1,'HeaderLines',0));
Time = cell2mat(textscan(fidin1,'%f',1,'HeaderLines',1));

Datafusion = cell2mat(textscan(fidin1,'%*f %*f %f %f %f %*f %*f %*f',Time,'HeaderLines',1));
Datatrue = cell2mat(textscan(fidin2,'%*f %*f %f %f %f %f %f %f %f %f %f',Time+1,'HeaderLines',3));
DataTrueDistance = (Datatrue(:,1).^2 + Datatrue(:,2).^2 + Datatrue(:,3).^2).^0.5;
DataTruePhi = Datatrue(:,4).^2 + Datatrue(:,5).^2 + Datatrue(:,6).^2;
DataTrueTheta = Datatrue(:,7).^2 + Datatrue(:,8).^2 + Datatrue(:,9).^2;
DataTrueTransform = cat(2,DataTrueDistance,DataTruePhi,DataTrueTheta);

while ~feof(fidin1)                                      % 判断是否为文件末尾                 
       Data1 = cell2mat(textscan(fidin1,'%*f %*f %f %f %f %*f %*f %*f',Time,'HeaderLines',3)); 
       Datafusion = cat(2,Datafusion,Data1);
end 
fclose(fidin1);

while ~feof(fidin2)                                      % 判断是否为文件末尾                 
       Data2 = cell2mat(textscan(fidin2,'%*f %*f %f %f %f %f %f %f %f %f %f',Time+1,'HeaderLines',3)); 
       Datatrue = cat(2,Datatrue,Data2);
end 
fclose(fidin2);

if Hostile ~= 1
for k=2:Hostile
DataTrueDistance = (Datatrue(:,9*k-8).^2 + Datatrue(:,9*k-7).^2 + Datatrue(:,9*k-6).^2).^0.5;
DataTruePhi = (Datatrue(:,9*k-5).^2 + Datatrue(:,9*k-4).^2 + Datatrue(:,9*k-3).^2).^0.5;
DataTrueTheta = (Datatrue(:,9*k-2).^2 + Datatrue(:,9*k-1).^2 + Datatrue(:,9*k).^2).^0.5;
DataTrueTransform = cat(2,DataTrueTransform,DataTrueDistance,DataTruePhi,DataTrueTheta);
end
end

for i=2:Hostile+1
    figure(i);
    set(figure(i),'outerposition',[672 100 500 950]);
for j=1:3
       subplot(3,1,j);
       plot(Datafusion(:,j+3*i-3));hold on;
       plot(DataTrueTransform(:,j+3*i-3),'red');
       grid on;
       xlabel('t/s');
       switch j
           case 1
           title(sprintf('第%d架敌机距离融合值与真值对比图',i));
           ylabel('distance/km');
           case 2
           title(sprintf('第%d架敌机俯仰角融合值与真值对比图',i));
           ylabel('phi/radian');
           case 3
           title(sprintf('第%d架敌机方向角融合值与真值对比图%d',i));
           ylabel('theta/radian');
       end
       legend('融合值','真实值');       
end
end
end


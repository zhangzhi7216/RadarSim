function [output] = matlab_dialog(plane_true, target_true, target_fusion, target_filter, global_var)
msgbox('Message 1');
t_current = 0;
PlaneCount = size(plane_true,1);
TargetCount = size(target_fusion,1);
t_count = size(plane_true,2)/3;
PlaneX = plane_true(:,1);
PlaneY = plane_true(:,2);
PlaneZ = plane_true(:,3);
TargetX = target_fusion(:,1);
TargetY = target_fusion(:,2);
TargetZ = target_fusion(:,3);
globalVarsMatrix = zeros(Tnum+Onum,25,25);
for k = 1:Tnum+Onum
    for i = 1:25
        globalVarsMatrix(k,:,i) = permute(global_var(k,(25*i-24):25*i),[3 2 1]);         
    end
end
msgbox('Message 2');
while(t_current~=t_count)
    if (plane_true(1,3*t_current+1)~=0) || (plane_true(1,3*t_current+2)~=0) || (plane_true(1,3*t_current+3)~=0)
    PlaneX = cat(2,PlaneX,plane_true(:,3*t_current+1));
    PlaneY = cat(2,PlaneY,plane_true(:,3*t_current+2));
    PlaneZ = cat(2,PlaneZ,plane_true(:,3*t_current+3));
    TargetX = cat(2,TargetX,target_fusion(:,9*t_current+1));
    TargetY = cat(2,TargetY,target_fusion(:,9*t_current+2));
    TargetZ = cat(2,TargetZ,target_fusion(:,9*t_current+3));
    t_current = t_current+1;
    else
    break
    end
end
msgbox('Message 3');
DrawFilter = figure(4);
set(DrawFilter,'units','normalized','position',[0.1,0.1,0.8,0.8]);
%全局变量中取出未关联点
InAssociatedPoint = zeros(64,3);
for k = 1:Tnum
        InAssociatedPoint(16*k-15:16*k,:) = permute(globalVarsMatrix(k,19:21,1:16),[3 2 1]);         
end
%全局变量中取出可疑点
SuspiciousPoint = permute(globalVarsMatrix(1,22:24,1:20),[3 2 1]);
SuspiciousCount = find(SuspiciousPoint(:,1)==0,1)-1;%求非零行数
SuspiciousPoint = SuspiciousPoint(1:SuspiciousCount,:); 

h = plot3(InAssociatedPoint(:,1),InAssociatedPoint(:,2),InAssociatedPoint(:,3),'g.','linewidth',0.5);
grid on;
pause(0.00005);
delete(h);
f = plot3(SuspiciousPoint(:,1),SuspiciousPoint(:,2),SuspiciousPoint(:,3),'k.','linewidth',0.5);
legend(f,sprintf('Suspicious Points %d',size(SuspiciousPoint,2)),'Location','NorthEast');
pause(0.00005);
delete(f);
pause(0.00005);

for PlaneNum = 1:1:PlaneCount
    if(PlaneX(PlaneNum,end)==0 && PlaneY(PlaneNum,end)==0 && PlaneZ(PlaneNum,end)==0)
    plot3(PlaneX(PlaneNum,1:end-1),PlaneY(PlaneNum,1:end-1),PlaneZ(PlaneNum,1:end-1),'linewidth',2);
    else
    plot3(PlaneX(PlaneNum,:),PlaneY(PlaneNum,:),PlaneZ(PlaneNum,:),'linewidth',2);
    end
    if(PlaneNum~=PlaneCount) 
    hold on;
    end
    grid on;
end
for TargetNum = 1:1:TargetCount
    if(TargetX(TargetNum,end)==0 && TargetY(TargetNum,end)==0 && TargetZ(TargetNum,end)==0)
    plot3(TargetX(TargetNum,1:end-1),TargetY(TargetNum,1:end-1),TargetZ(TargetNum,1:end-1),'red','linewidth',2);
    else
    plot3(TargetX(TargetNum,:),TargetY(TargetNum,:),TargetZ(TargetNum,:),'red','linewidth',2);
    end
    if(TargetNum~=TargetCount) 
    hold on;
    end
    grid on;
    xlabel('x/m');
    ylabel('y/m');
    zlabel('z/m');
end
end




    



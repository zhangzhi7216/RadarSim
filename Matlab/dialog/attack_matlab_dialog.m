function [output] = attack_matlab_dialog(attack_plane_true, attack_target_true, attack_target_fusion, attack_target_filter, global_var)
t_current = 0;
flag = 0;
ErrorEnd = 0;
PlaneCount = size(attack_plane_true,1);
TargetCount = size(attack_target_fusion,1);
t_count = size(attack_plane_true,2)/3;
PlaneX = attack_plane_true(:,1);
PlaneY = attack_plane_true(:,2);
PlaneZ = attack_plane_true(:,3);
TargetX = attack_target_fusion(:,1);
TargetY = attack_target_fusion(:,2);
TargetZ = attack_target_fusion(:,3);
TargetTrueX = attack_target_true(:,1);
TargetTrueY = attack_target_true(:,2);
TargetTrueZ = attack_target_true(:,3);

while(t_current~=t_count)
    if (attack_plane_true(1,3*t_current+1)~=0) || (attack_plane_true(1,3*t_current+2)~=0) || (attack_plane_true(1,3*t_current+3)~=0)
    PlaneX = cat(2,PlaneX,attack_plane_true(:,3*t_current+1));
    PlaneY = cat(2,PlaneY,attack_plane_true(:,3*t_current+2));
    PlaneZ = cat(2,PlaneZ,attack_plane_true(:,3*t_current+3));
    TargetTrueX = cat(2,TargetTrueX,attack_target_true(:,3*t_current+1));
    TargetTrueY = cat(2,TargetTrueY,attack_target_true(:,3*t_current+2));
    TargetTrueZ = cat(2,TargetTrueZ,attack_target_true(:,3*t_current+3));
    TargetX = cat(2,TargetX,attack_target_fusion(:,9*t_current+1));
    TargetY = cat(2,TargetY,attack_target_fusion(:,9*t_current+2));
    TargetZ = cat(2,TargetZ,attack_target_fusion(:,9*t_current+3));
    t_current = t_current+1;
    else
    break
    end
end
for k = 1:size(TargetTrueX,2)
    if(min(abs(TargetTrueX(:,k)))<1&&flag==0)
         ErrorEnd = k-1;
         flag = 1;
    end
end
if(ErrorEnd==0)
    ErrorEnd = size(TargetTrueX,2);
end
    TargetX = TargetX(:,1:ErrorEnd);
    TargetY = TargetY(:,1:ErrorEnd);
    TargetZ = TargetZ(:,1:ErrorEnd);
figure(1);
set(figure(1),'outerposition',[500 100 600 500]);

for PlaneNum = 1:1:PlaneCount-1
    if(PlaneX(PlaneNum,end)==0 && PlaneY(PlaneNum,end)==0 && PlaneZ(PlaneNum,end)==0)
    plot3(PlaneX(PlaneNum,1:end-1),PlaneY(PlaneNum,1:end-1),PlaneZ(PlaneNum,1:end-1),'linewidth',2);
    else
    plot3(PlaneX(PlaneNum,:),PlaneY(PlaneNum,:),PlaneZ(PlaneNum,:),'linewidth',2);
    end
    if(PlaneNum~=PlaneCount-1) 
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


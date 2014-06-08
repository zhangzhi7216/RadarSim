function [output] = matlab_dialog(plane_true, target_true, target_fusion, target_filter, global_var)
t_current = 0;
PlaneCount = size(plane_true,1);
TargetCount = size(target_fusion,1);
t_count = size(plane_true,2)/3;
PlaneX = plane_true(:,1);
PlaneY = plane_true(:,2);
PlaneZ = plane_true(:,3);
TargetX = target_fusion(:,1).*cos(target_fusion(:,3)).*sin(target_fusion(:,2));
TargetY = target_fusion(:,1).*cos(target_fusion(:,3)).*cos(target_fusion(:,2));
TargetZ = target_fusion(:,1).*sin(target_fusion(:,3));

while(t_current~=t_count)
    if (plane_true(1,3*t_current+1)~=0) || (plane_true(1,3*t_current+2)~=0) || (plane_true(1,3*t_current+3)~=0)
    PlaneX = cat(2,PlaneX,plane_true(:,3*t_current+1));
    PlaneY = cat(2,PlaneY,plane_true(:,3*t_current+2));
    PlaneZ = cat(2,PlaneZ,plane_true(:,3*t_current+3));
    TargetX = cat(2,TargetX,target_fusion(:,6*t_current+1).*cos(target_fusion(:,6*t_current+3)).*sin(target_fusion(:,6*t_current+2)));
    TargetY = cat(2,TargetY,target_fusion(:,6*t_current+1).*cos(target_fusion(:,6*t_current+3)).*cos(target_fusion(:,6*t_current+2)));
    TargetZ = cat(2,TargetZ,target_fusion(:,6*t_current+1).*sin(target_fusion(:,6*t_current+3)));
    t_current = t_current+1;
    else
    break
    end
end
%     PlaneX(:,1)=[];
%     PlaneY(:,1)=[];
%     PlaneZ(:,1)=[];
%     TargetX(:,1)=[];
%     TargetY(:,1)=[];
%     TargetZ(:,1)=[];
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
end
end




    



function [output] = matlab_dialog(plane_true, target_true, target_fusion, target_filter, global_var)
t_current = 0;
flag = 0;
ErrorEnd = 0;
PlaneCount = size(plane_true,1);
TargetCount = size(target_fusion,1);
t_count = size(plane_true,2)/3;
PlaneX = plane_true(:,1);
PlaneY = plane_true(:,2);
PlaneZ = plane_true(:,3);
TargetX = target_fusion(:,1);
TargetY = target_fusion(:,2);
TargetZ = target_fusion(:,3);
TargetTrueX = target_true(:,1);
TargetTrueY = target_true(:,2);
TargetTrueZ = target_true(:,3);

while(t_current~=t_count)
    if (plane_true(1,3*t_current+1)~=0) || (plane_true(1,3*t_current+2)~=0) || (plane_true(1,3*t_current+3)~=0)
    PlaneX = cat(2,PlaneX,plane_true(:,3*t_current+1));
    PlaneY = cat(2,PlaneY,plane_true(:,3*t_current+2));
    PlaneZ = cat(2,PlaneZ,plane_true(:,3*t_current+3));
    TargetTrueX = cat(2,TargetTrueX,target_true(:,3*t_current+1));
    TargetTrueY = cat(2,TargetTrueY,target_true(:,3*t_current+2));
    TargetTrueZ = cat(2,TargetTrueZ,target_true(:,3*t_current+3));
    TargetX = cat(2,TargetX,target_fusion(:,9*t_current+1));
    TargetY = cat(2,TargetY,target_fusion(:,9*t_current+2));
    TargetZ = cat(2,TargetZ,target_fusion(:,9*t_current+3));
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
    TargetTrueX = TargetTrueX(:,1:ErrorEnd);
    TargetTrueY = TargetTrueY(:,1:ErrorEnd);
    TargetTrueZ = TargetTrueZ(:,1:ErrorEnd);
    TargetX = TargetX(:,1:ErrorEnd);
    TargetY = TargetY(:,1:ErrorEnd);
    TargetZ = TargetZ(:,1:ErrorEnd);
%Îó²î¼ÆËã
TargetXError = (TargetX-TargetTrueX);
TargetYError = (TargetY-TargetTrueY);
TargetZError = (TargetZ-TargetTrueZ);
figure(1);
set(figure(1),'outerposition',get(0,'screensize'));
subplot(1,4,1:2);
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
    plot3(TargetX(TargetNum,1:end),TargetY(TargetNum,1:end),TargetZ(TargetNum,1:end),'red','linewidth',2);
    end
    if(TargetNum~=TargetCount) 
    hold on;
    end
    grid on;
    xlabel('x/m');
    ylabel('y/m');
    zlabel('z/m');
end
subplot(4,4,3:4);
   t=1:1:size(TargetXError,2);
   plot(t,TargetXError(1,:)','r-','linewidth',1.5);hold on;
   plot(t,TargetYError(1,:)','g-','linewidth',1.5);hold on;
   plot(t,TargetZError(1,:)','b-','linewidth',1.5);hold on;
   xlim([0 t_count]);
   legend('xÖá','yÖá','zÖá');
   grid on;
   title(sprintf('µÐ»ú¸÷ÖáÏòÎ»ÖÃÎó²î'));
   xlabel('Ö¡ºÅ');
   ylabel('Îó²î');
subplot(4,4,7:8);
   t=1:1:size(TargetXError,2);
   plot(t,TargetXError(2,:)','r-','linewidth',1.5);hold on;
   plot(t,TargetYError(2,:)','g-','linewidth',1.5);hold on;
   plot(t,TargetZError(2,:)','b-','linewidth',1.5);hold on;
   xlim([0 t_count]);
   grid on;
   xlabel('Ö¡ºÅ');
   ylabel('Îó²î');
subplot(4,4,11:12);
   t=1:1:size(TargetXError,2);
   plot(t,TargetXError(3,:)','r-','linewidth',1.5);hold on;
   plot(t,TargetYError(3,:)','g-','linewidth',1.5);hold on;
   plot(t,TargetZError(3,:)','b-','linewidth',1.5);hold on;
   xlim([0 t_count]);
   grid on;
   xlabel('Ö¡ºÅ');
   ylabel('Îó²î');
subplot(4,4,15:16);
   t=1:1:size(TargetXError,2);
   plot(t,TargetXError(4,:)','r-','linewidth',1.5);hold on;
   plot(t,TargetYError(4,:)','g-','linewidth',1.5);hold on;
   plot(t,TargetZError(4,:)','b-','linewidth',1.5);hold on;
   xlim([0 t_count]);
   grid on;
   xlabel('Ö¡ºÅ');
   ylabel('Îó²î');
end




    



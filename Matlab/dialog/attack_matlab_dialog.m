function [output] = attack_matlab_dialog(attack_plane_true, attack_target_true, attack_target_fusion, attack_target_filter)
t_current = 0;
TargetChoice = 1;
RelativeDistanceMin = 1000;
TargetCount = size(attack_target_fusion,1);
t_count = size(attack_plane_true,2)/3;
PlaneX = attack_plane_true(1,1);
PlaneY = attack_plane_true(1,2);
PlaneZ = attack_plane_true(1,3);
TargetX = attack_target_fusion(:,1);
TargetY = attack_target_fusion(:,2);
TargetZ = attack_target_fusion(:,3);
TargetInitDistanceMin = ((PlaneX(1,1)-TargetX(1,1))^2+(PlaneY(1,1)-TargetY(1,1))^2+(PlaneZ(1,1)-TargetZ(1,1))^2)^0.5;
if(TargetCount>1)
for i = 2:1:TargetCount
    TargetInitDistance = ((PlaneX(1,1)-TargetX(i,1))^2+(PlaneY(1,1)-TargetY(i,1))^2+(PlaneZ(1,1)-TargetZ(i,1))^2)^0.5;
    if(TargetInitDistance<=TargetInitDistanceMin)
       TargetInitDistanceMin = TargetInitDistance;
       TargetChoice = i;
    end
end
end

while(t_current~=t_count)
    if (attack_plane_true(1,3*t_current+1)~=0) || (attack_plane_true(1,3*t_current+2)~=0) || (attack_plane_true(1,3*t_current+3)~=0)
    PlaneX = cat(2,PlaneX,attack_plane_true(1,3*t_current+1));
    PlaneY = cat(2,PlaneY,attack_plane_true(1,3*t_current+2));
    PlaneZ = cat(2,PlaneZ,attack_plane_true(1,3*t_current+3));
    TargetX = cat(2,TargetX,attack_target_fusion(:,6*t_current+1));
    TargetY = cat(2,TargetY,attack_target_fusion(:,6*t_current+2));
    TargetZ = cat(2,TargetZ,attack_target_fusion(:,6*t_current+3));
    %TargetX = cat(2,TargetX,attack_target_fusion(:,3*t_current+1));
    %TargetY = cat(2,TargetY,attack_target_fusion(:,3*t_current+2));
    %TargetZ = cat(2,TargetZ,attack_target_fusion(:,3*t_current+3));
    t_current = t_current+1;
    else
    break
    end
end

RelativeDistance = ((TargetX(TargetChoice,end-1)-PlaneX(1,end-1))^2+(TargetY(TargetChoice,end-1)-PlaneY(1,end-1))^2+(TargetZ(TargetChoice,end-1)-PlaneZ(1,end-1))^2)^0.5;

figure(5);
if(PlaneX(1,end)==0 && PlaneY(1,end)==0 && PlaneZ(1,end)==0)
plot3(PlaneX(1,1:end-1),PlaneY(1,1:end-1),PlaneZ(1,1:end-1),'red','linewidth',2);
else
plot3(PlaneX(1,:),PlaneY(1,:),PlaneZ(1,:),'red','linewidth',2);
end
hold on;
grid on;
if(TargetX(TargetChoice,end)==0 && TargetY(TargetChoice,end)==0 && TargetZ(TargetChoice,end)==0)
plot3(TargetX(TargetChoice,2:end-1),TargetY(TargetChoice,2:end-1),TargetZ(TargetChoice,2:end-1),'linewidth',2);
else
plot3(TargetX(TargetChoice,2:end),TargetY(TargetChoice,2:end),TargetZ(TargetChoice,2:end),'linewidth',2);
end
hold on;
grid on;
if(RelativeDistance<RelativeDistanceMin)
    t=linspace(0,10*pi,25);
    p=linspace(0,20*pi,25);
   [theta,phi]=meshgrid(t,p);
   x=1000*sin(theta).*sin(phi)+TargetX(TargetChoice,end-1);
   y=1000*sin(theta).*cos(phi)+TargetY(TargetChoice,end-1);
   z=1000*cos(theta)+TargetZ(TargetChoice,end-1);
   hold on
   g=surf(x,y,z);
   grid on
   axis equal;
   set(g,'EdgeColor',[1 0 0],'FaceColor',[1 0 0],'linewidth',2);
   alpha(g,0.1);
end
xlabel('x/m');
ylabel('y/m');
zlabel('z/m');
end


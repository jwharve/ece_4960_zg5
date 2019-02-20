clearvars; close all; clc; % all units in inches
%% INPUTS
mount_radius = 0.5;
mount_height = 2;

post_height = 18;
post_radius = 18+1/3;

dish_radius = 6;
dish_height = 2;

%% LOCATIONS [x, y, z]

% relative hole location for cable attachments
h1 = [0, -mount_radius, mount_height];
h2 = [mount_radius*sind(60), mount_radius*cosd(60), mount_height];
h3 = [-mount_radius*sind(60), mount_radius*cosd(60), mount_height];
holes = [h1; h2; h3; h1];
clear h1 h2 h3

% post positions
p1 = [0, -post_radius, post_height];
p2 = [post_radius*sind(60), post_radius*cosd(60), post_height];
p3 = [-post_radius*sind(60), post_radius*cosd(60), post_height];
posts = [p1; p2; p3];
clear p1 p2 p3

% dish
x = linspace(-dish_radius, dish_radius, 300);
y1 = sqrt(dish_radius^2 - x.^2);
y2 = -y1;
x = [x fliplr(x) x(1)];
y = [y1 fliplr(y2) y1(1)];
z = repmat(dish_height, size(x));
dish = [x' y' z'];
clear x y1 y2 y z

%% PATH
r = 3;
theta = 180:10:360;
xs = r*cosd(theta);
ys = r*sind(theta) - 1;

r = 1;
theta = 0:20:360;
xe = r*cosd(theta);
ye = r*sind(theta);

x_path = [xs xe+2 xe-2];
y_path = [ys ye+2 ye+2];
%% PLOT
f = figure(1); hold on; grid on; axis equal; axis([-17 17 -19 10 0 20]);
set(gcf, 'Position',  [200, 150, 1000, 800])

scatter3(posts(:,1),posts(:,2), posts(:,3),'MarkerFaceColor','k','MarkerEdgeColor','k');
% plot3([posts(:,1); posts(1,1)], [posts(:,2); posts(1,2)], [posts(:,3); posts(1,3)],'r-');

plot3(dish(:,1),dish(:,2),dish(:,3),'k-');

view(-29,45)

handles = nan(1,4);
d = nan(1,3);
m = repmat(getframe,size(x_path));
for ii = 1:numel(x_path)
    x = x_path(ii);
    y = y_path(ii);
    z = 2;
    handles = nan(1,4);
    handles(1) = plot3(x+holes(:,1), y+holes(:,2), z+holes(:,3),'b-');
    scatter3(x,y,z,'r.');
    for jj = 1:3
        handles(jj+1) = plot3([posts(jj,1);x+holes(jj,1)], [posts(jj,2);y+holes(jj,2)], [posts(jj,3);z+holes(jj,3)], 'g-');
        d(jj) = sqrt((-posts(jj,1)+x+holes(jj,1))^2 + (-posts(jj,2)+y+holes(jj,2))^2 + (-posts(jj,3)+z+holes(jj,3))^2);
        fprintf('L%d - %0.3f\t\t',jj,d(jj));
    end
    fprintf('\n');
    pause(0.2);
    title(['L1 = ' num2str(d(1),'%2.3f') '     L2 = ' num2str(d(2),'%2.3f') '     L3 = ' num2str(d(3),'%2.3f')]);
    m(ii) = getframe(f);
    for jj = 1:4
        if ii ~= numel(x_path)
            delete(handles(jj));
        end
    end
end

v = VideoWriter('kinematics.avi');
v.FrameRate = 10;
open(v);
writeVideo(v,m);
close(v);

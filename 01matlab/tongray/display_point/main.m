clc;
%读取显示三向车地图
miniMAP=readmatrix("miniMAP01.txt");
plot(miniMAP(:,2),miniMAP(:,3),"*");

%读取显示mini车地图
hold on;
sanMAP=readmatrix("MAP01.txt");
plot(sanMAP(:,2),sanMAP(:,3),"r*");

%上一次车体的坐标
car_x=42.420376;
car_y=38.197437;
car_angle=270.100687;

%成功匹配的点(第2、3列)
hold on;
SUCCESS=readmatrix("success.txt");
plot(SUCCESS(:,2),SUCCESS(:,3),"ro");

%高反(第一、二列)
hold on;
gc=readmatrix("高反.txt");
distance=sqrt(gc(:,1).^2+gc(:,2).^2);
theta=atan2d(gc(:,2),gc(:,1))+car_angle;
x=distance.*cosd(theta)+car_x;
y=distance.*sind(theta)+car_y;
plot(x,y,"bo");

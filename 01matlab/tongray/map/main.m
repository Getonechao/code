%管子半径
pipe_R=0.035
%极坐标
point1_dist=33.910;
point1_angle=360-dms2degrees([0 4 30]);

point2_1_dist=10.180;
point2_1_angle=360-dms2degrees([264 2 10]);

point2_2_dist=24.066;
point2_2_angle=360-dms2degrees([269 23 13]);

point2_3_dist=16.547;
point2_3_angle=360-dms2degrees([246 5 59]);

%笛卡尔坐标
point1_x=point1_dist*cosd(point1_angle)+15;
point1_y=point1_dist*sind(point1_angle)+15;

point2_1_x=point2_1_dist*cosd(point2_1_angle)+point1_x;
point2_1_y=point2_1_dist*sind(point2_1_angle)+point1_y;

point2_2_x=point2_2_dist*cosd(point2_2_angle)+point1_x;
point2_2_y=point2_2_dist*sind(point2_2_angle)+point1_y;

point2_3_x=point2_3_dist*cosd(point2_3_angle)+point1_x;
point2_3_y=point2_3_dist*sind(point2_3_angle)+point1_y;

%参考点一
map_p2_1=readmatrix("2.1.txt");
mark2_1_x=(map_p2_1(:,5)+pipe_R).*cosd(360-dms2degrees(map_p2_1(:,2:4)))+point2_1_x;
mark2_1_y=(map_p2_1(:,5)+pipe_R).*sind(360-dms2degrees(map_p2_1(:,2:4)))+point2_1_y;


%参考点二
map_p2_2=readmatrix("2.2.txt");
mark2_2_x=(map_p2_2(:,5)+pipe_R).*cosd(360-dms2degrees(map_p2_2(:,2:4)))+point2_2_x;
mark2_2_y=(map_p2_2(:,5)+pipe_R).*sind(360-dms2degrees(map_p2_2(:,2:4)))+point2_2_y;

%参考点三
% map_p2_3=readmatrix("2.3.txt");
map_p2_3=[21 306 24 22 1.691];
mark2_3_x=(map_p2_3(:,5)+pipe_R).*cosd(360-dms2degrees(map_p2_3(:,2:4)))+point2_3_x;
mark2_3_y=(map_p2_3(:,5)+pipe_R).*sind(360-dms2degrees(map_p2_3(:,2:4)))+point2_3_y;

%合并矩阵
mark=[mark2_1_x mark2_1_y;
      mark2_2_x mark2_2_y;
      mark2_3_x mark2_3_y;]

%图形化显示  
plot(mark(:,1),mark(:,2),"r*");


%核查
map_p3=readmatrix("核查.txt");
mark3_x=(map_p3(:,5)+pipe_R).*cosd(360-dms2degrees(map_p3(:,2:4)))+point2_3_x;
mark3_y=(map_p3(:,5)+pipe_R).*sind(360-dms2degrees(map_p3(:,2:4)))+point2_3_y;

hold on;
plot(mark3_x,mark3_y,"b*")

%写到文件中
mark=round(mark,4);
frist_col=round((linspace(1,length(mark),length(mark))+1)',0);
four_col=round((linspace(0,0,length(mark)))',0);
mark=[frist_col mark four_col]
writematrix(mark,"MAP01.txt");
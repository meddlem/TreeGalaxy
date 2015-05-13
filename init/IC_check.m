fileID = fopen('galaxy.txt','r');


formatSpec = '%f\t%f\t%f\t%f';
sizeA = [4 2000];

A = fscanf(fileID,formatSpec,sizeA);
fclose(fileID);

scatter (A(1,:),A(2,:))
% quiver(A(1,:),A(2,:),A(3,:),A(4,:))
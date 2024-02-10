clc
clear
close all

data = csvread('mag.csv');
mx = data(:,1);
my = data(:,2);
mz = data(:,3);

mx = -mx;

figure;
plot3(mx, my, mz, '.', 'LineWidth', 2); grid on;

psi = atan2(-my, mz);

figure;
plot(rad2deg(psi), '.', 'LineWidth', 2); grid on;

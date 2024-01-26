% Plot PID response of motor omega

clc
clear
close all

% Import and parse csv file
data = csvread("response.csv");
sp = data(:,1);
response = data(:,1);

% Visualization
figure;
plot(sp, 'LineWidth', 2); hold on;
plot(response, 'LineWidth', 2);
xlim([0, length(data)]);
legend('set point', 'response');
title('Motor angular velocity [rad/s] response');
grid on;

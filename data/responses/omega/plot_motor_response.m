% Plot PID response of motor omega

clc
clear
close all

dt = 0.025; % Sample time, s

% Import and parse csv file
data = csvread("202401261954.csv");
sp = data(:,1);
response = data(:,2);

% Generate time vector
t = (0:length(sp)-1) * dt;

% Visualization
figure;
plot(t, sp, 'LineWidth', 2); hold on;
plot(t, response, 'LineWidth', 2);
xlim([0, max(t)]);
legend('set point', 'response');
title('PID control of motor angular velocity [rad/s]');
ylabel('Motor \omega, rad/s');
xlabel('Time, s');
grid on;

%{
  Simple script to tune value of cut-off frequency for RC-LPF

  Instructions:
    1. Set the value of sample time (dt).
    2. Manually play with the value of fc for suitable response.
       This value of fc can be used for c implementation.

  2024-01-26
%}

clc
clear
close all

% Set these two values manually
dt = 0.025; % Sample time, s
fc = 0.9; % Cut-off frequency

% set_point, raw w
w = csvread("motor_omega_25_ms.csv");

% Intermediate variables
RC = 1 / (2 * pi * fc);
xi = 2 * RC / dt;

% LPF
wz = w(1);
w_lpf = zeros(size(w));
for i = 2:length(w)
  w_lpf(i) = (-w_lpf(i-1) * (1 - xi) + w(i) + w(i-1)) / (1 + xi);
end

% Generate time vector using dt
figure;
t = 0:length(w)-1;
t = t * dt;

% Visualize
plot(t, w, 'LineWidth', 2); hold on;
plot(t, w_lpf, 'LineWidth', 2); grid on;
xlim([0, max(t)]);
title("RC-LPF on motor angular velocity [rad/s]");
legend('raw \omega', 'filtered \omega');
xlabel('Time, s');
ylabel('Motor \omega, rad/s');

clearvars
close all
clc

%% Ottimizzatore Simplex

trunk_a = deg2rad(18);
trunk_vel = 3.5;

Pham = 1.91;
Aham = deg2rad(5);
Dham = 0.2;
Pglu = 1.91;
Aglu = deg2rad(5);
Dglu = 0.2;
Phfl = 1.91;
Ahfl = deg2rad(5);
Dhfl = 0.2;

v = [ trunk_a trunk_vel Pham Aham Dham Pglu Aglu Dglu Phfl Ahfl Dhfl];
tic
cost = CallNewton(v);
toc
% fun = @CallNewton;
% x0 = v;
% options = optimset('PlotFcns',@optimplotfval);
% x = fminsearch(fun,x0, options)
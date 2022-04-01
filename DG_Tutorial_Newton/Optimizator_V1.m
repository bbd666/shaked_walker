clearvars
close all
clc

%% Ottimizzatore Simplex

trunk_a = -deg2rad(15);
AlphaR = -deg2rad(5);% alpha right see model notes figure 1
AlphaL = deg2rad(25);% alpha left leg see model notes figure 1
Beta = deg2rad(10);% beta see model notes figure 1
Gamma = deg2rad(-10);% gamma see model notes figure 1
trunk_vel = -2.0;

Pham = 2.0;
Aham = deg2rad(5);
Dham = 0.2;
Pglu = 2.0;
Aglu = deg2rad(5);
Dglu = 0.2;
Phfl = 2.0;
Ahfl = deg2rad(5);
Dhfl = 0.2;

Gf_glu = 0.1;
Gf_ham = 0.00;
Gf_vas = 1.4;
Gf_sol = 1.2;
Gf_gas = 2.0;
Gf_tasol = 0.4;

Glg_hfl = 4.5;
Glg_ham = 0.5;
Glg_ta = 0.45;

Glh_hfl = 1.0;
Glh_ham = 0.85;
Glh_ta = 0.72;

v = [ trunk_a AlphaR AlphaL Beta Gamma trunk_vel...
    Pham Aham Dham Pglu Aglu Dglu Phfl Ahfl Dhfl...
    Gf_glu Gf_ham Gf_vas Gf_sol Gf_gas Gf_tasol ...
    Glg_hfl Glg_ham Glg_ta Glh_hfl Glh_ham Glh_ta];

tic
cost = CallNewton(v);
toc

% fun = @CallNewton;
% x0 = v;
% options = optimset('PlotFcns',@optimplotfval,'MaxIter',100);
% x = fminsearch(fun,x0, options)
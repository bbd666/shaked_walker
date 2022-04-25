clearvars
close all
clc

%% Ottimizzatore Simplex

trunk_a = -deg2rad(5);
AlphaR = -deg2rad(5);% alpha right see model notes figure 1
AlphaL = deg2rad(30);% alpha left leg see model notes figure 1
Beta = deg2rad(10);% beta see model notes figure 1
Gamma = deg2rad(-6);% gamma see model notes figure 1
head = deg2rad(0);% gamma see model notes figure 1
vel = -3.2;% gamma see model notes figure 1

Pham = 2.0;
Aham = deg2rad(5);
Dham = 0.2;
Pglu = 2.0;
Aglu = deg2rad(5);
Dglu = 0.2;
Phfl = 2.0;
Ahfl = deg2rad(5);
Dhfl = 0.2;

Gf_glu = 0.5;
Gf_ham = 0.5;
Gf_vas = 1.0;
Gf_sol = 1.2;
Gf_gas = 1.5;
Gf_tasol = 0.3;

Glg_hfl = 4.5;
Glg_ham = 1.0;
Glg_ta = 0.9;

Glh_hfl = 0.65;
Glh_ham = 0.85;
Glh_ta = 0.72;

% coronal lead PD
trunk_p = 2000;
trunk_v = 200;
foot_p = 300;
foot_v = 30;

v = [ trunk_a AlphaR AlphaL Beta Gamma head vel...
    Pham Aham Dham Pglu Aglu Dglu Phfl Ahfl Dhfl...
    Gf_glu Gf_ham Gf_vas Gf_sol Gf_gas Gf_tasol ...
    Glg_hfl Glg_ham Glg_ta Glh_hfl Glh_ham Glh_ta ...
    trunk_p trunk_v foot_p foot_v];

% tic
% cost = CallNewton(v);
% toc

fun = @CallNewton;
x0 = v;
options = optimset('PlotFcns',@optimplotfval,'MaxIter',1000);
x = fminsearch(fun,x0, options);

save('Param','x');
close all
clearvars
clc

%% Main Optimization

% params
% load Param01.mat
% Params0 = abs(x);
% Params0(7) = 1.8;
% Params0 = [Params0 150 10 30 3 200 10];
Params0 = [deg2rad(5) deg2rad(5) deg2rad(10) deg2rad(5) deg2rad(5) 0 1.8 ...
            1.9 deg2rad(5) 0.2 1.9 deg2rad(5) 0.2 1.9 deg2rad(5) 0.2 ...% cambiata come in geyer
            0.5 0.5 1.0 1.2 1.4 0.3 ... % cambiata come in geyer
            4.5 1.0 0.63 0.65 0.85 0.72 ...% cambiata come in geyer
            1200 100 120 10 ...
            150 10 30 3 200.0 10.0];
        
%             trunk      %alfar     % alfaL     %beta      %gamma    %head
%             vel   
%             stance lead params as P, Angle, D of HAM GLU HFL
%             force feedback GLU HAM VAS SOL GAS TA_SOL
%             length feedback as g for HFL HAM TA and and h for HFL HAM TA
%             coronal lead  as P and V for trunk and foot
%             coronal as P and V for trunk and foot
Param_min = [deg2rad(0) deg2rad(0) deg2rad(0) deg2rad(0) deg2rad(0) deg2rad(0) 0 ...
                1.78   deg2rad(0.97) 0.1 1.78   deg2rad(0.97) 0.1 1.78   deg2rad(0.97) 0.1 ...
                0 0 0.82 0.97 0 0 ...
                0.17 0 0.55 0 0.6 0.59 ... % il quinto è 0.83 per geyer
                100 10 10 1 ...
                100 0 1 0 50 0];
Param_max = [deg2rad(20) deg2rad(20) deg2rad(20) deg2rad(20) deg2rad(20) deg2rad(20) 3 ...
                22   deg2rad(6.3) 0.5 22   deg2rad(6.3) 0.5 22   deg2rad(6.3) 0.5 ...
                0.52 0.67 13.5 2.17 10 10 ...
                5  100 3.2 0.67 10 0.8 ...%% aumentato il primo a 5
                3000 300 300 30 ...
                200.0 30.0 100.0 10.0 500 50];
Params_norm = (Params0 - Param_min) ./ (Param_max-Param_min);

%%%%%%%%%%%%%%%%% CMA %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
opts.LBounds = 0; opts.UBounds = 10; opts.MaxIter = 1000;
[xmin,fmin,counteval,stopflag,out,bestever]=cmaes('CallNewton', Params_norm, 0.005, opts);
%   search within lower bound of 0 and upper bound of 10. Bounds can
%   also be given as column vectors. If the optimum is not located
%   on the boundary, use rather a penalty approach to handle bounds. 

% %% Nelder Meada % non migliora velocemente
% 
% fun = @CallNewton;
% x0 = Params_norm;
% options = optimset('PlotFcns',@optimplotfval,'MaxIter',25);
% x = fminsearch(fun,x0, options);
% 
% % save('Param','x');
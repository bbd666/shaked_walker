close all
clearvars
clc

%% Main Optimization

% params
load Res_CMA.mat
Params0 = abs(bestever.x);

% Params0 = [0.087 0.087 0.17 0.087 0.087 8e-07 1.5 ...
%             1.91 0.105 0.25 1.91 0.105 0.25 1.91 0.105 0.25 ...% cambiata come in geyer
%             0.4 0.65 1.15 1.2 1.1 0.3 ... % cambiata come in geyer
%             0.35 0.6 4 0.85 1.1 0.71 ...% cambiata come in geyer
%             1258 150 120 10];
        
%             trunk      %alfar     % alfaL     %beta      %gamma    %head
%             vel   
%             stance lead params as P, Angle, D of HAM GLU HFL
%             force feedback GLU HAM VAS SOL GAS TA_SOL
%             length feedback as g for HFL HAM TA and and h for HFL HAM TA
%             coronal lead  as P and V for trunk and foot
Param_min = [deg2rad(0) deg2rad(0) deg2rad(0) deg2rad(0) deg2rad(0) deg2rad(0) 0 ...
                1.78   deg2rad(0.97) 0.1 1.78   deg2rad(0.97) 0.1 1.78   deg2rad(0.97) 0.1 ...
                0 0 0.82 0.97 0 0 ...
                0.17 0 0.55 0 0.6 0.59 ... % il quinto è 0.83 per geyer
                100 10 10 1];
Param_max = [deg2rad(20) deg2rad(20) deg2rad(20) deg2rad(20) deg2rad(20) deg2rad(20) 3 ...
                22   deg2rad(6.3) 0.5 22   deg2rad(6.3) 0.5 22   deg2rad(6.3) 0.5 ...
                0.52 0.67 13.5 2.17 10 10 ...
                5  100 3.2 0.67 10 0.8 ...%% aumentato il primo a 5
                3000 300 300 30];
Params_norm = (Params0 - Param_min) ./ (Param_max-Param_min);

%%%%%%%%%%%%%%%%% CMA %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
opts.LBounds = 0; opts.UBounds = 10; opts.MaxIter = 50;
[xmin,fmin,counteval,stopflag,out,bestever]=cmaes('CallNewton', Params_norm, 0.005, opts);
%   search within lower bound of 0 and upper bound of 10. Bounds can
%   also be given as column vectors. If the optimum is not located
%   on the boundary, use rather a penalty approach to handle bounds. 

save('Res_CMA','xmin','fmin','counteval','stopflag','out','bestever')
% %% Nelder Meada % non migliora velocemente
% 
% fun = @CallNewton;
% x0 = Params_norm;
% options = optimset('PlotFcns',@optimplotfval,'MaxIter',25);
% x = fminsearch(fun,x0, options);
% 
% % save('Param','x');
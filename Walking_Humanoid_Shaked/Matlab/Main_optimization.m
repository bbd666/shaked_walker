close all
clearvars
clc

%% Main Optimization

% parameter name 
% trunk
% alfaR
% alfaL
% beta
% gamma
% head
% vel
% hamP
% hamA
% hamD
% gluP
% glua
% gluD
% hflP
% hflA
% hflD
% glu
% ham
% vas
% sol
% gas
% tasol
% hflg
% hamg
% tag
% hflh
% hamh
% tah
% trunkp
% trunkv
% footp
% footv
% trunkp
% trunkv
% footp
% footv
% ttrunkp
% ttrunkv
% alpha
% elbow
% vas
% rf
% glu
% hfl
% vas
% hip
% hflk
% hfld
% hfla
% gluk
% glud
% glua
% vask
% vasd
% vasa
% cd
% cv
% cd1
% cv1
% hflk
% vask
% vasa

OUTPUT = 'params_500iter_roll10Hz';% CHANGE OUTPUT FILE NAME;
% load params_Initial.mat 
load params_500iter_roll07Hz.mat % CHANGE

%%%%%%%%%%%%%%%%% CMA %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% opts = cmaes('displayoptions');

opts.LBounds = 0; opts.UBounds = 1; opts.MaxIter = 500;
[xmin,fmin,counteval,stopflag,out,bestever]=cmaes('CallNewton', bestever.x, 0.2, opts);% explores large space around initial guess.
%   search within lower bound of 0 and upper bound of 1. Bounds can
%   also be given as column vectors. If the optimum is not located
%   on the boundary, use rather a penalty approach to handle bounds. 
% 
write_params_to_xml(bestever.x);
save(OUTPUT,'bestever','xmin','fmin','counteval','stopflag','out'); 

% %% Nelder Meada % explores around the local optimum of initial guess.
% 
% fun = @CallNewton;
% x0 = Params_norm;
% options = optimset('PlotFcns',@optimplotfval,'MaxIter',25);
% x = fminsearch(fun,x0, options);





function [cost] = CallNewton(v)
%UNTITLED Summary of this function goes here
%   Detailed explanation goes here

command= ['DG_Tutorial_NewtonGL '];

for ii = 1:length(v)
    command = [command ' ' char(string(v(ii)))];
end
cost = system(command);
end


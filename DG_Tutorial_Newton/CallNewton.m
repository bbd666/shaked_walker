function [cost] = CallNewton(v)
%UNTITLED Summary of this function goes here
%   Detailed explanation goes here

command= ['DG_Tutorial_NewtonGL ' char(string(v(1))) ' ' char(string(v(2))) ' '...
    char(string(v(3))) ' ' char(string(v(4))) ' ' char(string(v(5))) ' '...
    char(string(v(6))) ' ' char(string(v(7))) ' ' char(string(v(8))) ' '...
    char(string(v(9))) ' ' char(string(v(10))) ' ' char(string(v(11))) ' '];

cost = system(command);
end


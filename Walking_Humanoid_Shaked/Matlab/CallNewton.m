function [cost] = CallNewton(v)
%Calls newton executable with a set of parameters (V)
%   input: v is the vector containing simulation parameters
%   output: cost is the simulation cost returned from simulation
dir1 = cd;
cd ..
command= ['Walking_Humanoid_Shaked'];

write_params_to_xml(v);

cost = 1;
    while cost <= 1% .exe returns 1 if simulation did not start. In this case re-run simulation.
        cost = system(command);
%             system('TASKKILL /F /IM Walking_Humanoid_Shaked.exe /T')
    end
cd(dir1)
end
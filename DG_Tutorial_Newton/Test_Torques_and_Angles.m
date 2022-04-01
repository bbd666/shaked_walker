close all
clearvars
clc
mass = 73; % body mass
%% load data
filename = 'history.txt';
delimiter = ' ';
formatSpec = '%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%[^\n\r]';
fileID = fopen(filename,'r');
dataArray = textscan(fileID, formatSpec, 'Delimiter', delimiter, 'MultipleDelimsAsOne', true, 'TextType', 'string', 'EmptyValue', NaN,  'ReturnOnError', false);
fclose(fileID);
history = table(dataArray{1:end-1}, 'VariableNames', {'hamr_T','hamr_T1','glur_T','glur_T1','hflr_T','hflr_T1','rfr_T','rfr_T1',...
    'vasr_T','vasr_T1','solr_T','solr_T1','tar_T','tar_T1','gasr_T','gasr_T1',...
    'haml_T','haml_T1','glul_T','glul_T1','hfll_T','hfll_T1','rfl_T','rfl_T1',...
    'vasl_T','vasl_T1','soll_T','soll_T1','tal_T','tal_T1','gasl_T','gasl_T1',...
    'time','hipr_a','kneer_a','ankler_a','hipl_a','kneel_a','anklel_a','trunk_ang','trunk_vel',...
        'stance_r','swing_r','sp_r','si_r',...
    'stance_l','swing_l','sp_l','si_l',});
clearvars filename delimiter formatSpec fileID dataArray ans;

%% Plot
for x = 1:2
    if x == 1
        lat = 'r';
    else
        lat = 'l';
    end
figure(x)
subplot(3,2,1)
hold on
eval([char("plot(history.time,history.hip") lat char("_a/pi*180,'k','LineWidth',2)")])
ylabel('Hip flexion [°]')
yyaxis right
eval([char("plot(history.time,history.stance_") lat char(",'or')")])
eval([char("plot(history.time,history.swing_") lat char(",'ok')")])
eval([char("plot(history.time,history.sp_") lat char(",'ob')")])
eval([char("plot(history.time,history.si_") lat char(",'og')")])
xlabel('Time [s]')
ax6 = gca;
ax6.Box = 'On';
ax6.FontSize = 12;
legend('angle','Stance','Swing','SP','SI')
if x == 1
    title('Right')
else
    title('Left')
end
%ylim([100 250])
hold off

subplot(3,2,2)
hold on
eval([char("plot(history.time,history.hfl") lat char("_T/mass)")])
eval([char("plot(history.time,history.glu") lat char("_T/mass)")])
eval([char("plot(history.time,history.ham") lat char("_T/mass)")])
eval([char("plot(history.time,history.rf") lat char("_T/mass)")])
eval([char("plot(history.time,(history.hfl") lat char("_T+history.ham") ...
    lat char("_T+history.rf") lat char("_T+history.glu") lat char("_T)/mass,'-r','LineWidth',3)")])
ylabel('Hip extension [Nm/kg]')
xlabel('Time [s]')
legend('HFL','GLU','HAM','RF','total')
ax1 = gca;
ax1.Box = 'On';
ax1.FontSize = 12;
ylim([-4 4])
hold off

subplot(3,2,3)
hold on
eval([char("plot(history.time,history.knee") lat char("_a/pi*180,'k','LineWidth',2)")])
ylabel('Knee flexion [°]')
xlabel('Time [s]')
ax2 = gca;
ax2.Box = 'On';
ax2.FontSize = 12;
%ylim([0 200])
hold off

subplot(3,2,4)
hold on
eval([char("plot(history.time,history.ham") lat char("_T1/mass)")])
eval([char("plot(history.time,history.rf") lat char("_T1/mass)")])
eval([char("plot(history.time,history.vas") lat char("_T/mass)")])
eval([char("plot(history.time,history.gas") lat char("_T/mass)")])
eval([char("plot(history.time,(history.ham") lat char("_T1+history.rf") ...
    lat char("_T1+history.vas") lat char("_T+history.gas") lat char("_T)/mass,'-r','LineWidth',3)")])
ylabel('Knee extension [Nm/kg]')
xlabel('Time [s]')
legend('HAM','RF','VAS','GAS','total')
ax3 = gca;
ax3.Box = 'On';
ax3.FontSize = 12;
ylim([-4 4])
hold off

subplot(3,2,5)
hold on
eval([char("plot(history.time,history.ankle") lat char("_a/pi*180,'k','LineWidth',2)")])
ylabel('Ankle dorsiflection [°]')
xlabel('Time [s]')
ax4 = gca;
ax4.Box = 'On';
ax4.FontSize = 12;
hold off

subplot(3,2,6)
hold on
eval([char("plot(history.time,-history.ta") lat char("_T/mass)")])
eval([char("plot(history.time,-history.sol") lat char("_T/mass)")])
eval([char("plot(history.time,-history.gas") lat char("_T1/mass)")])
eval([char("plot(history.time,-(history.ta") lat char("_T+history.sol") ...
    lat char("_T+history.gas") lat char("_T1)/mass,'-r','LineWidth',3)")])
ylabel('Ankle plantarflexion [Nm/kg]')
xlabel('Time [s]')
legend('TA','SOL','GAS','total')
ax5 = gca;
ax5.Box = 'On';
ax5.FontSize = 12;
ylim([-2 2])
hold off

linkaxes([ax1 ax2 ax3 ax4 ax5 ax6],'x')
xlim(ax1,[0 max(history.time)])
end

figure
hold on
subplot(2,1,1)
plot(history.time,history.trunk_ang*180/pi)
ylabel('Trunk sagittal inclination [°]')
xlabel('Time [s]')
subplot(2,1,2)
plot(history.time,history.trunk_vel)
ylabel('Vel trunk [°/s]')
xlabel('Time [s]')
xlim([0 max(history.time)])
hold off
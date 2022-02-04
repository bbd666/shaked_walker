close all
clearvars
clc

%% load data
filename = 'history.txt';
delimiter = ' ';
formatSpec = '%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%[^\n\r]';
%formatSpec = '%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%[^\n\r]';
fileID = fopen(filename,'r');
dataArray = textscan(fileID, formatSpec, 'Delimiter', delimiter, 'MultipleDelimsAsOne', true, 'TextType', 'string', 'EmptyValue', NaN,  'ReturnOnError', false);
fclose(fileID);
% history = table(dataArray{1:end-1}, 'VariableNames', {'ham_l','glu_l','hfl_l','rf_l','vas_l',...
%     'sol_l', 'ta_l','gas_l','time','hip_r','knee_r','ankle_r','hip_l','knee_l','ankle_l'...
%     'stance_r','swing_r','sp_r','si_r',...
%     'stance_l','swing_l','sp_l','si_l'});
history = table(dataArray{1:end-1}, 'VariableNames', {'ham_r','glu_r','hfl_r','rf_r','vas_r',...
    'sol_r', 'ta_r','gas_r',...
    'ham_l','glu_l','hfl_l','rf_l','vas_l',...
    'sol_l', 'ta_l','gas_l','time','hip_r','knee_r','ankle_r','hip_l','knee_l','ankle_l'...
    'stance_r','swing_r','sp_r','si_r',...
    'stance_l','swing_l','sp_l','si_l'});
clearvars filename delimiter formatSpec fileID dataArray ans;

%% Plot
figure
subplot(3,2,1)
hold on
plot(history.time,history.hip_r/pi*180,'k','LineWidth',2)
ylabel('Hip rigth angle [°]')
yyaxis right
plot(history.time,history.stance_r,'or')
plot(history.time,history.swing_r,'ok')
plot(history.time,history.sp_r,'ob')
plot(history.time,history.si_r,'og')
xlabel('Time [s]')
ax6 = gca;
ax6.Box = 'On';
ax6.FontSize = 12;
%ylim([100 250])
legend('angle','Stance','Swing','SP','SI')
title('Right')
hold off

subplot(3,2,2)
hold on
plot(history.time,history.hfl_r)
plot(history.time,history.glu_r)
plot(history.time,history.ham_r)
plot(history.time,history.rf_r)
ylabel('Hip excitations [-]')
xlabel('Time [s]')
legend('HFL','GLU','HAM','RF')
ax1 = gca;
ax1.Box = 'On';
ax1.FontSize = 12;
ylim([0 2])
hold off

subplot(3,2,3)
hold on
plot(history.time,360-history.knee_r/pi*180,'k','LineWidth',2)
ylabel('Knee angle [°]')
xlabel('Time [s]')
ax2 = gca;
ax2.Box = 'On';
ax2.FontSize = 12;
%ylim([0 200])
hold off

subplot(3,2,4)
hold on
plot(history.time,history.vas_r)
plot(history.time,history.gas_r)
ylabel('Knee excitations [-]')
xlabel('Time [s]')
legend('VAS','GAS')
ax3 = gca;
ax3.Box = 'On';
ax3.FontSize = 12;
ylim([0 2])
hold off

subplot(3,2,5)
hold on
plot(history.time,history.ankle_r/pi*180,'k','LineWidth',2)
ylabel('Ankle angle [°]')
xlabel('Time [s]')
ax4 = gca;
ax4.Box = 'On';
ax4.FontSize = 12;
%ylim([50 200])
hold off

subplot(3,2,6)
hold on
plot(history.time,history.ta_r)
plot(history.time,history.sol_r)
ylabel('Ankle excitations [-]')
xlabel('Time [s]')
legend('TA','SOL')
ax5 = gca;
ax5.Box = 'On';
ax5.FontSize = 12;
%ylim([-8 8])
hold off

linkaxes([ax1 ax2 ax3 ax4 ax5 ax6],'x')
xlim(ax1,[0 max(history.time)])


%% second
figure
subplot(3,2,1)
hold on
plot(history.time,history.hip_l/pi*180,'k','LineWidth',2)
ylabel('Hip angle [°]')
yyaxis right
plot(history.time,history.stance_l,'or')
plot(history.time,history.swing_l,'ok')
plot(history.time,history.sp_l,'ob')
plot(history.time,history.si_l,'og')
xlabel('Time [s]')
xlabel('Time [s]')
ax6 = gca;
ax6.Box = 'On';
ax6.FontSize = 12;
title('Left')
legend('angle','Stance','Swing','SP','SI')
%ylim([100 250])
hold off

subplot(3,2,2)
hold on
plot(history.time,history.hfl_l)
plot(history.time,history.glu_l)
plot(history.time,history.ham_l)
plot(history.time,history.rf_l)
ylabel('Hip excitations [-]')
xlabel('Time [s]')
legend('HFL','GLU','HAM','RF')
ax1 = gca;
ax1.Box = 'On';
ax1.FontSize = 12;
ylim([0 2])
hold off

subplot(3,2,3)
hold on
plot(history.time,360-history.knee_l/pi*180,'k','LineWidth',2)
ylabel('Knee angle [°]')
xlabel('Time [s]')
ax2 = gca;
ax2.Box = 'On';
ax2.FontSize = 12;
%ylim([0 200])
hold off

subplot(3,2,4)
hold on
plot(history.time,history.vas_l)
plot(history.time,history.gas_l)
ylabel('Knee excitations [-]')
xlabel('Time [s]')
legend('VAS','GAS')
ax3 = gca;
ax3.Box = 'On';
ax3.FontSize = 12;
ylim([0 2])
hold off

subplot(3,2,5)
hold on
plot(history.time,history.ankle_l/pi*180,'k','LineWidth',2)
ylabel('Ankle angle [°]')
xlabel('Time [s]')
ax4 = gca;
ax4.Box = 'On';
ax4.FontSize = 12;
%ylim([50 200])
hold off

subplot(3,2,6)
hold on
plot(history.time,history.ta_l)
plot(history.time,history.sol_l)
ylabel('Ankle excitations [-]')
xlabel('Time [s]')
legend('TA','SOL')
ax5 = gca;
ax5.Box = 'On';
ax5.FontSize = 12;
%ylim([-8 8])
hold off

linkaxes([ax1 ax2 ax3 ax4 ax5 ax6],'x')
xlim(ax1,[0 max(history.time)])
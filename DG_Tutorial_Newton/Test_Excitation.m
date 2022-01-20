close all
clearvars
clc

%% load data
filename = 'history.txt';
delimiter = ' ';
formatSpec = '%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%[^\n\r]';
fileID = fopen(filename,'r');
dataArray = textscan(fileID, formatSpec, 'Delimiter', delimiter, 'MultipleDelimsAsOne', true, 'TextType', 'string', 'EmptyValue', NaN,  'ReturnOnError', false);
fclose(fileID);
history = table(dataArray{1:end-1}, 'VariableNames', {'ham_T','ham_T1','glu_T','glu_T1','hfl_T','hfl_T1','rf_T','rf_T1',...
    'vas_T','vas_T1','sol_T','sol_T1','ta_T','ta_T1','gas_T','gas_T1',...
    'time','hip_a','knee_a','ankle_a'});
clearvars filename delimiter formatSpec fileID dataArray ans;

%% Plot
figure
subplot(3,2,1)
hold on
plot(history.time,history.hip_a/pi*180,'k','LineWidth',2)
ylabel('Hip angle [°]')
xlabel('Time [s]')
ax6 = gca;
ax6.Box = 'On';
ax6.FontSize = 12;
%ylim([100 250])
hold off

subplot(3,2,2)
hold on
plot(history.time,history.hfl_T)
plot(history.time,history.glu_T)
plot(history.time,history.ham_T)
plot(history.time,history.rf_T)
ylabel('Hip excitations [-]')
xlabel('Time [s]')
legend('HFL','GLU','HAM','RF','Location','northeastoutside')
ax1 = gca;
ax1.Box = 'On';
ax1.FontSize = 12;
%%ylim([-8 8])
hold off

subplot(3,2,3)
hold on
plot(history.time,360-history.knee_a/pi*180,'k','LineWidth',2)
ylabel('Knee angle [°]')
xlabel('Time [s]')
ax2 = gca;
ax2.Box = 'On';
ax2.FontSize = 12;
%ylim([0 200])
hold off

subplot(3,2,4)
hold on
plot(history.time,history.vas_T)
plot(history.time,history.gas_T)
ylabel('Knee excitations [-]')
xlabel('Time [s]')
legend('VAS','GAS','Location','northeastoutside')
ax3 = gca;
ax3.Box = 'On';
ax3.FontSize = 12;
%ylim([-8 8])
hold off

subplot(3,2,5)
hold on
plot(history.time,history.ankle_a/pi*180,'k','LineWidth',2)
ylabel('Ankle angle [°]')
xlabel('Time [s]')
ax4 = gca;
ax4.Box = 'On';
ax4.FontSize = 12;
%ylim([50 200])
hold off

subplot(3,2,6)
hold on
plot(history.time,history.ta_T)
plot(history.time,history.sol_T)
ylabel('Ankle excitations [-]')
xlabel('Time [s]')
legend('TA','SOL','Location','northeastoutside')
ax5 = gca;
ax5.Box = 'On';
ax5.FontSize = 12;
%ylim([-8 8])
hold off

linkaxes([ax1 ax2 ax3 ax4 ax5 ax6],'x')
xlim(ax1,[0 1])
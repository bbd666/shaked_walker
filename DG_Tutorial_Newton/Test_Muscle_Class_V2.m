close all
clearvars
clc
mass = 73; % body mass
%% load data
filename = 'history.txt';
delimiter = ' ';
formatSpec = '%f%f%f%f%f%f%f%f%f%f%f%[^\n\r]';
fileID = fopen(filename,'r');
dataArray = textscan(fileID, formatSpec, 'Delimiter', delimiter, 'MultipleDelimsAsOne', true, 'TextType', 'string', 'EmptyValue', NaN,  'ReturnOnError', false);
fclose(fileID);
history = table(dataArray{1:end-1}, 'VariableNames', {'hfl_T','hfl_T1','glu_T','glu_T1','ham_T','ham_T1','rf_T','rf_T1','time','hip_a','knee_a'});
clearvars filename delimiter formatSpec fileID dataArray ans;

%% Plot
figure
subplot(2,2,1)
hold on
plot(history.time,history.hip_a/pi*180,'k','LineWidth',2)
ylabel('Hip angle [°]')
xlabel('Time [s]')
ax1 = gca;
ax1.Box = 'On';
ax1.FontSize = 12;
xlim([0 2])
ylim([100 250])
hold off

subplot(2,2,2)
hold on
plot(history.time,history.hfl_T/mass)
plot(history.time,history.glu_T/mass)
plot(history.time,history.ham_T/mass)
plot(history.time,history.rf_T/mass)
plot(history.time,(history.hfl_T+history.glu_T+history.ham_T+history.rf_T)/mass,'-k','LineWidth',4)
ylabel('Normalized Hip Torque [Nm/kg]')
xlabel('Time [s]')
legend('HFL','GLU','HAM','RF','total')
ax1 = gca;
ax1.Box = 'On';
ax1.FontSize = 12;
xlim([0 2])
ylim([-8 8])
hold off

subplot(2,2,3)
hold on
plot(history.time,history.knee_a/pi*180,'k','LineWidth',2)
ylabel('Knee angle [°]')
xlabel('Time [s]')
ax1 = gca;
ax1.Box = 'On';
ax1.FontSize = 12;
xlim([0 2])
ylim([100 250])
hold off

subplot(2,2,4)
hold on
plot(history.time,history.ham_T1/mass)
plot(history.time,history.rf_T1/mass)
plot(history.time,(history.ham_T1+history.rf_T1)/mass,'-k','LineWidth',4)
ylabel('Normalized Knee Torque [Nm/kg]')
xlabel('Time [s]')
legend('HAM','RF','total')
ax1 = gca;
ax1.Box = 'On';
ax1.FontSize = 12;
xlim([0 2])
ylim([-8 8])
hold off


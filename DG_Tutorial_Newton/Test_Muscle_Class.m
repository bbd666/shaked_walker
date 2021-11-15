close all
clearvars
clc

%% load data
filename = 'history.txt';
delimiter = ' ';
formatSpec = '%f%f%f%f%f%f%f%f%f%f%f%[^\n\r]';
fileID = fopen(filename,'r');
dataArray = textscan(fileID, formatSpec, 'Delimiter', delimiter, 'MultipleDelimsAsOne', true, 'TextType', 'string', 'EmptyValue', NaN,  'ReturnOnError', false);
fclose(fileID);
history = table(dataArray{1:end-1}, 'VariableNames', {'time','angle','fSE','fCE','fPE','fDE','iteration','lCE','dlCE','act','vel'});
clearvars filename delimiter formatSpec fileID dataArray ans;

%% Plot
figure(1)
subplot(311)
hold on
plot(history.time,history.fSE)
plot(history.time,history.fCE)
% plot(history.time,history.fCE+history.fPE+history.fDE)
plot(history.time,history.fPE)
plot(history.time,history.fDE)
ylabel('Normalized Force [-]')
yyaxis right
plot(history.time,history.iteration/100,'*')
plot(history.time,history.act)
ylabel('% iteration')
xlabel('Time [s]')
% legend('fSE','fCE+fPE+fDE','iteration','activation')
legend('fSE','fCE','fPE','fDE','iteration','activation')
ax1 = gca;
hold off

subplot(312)
hold on
yyaxis left
plot(history.time,history.angle*180/pi) %% theta
ylabel('Joint Angle [°]')
yyaxis right
plot(history.time,history.dlCE*100) %% lce
ylabel('dlCE [cm]')
xlabel('Time [s]')
hold off
ax2 = gca;

subplot(313)
yyaxis left
plot(history.time,history.lCE) %% lce
ylabel('lCE [cm]')
xlabel('Time [s]')
yyaxis right
plot(history.time,history.vel) %% vel ce
xlabel('Time [s]')
ylabel('vCE [cm/s]')
ax3 = gca;
linkaxes([ax1, ax2, ax3], 'x')

% name = input('test name','s');
% save(char(name),'history')
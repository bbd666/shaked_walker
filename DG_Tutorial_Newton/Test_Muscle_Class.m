close all
clearvars
clc

%% load data
filename = 'history.txt';
delimiter = ' ';
formatSpec = '%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f%[^\n\r]';
fileID = fopen(filename,'r');
dataArray = textscan(fileID, formatSpec, 'Delimiter', delimiter, 'MultipleDelimsAsOne', true, 'TextType', 'string', 'EmptyValue', NaN,  'ReturnOnError', false);
fclose(fileID);
history = table(dataArray{1:end-1}, 'VariableNames', {'time','angle','angle1','fSE','fCE','fPE','fDE','iteration','lCE','dlCE','act','vel','lmtu','T','T1'});
clearvars filename delimiter formatSpec fileID dataArray ans;

%% Plot
figure
subplot(311)
hold on
plot(history.time,history.fSE)
plot(history.time,history.fCE)
% plot(history.time,history.fCE+history.fPE+history.fDE)
plot(history.time,history.fPE)
plot(history.time,history.fDE)
ylabel('Normalized Force [-]')
% yyaxis right
% plot(history.time,history.iteration/100,'*')
% plot(history.time,history.act)
% ylabel('% iteration')
xlabel('Time [s]')
% legend('fSE','fCE+fPE+fDE','iteration','activation')
legend('fSE','fCE','fPE','fDE','iteration','activation')
ax1 = gca;
hold off

subplot(312)
hold on
yyaxis left
plot(history.time,history.angle*180/pi) %% theta
plot(history.time,history.angle1*180/pi) %% theta
legend('\theta', '\theta_1')
ylabel('Joint Angle [°]')
yyaxis right
plot(history.time,history.dlCE*100) %% lce
ylabel('dlCE [cm]')
xlabel('Time [s]')
hold off
ax2 = gca;

subplot(313)
yyaxis left
plot(history.time,history.lmtu) %% lce
ylabel('lmtu [cm]')
xlabel('Time [s]')
yyaxis right
plot(history.time,history.vel) %% vel ce
xlabel('Time [s]')
ylabel('vCE_t [l_o_p_t/s]')
ax3 = gca;
linkaxes([ax1, ax2, ax3], 'x')
xlim([0 1])

figure(2)
hold on
plot(history.time,history.fCE+history.fPE+history.fDE-history.fSE)
ylabel('Balance Error')
yyaxis right
plot(history.time,history.act,'*')
ylabel('Activation')

figure(3)
hold on
plot(history.time,history.T,history.time,history.T1)
legend('T','T1')
xlabel('Time [s]')
ylabel('Torque [nm]')
% name = input('test name','s');
% save(char(name),'history')


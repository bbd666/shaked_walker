close all
clearvars
clc
mass = 73; % body mass
%% load data
Data = import_timeseries('..\Gait_timeseries.txt');

% Data = table(dataArray{1:end-1}, 'VariableNames', {'hamr_T','hamr_T1','glur_T','glur_T1','hflr_T','hflr_T1','rfr_T','rfr_T1',...
%     'vasr_T','vasr_T1','solr_T','solr_T1','tar_T','tar_T1','gasr_T','gasr_T1',...
%     'haml_T','haml_T1','glul_T','glul_T1','hfll_T','hfll_T1','rfl_T','rfl_T1',...
%     'vasl_T','vasl_T1','soll_T','soll_T1','tal_T','tal_T1','gasl_T','gasl_T1',...
%     'time','hipr_a','kneer_a','ankler_a','hipl_a','kneel_a','anklel_a','trunk_ang','trunk_vel',...
%         'stance_r','swing_r','sp_r','si_r',...
%     'stance_l','swing_l','sp_l','si_l',...
%     'footr_X','footr_Y','footr_Z','footl_X','footl_Y','footl_Z',...
%     'comX', 'comY', 'comZ', 'comvelX', 'comvelY', 'comvelZ'});


%% plot grezzo
Data = Data(1:50:end,:);

for x = 1:2
    if x == 1
        lat = 'r';
    else
        lat = 'l';
    end
figure(x)
subplot(3,2,1)
hold on
eval([char("plot(Data.Time,Data.Hip") lat char("A/pi*180,'k','LineWidth',2)")])
ylabel('Hip flexion [°]')
yyaxis right
eval([char("plot(Data.Time,Data.Stance") lat char(",'or')")])
eval([char("plot(Data.Time,Data.Swing") lat char(",'ok')")])
eval([char("plot(Data.Time,Data.SP") lat char(",'ob')")])
eval([char("plot(Data.Time,Data.SI") lat char(",'og')")])
ax6 = gca;
ax6.Box = 'On';
ax6.FontSize = 12;
legend('angle','Stance','Swing','SP','SI')
% if x == 1
%     title('Right')
% else
%     title('Left')
% end
ylim([0.2 1])
hold off

subplot(3,2,2)
hold on
eval([char("plot(Data.Time,-Data.HFL") lat char("T1/mass,'g')")])
eval([char("plot(Data.Time,-Data.GLU") lat char("T1/mass,'r')")])
eval([char("plot(Data.Time,-Data.HAM") lat char("T1/mass,'b')")])
eval([char("plot(Data.Time,-Data.RF") lat char("T1/mass,'--k')")])
eval([char("plot(Data.Time,(Data.HFL") lat char("T1+Data.HAM") ...
    lat char("T1+Data.RF") lat char("T1+Data.GLU") lat char("T1)/mass,'-k','LineWidth',2)")])
ylabel('Hip extension [Nm/kg]')
legend('HFL','GLU','HAM','RF','total')
ax1 = gca;
ax1.Box = 'On';
ax1.FontSize = 12;
ylim([-4 4])
hold off

subplot(3,2,3)
hold on
eval([char("plot(Data.Time,Data.Knee") lat char("A/pi*180,'k','LineWidth',2)")])
ylabel('Knee flexion [°]')
ax2 = gca;
ax2.Box = 'On';
ax2.FontSize = 12;
%ylim([0 200])
hold off

subplot(3,2,4)
hold on
eval([char("plot(Data.Time,Data.HAM") lat char("T2/mass,'b')")])
eval([char("plot(Data.Time,Data.RF") lat char("T2/mass,'--k')")])
eval([char("plot(Data.Time,Data.VAS") lat char("T1/mass,'m')")])
eval([char("plot(Data.Time,Data.GAS") lat char("T1/mass,'c')")])
eval([char("plot(Data.Time,(Data.HAM") lat char("T2+Data.RF") ...
    lat char("T2+Data.VAS") lat char("T1+Data.GAS") lat char("T1)/mass,'-k','LineWidth',2)")])
ylabel('Knee extension [Nm/kg]')
legend('HAM','RF','VAS','GAS','total')
ax3 = gca;
ax3.Box = 'On';
ax3.FontSize = 12;
ylim([-4 4])
hold off

subplot(3,2,5)
hold on
eval([char("plot(Data.Time,Data.Ankle") lat char("A/pi*180,'k','LineWidth',2)")])
ylabel('Ankle dorsiflection [°]')
xlabel('Time [s]')
ax4 = gca;
ax4.Box = 'On';
ax4.FontSize = 12;
hold off

subplot(3,2,6)
hold on
eval([char("plot(Data.Time,-Data.TA") lat char("T1/mass,'--m')")])
eval([char("plot(Data.Time,-Data.SOL") lat char("T1/mass,'--b')")])
eval([char("plot(Data.Time,-Data.GAS") lat char("T2/mass,'c')")])
eval([char("plot(Data.Time,-(Data.TA") lat char("T1+Data.SOL") ...
    lat char("T1+Data.GAS") lat char("T2)/mass,'-k','LineWidth',2)")])
ylabel('Ankle plantarflexion [Nm/kg]')
xlabel('Time [s]')
legend('TA','SOL','GAS','total')
ax5 = gca;
ax5.Box = 'On';
ax5.FontSize = 12;
ylim([-2 2])
hold off

linkaxes([ax1 ax2 ax3 ax4 ax5 ax6],'x')
xlim(ax1,[0 max(Data.Time)])
end

% figure
% hold on
% subplot(2,1,1)
% plot(Data.Time,Data.trunk_ang*180/pi,'-k')
% ylabel('Trunk sagittal inclination [°]')
% xlabel('Time [s]')
% subplot(2,1,2)
% plot(Data.Time,Data.trunk_vel,'-k')
% ylabel('Trunk sagittal inclination velocity [°/s]')
% xlim([0 max(Data.Time)])
% hold off

figure
subplot(3,1,1)
plot(Data.Time,Data.FootrPX,'-k',Data.Time,Data.FootlPX,'-r')
title('Foot COM positon')
ylabel('Lateral [m]')
legend('Right','Left')
subplot(3,1,2)
plot(Data.Time,Data.FootrPY,'-k',Data.Time,Data.FootlPY,'-r')
ylabel('Vertical [m]')
subplot(3,1,3)
plot(Data.Time,Data.FootrPZ,'-k',Data.Time,Data.FootlPZ,'-r')
ylabel('Forward [m]')
xlabel('Time [s]')

figure
subplot(3,2,1)
plot(Data.Time,Data.COMPX,'-k')
title('COM position')
ylabel('Lateral [m]')
subplot(3,2,3)
plot(Data.Time,Data.COMPY,'-k')
ylabel('Vertical [m]')
subplot(3,2,5)
plot(Data.Time,-Data.COMPZ,'-k')
ylabel('Forward [m]')
xlabel('Time [s]')

subplot(3,2,2)
plot(Data.Time,Data.COMVX,'-k')
title('COM velocity')
subplot(3,2,4)
plot(Data.Time,Data.COMVY,'-k')
subplot(3,2,6)
plot(Data.Time,-Data.COMVZ,'-k')
xlabel('Time [s]')

%% gait metrics (MANUALLY TUNE HEEL STRIKES AND TOE OFF
HS_ind = [1];
TO_ind = [1];
ind_3s = find(Data.Time>=3);
for ii = ind_3s(1):length(Data.stance_r)-1
    if Data.stance_r(ii)==0 && Data.stance_r(ii+1)==1
            if Data.Time(ii)-Data.Time(HS_ind(end))>0.5
                HS_ind=[HS_ind ii];
            end
    end
    if Data.stance_r(ii)==1 && Data.stance_r(ii+1)==0
        
            if Data.Time(ii)-Data.Time(TO_ind(end))>0.5
                TO_ind=[TO_ind ii];
            end
    end

end
HS_ind=[HS_ind(2:end) ];
TO_ind=[TO_ind(2:end) ];


HSl_ind = [1];
TOl_ind = [1];
ind_3s = find(Data.Time>=3);
for ii = ind_3s(1):length(Data.stance_l)-1
    if Data.stance_l(ii)==0 && Data.stance_l(ii+1)==1
                   if Data.Time(ii)-Data.Time(HSl_ind(end))>0.5
                HSl_ind=[HSl_ind ii];
            end
        
    end
    if Data.stance_l(ii)==1 && Data.stance_l(ii+1)==0
                    if Data.Time(ii)-Data.Time(TOl_ind(end))>0.5
                TOl_ind=[TOl_ind ii];
            end
    end

end
HSl_ind=[HSl_ind(2:end) ];
TOl_ind=[TOl_ind(3:end) ];

figure
subplot(3,1,1)
plot(Data.Time,Data.FootrPX,'-k',Data.Time,Data.FootlPX,'-r')
title('Foot COM positon')
ylabel('Lateral [m]')
legend('Right','Left')
subplot(3,1,2)
hold on
plot(Data.Time,Data.FootrPY,'-k',Data.Time,Data.FootlPY,'-r',Data.Time(HS_ind),Data.FootrPY(HS_ind),'ok',Data.Time(TO_ind),Data.FootrPY(TO_ind),'xk')
plot(Data.Time(HSl_ind),Data.FootlPY(HSl_ind),'or')
plot(Data.Time(TOl_ind),Data.FootlPY(TOl_ind),'xr')
ylabel('Vertical [m]')
hold off
subplot(3,1,3)
plot(Data.Time,Data.FootrPZ,'-k',Data.Time,Data.FootlPZ,'-r')
ylabel('Forward [m]')
xlabel('Time [s]')

cycle_time_r = diff(Data.Time(HS_ind));
stance_r = (Data.Time(TO_ind(1:end-1))-Data.Time(HS_ind(1:end-1)))./cycle_time_r*100;
length_r = diff(abs(Data.FootrPZ(HS_ind)));
frequency_r = 1./cycle_time_r;

cycle_time_l = diff(Data.Time(HSl_ind));
stance_l = (Data.Time(TOl_ind(1:end-1))-Data.Time(HSl_ind(1:end-1)))./cycle_time_l*100;
length_l = diff(abs(Data.FootlPZ(HSl_ind)));
frequency_l = 1./cycle_time_l;

width = Data.FootrPX(HSl_ind(1:end))-Data.FootlPX(HSl_ind(1:end));

disp(['cycle time right ' num2str(mean(cycle_time_r)) ' +- ' num2str(std(cycle_time_r)) ' s']);
disp(['stance right ' num2str(mean(stance_r)) ' +- ' num2str(std(stance_r)) ' %']);
disp(['length right ' num2str(mean(length_r)) ' +- ' num2str(std(length_r)) ' m']);
disp(['frequency right ' num2str(mean(frequency_r)) ' +- ' num2str(std(frequency_r)) ' Hz']);

disp(['cycle time left ' num2str(mean(cycle_time_l)) ' +- ' num2str(std(cycle_time_l)) ' s']);
disp(['stance left ' num2str(mean(stance_l)) ' +- ' num2str(std(stance_l)) ' %']);
disp(['length left ' num2str(mean(length_l)) ' +- ' num2str(std(length_l)) ' m']);
disp(['frequency left ' num2str(mean(frequency_l)) ' +- ' num2str(std(frequency_l)) ' Hz']);

disp(['width ' num2str(mean(width)) ' +- ' num2str(std(width)) ' m']);

%% plot finale

inizio = 235;
fine = 282;

Data = Data(inizio:fine,:);

time = Data.Time;
time = (time-time(1));
stride = time/time(end)*100;

% load data_wang_125.mat %% std from wang

figure(100)
subplot(3,2,1)
hold on
plot(stride,180-Data.hipr_a/pi*180,'k','LineWidth',2)
% plot(data(:,5),data(:,6),'k','LineWidth',2)
% add_std(data,2,4);
ylabel('Hip flexion [°]')
yyaxis right
ylim([0.2 1])
plot(stride,Data.stance_r,'or')
plot(stride,Data.swing_r,'ok')
plot(stride,Data.sp_r,'ob')
plot(stride,Data.si_r,'og')
ax6 = gca;
ax6.Box = 'On';
ax6.FontSize = 12;
legend('Numeric','SD','Stance','Swing','SP','SI')


hold off

subplot(3,2,2)
hold on
plot(stride,-Data.hflr_T/mass,'g')
plot(stride,-Data.glur_T/mass,'r')
plot(stride,-Data.hamr_T/mass,'b')
plot(stride,-Data.rfr_T/mass,'--k')
plot(stride,-(Data.hflr_T+Data.hamr_T+Data.rfr_T+Data.glur_T)/mass,'-k','LineWidth',2)
% plot(data(:,23),data(:,24),'k','LineWidth',2)
% add_std(data,20,22);
ylabel('Hip extension [Nm/kg]')
legend('HFL','GLU','HAM','RF','Numeric')
ax1 = gca;
ax1.Box = 'On';
ax1.FontSize = 12;
ylim([-4 4])
hold off

subplot(3,2,3)
hold on
plot(stride,180-Data.kneer_a/pi*180,'k','LineWidth',2)
% plot(data(:,11),data(:,12),'k','LineWidth',2)
% add_std(data,8,10);
ylabel('Knee flexion [°]')
ax2 = gca;
ax2.Box = 'On';
ax2.FontSize = 12;
%ylim([0 200])
% legend('Numeric','SD')
hold off

subplot(3,2,4)
hold on
plot(stride,Data.hamr_T1/mass,'b')
plot(stride,Data.rfr_T1/mass,'--k')
plot(stride,Data.vasr_T/mass,'m')
plot(stride,Data.gasr_T/mass,'c')
plot(stride,(Data.hamr_T1+Data.rfr_T1+Data.vasr_T+Data.gasr_T)/mass,'-k','LineWidth',2)
% plot(data(:,29),data(:,30),'k','LineWidth',2)
% add_std(data,26,28);
ylabel('Knee extension [Nm/kg]')
legend('HAM','RF','VAS','GAS','Numeric')
ax3 = gca;
ax3.Box = 'On';
ax3.FontSize = 12;
ylim([-4 4])
hold off

subplot(3,2,5)
hold on
plot(stride,90-Data.ankler_a/pi*180,'k','LineWidth',2)
% plot(data(:,17),data(:,18),'k','LineWidth',2)
% add_std(data,14,16);
ylabel('Ankle dorsiflection [°]')
xlabel('Step cycle [%]')
ax4 = gca;
ax4.Box = 'On';
ax4.FontSize = 12;
% legend('Numeric','SD')
hold off

subplot(3,2,6)
hold on
plot(stride,-Data.tar_T/mass,'--m')
plot(stride,-Data.solr_T/mass,'--b')
plot(stride,-Data.gasr_T1/mass,'c')
plot(stride,-(Data.tar_T+Data.solr_T+Data.gasr_T1)/mass,'-k','LineWidth',2)
% plot(data(:,35),data(:,36),'k','LineWidth',2)
% add_std(data,32,34);
ylabel('Ankle plantarflexion [Nm/kg]')
xlabel('Step cycle [%]')
legend('TA','SOL','GAS','Numeric')
ax5 = gca;
ax5.Box = 'On';
ax5.FontSize = 12;
ylim([-2 2])
hold off

% linkaxes([ax1 ax2 ax3 ax4 ax5 ax6],'x')
% xlim(ax1,[0 max(Data.Time)])



function add_std(data, ind_min, ind_max)
freq = data(:,ind_min-1);
freq = freq(~isnan(freq));
min = data(1:length(freq),ind_min);
freq2 = data(:,ind_max-1);
freq2 = freq2(~isnan(freq2));
max = interp1(freq2,data(1:length(freq2),ind_max),freq);

sig = mean([min'; max']);
sig_std = max'-sig;
fv=GraphSTD(freq, sig, sig_std, [0.6 0.6 0.6]);
patch(fv);alpha(0.5);
end

function fv=GraphSTD(x, Sm, Sstd, Color)
x = x(:);Sm=Sm(:);Sstd=Sstd(:);
y1 = Sm + Sstd; 
y2 = Sm - Sstd;
x = x(~isnan(y1) & ~isnan(y2));
y1_ = y1(~isnan(y1) & ~isnan(y2));
y2 = y2(~isnan(y1) & ~isnan(y2)); y1 = y1_; clear y1_;
fv.Vertices = [x(1) y1(1)
    x(:) y2(:)
    x(end:-1:1) y1(end:-1:1)];
fv.Faces = 1:size(fv.Vertices,1);
fv.EdgeColor = Color;
fv.FaceColor = Color;
end
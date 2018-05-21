function showRewardSchedule(max_time,scaling,cutoff)
% Shows the reward schedule at different settings of prrew in AnticipatorySaccade
% Reward is an exponential function of RT
% prrew(max_time,scaling,cutoff)
% reward = max_time * exp (-RT/scaling)
% if timeout > cutoff, then reward = cutoff


RT = 1:800;
reward = max_time * exp (-RT/scaling);

reward(reward>cutoff) = cutoff;

plot(RT,reward,'linewidth',2)
ylabel 'Rewards (ms)'
xlabel 'RT (ms)'
set(gca,'TickDir','out','FontSize',18,'Box','off');
grid on 
grid minor
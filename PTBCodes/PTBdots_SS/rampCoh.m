function rampCoh(index,coherence,coh_std)

global dots_struct screen_struct


time2pk = ((mod(coh_std,10)*50)+100)/1000;
frame2pk = round(time2pk*screen_struct.mon_refresh);

% dots_struct(index).coherence(1:time2pk) = linspace(0.04,0.64,time2pk); % linear
if coh_std<20 % Exponential coherence ramp
    cohRamp = 1e-2*exp(linspace(log(1),log(coherence*100),frame2pk)); 
elseif coh_std<30 % Step rise from 0 to full 
    cohRamp = zeros(1,length(frame2pk));
end

dots_struct(index).coherence(1:frame2pk) = cohRamp;


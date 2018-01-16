function rampCoh(index,coherence,coh_std)

global dots_struct screen_struct


time2pk = ((mod(coh_std,10)*50)+100)/1000;
frame2pk = round(time2pk*screen_struct.mon_refresh);

cohRamp = 1e-2*exp(linspace(log(1),log(coherence*100),frame2pk)); % exponential

% dots_struct(index).coherence(1:time2pk) = linspace(0.04,0.64,time2pk); % linear


dots_struct(index).coherence(1:frame2pk) = cohRamp;


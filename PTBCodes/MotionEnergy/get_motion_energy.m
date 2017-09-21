function [coh, motion_energy] = get_motion_energy(eMat,dotpos,scr_info)
% dots_xyt = dotpos; % this is the output of the dot-generation

E = eventCodes;
MON_REFRESH = scr_info.MON_REFRESH;
PIX_PER_DEG = scr_info.PIX_PER_DEG;

[coh,motion_energy] = calc_ME(eMat(:,E.coherence),eMat(:,E.dot_speed),...
    eMat(:,E.dot_dir),dotpos ,MON_REFRESH,PIX_PER_DEG);
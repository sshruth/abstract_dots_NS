function [dotpos, coh_frame, scr_info] = regen_dots(eMat)

E = eventCodes;
ntr = size(eMat,1); % Number of trials
dotpos = cell(ntr,1); coh_frame = cell(ntr,1);

for i=1:ntr
    disp(['trial: ', num2str(i)])
    
    duration                    = (eMat(i,E.time_dots_off)-eMat(i,E.time_dots_on))/1000 + .2; %add 200 msec
    rseed                       = [eMat(i,E.seed) eMat(i,E.seedvar)];

    dots_struct.aperture        = [0 0 eMat(i,E.dot_diam)/10 eMat(i,E.dot_diam)/10];
    dots_struct.direction       = eMat(i,E.dot_dir);

    dots_struct.speed           = eMat(i,E.dot_speed)/10;
    dots_struct.coherence       = eMat(i,E.coherence)/1000;
    dots_struct.coherence_std   = 0;

    dots_struct.density         = 16.7;

    dots_struct.interval        = 3;
    MAX_DOTS_PER_FRAME          = 200;
    screen_struct.screen_rect   = [0 0 1024 768]; %[0 0 800 600];
    screen_struct.mon_refresh   = 75;
    screen_width_cm             = 32;
    distance_to_screen_cm       = 48;

    if ~isnan(duration)
        [dotpos{i},coh_frame{i}] = dotsRemake(rseed,duration,dots_struct,screen_struct,MAX_DOTS_PER_FRAME,...
            screen_width_cm,distance_to_screen_cm);
    else
        dotpos{i} = [];
        coh_frame{i} = [];
    end
end

scr_info.MON_REFRESH = screen_struct.mon_refresh;

resolution_w = screen_struct.screen_rect(3);
scr_info.PIX_PER_DEG = pi * resolution_w / ...
    atan(screen_width_cm/distance_to_screen_cm/2) / 360;

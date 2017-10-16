addpath('make_huk_stim/')
initScreen

% rect = [0 0 100 100];
clear color
cont = 0;
Screen('FillRect',screen_struct.cur_window,0);
for j=1:2
    for i=1:5
        cont = cont+1;
        d_rect = [(i-1)*140 (j-1)*200 (i-1)*140+120 (j-1)*200+100];
        color(i,j) = gamma_correct(cont - 1,'shin',10);
        Screen('FillRect',screen_struct.cur_window,color(i,j),d_rect);
    end
end
Screen('Flip',screen_struct.cur_window,0,1)
        
        
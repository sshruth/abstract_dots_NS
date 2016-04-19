
function allOff()


global screen_struct target_struct PhotoDetectorFlash

    %clear the screen
Screen('FillRect', screen_struct.cur_window, screen_struct.screen_bkg);    

    % update the status of targets on the screen 
for i = 1 : length(target_struct),
    target_struct(i).show = 0;
end;

PhotoDetectorFlash = 0; % reset for beginning of next trial

Screen('Flip', screen_struct.cur_window);

function targDraw(show_target_index, hide_target_index, flip_screen, blink)

%
% function drawTarg(show_hide)
% 
% shows or hides targets 
% 
%   show_index      show these targets
%   hide_index      hide these targets
%   flip_screen     1 (default), flip the screen, 0 do not flip
%   blink           1 (default), flash the photodetector or not
% 
% the targets that are not specified in show_index or hide_index maintain
% their status on the screen. 
% if a target index is defined both in show_index and hide_index, it will
% be hidden.
% 
% flip_screen is a very useful parameter which permits targets to be drawn
% on the background buffer without being shown. this is needed when the
% targets are to be drawn along with other things, or when very precise
% temporal control is required. after calling the function with
% flip_screen=0, call flipScreen(1) to make the changes show up on the
% screen and send a went message
% 



global screen_struct target_struct PhotoDetectorFlash

    %find the index of all targets to be shown on the screen 
show = cat(1, target_struct.show);
if nargin>=1 && ~isempty(show_target_index),
    show(show_target_index) = 1;
end;
if nargin>=2 && ~isempty(hide_target_index),
    show(hide_target_index) = 0;
end;
show_index = find(show);

    %draw the targets 
for i = 1 : length(show_index),
    index = show_index(i);
    
        %make sure the target has a valid size 
    if any(target_struct(index).diameter<=0),
        break;
    end;
    
        %calculate the rectangle surrounding the target 
    rect = round(deg2screen(rectAround(target_struct(index).center, target_struct(index).diameter), screen_struct, 'v'));
    rect([2 4]) = rect([4 2]);      %this swap is necessary because Y decreases from bottom to top on the screen 
    
        %show the target on the screen 
    switch target_struct(index).shape,
        case 'rect',
            Screen('FillRect', screen_struct.cur_window, target_struct(index).color, rect);
        case 'ellipse',
            Screen('FillOval', screen_struct.cur_window, target_struct(index).color, rect);
    end;
end;

    % update the status of targets on the screen 
for i = 1 : length(target_struct),
    target_struct(i).show = show(i);
end;

if nargin<3 || isempty(flip_screen),
    flip_screen = 1;
end;

if nargin<4 || isempty(blink),
    blink = 1;
end

% % for photo detector
% %rect = [50,screen_struct.screen_rect(4)/2-25,100,screen_struct.screen_rect(4)/2+25];
% rect = [0,0,75,75];
% %blink = 0; % override no blinking, this will generate a black
%            % square on top of anything in the top left corner (not
%            % usually an issue)
% if blink,
%     if PhotoDetectorFlash == 0, % then flip it up
%         %Screen('FillOval', screen_struct.cur_window, [255 255 255], rect);
%         Screen('FillRect', screen_struct.cur_window, [255 255 255], rect);
%         PhotoDetectorFlash = 1;
%     else, % then flip it down
%         %Screen('FillOval', screen_struct.cur_window, [0 0 0], rect);
%         Screen('FillRect', screen_struct.cur_window, [0 0 0], rect);
%         PhotoDetectorFlash = 0;
%     end
% else,
%     if PhotoDetectorFlash == 0, % keep it down
%         %Screen('FillOval', screen_struct.cur_window, [0 0 0], rect);
%         Screen('FillRect', screen_struct.cur_window, [0 0 0], rect);
%     else, % keep it up
%         %Screen('FillOval', screen_struct.cur_window, [255 255 255], rect);
%         Screen('FillRect', screen_struct.cur_window, [255 255 255], rect);
%     end
% end

if flip_screen,
    Screen('Flip', screen_struct.cur_window);
end;









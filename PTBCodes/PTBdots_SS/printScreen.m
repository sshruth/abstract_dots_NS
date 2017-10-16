
function printScreen(screen_struct)
% function printScreen(screen_struct)
% prints the structure screen_struct
% 
% Input:
%   screen_struct
% 

% 
% 9/23/07  Developed by RK 
% 


fprintf('\n\n\n________________________\n\n');
fprintf('Screen specifications:\n\n');
fprintf('current screen: %d\n', screen_struct.cur_screen);
fprintf('monitor horizontal size: %3.1f cm\n', screen_struct.mon_width_cm);
fprintf('viewing distance: %3.1f cm\n', screen_struct.view_dist_cm);
fprintf('screen rect: [');  fprintf(' %d ',screen_struct.screen_rect);  fprintf(']\n');
fprintf('refresh rate: %3.1f Hz\n', screen_struct.mon_refresh);
fprintf('pixel per degree: %3.2f\n', screen_struct.pix_per_deg);
fprintf('\n________________________\n\n\n');




global screen_struct dots_struct target_struct MAX_DOTS_PER_FRAME udpmsg_struct abort_key PhotoDetectorFlash TR_NUM save_struct
%May 2014. modif by adz.

% define the screen structure for the experiment
% the physical dimensions of the monitor is [32.5 24] cm
screen_struct = struct ( 'cur_screen'  , 1, ...        % which screen will be showing the stimuli
                         'screen_bkg'  , [0 0 0], ...  % screen background [r g b]. See below. adz
                         'mon_width_cm', 32, ...     % initScreen will generate an error if the monitor's size is different
                         'view_dist_cm', 48, ... 
                         'mon_refresh' , 75, ...     % initScreen will generate an error if the monitor's frame rate is different 
                         'cur_window'  , [], ...
                         'screen_rect' , [], ...
                         'pix_per_deg' , []);

% define maximum number of dots in each dots patch 
MAX_DOTS_PER_FRAME = 200;


% trial counter for saving dot positions (adz)
TR_NUM = 0;
save_struct = struct();

% defines structure for the dots patch. each patch has its own set
% of parameters. the patches on the screen are therefore defined by an
% array of structures. see dotsDefine. 
dots_struct = struct ( 'aperture'    , [], ...
                       'direction'   , [], ...
                       'coherence'   , 0, ...
                       'speed'       , 5, ...
                       'coherence_std'  , 0, ... %adz
                       'density'     , 16.7, ...
                       'interval'    , 3, ...
                       'dot_size'    , 3, ...
                       'dot_color'   , [255 255 255], ...
                       'dot_pos'     , [], ...
                       'shown_frames', 0 );


% defines structure for the targets (including FP). each target has its
% own set of parameters. the targets on the screen are therefore
% defined by an array of structures. see targDefine.
target_struct = struct ( 'show'    , 0, ...
                         'center'  , [], ...
                         'diameter', [], ...
                         'shape'   , 'ellipse', ...
                         'color'   , [] );

PhotoDetectorFlash = 0; % 0 = down, 1 = up

% define the IP address of PTB and Rex 
if 1,    
  localIP = '192.168.0.4';
  rexIP   = '192.168.0.5';
else    % cross over cable
  localIP = '169.254.153.78';
  rexIP   = '171.65.19.119';
end;
port = 6665;

% define the structure for the last udpmsg
% this structure will be used to avoid multiple execution of the same
% command 
udpmsg_struct = struct ( 'raw_recv', '', ...    % last raw message received, can be valid or invalid 
                         'recv'    , '', ...        % last valid message received [id is stripped off]
                         'recv_id' , '', ...      % id of the last valid message received 
                         'raw_sent', '', ...    % last raw message sent
                         'sent_id' , '', ...      % id of the last valid message sent 
                         'sent'    , '' );          % last valid message sent 


% the key combination that aborts the program 
abort_key = [KbName('leftGUI') KbName('escape')];


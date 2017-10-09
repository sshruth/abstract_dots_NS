% function dotsDefine(patch_index, aperture, direction, coherence, speed, coherence_std, density, interval, dot_size, dot_color)
function dotsDefine_ramp(patch_index, aperture, direction, coherence, speed, coherence_std, density, interval, dot_size, dot_color)

% 2017-09 modified by SS to allow control of coherences during trials 


global screen_struct dots_struct
color_dots = false; % Should the dots be colored

if nargin<1 || isempty(patch_index)
    return;
end

for i = 1 : length(patch_index)
    index = patch_index(i);
    
        %fill in the structure based on the supplied parameters 
    if nargin>=2 && ~isempty(aperture)
        dots_struct(index).aperture = aperture(i,:);
    end
    if nargin>=3 && ~isempty(direction)
        dots_struct(index).direction = direction(i);
    end
    if nargin>=4 && ~isempty(coherence)
        dots_struct(index).coherence = coherence(i);
    end
    if nargin>=5 && ~isempty(speed)
        dots_struct(index).speed = speed(i);
    end
    
    % coh_std is a flag to indicate type of ramp
    if nargin>=6 && ~isempty(coherence_std)
        dots_struct(index).coherence_std = coherence_std(i);
        
    end
    
    if nargin>=7 && ~isempty(density)
        dots_struct(index).density = density(i);
    end
    if nargin>=8 && ~isempty(interval)
        dots_struct(index).interval = interval(i);
    end
    if nargin>=9 && ~isempty(dot_size)
        dots_struct(index).dot_size = dot_size(i);
    end
    if nargin>=10 && ~isempty(dot_color)
        dots_struct(index).dot_color = dot_color(i,:);
    end
    dots_struct(index).dot_pos = [];
    dots_struct(index).shown_frames = 0;
    
        %make sure that the dot patches are properly defined
    if length(dots_struct(index).aperture)~=4 || any(~isfinite(dots_struct(index).aperture)),
        dots_struct(index).aperture = [0 0 0 0];
    end;
    if length(dots_struct(index).direction)~=1 || ~isfinite(dots_struct(index).direction),
        dots_struct(index).direction = 0;
    end;
    if length(dots_struct(index).coherence)~=1 || ~isfinite(dots_struct(index).coherence),
        dots_struct(index).coherence = 0;
    end;
    if length(dots_struct(index).speed)~=1 || ~isfinite(dots_struct(index).speed),
        dots_struct(index).speed = 0;
    end;
    if length(dots_struct(index).density)~=1 || ~isfinite(dots_struct(index).density),
        dots_struct(index).density = 0;
    end;
    if length(dots_struct(index).interval)~=1 || ~isfinite(dots_struct(index).interval),
        dots_struct(index).interval = 3;
    end;
    if length(dots_struct(index).dot_size)~=1 || ~isfinite(dots_struct(index).dot_size),
        dots_struct(index).dot_size = 1;
    end;
    if length(dots_struct(index).dot_color)~=3 || any(~isfinite(dots_struct(index).dot_color)),
        dots_struct(index).dot_color = screen_struct.screen_bkg;
    end;
    
    % Change dot color based on dot direction
    if color_dots
        if dots_struct(index).direction==0
            dots_struct(index).dot_color = [212 255 0];
        elseif dots_struct(index).direction==180
            dots_struct(index).dot_color = [0 255 255];
        end
    end
    
    % Coherence specified as vector to allow control during trial
    % By default, fill in the vector for 10s 
    dots_struct(index).coherence = ones(1,10*screen_struct.mon_refresh)*coherence;
%     % Ramp to 300 ms
%     if coherence==0.64
%         time2pk = round(0.25*screen_struct.mon_refresh);
%         dots_struct(index).coherence(1:time2pk) = linspace(0.04,0.64,time2pk);
%     end
end
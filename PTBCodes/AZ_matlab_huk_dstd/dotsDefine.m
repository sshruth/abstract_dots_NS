function dotsDefine(patch_index, aperture, direction, coherence, speed, coherence_std, density, interval, dot_size, dot_color)
% modified by adz for coherence_std
% August 2014. modified by adz. Load the huk textures and adapt them to the
% aperture size. Then add them to the huk struct.


global screen_struct dots_struct huk_struct

if nargin<1 || isempty(patch_index),
    return;
end;

for i = 1 : length(patch_index),
    index = patch_index(i);
    
        %fill in the structure based on the supplied parameters 
    if nargin>=2 && ~isempty(aperture),
        dots_struct(index).aperture = aperture(i,:);
    end;
    if nargin>=3 && ~isempty(direction),
        dots_struct(index).direction = direction(i);
    end;
    if nargin>=4 && ~isempty(coherence),
        dots_struct(index).coherence = coherence(i);
    end;
    if nargin>=5 && ~isempty(speed),
        dots_struct(index).speed = speed(i);
    end;
    
    % added by adz
    if nargin>=6 && ~isempty(coherence_std),
        dots_struct(index).coherence_std = coherence_std(i);
    end;
    
    if nargin>=7 && ~isempty(density),
        dots_struct(index).density = density(i);
    end;
    if nargin>=8 && ~isempty(interval),
        dots_struct(index).interval = interval(i);
    end;
    if nargin>=9 && ~isempty(dot_size),
        dots_struct(index).dot_size = dot_size(i);
    end;
    if nargin>=10 && ~isempty(dot_color),
        dots_struct(index).dot_color = dot_color(i,:);
    end;
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
end;

%adz - huk textures
if ~isempty(aperture) && isempty(huk_struct.stimuli)
    ap_radius_px = floor(mean(aperture(1,3:4)) * screen_struct.pix_per_deg);
    
    xx = -ap_radius_px:ap_radius_px;
    [X,Y] = meshgrid(xx,xx);
    R = sqrt(X.^2+Y.^2);
    nt = huk_struct.max_dur*screen_struct.mon_refresh;
    R3D = repmat(R,[1,1,nt]);
    idxBkg = R3D>ap_radius_px;
    nx = length(xx);
    
    huk_struct.stimuli = cell(huk_struct.num_stims,1);
    for i = 1:huk_struct.num_stims
        archiv    = ['huknoise_fid',num2str(huk_struct.file_identifier),'_stimid',num2str(i)];
        s = load(fullfile('HukStim',archiv)); %this takes too long !!
        Igamma = s.Igamma(1:nx,1:nx,:);
        huk_struct.stimuli{i} = Igamma;
        %sanity check
        if ~all(size(Igamma))==size(R3D)
            error('sizes dont match')
        end
        
        media(i) = nanmean(Igamma(:));
    end
    
    %re-set background
    screen_struct.screen_bkg = round(mean(media));
    for i = 1:huk_struct.num_stims
        %Igamma = huk_struct.stimuli{i};
        huk_struct.stimuli{i}(idxBkg) = screen_struct.screen_bkg;

    end
    
    
end


function [coh,motion_energy] = calc_ME(dot_coh,dot_speed,dot_dir,dots_xyt,MON_REFRESH,PIX_PER_DEG)

% load(datafile);

% aperture1 = value.ap_x;
% aperture2 = value.ap_y;
% aperture3 = value.ap_diameter;
% rseed = value.see_bas.*value.see_var; % yes ????
%
coh = dot_coh/1000;
speed = dot_speed;
direction = dot_dir;

% duration = (time.dot_off-time.dots_on)/1000;

%%

nTrials = length(coh);
clear data

motion_energy  = cell(nTrials,1);
% parfor n = 1:nTrials
for n = 1:nTrials
    disp(['Num trials: ',num2str(n)]);
    if isempty(dots_xyt{n})
        continue
    end
    
    temp = cat(2,dots_xyt{n}{:}); temp = temp(:);
    msize = round(2*max(abs(temp)))+1;
    %     disp(msize)
    stim = motionenergy.dots2hist(dots_xyt{n},'msize',msize);
    stim = permute(stim,[3,2,1]);
    
    [I,J] = find(sum(stim,3)>0);
    stim = stim(min(I):max(I),min(J):max(J),:);
    
    alpha = direction(n);
    stim_rotate = rotate_matrix(stim,alpha);
    
    %unpermute
    st = permute(stim_rotate,[3,2,1]);
    
    % hack for speed - optional
    sampleduration = 1/MON_REFRESH;
    sp = speed(n)/10;
    degperbin = 1/PIX_PER_DEG;
    degperbin_y = degperbin; % new
    do_hack = true;
    if do_hack
        degperbin = degperbin * 5/sp;% filter HACK
        sp = 5;
    end
    
    %motion energy
%     [energy_right,energy_left] = motionenergy.motionfilter_shadlen(st,sampleduration,...
%         'degperbin',degperbin,...
%         'speed_deg_per_sec',sp,'sigmac',0.35,...
%         'doplot',false,'speed_deg_per_sec',sp,'t_const',60);
    
    motion_energy{n} = motionenergy.motionfilter_shadlen(st,sampleduration,...
        'degperbin',degperbin,...
        'degperbin_y',degperbin_y,... % new
        'speed_deg_per_sec',sp,'sigmac',0.5,... % was 0.35
        'doplot',false,'speed_deg_per_sec',sp,'t_const',60);
    
    % me_coh_dir{n}  = media_dim(energy_right,[2,3]);
    % me_ncoh_dir{n} = media_dim(energy_left,[2,3]);
    
    %     figure(1)
    %     plot(me_coh_dir{n})
    %     hold all
    %     plot(me_ncoh_dir{n},'r')
    %     hold on
    %     xx=ceil(duration(n)*75);
    %     plot([xx xx],ylim,'k')
    %     title([num2str(id.dot_dir(n)),' - ',num2str(id.dot_coh(n))])
    %     pause
    %     clf
    
    
    
    
end
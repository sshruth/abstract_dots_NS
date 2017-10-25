function [coh, motion_energy] = getME(eMat,saveStruct,scr_info)
% Newer version derived for get_motion_energy

E = eventCodes;
MON_REFRESH = scr_info.mon_refresh;
PIX_PER_DEG = scr_info.pix_per_deg;

dotpos = cell(length(saveStruct),1);
for f = 1:length(saveStruct)
    cdotpos = saveStruct(f).dots_struct.dot_pos;
    if ~isempty(cdotpos)
        dotpos{f} = cell(size(cdotpos,3),1);
        for g = 1:size(cdotpos,3)
            idx = squeeze(~isnan(cdotpos(:,1,g)));
            dotpos{f}{g}(1,:) = cdotpos(idx,1,g);
            dotpos{f}{g}(2,:) = cdotpos(idx,2,g);
        end
    end
end

[coh,motion_energy] = calc_ME(eMat(:,E.coherence),eMat(:,E.dot_speed),...
    eMat(:,E.dot_dir),dotpos,MON_REFRESH,PIX_PER_DEG);


function runMakeHukNoise()
% makes a bunch of Huk noise stimuli. Saves them to the 
% 'HukStim' folder.

run('../defineGlobals');

nStims = huk_struct.num_stims;
for ist=1:nStims
    disp(['making stim num. ',num2str(ist)])
    
    frate       = screen_struct.mon_refresh;
    doMovie     = false;
    convMethod  = 1;
    
    dims    = [huk_struct.max_px, huk_struct.max_px, huk_struct.max_dur*screen_struct.mon_refresh];
    W       = TruncatedGaussian(huk_struct.sigma_gauss,huk_struct.cutoff,dims);
    W       = removeMeanFromColumns(W);
    [D,S]   = huk_filter(W,frate,doMovie,convMethod);

    I       = (D+S)/2;
    range_desired   = huk_struct.range100;
    
    range_im(1) = -1*max(abs(I(:)));
    range_im(2) = max(abs(I(:)));
    
%     range_im(1) = min([min(I(:)),min(D(:)),min(S(:))]);
%     range_im(2) = max([max(I(:)),max(D(:)),max(S(:))]);
%     Dnorm   = norm_image(D,1,range_desired,range_im);
%     Snorm   = norm_image(S,1,range_desired,range_im);
    Inorm   = norm_image(I,1,range_desired,range_im);
    %Inorm(:,:,25:40) = Dnorm(:,:,25:40);
    
    
    %to have right-left motion
%     Dnorm = permute(Dnorm,[2,1,3]);
%     Snorm = permute(Snorm,[2,1,3]);
    Inorm = permute(Inorm,[2,1,3]);
    
    %gamma correct (lookup table)
%     Dgamma = gamma_correct(Dnorm,[],100);
%     Sgamma = gamma_correct(Snorm,[],100);
    Igamma = gamma_correct(Inorm,[],100);
    
    
    archiv = ['huknoise_fid',num2str(huk_struct.file_identifier),'_stimid',num2str(ist)];
    filename = fullfile('..','HukStim',archiv);
    save(filename,'Igamma');
    
end


end
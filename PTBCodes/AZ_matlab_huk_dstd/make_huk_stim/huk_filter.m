function [D,S] = huk_filter(W,frate,doMovie,method)

%% to-do: 
%% 1. takes time for filter to start. Start a bit later
%% 2. gamma correction

%%

% std. deviation of the truncated Gaussian used to make texture
% sigma = 1;

% cutoff for truncated gaussian
% cutoff = 2;

% number of timesteps
dt      = 1/frate;
max_time_filt = 0.5;
tt      = 0:dt:max_time_filt;
tsteps_filt    = length(tt);

% dimensions of each frame
nx      = size(W,1);
ny      = size(W,2);
nt      = size(W,3);


%%

% parameters of the Adelson & Bergen impulse response filters
k = 40;
n = 3;
m = 5;

% Adelson & Bergen impulse response filters
alpha = @(tt) (k*tt).^n .* exp(-k*tt) .* ( (1/factorial(n)) - (k*tt).^2 / factorial(n+2));
beta  = @(tt) (k*tt).^m .* exp(-k*tt) .* ( (1/factorial(m)) - (k*tt).^2 / factorial(m+2));


% arrays to store noisy array and its fourier transform
Wh = zeros(size(W));

% assign each frame to be an array of random numbers
% drawn independently from a truncated gaussian
% with parameters given above
% Wh contains the columnwise hilbert transform of each frame
for ii=1:nt
    Wh(:,:,ii) = imag(hilbert(W(:,:,ii)));
end

% construct arrays containing impulse responses as a function
% of time at each pixel (amat and bmat), plus their fourier transforms (af and bf)
at   = alpha(tt);
% at = at/sum(at);
att  = reshape(at,[1 1 tsteps_filt]);
bt   = beta(tt);
% bt = bt/sum(bt);
btt  = reshape(bt,[1 1 tsteps_filt]);

convMethod = method;
switch convMethod
    case 1
        % temporal fourier transforms for computing convolution with temporal
        % impulse responses
        Wf  = fft(W,[],3);
        Wfh = fft(Wh,[],3);

        amat = att(ones(nx,1),ones(ny,1),1:tsteps_filt);
        amat = cat(3,amat,zeros(nx,ny,nt-tsteps_filt));
        af   = fft(amat,[],3);

        bmat = btt(ones(nx,1),ones(ny,1),1:tsteps_filt);
        bmat = cat(3,bmat,zeros(nx,ny,nt-tsteps_filt));
        bf   = fft(bmat,[],3);

        Wconva  = ifft(Wf.*af,[],3);
        Whconvb = ifft(Wfh.*bf,[],3);
        Wconvb  = ifft(Wf.*bf,[],3);
        Whconva = ifft(Wfh.*af,[],3);
    
    case 2
        % convolve in time with temporal impulse responses
        Wconva   = convn(W,att);
        Whconvb  = convn(Wh,btt);
        Wconvb   = convn(W,btt);
        Whconva  = convn(Wh,att);
        
    case 3 %matrix conv, faster
        sw  = size(W);
        Wr  = reshape(W,sw(1)*sw(2),sw(3));
        Whr = reshape(Wh,sw(1)*sw(2),sw(3));
        
        Mat = matrix_conv(sw(3),at);
        Mbt = matrix_conv(sw(3),bt);
        
        Wconva  = Wr*Mat;
        Wconva  = reshape(Wconva,[sw(1),sw(2),size(Mat,2)]);
        
        Whconvb = Whr*Mbt;
        Whconvb = reshape(Whconvb,[sw(1),sw(2),size(Mat,2)]);
        
        Wconvb  = Wr*Mbt;
        Wconvb  = reshape(Wconvb,[sw(1),sw(2),size(Mat,2)]);
        
        Whconva = Whr*Mat;
        Whconva  = reshape(Whconva,[sw(1),sw(2),size(Mat,2)]);
end


% construct D and S, textured stimuli with motion in opposite directions,
% as in Huk & Shadlen, 2005, J. Neuroscience, methods section.
D = Wconva + Whconvb;
S = Wconvb + Whconva;



%%
limi = [min(D(:)),max(D(:))];
% doMovie = false;
if doMovie
    % indices of the pulse times (must be between 1 and tsteps)
%     pulse_times = [100 200];
    
    % weight of direction of motion during pulse
%     w = 1;
    
    writerObj = VideoWriter(fullfile(pwd,'movie.avi'));
    writerObj.FrameRate = frate;
    writerObj.Quality   = 100;
    
    open(writerObj);
    
    fhd = figure;
    for ii=1:nt
%         imagesc(D(:,:,ii));
        imagesc(S(:,:,ii),limi);
        colormap(gray)
%         if ii < pulse_times(1) || ii > pulse_times(2)
%             imagesc(0.5 * ( (D(:,:,ii)) + (S(:,:,ii)) ) ,limi);
%             colormap(gray);
%         else
%             imagesc( w * (D(:,:,ii)) + (1 - w) * (S(:,:,ii)) ,limi);
%             colormap(gray);
%         end
        set(gca,'xtick',[],'ytick',[]);
        axis image;
        drawnow;
        frame=getframe(fhd);
        writeVideo(writerObj,frame);
    end
    close(writerObj);
end




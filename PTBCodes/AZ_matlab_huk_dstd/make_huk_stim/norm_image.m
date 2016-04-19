function W = norm_image(W,flag,range_desired,im_range)

if nargin<2 || isempty(flag)
    flag = 2;
end

if nargin<3 || isempty(range_desired)
    range_desired = [0 1];
end

if nargin<4 || isempty(im_range)
    im_range(1) = nanmin(W(:));
    im_range(2) = nanmax(W(:));
else
    if flag==2
%         error('flag==2 and im_range provided');
    end
end


%normalize to 0,1
switch flag
    case 1 %normalize all movie together
        W = (W - im_range(1)) / (im_range(2) - im_range(1));
    case 2 %normalize each frame independently; ignores im_range
        for i=1:size(W,3)
            w = W(:,:,i);
            w = w - nanmin(w(:));
            w = w / nanmax(w(:));
            W(:,:,i) = w;
        end
end

W = W * diff(range_desired) + range_desired(1);
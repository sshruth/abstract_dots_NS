function R = rotate_matrix(S,alpha)

alpha = alpha*2*pi/360; % convert

M = [cos(alpha),-sin(alpha);sin(alpha),cos(alpha)];
nframes = size(S,3);
K = cell(nframes,1);
for i=1:nframes
    [I,J] = find(S(:,:,i));
    K{i} = round( M*[I,J]' )';
%     K{i} = ( [I,J]*M );
end

cK = cat(1,K{:});

mink = min(cK(:));
maxk = max(cK(:));

% dim = abs(mink)+abs(maxk);
xx = mink:maxk;
nxx = length(xx);
R = zeros(nxx,nxx,nframes);

for i=1:nframes
    L = sub2ind([nxx,nxx],K{i}(:,1) - mink+1, K{i}(:,2) - mink+1);

    mat = zeros(nxx,nxx);
    mat(L) = 1;
    R(:,:,i) = mat;
    %             stim_rotate(:,:,i) = imrotate(stim(:,:,i),direction(n) + vangle(k),'crop');
    %             stim_rotate(:,:,i) = imrotate(stim(:,:,i),direction(n) + vangle(k),'bilinear');
%     stim_rotate(:,:,i) = imrotate(stim(:,:,i),direction(n) + vangle(k));

end

[I,J] = find(sum(R,3));
R = R(min(I):max(I),min(J):max(J),:);
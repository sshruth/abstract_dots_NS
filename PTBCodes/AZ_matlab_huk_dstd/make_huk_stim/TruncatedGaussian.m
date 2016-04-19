function trunc_gauss = TruncatedGaussian(sigma,cutoff,dims)

% xdim = dims(1);
% ydim = dims(2);

trunc_gauss = sigma*randn(dims);
inds = abs(trunc_gauss)>cutoff;
while sum(inds(:))>0
    trunc_gauss(inds) = sigma*randn(sum(inds(:)),1);
    inds = abs(trunc_gauss)>cutoff;
end

end
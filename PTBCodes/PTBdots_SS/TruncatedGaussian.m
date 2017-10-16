function trunc_gauss = TruncatedGaussian(sigma,cutoff,dims)
% function trunc_gauss = TruncatedGaussian(sigma,cutoff,dims)

trunc_gauss = sigma*randn(dims);
inds = abs(trunc_gauss)>cutoff;
while sum(inds(:))>0
    trunc_gauss(inds) = sigma*randn(sum(inds(:)),1);
    inds = abs(trunc_gauss)>cutoff;
end

end
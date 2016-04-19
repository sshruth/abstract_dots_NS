function W = removeMeanFromColumns(W)

for i=1:size(W,2)
    for j=1:size(W,3)
        W(:,i,j) = W(:,i,j) - nanmean(W(:,i,j));
    end
end
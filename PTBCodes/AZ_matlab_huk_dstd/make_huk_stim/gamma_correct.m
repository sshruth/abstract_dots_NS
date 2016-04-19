function Igamma = gamma_correct(I,monitor,max_I)

if nargin<2 || isempty(monitor)
    monitor = 'shin';
end
if nargin<3 || isempty(max_I)
    max_I = 255;
end

persistent CorrectIntensityLookup255

if isempty(CorrectIntensityLookup255) && isequal(monitor,'shin')
    load('GammaCorrectedShin')
end

Isc = I*255/max_I;
Isc = round(10*Isc(:))+1;
Igamma = CorrectIntensityLookup255(Isc);
Igamma = reshape(Igamma,size(I));
    
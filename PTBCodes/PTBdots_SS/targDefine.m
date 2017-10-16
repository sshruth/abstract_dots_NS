function targDefine(trg_index, center, diameter, shape, color)

%
% function defineTarg(trg_index, center, diameter, shape, color)
% 
%   trg_index   which target. can be and array of target indices
%               if an array, other arguments of the function must have
%               appropriate size 
%   center      in degrees, N*2 matrix where N is the length of trg_index 
%   diameter    in degrees, N*2 matrix 
%   shape       can be 'rect' or 'ellipse' 
%   color       must be an N*3 matrix  
% 
   


global screen_struct target_struct 

if nargin<1 || isempty(trg_index),
    return;
end;

for i = 1 : length(trg_index),
    index = trg_index(i);
    
        %fill in the structure based on the supplied parameters 
    if ~isempty(center),
        target_struct(index).center = center(i,:);
    end;
    if ~isempty(diameter),
        target_struct(index).diameter = diameter(i,:);
    end;
    if ~isempty(shape),
        target_struct(index).shape = shape{index};
    end;
    if ~isempty(color),
        target_struct(index).color = color(i,:);
    end;
     
        %do not modify the show status unless the target is a new addition
        %to target_struct  
    if isempty(target_struct(index).show),
        target_struct(index).show = 0;
    end;
        
        %make sure that the targets are properly defined
    if length(target_struct(index).center)~=2 || any(~isfinite(target_struct(index).center)),
        target_struct(index).center = [0 0];
    end;
    if length(target_struct(index).diameter)~=2 || any(~isfinite(target_struct(index).diameter)),
        target_struct(index).diameter = [0 0];
    end;
    if isempty(target_struct(index).shape),
        target_struct(index).shape = 'ellipse';
    end;
    if length(target_struct(index).color)~=3 || any(~isfinite(target_struct(index).color)),
        target_struct(index).color = screen_struct.screen_bkg;
    end;
    
end;



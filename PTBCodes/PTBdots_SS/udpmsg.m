function varargout = udpmsg(varargin)

% function varargout = udpmsg(varargin)
% 
% The function can be called in the following ways
% 
%   socketIsOpen = udpmsg('open', localIP, rexIP, port)
% 
%       opens the udp port for communication
%       localIP is the address of the PTB machine
%       rexIP is the address of the Rex machine
%       port is the port number. If port is not specified it is
%       automatically set to 6665.
% 
%   message_available = udpmsg('check')
% 
%       To check the avaiability of a message. message_available is set to
%       1 if a message is ready to be picked up. 
%       
%   udpmsg('send', message, id)
% 
%       message is a string of characters. handshaked is set to 1 if the
%       receiver acknowledges the message by sending back the message ID.
% 
%   [message, id] = udpmsg('receive');
%   
%       message is a string of character. it is empty if the received
%       message has the same ID as the most recently received message. this
%       is needed to avoid multiple execution of the same request. 
%   
%       
% The communication between Rex and Matlab uses a special hand-shaking
% method. The first nine characters of any message is a nonzero, nine digit
% ID number. The ID is followed by a space and the actual message. 
% 

global udpmsg_struct;


if nargin < 1,
    varargout{1} = NaN;
    return;
end;

switch lower(varargin{1}),
    % socketIsOpen = udpmsg('open', localIP, rexIP, port)
    case 'open',
        if nargin<3,
            varargout{1} = 0;
            return;
        end;
        localIP = varargin{2};
        rexIP = varargin{3};
        if nargout<4 || isempty(varargin{4}),
            port = 6665;
        else
            port = varargin{4};
        end;
        varargout{1} = matlabUDP('open', localIP, rexIP, port);
    
    
    % message_available = udpmesg('check')
    case 'check',
        varargout{1} = matlabUDP('check');
    
        
    % handshaked = udpmsg('send', message)
    case 'send',
                %abort if there is no message to send 
        if length(varargin)<2 || ~ischar(varargin{2}),
            return;
        end;
                %generate the random ID unless it is supplied in the 3rd
                %argument 
        if length(varargin)<3 || isempty(varargin{3}),
            id = num2str(round((rand*0.9+0.1)*1e9));
        else
            id = varargin{3};
        end;
        message = [id ' ' varargin{2}];
                %send it 
        matlabUDP('send', message);
        udpmsg_struct.raw_sent = message;
        udpmsg_struct.sent_id = id;
        udpmsg_struct.sent = varargin{2};
        
        
    % message = udpmsg('receive')
    case 'receive',
        udpmsg_struct.raw_recv=[];
        message = matlabUDP('receive');
        id = '';
            % if there is a message and the message is different from the
            % last received 
        if ~isempty(message) && strcmp(message,udpmsg_struct.raw_recv)==0,
            udpmsg_struct.raw_recv = message;
                %check if this a new valid message. a valid message must
                %have a 9 digit id followed by a space and the message
            if length(message)>10 && message(10)==' ',
                id = message(1:9);
                message = message(11:end);
                udpmsg_struct.recv_id = id;
                udpmsg_struct.recv = message;
                    %confirm the receipt of the message 
% disable the recipt message to postpone Rex's cpu overload!                     
%                matlabUDP('send', [id ' RECEIVED']);
            else
                message = [];
            end;
        end;
        varargout{1} = message;
        varargout{2} = id;
        
        
    otherwise,
        varargout{1} = NaN;
end;



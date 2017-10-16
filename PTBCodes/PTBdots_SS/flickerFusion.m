% make sure the globals are declared
clear

declareGlobals;
setGlobalDefaults;
defineGlobals;
KbName('UnifyKeyNames');

freq = 10;
interval = 1/freq;
duration = 3;
n = duration/interval;
gain = 1;
end_flag = 0;
CBLUE 	= [0 100 255];
CYELLOW = [128 128 0];
keyIsDown = 0;
rr = [60 70 75 85];
rr_ind = 3;
refreshRate = rr(rr_ind);

initScreen([],[],[],[]);
printScreen(screen_struct);
Priority(MaxPriority(screen_struct.cur_screen));
Screen('Flip',CUR_WINDOW);
tic
while ~end_flag
    evalstr = sprintf('!refresh %d',refreshRate);
    eval(evalstr);
    while ~end_flag
        t1=toc;
        tic;
        %operation2 = t1-t2;
        if i==0
            Screen('FillRect',CUR_WINDOW,CYELLOW*gain);
            i = 1;
        else
            Screen('FillRect',CUR_WINDOW,CBLUE);
            i = 0;
        end
        Screen('Flip',CUR_WINDOW);
        pause(interval);
        %t2=toc;
        %operation1=t2-t1;
        [keyIsDown, secs, keyCode] = KbCheck;
        if keyIsDown   
            switch find(keyCode)
                case 79 % right
                    gain = gain+0.1;
                case 80 % left
                    gain = gain-0.1;
                case 81 % down
                    freq = freq-1;
                case 82 % up
                    freq = freq+1;
                case 41
                    end_flag = 1;
                case 55
                    if rr_ind<4
                        rr_ind = rr_ind+1;
                        refreshRate = rr(rr_ind);
                        break
                    end
                case 54
                    if rr_ind>1
                        rr_ind = rr_ind-1;
                        refreshRate = rr(rr_ind);
                        break
                    end
            end
            interval = 1/freq;
            %disp(['operation1 = ',num2str(operation1),' operation2 = ',num2str(operation2)]);
            disp(['gain = ',num2str(gain),' freq = ',num2str(1/(t1)),'Hz',...
                ' set freq = ',num2str(freq),'Hz',' refresh rate = ',num2str(rr(rr_ind)),'Hz']);
            pause(0.1)
        end
        disp(['gain = ',num2str(gain),' freq = ',num2str(1/(t1)),'Hz',' set freq = ',num2str(freq),'Hz']);
    end
end
Screen('CloseAll');
fprintf('\n\n **************************************** \n\n');

% Author: Dan Lofaro
% --------------------------------
% --------------------------------
% --------[ this is the final verions  of the filter for hubo's arm
% movement.  The k1 and L can be modified in order to change the filter
% size and properties]-------------


close all
clear all

t = 200;        % time for simulation

hold on

% -------------------------------------
% -------- pSharedMemory Start --------
% -------------------------------------
L = 1;        % length of buffer
yn = 0;
yn1 = 0;
xn = 0;
k1 = 0.94;
k0 = 1-k1;
zn = 0;
% -------------------------------------
% --------- pSharedMemory End ---------
% -------------------------------------


buffer = zeros(L,1);   % the buffer
znPlot = zeros(t,1);    % to record the 

xn = 20;        % target pos


for i = 1:t
    
    yn = k1*yn1+xn;
    zn = k0*yn;
    yn1 = yn;

    
    for ii = length(buffer):-1:2
        buffer(ii) = buffer(ii-1);
    end
    
    buffer(1) = yn*k0;
    
    
%     buffer(1) = 20;
    
%     bufferTemp = buffer;
%     for ii = 1:length(buffer)
%         bufferTemp(ii) = bufferTemp(ii)*ii/length(bufferTemp)*2;
%     end
    
    
%     zn = mean(buffer);


    
    znPlot(i) = mean(buffer);
end

stairs(znPlot)









legend('With Mean');

shg
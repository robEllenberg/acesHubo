%% Linear velocity and acceleration limiting filter for Hubo
% Author: Rob Ellenberg
% This filter uses cascaded moving average filters to limit the normalized
% velocity, acceleration, and jerk of a signal.  Normalized velocity is
% defined as:
% (position/input) / (t/Ts) = %input / sample
% Normalized acceleration and jerk are similar.
% This should be useful as a non-realtime smoothing algorithm for simple
% gestures. If the maximum possible step input is known (i.e. from one
% position limit to the other), then it is possible to cap velocity as
% well.

close all
clear all

L=15
fprintf('Filter takes %d samples to reach steady state\n',3*L)
n = 20*L+5;

xBuffer=zeros(L,1);
vBuffer=zeros(L,1);
aBuffer=zeros(L,1);
y=zeros(n,1);
jn1=0;
K=0;
for k = 0:n
    x(k+1)=10*sin(k/40);        % target pos, degrees (though the filter is unitless)
    xn = x(k+1);
    %The circular buffer here is ugly due to the 1-indexing of matlab
    %new = previous + (new-oldest)/Length
    if length(xBuffer)==10 || k <5*L
        vn=vBuffer(1) + (xn-xBuffer(L))/L;
        an=aBuffer(1) + (vn-vBuffer(L))/L;
        jn=jn1 + (an - aBuffer(L))/L;
        
        xBuffer=[xn;xBuffer(1:L-1)];
        vBuffer=[vn;vBuffer(1:L-1)];
        aBuffer=[an;aBuffer(1:L-1)];
        
    else
        vn=vBuffer(1)*L/(L-1) + (xn-xBuffer(L)-xBuffer(L-1))/(L-1);
        an=aBuffer(1)*L/(L-1) + (vn-vBuffer(L)-vBuffer(L-1))/(L-1);
        jn=jn1*L/(L-1) + (an - aBuffer(L)-aBuffer(L-1))/(L-1);
        
        xBuffer=[xn;xBuffer(1:L-2)];
        vBuffer=[vn;vBuffer(1:L-2)];
        aBuffer=[an;aBuffer(1:L-2)];
        L=L-1;
        K=k;
        
    end
    disp([xBuffer,vBuffer,aBuffer])
    disp([0,mean(xBuffer),mean(vBuffer),mean(aBuffer),jn])
        %pause
    %Store old value
    jn1=jn;
    %Output value for each iteration is jn
    y(k+1)=jn;
end

%% Plot the data
% The index goofiness below is due the diff cutting the vectors down by 1
% each time
Ts=.01;


y=[zeros(5,1);y];
t=[(-5*Ts:Ts:-Ts)';Ts*(0:n)'];
disp('Switch time = ')
t(K+5)
dydt=diff(y)/Ts;
d2ydt2=diff(y,2)/Ts^2;

figure(2)
subplot(3,1,1)
hold on
stairs(t(1:end-5),y(1:end-5));
stairs(t(1:end-5),x([1,1,1,1,1,1:end-5]));
hold off
title(sprintf('Position vs. time with T_s=%0.2f',Ts))
ylabel('Degrees')
subplot(3,1,2)
stairs(t(1:end-5),dydt(1:end-4));
title(sprintf('Velocity vs. time with T_s=%0.2f',Ts))
ylabel('Degrees / sec')

subplot(3,1,3)
stairs(t(1:end-5),d2ydt2(1:end-3));
title(sprintf('Acceleration vs. time with T_s=%0.2f',Ts))
ylabel('Degrees/sec^2')
xlabel('time, sec')





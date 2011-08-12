%% Linear velocity and acceleration limiting filter for Hubo
% Author: Rob Ellenberg
% This filter uses cascaded moving average filters to limit the normalized
% velocity, acceleration, and jerk of a signal.  Normalized velocity is
% defined as:
% (position/input) / (t/Ts) = %input / sample
% Normalized acceleration and jerk are similar.
% This should be useful as a non-realtime smoothing algorithm for simple
% gestures. If the maximum possible step input is known (i.e. from one
% position limit to the other), then it is possible 

close all
clear all


Lx = 10; 
Lv = 5; 
La = 2;

L=(Lx+Lv+La); 
fprintf('Filter takes %d samples to reach steady state\n',L)
n = L+5;

xBuffer=zeros(Lx,1);
vBuffer=zeros(Lv,1);
aBuffer=zeros(La,1);
y=zeros(n,1);
jn1=0;


for k = 0:n
    xn = 1;        % target pos
    
    %The circular buffer here is ugly due to the 1-indexing of matlab
    % With a mod function in orocos script, this would be relatively easy
    % to implement
    
    %The previous value is 1 step behind k in the circular buffer, while
    %the oldest value resides at index k.  Use mod to convert circular
    %index to linear index
    vn=xn/Lx+vBuffer(mod(k,Lv)+1)-xBuffer(mod(k+1,Lx)+1)/Lx;
    an=vn/Lv+aBuffer(mod(k,La)+1)-vBuffer(mod(k+1,Lv)+1)/Lv;
    jn=an/La+jn1-aBuffer(mod(k,La)+1)/La;
    
    %Update value at current index in circular buffers with new data
    xBuffer(mod(k+1,Lx)+1)=xn;
    vBuffer(mod(k+1,Lv)+1)=vn;
    aBuffer(mod(k+1,La)+1)=an;
    jn1=jn;
    y(k+1)=jn;
end

%% Plot the data
% The index goofiness below is due the diff cutting the vectors down by 1
% each time
Ts=.01;

y=[zeros(5,1);y];
t=[(-5*Ts:Ts:-Ts)';Ts*(0:n)'];
dydt=diff(y)/Ts;
d2ydt2=diff(y,2)/Ts^2;

figure(2)
subplot(3,1,1)
stem(t(1:end-5),y(1:end-5));
title(sprintf('Position vs. time with ts=%0.2f',Ts))
ylabel('Degrees')
subplot(3,1,2)
title(sprintf('Velocity vs. time with ts=%0.2f',Ts))
ylabel('Degrees / sec')
stem(t(1:end-5),dydt(1:end-4));
subplot(3,1,3)
title(sprintf('Acceleration vs. time with ts=%0.2f',Ts))
ylabel('Degrees/sec^2')
xlabel('time, sec')
stem(t(1:end-5),d2ydt2(1:end-3));


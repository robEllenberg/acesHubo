fprintf('Open the states\n');
M1 = initState(2827);
M2 = initState(2828);
M3 = initState(2829);
M4 = initState(2830);

startport = 2831;
X = initState(startport);
Y = initState(startport+1);
Z = initState(startport+2);
P = initState(startport+3);
K = initState(startport+4);
R = initState(startport+5);

%cmdState(X, 1.1)
%sampleState(Z)
%readState(X)
%pause(.01);

%G = [gz1, gy1, gb1; ...
%     gz2, gy2, gb2; ...
%     gz3, gy3, gb3; ...
%     gz4, gy4, gb4;];

base = 1/2;
G = [base, -base,  base; ...
     base,  base,  base; ...
     base, -base, -base; ...
     base,  base, -base;];

while true
    z = readState(Z);
    x = readState(X);
    b = readState(K);
    u = [z;x;b]
    a = G*u
    cmdState(M1, a(1));
    cmdState(M2, a(2));
    cmdState(M3, a(3));
    cmdState(M4, a(4));
    pause(.1);
end

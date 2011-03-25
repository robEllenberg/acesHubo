fprintf('Open the states\n');
X = initState(2827);
Y = initState(2828);
Z = initState(2829);

fprintf('Set X to 1.1\n');
cmdState(X, 1.1)
pause(.01);
readState(X)

fprintf('Set Y to 2.2\n');
cmdState(Y, 2.2)
pause(.01);
readState(Y)

fprintf('Set Z to 3.3\n');
cmdState(Z, 3.3)
pause(.01);
readState(Z)
readState(Z)
readState(Z)
fprintf('Sample & read Z\n');
sampleState(Z)
pause(.01);
readState(Z)
fprintf('Sample & read Z\n');
sampleState(Z)
pause(.01);
readState(Z)

fprintf('Read X\n');
readState(X)
fprintf('Read Y\n');
readState(Y)
fprintf('Read Z\n');
readState(Z)

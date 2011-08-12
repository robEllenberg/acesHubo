function parseTestData(filename)
    close all
    if nargin == 0 
        filename = 'SmoothingTest-log.txt';
    end
    data=importdata(filename)
    figure(1)
    plot(data.data(:,1),data.data(:,2:3))
    legend('LKP smoothed','LHP not smoothed')
    grid on;

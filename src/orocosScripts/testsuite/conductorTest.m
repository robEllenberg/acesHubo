targets = [50, 100, 500, 1000, 5000, 10000];
style = {'r','g','b','k', 'r--','g--','b--','k--', 'r-.','g-.','b-.','k-.',...
         'r:','g:','b:','k:', 'c','m','y'};

figure;
hold;
hand = gcf;
i = 1;
for tar=targets,
    importLogger(['log', num2str(tar), '.txt']);
    t = data(:,1);
    err = sin(t)-data(:,2);
    figure(hand);
    plot(t, err, style{i});
    i = i + 1;
end
figure(hand);
legend('50', '100', '500', '1000', '5000', '10000');

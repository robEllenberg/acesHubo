t=0:1/100:200;
x=double(sin(2*pi*.2*t)) * 20;
y=20*(t>0);

fid=fopen('traj-smoothingStep.txt','w');
fprintf(fid,'LKP\n');
for k = 1:length(t)
    fprintf(fid,'%f\n',x(k));
end

fclose(fid);

function f = readState(state)
    fprintf(state, 'GET');
    f = fscanf(state, '%f');
end

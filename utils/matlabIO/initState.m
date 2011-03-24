function s = initState(port)
    s = udp('localhost', port);
    fopen(s);
end

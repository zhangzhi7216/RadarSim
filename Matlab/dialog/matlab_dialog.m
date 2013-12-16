function [] = matlab_dialog(target_positions)
%UNTITLED Summary of this function goes here
%   Detailed explanation goes here

close all

[target_count,position_count] = size(target_positions);

target = 1;

while 1
    target_position = target_positions(mod(target, target_count) + 1, :);
    plot(target_position)
    target = target + 1;
    pause(1)
end

end


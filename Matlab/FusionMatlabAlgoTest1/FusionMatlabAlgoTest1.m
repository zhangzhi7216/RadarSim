function [ fusionDatas, filterDatas, controlDatas ] = FusionMatlabAlgoTest1( planeTrueDatas, targetNoiseDatas )
%msgbox(num2str(targetNoiseDatas(1:1, 2:2)));
%msgbox(num2str(targetNoiseDatas(2:2, 2:2)));
%msgbox(num2str(targetNoiseDatas(3:3, 2:2)));
fusionDatas = targetNoiseDatas(1:3, :);
filterDatas = targetNoiseDatas(4:6, :);
controlDatas = planeTrueDatas;
end

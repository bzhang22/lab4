img = imread("image2026.jpg");
gray = rgb2gray(img);
resized_gray = imresize(gray, [5000, 5000]);
writematrix(resized_gray, 'input.txt', 'Delimiter', 'tab');

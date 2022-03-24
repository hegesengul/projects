function res = road(input_file_path , output_folder)
    files1 = dir(input_file_path + '*.png');
    files2 = dir(input_file_path + '*.jpg');
    
    for i=1:length(files1)
        im = imread(input_file_path + files1(i).name);
     
        imwrite(q(im) , output_folder + files1(i).name) ;
    end
    
    for i=1:length(files2)
        im = imread(input_file_path + files2(i).name);
     
        imwrite(q(im) , output_folder + files2(i).name) ;
    end
    
    res = 0;
end

function res = q(org)
    kek = org;
    r = org(:,:,1);
    g = org(:,:,2);
    b = org(:,:,3);

    im = rgb2hsv(histeq(org));

    h = im(:,:,1);
    s = im(:,:,2);

    t = s > 0.1 & (h > 0.15 & h < 0.5);

    r(t) = 0;
    g(t) = 0;
    b(t) = 0;

    org(:,:,1) = r;
    org(:,:,2) = g;
    org(:,:,3) = b;

    im = rgb2hsv(org);

    h = im(:,:,1);
    s = im(:,:,2);
    v = im(:,:,3);

    t = s > 0.1 | v > 0.8;


    h(t) = 0;
    v(t) = 0;
    s(t) = 0;

    im(:,:,1) = h;
    im(:,:,2) = s;
    im(:,:,3) = v;

    im = im2gray(hsv2rgb(im));

    [H,T,R] = hough(im);

    P  = houghpeaks(H, 200 , 'threshold' , ceil(0.2 * max(H(:))));

    lines = houghlines(im , T , R , P , 'FillGap' , 5 , 'MinLength' , 15);

    for k = 1:length(lines)
       xy = [lines(k).point1; lines(k).point2];
       
       kek = insertShape(kek ,'Line',[xy(1,1) xy(1,2) xy(2,1) xy(2,2)] , 'LineWidth' , 2 , 'Color', 'green');
    end
    
    res = kek;
end

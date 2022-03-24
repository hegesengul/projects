function res = n_cut(input_file_path , output_folder)
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

function res = q(im)
     A = rgb2hsv(im);

    num_of_nodes = 1000;

    [L,N] = superpixels(A , num_of_nodes);

    num_of_nodes = N;

    outputImage = zeros(size(A) , 'like' , A);
    idx = label2idx(L);

    numRows = size(A,1);
    numCols = size(A,2);

    means_red = zeros(1 , num_of_nodes , "double");
    means_gre = zeros(1 , num_of_nodes , "double");
    means_blu = zeros(1 , num_of_nodes , "double");

    centroidx = zeros(1 , num_of_nodes , "double");
    centroidy = zeros(1 , num_of_nodes , "double");

    for labelVal = 1:N
        redIdx   = idx{labelVal};
        greenIdx = idx{labelVal} + numRows * numCols;
        blueIdx  = idx{labelVal} + numRows * numCols * 2;

        means_red(labelVal) = mean(A(redIdx  ));
        means_gre(labelVal) = mean(A(greenIdx));
        means_blu(labelVal) = mean(A(blueIdx ));

        centroidx(labelVal) = mean(floor(mod(redIdx , numCols)));
        centroidy(labelVal) = mean(floor(redIdx / numCols));

         outputImage(redIdx)   = mean(A(redIdx  ));
         outputImage(greenIdx) = mean(A(greenIdx));
         outputImage(blueIdx)  = mean(A(blueIdx ));
    end    

    W = zeros(num_of_nodes , num_of_nodes , "double");

    var_intensity = var((means_blu.^2 + (means_blu .* means_gre .* sin(means_red)).^2 + (means_blu .* means_gre .* cos(means_red)).^2));
    var_distance = var((mean(centroidx) - centroidx).^2 + (mean(centroidy) - centroidy).^2);

    for i = 1 : num_of_nodes
        for j = 1 : num_of_nodes
            hsvi = [means_red(i) means_gre(i) means_blu(i)];
            hsvj = [means_red(j) means_gre(j) means_blu(j)];

            hsvi = [hsvi(3) hsvi(3)*hsvi(2)*sin(hsvi(1)) hsvi(3)*hsvi(2)*cos(hsvi(1))];
            hsvj = [hsvj(3) hsvj(3)*hsvj(2)*sin(hsvj(1)) hsvj(3)*hsvj(2)*cos(hsvj(1))];

            color_dist = (hsvi(1) - hsvj(1))^2 + (hsvi(2) - hsvj(2))^2 + (hsvi(3) - hsvj(3))^2;
            spati_dist = (centroidx(i) - centroidx(j))^2 + (centroidy(i) - centroidy(j))^2;

            W(i , j) = exp(- color_dist / var_intensity  - spati_dist / var_distance);
        end
    end

    D = zeros(num_of_nodes , num_of_nodes , "double");

    for i = 1 : num_of_nodes
        D(i , i) = sum(W(i , :));
    end

    [eigenvectors , eigenvalues] = eig(D^(-1/2) * (D - W) * D^(-1/2));

    eigenvector = D^(-1/2) * eigenvectors(: , 2);

    groups = zeros(1 , num_of_nodes);

    for i = 1 : num_of_nodes
        if eigenvector(i) > 0
            groups(i) = 1;
        else
            groups(i) = 0;
        end
    end

    for labelVal = 1 : num_of_nodes
        redIdx   = idx{labelVal};
        greenIdx = idx{labelVal} + numRows * numCols;
        blueIdx  = idx{labelVal} + numRows * numCols * 2;

        if groups(labelVal) == 1
            outputImage(redIdx)   = 0;
            outputImage(greenIdx) = 255;
            outputImage(blueIdx)  = 0;
        else
            outputImage(redIdx)   = 255;
            outputImage(greenIdx) = 0;
            outputImage(blueIdx)  = 0;
        end
    end
    
    res = outputImage;
end

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <opencv4/opencv2/opencv.hpp>

// Функция для применения фильтра Собела к изображению
void applySobelFilter(int** image, int row, int cols, int** result) 
{
    int gx, gy;

    // Ядро фильтра Собела по оси X
    int kernelX[3][3] = {
        {-1, 0, 1},
        {-2, 0, 2},
        {-1, 0, 1}
    };

    // Ядро фильтра Собела по оси Y
    int kernelY[3][3] = {
        {1, 2, 1},
        {0, 0, 0},
        {-1, -2, -1}      
    };

    gx = 0;
    gy = 0;

    // Применяем ядра фильтра Собела к пикселям изображения
    for (int k = -1; k <= 1; k++) 
    {
        for (int l = -1; l <= 1; l++) 
        {
            gx += image[row + k][cols + l] * kernelX[k + 1][l + 1];
            gy += image[row + k][cols + l] * kernelY[k + 1][l + 1];
        }
    }

    // Вычисляем градиент изображения
    
    result[row][cols] = fmin(255, fmax(0, sqrt(gx * gx + gy * gy)));
}

int main(int argc, char * argv[]) 
{
    char path[250] = "\0";        // Путь к файлу
    //strcpy(path, argv[2]);      // Получение пути из параметров запуска

    strcpy(path, "/home/jenkism/Desktop/laba4/nature-landscape-with-vegetation-flora.jpg");

    // Загрузка изображения по path
    cv::Mat img = cv::imread(path, cv::IMREAD_COLOR);
    
    // Не открылось
    if(img.empty()) {
        std::cout << " Невозможно прочитать изображение \n.";
        return 1;
    }

    //Все пиксели изображения (image[img.rows][img.cols] )
    int ** image = new int *[img.rows];
    int ** result = new int*[img.rows];
    for(int i = 0; i < img.rows; ++i)
    {
        image[i] = new int[img.cols];
        result[i] = new int[img.cols];
    }

    // Получение пикселей изображения
    for(int i = 0; i < img.rows; ++i) {
        for(int j = 0; j < img.cols; ++j) {
            // Пиксели (Доступ к отдельным каналам (R, G, B) uchar = 0..255)
            cv::Vec3b pixel = img.at<cv::Vec3b>(i, j);

            // Заполнение массива пикселей каналом RED
            image[i][j] = int(pixel[2]);

            //std::cout << image[i][j]<< " ";
        }
        //std::cout <<std::endl;
    }

    // Передача изображения в фильтр Собеля
    for(int row = 0; row < 3; ++row)
    {
        for(int cols = 0; cols < 3; ++cols)
        {
            std::cout<<image[row][cols] << " ";
        }
        std::cout<<std::endl;
    } 

    // Передача изображения в фильтр Собеля
    for(int row = 1; row < img.rows - 1; ++row)
    {
        for(int cols = 1; cols < img.cols - 1; ++cols)
        {
            applySobelFilter(image, row, cols, result);
        }
    }

        // Передача изображения в фильтр Собеля
    for(int row = 0; row < 3; ++row)
    {
        for(int cols = 0; cols < 3; ++cols)
        {
            std::cout<<result[row][cols] << " ";
        }
        std::cout<<std::endl;
    } 

    // Создание матрицы для изображения
    cv::Mat image_result(img.rows, img.cols, CV_8UC1, cv::Scalar(0));

    // Заполнение изображения значениями из матрицы
    for (int y = 1; y < img.rows - 1; ++y) {
        for (int x = 1; x < img.cols - 1; ++x) {
            // Пример: установка значений пикселя
            image_result.at<uchar>(y, x) = image[y][x];  // Значение пикселя зависит от его координат
        }
    }

    // Сохранение изображения
    cv::imwrite("/home/jenkism/Desktop/laba4/new_image_gray.jpg", image_result);

    return 0;
}

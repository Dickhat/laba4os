#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <opencv4/opencv2/opencv.hpp>

// Параметры для фунции
typedef struct
{
    int** image;    // Исходное изображение
    int** result;   // результат применения Собеля к изображению
    int row;        // текущая обрабатываемая строка
    int image_rows; // общее число столбцов изображения
    int image_cols; // общее число строк изображения
    int threads;     // число потоков обрабатывающих изображение
} args_t;

// Функция для применения фильтра Собела к изображению
void * applySobelFilter(void * args)   
{
    args_t *data = (args_t*) args;
    // Цикл обработки строк каждым потоком (приращение на число потоков)
    for(; data->row < (data->image_rows - 1); data->row += data->threads)
    {
        for(int cols = 1; cols < data->image_cols - 1; ++cols)
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
                    gx += data->image[data->row + k][cols + l] * kernelX[k + 1][l + 1];
                    gy += data->image[data->row + k][cols + l] * kernelY[k + 1][l + 1];
                }
            }

            // Вычисляем градиент изображения
            data->result[data->row][cols] = fmin(255, fmax(0, sqrt(gx * gx + gy * gy)));
        }
    }

    pthread_exit(0);
}

int main(int argc, char * argv[]) 
{
    char path[250] = "\0";          // Путь к файлу
    strcpy(path, argv[1]);          // Получение пути из параметров запуска

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
        }
    }

    
    int threads = 32;                                        // Число работающих потоков: 1, 2, 4, 8, 16, 32.
    pthread_t * massive_threads = new pthread_t[threads];   // Массив созданных потоков
    args_t * massive_threads_args = new args_t[threads];    // Массив данных(аргументов) для потоков

    // Передача изображения в фильтр Собеля
    for(int row = 1; (row < img.rows - 1) && (row <= threads); ++row)
    {
        massive_threads_args[row - 1].image = image;
        massive_threads_args[row - 1].image_cols = img.cols;
        massive_threads_args[row - 1].image_rows = img.rows;
        massive_threads_args[row - 1].result = result;
        massive_threads_args[row - 1].row = row;
        massive_threads_args[row - 1].threads = threads;

        // Создание потока
        pthread_create(&massive_threads[row - 1], NULL, applySobelFilter, (void*) &massive_threads_args[row - 1]);

        //applySobelFilter(image, result, row, img.rows, img.cols, threads);
    }

    // Ожидание окончания работы потоков
    for(int i = 0; i < threads; ++i)
    {
        pthread_join(massive_threads[i], NULL);
    }

    // Создание матрицы для изображения
    cv::Mat image_result(img.rows, img.cols, CV_8UC1, cv::Scalar(0));

    // Заполнение изображения значениями из матрицы
    for (int y = 1; y < img.rows - 1; ++y) {
        for (int x = 1; x < img.cols - 1; ++x) {
            // Пример: установка значений пикселя
            image_result.at<uchar>(y, x) = result[y][x];  // Значение пикселя зависит от его координат
        }
    }

    // Изменение качества выходного изображения
    //std::vector<int> compression_params;
    //compression_params.push_back(cv::IMWRITE_JPEG_QUALITY);
    //compression_params.push_back(30);

    // Сохранение изображения
    cv::imwrite("new_image_gray.jpg", image_result);//, compression_params);

    // О Ч И С Т К А    П А М Я Т И
    for(int i = 0; i < img.rows; ++i)
    {
        delete[] image[i];
        delete[] result[i];
    }
    delete[] image;
    delete[] result;
    delete[] massive_threads;
    delete[] massive_threads_args;

    return 0;
}

#include"Fore_extraction.h"
namespace hlg
{
    ThingInterface::ForeExtraction::ForeExtraction(const double &inputFrame_rows, const double &inputFrame_cols, bool show_flag )
    {
        this->show_flag = show_flag;
        frame_buff.reserve(init_background_frame_length);
        scaledSize = Size(image_width, image_width * int(inputFrame_rows) / int(inputFrame_cols));
        //printf("int(inputFrame_rows)=%d,int(inputFrame_cols)=%d\n", int(inputFrame_rows), int(inputFrame_cols));
        //cout << "scaledSize:" << scaledSize.width << " " << scaledSize.height << endl;
        model = createBackgroundSubtractorKNN(history, dist2thrshold, detectShadow);
    }
    bool ThingInterface::ForeExtraction::extract(const Mat &inputFrame, Mat &foregroundMask)
    {
        Mat src = inputFrame.clone();
        resize(src, src, scaledSize, 0, 0, INTER_LINEAR);
        static int count = 0;
        static vector<Mat>frame_buff(init_background_frame_length);
        if (count < init_background_frame_length)
        {       
            frame_buff[count] = src;
            ++count;
            if (count == init_background_frame_length)
            {
                count = INT32_MAX;
                for (int i = 0; i < init_background_length; i++)
                {
                    Mat image_temp, foregroundMask_temp;
                    image_temp = frame_buff[i%init_background_frame_length];
                    model->apply(image_temp, foregroundMask_temp, learning_rate);
                }
            }
            return false;
        }
        else
        {
            model->apply(src, foregroundMask, learning_rate);
            if (show_flag)
            {
                static Mat foreground,background;
                getBackgroundImage(background);
                if (!background.empty())
                    imshow("knn_mean background image", background);
                if (foreground.empty())
                    foreground.create(scaledSize, src.type());
                foreground = Scalar::all(0);
                src.copyTo(foreground, foregroundMask);
                if (!foregroundMask.empty())
                    imshow("knn_foreground mask", foregroundMask);
                if (!foreground.empty())
                    imshow("knn_foreground image", foreground);
                const char key = (char)waitKey(1);
            }
            return true;
        }

    }
    // show background image
    void ThingInterface::ForeExtraction::getBackgroundImage(Mat&background)
    {
        model->getBackgroundImage(background);
    }
}

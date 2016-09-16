#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <math.h>

using namespace cv; 
using namespace std;
 

int main(int argc, char** argv) {
	namedWindow("Result", 0);	
    
    VideoCapture video("movie.mp4");
	if (!video.isOpened()) return -1;
	
	double fps = video.get(CV_CAP_PROP_FPS);
	int frame_count = video.get(CV_CAP_PROP_FRAME_COUNT);
	int frame_width = video.get(CV_CAP_PROP_FRAME_WIDTH);
	int frame_height = video.get(CV_CAP_PROP_FRAME_HEIGHT);


	int fourcc = video.get(CV_CAP_PROP_FOURCC);

	Size frame_size = Size(frame_width, frame_height);

	double desired_frame_rate = 60.0;
	if (isnan(fps)) fps = 25.0;
	
	Mat frames[frame_count];
		
	for (;;) {
		int pos = video.get(CV_CAP_PROP_POS_FRAMES);
		Mat f;
		video >> f;
		if (!f.empty()) frames[pos] = f;
		else break;		
	}

	cout << frame_count << endl;

	//VideoWriter writer = VideoWriter("new_movie.mp", fourcc, desired_frame_rate, frame_size, true);
	//if (!writer.isOpened()) return -1;

    for (double pos = 0; pos < frame_count; pos += fps / desired_frame_rate) {
		int int_pos = floor(pos);
		double transition = pos - int_pos;

		Mat current_image = frames[int_pos];
		Mat next_image = frames[int_pos + 1];
		Mat merged_image = Mat(current_image.size(), current_image.type());

		if (transition == 0) merged_image = current_image;
		else {
			for (int i = 0; i < current_image.rows; i++) {
			    for (int j = 0; j < current_image.cols; j++) {
					Vec3b color1 = current_image.at<Vec3b>(i, j);
					Vec3b color2 = next_image.at<Vec3b>(i, j);
					Vec3b merged_color;
					for (int channel = 0; channel <= 2; channel++) {
						merged_color[channel] = floor(color1[channel] * (1 - transition) + color2[channel] * transition);
					}
		
       				merged_image.at<Vec3b>(i, j) = merged_color;					
				}
			}
		}
		
		imshow("Result", merged_image);
		waitKey(10);
	}

 
    video.release();
    destroyWindow("Camera Output");

}

/*
NOTE:

CV_CAP_PROP_POS_MSEC Current position of the video file in milliseconds.
CV_CAP_PROP_POS_FRAMES 0-based index of the frame to be decoded/captured next.
CV_CAP_PROP_POS_AVI_RATIO Relative position of the video file: 0 - start of the film, 1 - end of the film.
CV_CAP_PROP_FRAME_WIDTH Width of the frames in the video stream.
CV_CAP_PROP_FRAME_HEIGHT Height of the frames in the video stream.
CV_CAP_PROP_FPS Frame rate.
CV_CAP_PROP_FOURCC 4-character code of codec.
CV_CAP_PROP_FRAME_COUNT Number of frames in the video file.
CV_CAP_PROP_FORMAT Format of the Mat objects returned by retrieve() .
*/

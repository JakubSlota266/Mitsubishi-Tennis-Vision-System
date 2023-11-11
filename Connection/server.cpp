// Include files to use OpenCV API.
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <vector>
#include <chrono>

// Include files to use the PYLON API.
#include <pylon/PylonIncludes.h>

// Use sstream to create image names including integer
#include <sstream>

// Sockets
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define MAX_MSG_LEN 4096
#define SERWER_IP "192.168.192.125"
#define MAX_CONNECTION 10

// Namespace for using pylon objects.
using namespace Pylon;

// Namespace for using GenApi objects
using namespace GenApi;

// Namespace for using opencv objects.
using namespace cv;

// Namespace for using cout.
using namespace std;

// Namespace for time
using namespace std::chrono;

// Number of images to be grabbed.
static const uint32_t c_countOfImagesToGrab = 1000;
Scalar orangeLow = Scalar(15, 200, 140);
Scalar orangeHigh = Scalar(20, 255, 255);

int main(int argc, char* argv[])
{
    // The exit code of the sample application.
    int exitCode = 0;
    int frames = 0;
    int x_s = 0;
    int y_s = 0;
    int port = 55555;
    char buffer[ MAX_MSG_LEN ] = {};
    struct sockaddr_in client = { };
    struct sockaddr_in serwer =
    {
        .sin_family = AF_INET,
        .sin_port = htons( port )
    };
    if( inet_pton( AF_INET, SERWER_IP, & serwer.sin_addr ) <= 0 )
    {
        perror( "inet_pton() ERROR" );
        exit( 1 );
    }
   
    const int socket_ = socket( AF_INET, SOCK_STREAM, 0 );
    if( socket_ < 0 )
    {
        perror( "socket() ERROR" );
        exit( 2 );
    }
    socklen_t len = sizeof( serwer );
    
    if( bind( socket_,( struct sockaddr * ) & serwer, len ) < 0 )
    {
        perror( "bind() ERROR" );
        exit( 3 );
    }
   
    if( listen( socket_, MAX_CONNECTION ) < 0 )
    {
        perror( "listen() ERROR" );
        exit( 4 );
    }
   
    printf( "Waiting for connection...\n" );
    
    const int clientSocket = accept( socket_,( struct sockaddr * ) & client, & len );
    if( clientSocket < 0 )
    {
	perror( "accept() ERROR" );
	return -1;
    }
    bool alreadyCounting = false;
    
    // Automagically call PylonInitialize and PylonTerminate to ensure the pylon runtime system
    // is initialized during the lifetime of this object.
    Pylon::PylonAutoInitTerm autoInitTerm;

    try
    {
        // Create an instant camera object with the camera device found first.
	cout << "Creating Camera..." << endl;
	CInstantCamera camera(CTlFactory::GetInstance().CreateFirstDevice());

	cout << "Camera Created." << endl;
        // Print the model name of the camera.
        cout << "Using device " << camera.GetDeviceInfo().GetModelName() << endl;

        // The parameter MaxNumBuffer can be used to control the count of buffers
        // allocated for grabbing. The default value of this parameter is 10.
        camera.MaxNumBuffer = 10;

	// create pylon image format converter and pylon image
	CImageFormatConverter formatConverter;
	formatConverter.OutputPixelFormat= PixelType_BGR8packed;
	CPylonImage pylonImage;

	// Create an OpenCV image
	Mat openCvImage;

        // Start the grabbing of c_countOfImagesToGrab images.
        // The camera device is parameterized with a default configuration which
        // sets up free-running continuous acquisition.
        camera.StartGrabbing( c_countOfImagesToGrab);

        // This smart pointer will receive the grab result data.
        CGrabResultPtr ptrGrabResult;

        // Camera.StopGrabbing() is called automatically by the RetrieveResult() method
        // when c_countOfImagesToGrab images have been retrieved.
		std::chrono::high_resolution_clock::time_point  start;
		std::chrono::high_resolution_clock::time_point  end;
		Mat background;
		Mat mask;
		vector < vector < Point >> contours;
        while ( camera.IsGrabbing())
        {
            
            if(!alreadyCounting){
				start = high_resolution_clock::now();
				alreadyCounting = true;
			}
            frames++;
            // Wait for an image and then retrieve it. A timeout of 5000 ms is used.
            camera.RetrieveResult( 50, ptrGrabResult, TimeoutHandling_ThrowException);

            // Image grabbed successfully?
            if (ptrGrabResult->GrabSucceeded())
            {
				/*
                // Access the image data.
                cout << "SizeX: " << ptrGrabResult->GetWidth() << endl;
                cout << "SizeY: " << ptrGrabResult->GetHeight() << endl;
                const uint8_t *pImageBuffer = (uint8_t *) ptrGrabResult->GetBuffer();
                cout << "Gray value of first pixel: " << (uint32_t) pImageBuffer[0] << endl << endl;
				*/
		
		// Convert the grabbed buffer to pylon imag
		formatConverter.Convert(pylonImage, ptrGrabResult);
		// Create an OpenCV image out of pylon image
		openCvImage= cv::Mat(ptrGrabResult->GetHeight(), ptrGrabResult->GetWidth(), CV_8UC3, (uint8_t *) pylonImage.GetBuffer());

		// Create a display window
		//namedWindow( "OpenCV Display Window", WINDOW_NORMAL);//AUTOSIZE //FREERATIO
		// Display the current image with opencv
		
		openCvImage.copyTo(background);
		cvtColor(openCvImage, openCvImage, COLOR_BGR2HSV);

		
		inRange(openCvImage, orangeLow, orangeHigh, mask);
		

		findContours(mask, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
		//drawContours(background, contours, -1, (0, 255, 0), 3);
		
		for (size_t i = 0; i < contours.size(); i++)
		{
			Rect boundRect = boundingRect(contours[i]);
			if (boundRect.area() > 200 && (boundRect.width < 70 || boundRect.height < 70))
			{
			x_s = boundRect.tl().x + 1 / 2 * boundRect.br().x;
			y_s = boundRect.tl().y - 1 / 2 * boundRect.br().y;
			std::cout 
			<< "Coordinates: X = " << x_s
			<< ", Y = , " << y_s << endl;
			rectangle(background, boundRect.tl(), boundRect.br(), (0, 0, 0), 3);
			
			sprintf(buffer,"{\"X\": %d, \"Y\": %d}", x_s, y_s);
			if( send( clientSocket, buffer, strlen( buffer ), 0 ) <= 0 )
			{
				perror( "send() ERROR" );
				exit( 6 );
			}
			}  
		}
	
		end = high_resolution_clock::now();
		if(duration_cast<microseconds>(end - start) > 1000000us)
		{
			std::cout << "FPS: " << frames << endl;
			alreadyCounting = false;
			frames = 0;
		}
		
		//imshow("Contorus", background);
		// Define a timeout for customer's input in ms.
		// '0' means indefinite, i.e. the next image will be displayed after closing the window 
		// '1' means live stream
		waitKey(1);
		//imshow("tracking", background); 

            }
            else
            {
                cout << "Error: " << ptrGrabResult->GetErrorCode() << " " << ptrGrabResult->GetErrorDescription() << endl;
            }
        }
    }
    catch (GenICam::GenericException &e)
    {
        // Error handling.
        cerr << "An exception occurred." << endl
        << e.GetDescription() << endl;
        exitCode = 1;
    }

    // Comment the following two lines to disable waiting on exit.
    cerr << endl << "Press Enter to exit." << endl;
    while( cin.get() != '\n');

    return exitCode;
}

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

#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include <string.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>


// Thread

#include<pthread.h>
#include<stdlib.h>
// classroom
#define MAX_MSG_LEN_R 8
#define MAX_MSG_LEN 24
#define SERWER_IP "192.168.169.208"
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
pthread_t tid[2];

#define GPSET0 7
#define GPSET1 8

#define GPCLR0 10
#define GPCLR1 11

#define GPLEV0 13
#define GPLEV1 14

#define GPPUD     37
#define GPPUDCLK0 38
#define GPPUDCLK1 39
#define LED_RED 18

unsigned piModel;
unsigned piRev;

static volatile uint32_t  *gpioReg = nullptr;

#define PI_BANK (gpio>>5)
#define PI_BIT  (1<<(gpio&0x1F))

/* gpio modes. */

#define PI_INPUT  0
#define PI_OUTPUT 1
#define PI_ALT0   4
#define PI_ALT1   5
#define PI_ALT2   6
#define PI_ALT3   7
#define PI_ALT4   3
#define PI_ALT5   2

void gpioSetMode(unsigned gpio, unsigned mode)
{
   int reg, shift;

   reg   =  gpio/10;
   shift = (gpio%10) * 3;

   gpioReg[reg] = (gpioReg[reg] & ~(7<<shift)) | (mode<<shift);
}

int gpioGetMode(unsigned gpio)
{
   int reg, shift;

   reg   =  gpio/10;
   shift = (gpio%10) * 3;

   return (*(gpioReg + reg) >> shift) & 7;
}

/* Values for pull-ups/downs off, pull-down and pull-up. */

#define PI_PUD_OFF  0
#define PI_PUD_DOWN 1
#define PI_PUD_UP   2

void gpioSetPullUpDown(unsigned gpio, unsigned pud)
{
   *(gpioReg + GPPUD) = pud;

   usleep(20);

   *(gpioReg + GPPUDCLK0 + PI_BANK) = PI_BIT;

   usleep(20);
  
   *(gpioReg + GPPUD) = 0;

   *(gpioReg + GPPUDCLK0 + PI_BANK) = 0;
}

int gpioRead(unsigned gpio)
{
   if ((*(gpioReg + GPLEV0 + PI_BANK) & PI_BIT) != 0) return 1;
   else                                         return 0;
}

void gpioWrite(unsigned gpio, unsigned level)
{
   if (level == 0) *(gpioReg + GPCLR0 + PI_BANK) = PI_BIT;
   else            *(gpioReg + GPSET0 + PI_BANK) = PI_BIT;
}

void gpioTrigger(unsigned gpio, unsigned pulseLen, unsigned level)
{
   if (level == 0) *(gpioReg + GPCLR0 + PI_BANK) = PI_BIT;
   else            *(gpioReg + GPSET0 + PI_BANK) = PI_BIT;

   usleep(pulseLen);

   if (level != 0) *(gpioReg + GPCLR0 + PI_BANK) = PI_BIT;
   else            *(gpioReg + GPSET0 + PI_BANK) = PI_BIT;
}

/* Bit (1<<x) will be set if gpio x is high. */

uint32_t gpioReadBank1(void) { return (*(gpioReg + GPLEV0)); }
uint32_t gpioReadBank2(void) { return (*(gpioReg + GPLEV1)); }

/* To clear gpio x bit or in (1<<x). */

void gpioClearBank1(uint32_t bits) { *(gpioReg + GPCLR0) = bits; }
void gpioClearBank2(uint32_t bits) { *(gpioReg + GPCLR1) = bits; }

/* To set gpio x bit or in (1<<x). */

void gpioSetBank1(uint32_t bits) { *(gpioReg + GPSET0) = bits; }
void gpioSetBank2(uint32_t bits) { *(gpioReg + GPSET1) = bits; }

unsigned gpioHardwareRevision(void)
{
   static unsigned rev = 0;

   FILE * filp;
   char buf[512];
   char term;
   int chars=4; /* number of chars in revision string */

   if (rev) return rev;

   piModel = 0;

   filp = fopen ("/proc/cpuinfo", "r");

   if (filp != NULL)
   {
      while (fgets(buf, sizeof(buf), filp) != NULL)
      {
         if (piModel == 0)
         {
            if (!strncasecmp("model name", buf, 10))
            {
               if (strstr (buf, "ARMv6") != NULL)
               {
                  piModel = 1;
                  chars = 4;
               }
               else if (strstr (buf, "ARMv7") != NULL)
               {
                  piModel = 2;
                  chars = 6;
               }
               else if (strstr (buf, "ARMv8") != NULL)
               {
                  piModel = 2;
                  chars = 6;
               }
            }
         }

         if (!strncasecmp("revision", buf, 8))
         {
            if (sscanf(buf+strlen(buf)-(chars+1),
               "%x%c", &rev, &term) == 2)
            {
               if (term != '\n') rev = 0;
            }
         }
      }

      fclose(filp);
   }
   return rev;
}

int gpioInitialise(void)
{
   int fd;

   piRev = gpioHardwareRevision(); /* sets piModel and piRev */

   fd = open("/dev/gpiomem", O_RDWR | O_SYNC) ;

   if (fd < 0)
   {
      fprintf(stderr, "failed to open /dev/gpiomem\n");
      return -1;
   }

   gpioReg = (uint32_t *)mmap(NULL, 0xB4, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);

   close(fd);

   if (gpioReg == MAP_FAILED)
   {
      fprintf(stderr, "Bad, mmap failed\n");
      return -1;
   }
   return 0;
}

int main(int argc, char* argv[])
{
    gpioInitialise();
    
    gpioSetMode(LED_RED, PI_OUTPUT);
    gpioWrite(LED_RED, 1);
    bool gpio_red_state = gpioRead(LED_RED);
    
    if(gpio_red_state= gpioRead(LED_RED)){
	std::cerr << "GPIO ON" << std::endl;
	}
    gpioWrite(LED_RED, 0);
    gpio_red_state = gpioRead(LED_RED);
    if(!gpio_red_state){
	std::cerr <<"GPIO OFF - wygralem" << std::endl;
	}
    // The exit code of the sample application.
    int exitCode = 0;
    int frames = 0;
    int x_s = 0;
    int y_s = 0;
    int port = 55555;
    int bytesRead = 0;
    
    char buffer[ MAX_MSG_LEN ] = {};
    char rcv_buffer[ MAX_MSG_LEN ] = {};
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
        camera.StartGrabbing( GrabStrategy_LatestImageOnly);

        // This smart pointer will receive the grab result data.
        CGrabResultPtr ptrGrabResult;

        // Camera.StopGrabbing() is called automatically by the RetrieveResult() method
        // when c_countOfImagesToGrab images have been retrieved.
	std::chrono::high_resolution_clock::time_point  start;
	std::chrono::high_resolution_clock::time_point  end;
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
		
		//openCvImage.copyTo(background);
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
			
			//rectangle(background, boundRect.tl(), boundRect.br(), (0, 0, 0), 3);
			
			memset(rcv_buffer, 0, sizeof(rcv_buffer));
			
			recv(clientSocket, rcv_buffer, sizeof(rcv_buffer), MSG_DONTWAIT);
			{
			if(strcmp(rcv_buffer, "get_data") == 0)
			{
			    std::cout 	<< "Coordinates: X = " << x_s
					<< ", Y = , " << y_s << endl;
					
			    sprintf(buffer,"{\"X\": %d, \"Y\": %d}\n", x_s, y_s);
			    memset(rcv_buffer, 0, sizeof(rcv_buffer));
	    
			if( send( clientSocket, buffer, strlen( buffer ), 0 ) <= 0 )
			{
				perror( "send() ERROR" );
				exit( 6 );
			}
			}
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


// 
// Example 7-5. Template matching
// Yes, I switched the order of image and template from the text.  It's now
// 
// Usage: matchTemplate template image
//
// Puts results of all types of matching methods listed i help() below. 
//        Gary Bradski Oct 3, 2008
//
/* *************** License:**************************
   Oct. 3, 2008
   Right to use this code in any way you want without warrenty, support or any guarentee of it working.

   BOOK: It would be nice if you cited it:
   Learning OpenCV: Computer Vision with the OpenCV Library
     by Gary Bradski and Adrian Kaehler
     Published by O'Reilly Media, October 3, 2008
 
   AVAILABLE AT: 
     http://www.amazon.com/Learning-OpenCV-Computer-Vision-Library/dp/0596516134
     Or: http://oreilly.com/catalog/9780596516130/
     ISBN-10: 0596516134 or: ISBN-13: 978-0596516130    

   OTHER OPENCV SITES:
   * The source code is on sourceforge at:
     http://sourceforge.net/projects/opencvlibrary/
   * The OpenCV wiki page (As of Oct 1, 2008 this is down for changing over servers, but should come back):
     http://opencvlibrary.sourceforge.net/
   * An active user group is at:
     http://tech.groups.yahoo.com/group/OpenCV/
   * The minutes of weekly OpenCV development meetings are at:
     http://pr.willowgarage.com/wiki/OpenCV
   ************************************************** */
#include <cv.h>
#include <cxcore.h>
#include <highgui.h>
#include <stdio.h>

#define USE_CV_BLOB 0
#define USE_CV_BLOB_LIB 1

#if USE_CV_BLOB
#include <cvblob.h>
using namespace cvb;
#endif // USE_CV_BLOB

void help(){
printf("\n"
"Example of using cvMatchTemplate().  The call is:\n"
"\n"
"ch7_ex7_5 template image_to_be_searched\n"
"\n"
"   This routine will search using all methods:\n"
"         CV_TM_SQDIFF        0\n"
"         CV_TM_SQDIFF_NORMED 1\n"
"         CV_TM_CCORR         2\n"
"         CV_TM_CCORR_NORMED  3\n"
"         CV_TM_CCOEFF        4\n"
"         CV_TM_CCOEFF_NORMED 5\n"
"\n"
"The function prototype is:\n"
"   cvMatchTemplate( const CvArr* image, const CvArr* templ,\n"
"                              CvArr* result, int method );\n"
"      image\n"
"         Image to be searched. It should be 8-bit or 32-bit floating-point. \n"
"      templ\n"
"         template which must not larger than the above image and is the same type as the image. \n"
"      result\n"
"         A map of comparison results; single-channel 32-bit floating-point. \n"
"      method\n"
"         See the above methods 0-5 starting with CM_TM_SQDIFF\n"
"         \n"
"	If image is W×H and templ is w×h then result must be W-w+1×H-h+1.		\n"
"\n");
}


// Call is
// matchTemplate template image 
//
// Will Display the results of the matchs
// 
int main( int argc, char** argv ) {

    IplImage *src, *templ,*ftmp[6]; //ftmp is what to display on
    int i;
    int nonZeroCount[8];
    memset(&nonZeroCount, 0, sizeof(nonZeroCount));

    if( argc == 3){ 
		//Read in the template to be used for matching:
		if((templ=cvLoadImage(argv[1], -1))== 0) {
				printf("Error on reading template %s\n",argv[2]); help();
				return(-1);
		}

		//Read in the source image to be searched:
		if((src=cvLoadImage(argv[2], -1))== 0) {
				printf("Error on reading src image %s\n",argv[i]); help();
				return(-1);
		}

        printf( "template depth %d channels %d\n", templ->depth, templ->nChannels );
        printf( "source depth %d channels %d\n", src->depth, src->nChannels );

 		int patchx = templ->width;
		int patchy = templ->height;
		int iwidth = src->width - patchx + 1;
		int iheight = src->height - patchy + 1;
		for(i=0; i<6; ++i){
			ftmp[i] = cvCreateImage( cvSize(iwidth,iheight),32,1);
		}

		//DO THE MATCHING OF THE TEMPLATE WITH THE IMAGE
		for(i=0; i<6; ++i){
			cvMatchTemplate( src, templ, ftmp[i], i); 
//		double min,max;
//		cvMinMaxLoc(ftmp,&min,&max);
            cvPow(ftmp[i], ftmp[i], 5);
			cvNormalize(ftmp[i],ftmp[i],1,0,CV_MINMAX);
		}

        nonZeroCount[5] = cvCountNonZero(ftmp[5]);
        //nonZeroCount[6] = cvCountNonZero(src);
        //nonZeroCount[7] = cvCountNonZero(templ);

        //DISPLAY
		cvNamedWindow( "Template", 0 );
        cvShowImage(   "Template", templ );
        cvNamedWindow( "Image", 0 );
        cvShowImage(   "Image", src );

        /*
		cvNamedWindow( "SQDIFF", 0 );
        cvShowImage(   "SQDIFF", ftmp[0] );

		cvNamedWindow( "SQDIFF_NORMED", 0 );
        cvShowImage(   "SQDIFF_NORMED", ftmp[1] );

		cvNamedWindow( "CCORR", 0 );
        cvShowImage(   "CCORR", ftmp[2] );

		cvNamedWindow( "CCORR_NORMED", 0 );
        cvShowImage(   "CCORR_NORMED", ftmp[3] );

		cvNamedWindow( "CCOEFF", 0 );
        cvShowImage(   "CCOEFF", ftmp[4] );
        */

        IplImage* color = ftmp[5];
        IplImage* gray = cvCreateImage(cvSize(color->width, color->height), IPL_DEPTH_8U, 1);
        printf("\nbefore conversion\n");
        printf("color depth %d channel %d\n", color->depth, color->nChannels);
        printf("gray depth %d channel %d\n", gray->depth, gray->nChannels);

        printf("\nafter conversion\n");
        cvConvert(color, gray);
        printf("color depth %d channel %d\n", color->depth, color->nChannels);
        printf("gray depth %d channel %d\n", gray->depth, gray->nChannels);

        IplImage* colorImg = cvCreateImage(cvGetSize(gray), 8, 3);
        CvMemStorage* memStorage = cvCreateMemStorage(0);
        CvSeq* contours = 0;
        cvFindContours(gray, memStorage, &contours);

        //cvCvtColor(color, gray, CV_BGR2GRAY);

		cvNamedWindow( "CCOEFF_NORMED", 0 );
        cvShowImage(   "CCOEFF_NORMED", ftmp[5] );

		cvNamedWindow( "test", 0 );
        cvShowImage(   "test", gray );

		cvNamedWindow( "colorimg", 0 );
        cvShowImage(   "colorimg", colorImg );

        int contourCount = 0;
        for (CvSeq* c = contours; c != NULL; c = c->h_next)
        {
            ++contourCount;
            printf( "%p\n", c);
        }
        printf( "Matches found: %d\n", contourCount );

#if USE_CV_BLOB
        IplImage* gray = cvCreateImage(cvGetSize(ftmp[5]), IPL_DEPTH_8U, 1);
        cvCvtColor(ftmp[5], gray, CV_BGR2GRAY);
        cvThreshold(gray, gray, 150, 255, CV_THRESH_BINARY);
        
        IplImage* labelImg = cvCreateImage(cvGetSize(gray), IPL_DEPTH_LABEL, 1);
        CvBlobs blobs;
        unsigned int result = cvLabel(gray, labelImg, blobs);
#endif // USE_CV_BLOB


/*
        IplImage* colorImg = cvCreateImage(cvGetSize(ftmp[5]), 8, 3);
        CvMemStorage* memStorage = cvCreateMemStorage(0);
        CvSeq* contours = 0;
        cvFindContours(ftmp[5], memStorage, &contours);
        */

		//LET USER VIEW RESULTS:
        cvWaitKey(0);
    }
	else { help();}

}

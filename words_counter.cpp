// words_counter

#include <cv.h>
#include <cxcore.h>
#include <highgui.h>
#include <stdio.h>

void help(){
printf("\n"
"Example of using cvMatchTemplate().  The call is:\n"
"\n"
"words_counter template image_to_be_searched\n"
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

        //DISPLAY
		cvNamedWindow( "Template", 0 );
        cvShowImage(   "Template", templ );
        cvNamedWindow( "Image", 0 );
        cvShowImage(   "Image", src );

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

		cvNamedWindow( "CCOEFF_NORMED", 0 );
        cvShowImage(   "CCOEFF_NORMED", ftmp[5] );

		cvNamedWindow( "gray", 0 );
        cvShowImage(   "gray", gray );

        int contourCount = 0;
        for (CvSeq* c = contours; c != NULL; c = c->h_next)
            ++contourCount;
        printf( "Matches found: %d\n", contourCount );

		//LET USER VIEW RESULTS:
        cvWaitKey(0);
    }
	else { help();}
}

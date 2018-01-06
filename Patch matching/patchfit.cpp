#include <bits/stdc++.h>
#define MAX_COMPRGB 255 //maximum rgb component is 255
#define PSL 120 //patch size can be varied in multiples of 20,kept as 120
#define BRDR_WIDTH 40//border width kept as 40 pixel values according to the algorithm used
#define BASE_SOURCE_FILE_INDEX 0 //take out this #def
using namespace std;

typedef struct _PPMPixel
{
    unsigned int red;
    unsigned int green;
    unsigned int blue;
} PPMPixel;

typedef struct _PPMPixel_double
{
    double red;
    double green;
    double blue;
} PPMPixel_double;

typedef struct _PPMImage
{
    int w;
    int h;
    PPMPixel data[1000][1500]; 
} PPMImage;

typedef struct _answer
{
    int cw;
    int ch;
    string f_nameFinal;
}answer;

void chk_readPPM_P3(const char *f_name)
{
   PPMImage *img;
    ifstream fin;
    fin.open(f_name);
    if(!fin.good())
    {
        fprintf(stderr, "Error in opening file '%s' !!\n", f_name);
        exit(1);
    }

    // Check file format
    char buf[200];
    fin >> buf;
    if(buf[0] != 'P' ||buf[1] != '3')
    {
		fprintf(stderr, "This is an invalid format! %s here should be P3 instead\n", buf);
        fin.close();
        exit(1);
    }

    fin.getline(buf, 200);

    // Allocate memory for image
    img = (PPMImage *)malloc(sizeof(PPMImage));
    if (!img) 
    {
        fprintf(stderr, "Error in MEMORY allocation!!\n");
        fin.close();
        exit(1);
    }

    // Check for comments
    while('#' ==fin.peek()) 
    {
        fin.getline(buf, 200);
    }
	// no. of horizontal pixels
    if(!(fin >> img->w))
    {
        fprintf(stderr, "This is an error due to invalid width of image which is a problem in loading of '%s'!\n", f_name);
        fin.close();
        exit(1);
    }
	//no. of vertical pixels
    if(!(fin >> img->h))
    {
        fprintf(stderr, "This is an error due to invalid height of image which is a problem in loading of '%s'!\n", f_name);
        fin.close();
        exit(1);
    }
	if(img->w !=1280 ||img->h !=720)
	{
        printf("The image size is %d X %d and not 1280 X 720 in %s\n", img->w, img->h, f_name);
        fin.close();
        exit(1);
    }	
    //read rgb component
    int rgb_max;
    if (!(fin >> rgb_max)) 
    {
        fprintf(stderr, "Wrong RGB component which is a problem in loading of '%s'\n", f_name);
        exit(1);
    }

    //check rgb component depth
    if (rgb_max!=MAX_COMPRGB) 
    {
        fprintf(stderr, "max is not upto 255 in '%s'!!\n", f_name);
        exit(1);
    }
}

static PPMImage *readPPM_P3(const char *f_name)
{
   PPMImage *img;
    ifstream fin;
    fin.open(f_name);
    if(!fin.good())
    {
        fprintf(stderr, "Error in opening file '%s' !!\n", f_name);
        exit(1);
    }

    // Check file format
    char buf[200];
    fin >> buf;
    if(buf[0] != 'P' || buf[1] != '3')
    {
        fprintf(stderr, "This is an invalid format! %s here should be P3 instead\n", buf);
        fin.close();
        exit(1);
    }
	fin.getline(buf, 200);
	// Allocate memory for image
    img = (PPMImage *)malloc(sizeof(PPMImage));
    if (!img) 
    {
        fprintf(stderr, "Error in MEMORY allocation!!\n");
        fin.close();
        exit(1);
    }
	// Check for comments
    while('#' ==fin.peek()) 
    {
        fin.getline(buf, 200);
    }
	// width (no. of horizontal pixels)
    if(!(fin >> img->w))
    {
        fprintf(stderr,"This is an error due to invalid width of image which is a problem in loading of '%s'!\n", f_name);
        fin.close();
        exit(1);
    }

    // height (no. of vertical pixels)
    if(!(fin >> img->h))
    {
        fprintf(stderr,"This is an error due to invalid height of image which is a problem in loading of '%s'!\n", f_name);
        fin.close();
        exit(1);
    }

    if(img->w != 1280 || img->h != 720)
    {
        printf("The image size is %d X %d and not 1280 X 720 in %s\n", img->w, img->h,f_name);
        fin.close();
        exit(1);
    }

    //read rgb component
    int rgb_max;
    if (!(fin >> rgb_max)) 
    {
        fprintf(stderr, "Wrong RGB component which is a problem in loading of '%s'\n", f_name);
        exit(1);
    }

    //check rgb component depth
    if (rgb_max != MAX_COMPRGB) 
    {
        fprintf(stderr, "max is not upto 255 in '%s'!!\n", f_name);
        exit(1);
    }
	// Read all the pixel rgb's
    for(int i=0;i<img->h;i++)
    {
        for(int j=0;j<img->w;j++)
        {
            if(!(fin >> img->data[i][j].red))
            {
                fprintf(stderr,"Error at pixel(%d,%d) due to low rgb val\n", i, j);
                free(img);
                fin.close();
                exit(1);
            }
            if(!(fin >> img->data[i][j].green))
            {
                fprintf(stderr,"Error at pixel(%d,%d) due to low rgb val\n", i, j);
                free(img);
                fin.close();
                exit(1);
            }
            if(!(fin >> img->data[i][j].blue))
            {
                fprintf(stderr,"Error at pixel(%d,%d) due to low rgb val\n", i, j);
                free(img);
                fin.close();
                exit(1);
            }
        }
    }
	fin.close();
    return img;
}

void blacken(PPMImage *img, int cw, int ch, int sl)
{
	int half=sl/2;
    for(int i=ch-half;i<=ch+half;i++)
    {
        for(int j=cw-half;j<=cw+half;j++)
        {
            img->data[i][j].red= 0;img->data[i][j].green= 0;img->data[i][j].blue= 0;
        }
    }
}

// To create the ppm (P3) image file
void writePPM_P3(const char *f_name, PPMImage *img)
{
    ofstream fout;
    fout.open(f_name);
    if(!fout.good())
    {
        fprintf(stderr, "Error in opening the temp file '%s'\n", f_name);
        exit(1);
    }
	fout<<"P3\n";
    fout<<img->w<<" "<<img->h<<'\n';
    fout<<MAX_COMPRGB<<'\n';
    for(int i=0;i<img->h;i++)
    {
        for(int j=0;j<img->w;j++)
        {
            fout<<img->data[i][j].red<<" ";
            fout<<img->data[i][j].green<<" ";
            fout<<img->data[i][j].blue<<" ";
        }
        fout<<'\n';
    }
	fout.close();
}
void addPatch(PPMImage *img1, int cw1, int ch1, PPMImage *img2, int cw2, int ch2)
{
    for(int i0=ch1-PSL/2,i1=ch2-PSL/2; i0 <= ch1+PSL/2; i0++,i1++)
    {
        for(int j0=cw1-PSL/2, j1=cw2-PSL/2; j0<= cw1+PSL/2; j0++, j1++)
        {
            img1->data[i0][j0].red = img2->data[i1][j1].red;img1->data[i0][j0].green = img2->data[i1][j1].green;img1->data[i0][j0].blue = img2->data[i1][j1].blue;
        }
    }
}
bool islowVar(PPMPixel_double &a, double b)
{
    return ((a.red+a.green+a.blue)/3.0 <= b);
}
double meanrgb(PPMPixel_double &a)
{
    return (a.red+a.green+a.blue)/3.0;
}
//score after doing vertical traversal
double vertTraversalScore(PPMImage *img1,int cw1,int ch1)
{
    int threshSqrWidth=10;
    int part1=BRDR_WIDTH/threshSqrWidth;
    int part2=((PSL/2)/threshSqrWidth);

    // across a single vertical traversal (column)
    //here part1=4,part2=6,which means there will be 4 threshold squares in top and bottom regions and 6 threshold squares in patch1 and patch2 regions
    PPMPixel_double meanTop[part1]; 
    PPMPixel_double meanPatch1[part2];
    PPMPixel_double meanPatch2[part2];
    PPMPixel_double meanBottom[part1];
    
    int vstripsnum = PSL/threshSqrWidth;
    PPMPixel_double meanofmeansTop[vstripsnum];
    PPMPixel_double varofmeansTop[vstripsnum];
    PPMPixel_double meanofmeansBottom[vstripsnum];
    PPMPixel_double varofmeansBottom[vstripsnum];
    PPMPixel_double meanofmeansPatch1[vstripsnum];
    PPMPixel_double varofmeansPatch1[vstripsnum];
    PPMPixel_double meanofmeansPatch2[vstripsnum];
    PPMPixel_double varofmeansPatch2[vstripsnum];
    PPMPixel_double diffmeanofmeansTop[vstripsnum];
    PPMPixel_double diffmeanofmeansBottom[vstripsnum];

    double score[vstripsnum];
    double fin_score;
    double incval, incval2;
    bool islowVT, islowVP1, islowVP2, islowVB, isdifflessMT_12, isdifflessMB_12, isdifflessMP1P2_15, isdiffmoreMT_30, isdiffmoreMB_30, isdifflessMTB_20, isdifflessMT_15, isdifflessMB_15, isdifflessMTB_25;

    // go vertically:
    int k=0;
    // initialize all means
    for(int k1=0;k1<2*(part1+part2);k1++)
    {
        if(k1<part1)
        {
            meanTop[k1].red=0;meanTop[k1].green=0;meanTop[k1].blue=0;
        }
        else if(k1<part1+part2)
        {
        	int fu=k1-part1;
            meanPatch1[fu].red = 0;meanPatch1[fu].green = 0;meanPatch1[fu].blue = 0;
        }
        else if(k1<2*part2+part1)
        {
        	int fu1=k1-part1-part2;
            meanPatch2[fu1].red = 0;meanPatch2[fu1].green = 0;meanPatch2[fu1].blue = 0;
        }
        else
        {
        	int fu2=k1-part1-2*part2;
            meanBottom[fu2].red = 0;meanBottom[fu2].green = 0;meanBottom[fu2].blue = 0;
        }
    }
    // go vertically:
    for(int j = cw1-PSL/2,vStripInd = 0; j < cw1+PSL/2; j+=10, vStripInd++) 
    {
        for (int i=ch1-(PSL/2)-BRDR_WIDTH; i<ch1+(PSL/2)+BRDR_WIDTH;i+=10)
        {
            for(int si=i;si<=i+threshSqrWidth; si++)
            {
                for(int sj=j;sj<=j+threshSqrWidth;sj++)
                {
                    if(k<part1)
                    {
                        meanTop[k].red += img1->data[si][sj].red;meanTop[k].green += img1->data[si][sj].green;meanTop[k].blue += img1->data[si][sj].blue;
                    }
                    else if(k<part1 + part2)
                    {
                    	int fu=k-part1;
                        meanPatch1[fu].red += img1->data[si][sj].red;meanPatch1[fu].green += img1->data[si][sj].green;meanPatch1[fu].blue += img1->data[si][sj].blue;
                    }
                    else if(k<part1+2*part2) 
                    {
                    	int fu1=k-part1-part2;
                        meanPatch2[fu1].red += img1->data[si][sj].red;meanPatch2[fu1].green += img1->data[si][sj].green;meanPatch2[fu1].blue += img1->data[si][sj].blue;
                    }
                    else
                    {
                    	int fu2=k-part1-2*part2;
                        meanBottom[fu2].red += img1->data[si][sj].red;meanBottom[fu2].green += img1->data[si][sj].green;meanBottom[fu2].blue += img1->data[si][sj].blue;
                    }
                }
            }
            k++;
        }

        meanofmeansTop[vStripInd].red=0;varofmeansTop[vStripInd].red = 0;
        meanofmeansTop[vStripInd].green=0;varofmeansTop[vStripInd].green = 0;
        meanofmeansTop[vStripInd].blue=0;varofmeansTop[vStripInd].blue = 0;

        meanofmeansPatch1[vStripInd].red=0;varofmeansPatch1[vStripInd].red = 0;
        meanofmeansPatch1[vStripInd].green=0;varofmeansPatch1[vStripInd].green = 0;
        meanofmeansPatch1[vStripInd].blue=0;varofmeansPatch1[vStripInd].blue = 0;

        meanofmeansPatch2[vStripInd].red=0;varofmeansPatch2[vStripInd].red = 0;
        meanofmeansPatch2[vStripInd].green=0;varofmeansPatch2[vStripInd].green = 0;
        meanofmeansPatch2[vStripInd].blue=0;varofmeansPatch2[vStripInd].blue = 0;
        
        meanofmeansBottom[vStripInd].red=0;varofmeansBottom[vStripInd].red = 0;
        meanofmeansBottom[vStripInd].green=0;varofmeansBottom[vStripInd].green = 0;
        meanofmeansBottom[vStripInd].blue=0;varofmeansBottom[vStripInd].blue = 0;

        // limits means' rgb's to (0-255)
        // variance varies from 0 to (255/2)^2 for even #elements and ( ((255^2)*n*(n+1)) / ((2n+1)^2) ) for (2n+1) elements
        int index;
        for(int k1=0;k1<2*(part1+part2);k1++)
        {
            if(k1<part1)
            {
            	int sqs=threshSqrWidth*threshSqrWidth;
                meanTop[k1].red /= (sqs*1.0); meanTop[k1].green /= (sqs*1.0);meanTop[k1].blue /= (sqs*1.0);
                // mean = (Summation_X)/N
                meanofmeansTop[vStripInd].red += meanTop[k1].red;meanofmeansTop[vStripInd].green += meanTop[k1].green;meanofmeansTop[vStripInd].blue += meanTop[k1].blue;
                // variance = (Summation_X^2)/N - mean^2
                varofmeansTop[vStripInd].red += meanTop[k1].red * meanTop[k1].red;varofmeansTop[vStripInd].green += meanTop[k1].green * meanTop[k1].green;varofmeansTop[vStripInd].blue += meanTop[k1].blue * meanTop[k1].blue;
            }
            else if(k1 < part1+part2)
            {
                index = k1-part1;
                int sqs=threshSqrWidth*threshSqrWidth;
                meanPatch1[index].red /= (sqs*1.0);meanPatch1[index].green /= (sqs*1.0);meanPatch1[index].blue /= (sqs*1.0);
                meanofmeansPatch1[vStripInd].red +=meanPatch1[index].red;
                meanofmeansPatch1[vStripInd].green +=meanPatch1[index].green;
                meanofmeansPatch1[vStripInd].blue +=meanPatch1[index].blue;
                varofmeansPatch1[vStripInd].red +=meanPatch1[index].red *meanPatch1[index].red;
                varofmeansPatch1[vStripInd].green +=meanPatch1[index].green *meanPatch1[index].green;
                varofmeansPatch1[vStripInd].blue +=meanPatch1[index].blue *meanPatch1[index].blue;
            }
            else if(k1 < part1+2*part2)
            {
                index = k1-part1-part2;
                int sqs=threshSqrWidth*threshSqrWidth;
                meanPatch2[index].red /= (sqs*1.0);meanPatch2[index].green /= (sqs*1.0);meanPatch2[index].blue /= (sqs*1.0);
                meanofmeansPatch2[vStripInd].red += meanPatch2[index].red;
                meanofmeansPatch2[vStripInd].green += meanPatch2[index].green;
                meanofmeansPatch2[vStripInd].blue += meanPatch2[index].blue;
                varofmeansPatch2[vStripInd].red += meanPatch2[index].red * meanPatch2[index].red;
                varofmeansPatch2[vStripInd].green += meanPatch2[index].green * meanPatch2[index].green;
                varofmeansPatch2[vStripInd].blue += meanPatch2[index].blue * meanPatch2[index].blue;
            }
            else
            {
                index = k1-part1-2*part2;
                int sqs=threshSqrWidth*threshSqrWidth;
                meanBottom[index].red /= (sqs*1.0);meanBottom[index].green /= (sqs*1.0);meanBottom[index].blue /= (sqs*1.0);
                meanofmeansBottom[vStripInd].red += meanBottom[index].red;
                meanofmeansBottom[vStripInd].green += meanBottom[index].green;
                meanofmeansBottom[vStripInd].blue += meanBottom[index].blue;
                varofmeansBottom[vStripInd].red += meanBottom[index].red*meanBottom[index].red;
                varofmeansBottom[vStripInd].green += meanBottom[index].green*meanBottom[index].green;
                varofmeansBottom[vStripInd].blue += meanBottom[index].blue*meanBottom[index].blue;
            }
        }

        // limits all the mean_mean*'s to (0-255)
        // diffmean's - (0-255). Then diffmeans will be normalized. So, finally (0,1)

        meanofmeansTop[vStripInd].red /=part1*1.0;
        meanofmeansTop[vStripInd].green /=part1*1.0;
        meanofmeansTop[vStripInd].blue /=part1*1.0;
        varofmeansTop[vStripInd].red =fabs(varofmeansTop[vStripInd].red/part1 - meanofmeansTop[vStripInd].red*meanofmeansTop[vStripInd].red);
        varofmeansTop[vStripInd].green =fabs(varofmeansTop[vStripInd].green/part1 - meanofmeansTop[vStripInd].green*meanofmeansTop[vStripInd].green);
        varofmeansTop[vStripInd].blue =fabs(varofmeansTop[vStripInd].blue/part1 - meanofmeansTop[vStripInd].blue*meanofmeansTop[vStripInd].blue);

        meanofmeansBottom[vStripInd].red /= part1*1.0;
        meanofmeansBottom[vStripInd].green /= part1*1.0;
        meanofmeansBottom[vStripInd].blue /= part1*1.0;
        varofmeansBottom[vStripInd].red = fabs(varofmeansBottom[vStripInd].red/part1 - meanofmeansBottom[vStripInd].red*meanofmeansBottom[vStripInd].red);
        varofmeansBottom[vStripInd].green = fabs(varofmeansBottom[vStripInd].green/part1 - meanofmeansBottom[vStripInd].green*meanofmeansBottom[vStripInd].green);
        varofmeansBottom[vStripInd].blue = fabs(varofmeansBottom[vStripInd].blue/part1 - meanofmeansBottom[vStripInd].blue*meanofmeansBottom[vStripInd].blue);

        meanofmeansPatch1[vStripInd].red /= part2*1.0;
        meanofmeansPatch1[vStripInd].green /= part2*1.0;
        meanofmeansPatch1[vStripInd].blue /= part2*1.0;
        diffmeanofmeansTop[vStripInd].red = fabs(meanofmeansTop[vStripInd].red-meanofmeansPatch1[vStripInd].red);
        diffmeanofmeansTop[vStripInd].green = fabs(meanofmeansTop[vStripInd].green-meanofmeansPatch1[vStripInd].green);
        diffmeanofmeansTop[vStripInd].blue = fabs(meanofmeansTop[vStripInd].blue-meanofmeansPatch1[vStripInd].blue);
        varofmeansPatch1[vStripInd].red = fabs(varofmeansPatch1[vStripInd].red/part2 - meanofmeansPatch1[vStripInd].red*meanofmeansPatch1[vStripInd].red);
        varofmeansPatch1[vStripInd].green = fabs(varofmeansPatch1[vStripInd].green/part2 - meanofmeansPatch1[vStripInd].green*meanofmeansPatch1[vStripInd].green);
        varofmeansPatch1[vStripInd].blue = fabs(varofmeansPatch1[vStripInd].blue/part2 - meanofmeansPatch1[vStripInd].blue*meanofmeansPatch1[vStripInd].blue);

        meanofmeansPatch2[vStripInd].red /= part2*1.0;
        meanofmeansPatch2[vStripInd].green /= part2*1.0;
        meanofmeansPatch2[vStripInd].blue /= part2*1.0;
        diffmeanofmeansBottom[vStripInd].red = fabs(meanofmeansBottom[vStripInd].red-meanofmeansPatch2[vStripInd].red);
        diffmeanofmeansBottom[vStripInd].green = fabs(meanofmeansBottom[vStripInd].green-meanofmeansPatch2[vStripInd].green);
        diffmeanofmeansBottom[vStripInd].blue = fabs(meanofmeansBottom[vStripInd].blue-meanofmeansPatch2[vStripInd].blue);
        varofmeansPatch2[vStripInd].red = fabs(varofmeansPatch2[vStripInd].red/part2 - meanofmeansPatch2[vStripInd].red*meanofmeansPatch2[vStripInd].red);
        varofmeansPatch2[vStripInd].green = fabs(varofmeansPatch2[vStripInd].green/part2 - meanofmeansPatch2[vStripInd].green*meanofmeansPatch2[vStripInd].green);
        varofmeansPatch2[vStripInd].blue = fabs(varofmeansPatch2[vStripInd].blue/part2 - meanofmeansPatch2[vStripInd].blue*meanofmeansPatch2[vStripInd].blue);

        islowVT = islowVar(varofmeansTop[vStripInd], 90);
        islowVP1 = islowVar(varofmeansPatch1[vStripInd], 90);
        islowVP2 = islowVar(varofmeansPatch2[vStripInd], 90);
        islowVB = islowVar(varofmeansBottom[vStripInd], 90);
        isdifflessMT_12 = meanrgb(diffmeanofmeansTop[vStripInd]) < 12;
        isdifflessMB_12 = meanrgb(diffmeanofmeansBottom[vStripInd]) < 12;
        isdifflessMP1P2_15 = fabs(meanrgb(meanofmeansPatch1[vStripInd]) - meanrgb(meanofmeansPatch2[vStripInd])) < 15;
        isdiffmoreMT_30 = meanrgb(diffmeanofmeansTop[vStripInd]) > 30;
        isdiffmoreMB_30 = meanrgb(diffmeanofmeansBottom[vStripInd]) > 30;
        isdifflessMTB_20 = fabs(meanrgb(meanofmeansTop[vStripInd]) - meanrgb(meanofmeansBottom[vStripInd])) < 20;
        isdifflessMT_15 = meanrgb(diffmeanofmeansTop[vStripInd]) < 15;
        isdifflessMB_15 = meanrgb(diffmeanofmeansBottom[vStripInd]) < 15;
        isdifflessMTB_25 = fabs(meanrgb(meanofmeansTop[vStripInd]) - meanrgb(meanofmeansBottom[vStripInd])) < 20;

        score[vStripInd]=0;
        // points also proportional to diffmean* and mean differences- also normalized
        // top continuity
        // 1
        if(islowVT && islowVP1 && isdifflessMT_12)
        {
            incval=meanrgb(diffmeanofmeansTop[vStripInd])*1.0/MAX_COMPRGB;
            score[vStripInd]+=4.0*(1-incval);
        }
        // bottom continuity
        // 2
        if(islowVB && islowVP2 && isdifflessMB_12)
        {
            incval = meanrgb(diffmeanofmeansBottom[vStripInd])*1.0/MAX_COMPRGB;
            score[vStripInd] += 4.0*(1-incval);
        }
        // vertical continuity throughout
        // 3
        if(islowVT && islowVP1 && islowVB&& islowVP2&& isdifflessMT_12  && isdifflessMB_12 && isdifflessMP1P2_15)
        {
            incval =fabs(meanrgb(meanofmeansPatch1[vStripInd])-meanrgb(meanofmeansPatch2[vStripInd]))*1.0 / MAX_COMPRGB;
            score[vStripInd] += 8.0*(1-incval);
        }
        // 4
        if(islowVT && islowVP1 && isdiffmoreMT_30)
        {
            incval=meanrgb(diffmeanofmeansTop[vStripInd])*1.0/MAX_COMPRGB;
            score[vStripInd] -= 4.0*incval;
        }
        // 5
        if(islowVB && islowVP2 && isdiffmoreMB_30){
            incval = meanrgb(diffmeanofmeansBottom[vStripInd])*1.0/MAX_COMPRGB;
            score[vStripInd] -= 4.0*incval;
        }
        // 6
        if(islowVT&&islowVB&&(isdiffmoreMB_30||isdiffmoreMT_30)&&isdifflessMTB_20)
        {
        	if(meanrgb(diffmeanofmeansTop[vStripInd]) > meanrgb(diffmeanofmeansBottom[vStripInd]))
        		incval=meanrgb(diffmeanofmeansTop[vStripInd])*1.0/MAX_COMPRGB;
        	else
        		incval=meanrgb(diffmeanofmeansBottom[vStripInd])*1.0/MAX_COMPRGB;
            
            if(islowVP1&&islowVP2)
                score[vStripInd] -= 10.0*incval;
            else if(islowVP1||islowVP2)
                score[vStripInd] -= 9.0*incval;
            else
                score[vStripInd] -= 8.0*incval;
        }
        // 7
        if((islowVT==0)&&isdifflessMT_15)
        {
            incval=meanrgb(diffmeanofmeansTop[vStripInd])*1.0/MAX_COMPRGB;
            score[vStripInd]+= 2.0*(1-incval);
        }
        // 8
        if(isdifflessMB_15 &&(islowVB==0))
        {
            incval=meanrgb(diffmeanofmeansBottom[vStripInd])*1.0/MAX_COMPRGB;
            score[vStripInd]+=2.0*(1-incval);
        }
        // 9
        if(islowVT==0 && islowVB==0  && isdifflessMB_15 && isdifflessMT_15 && isdifflessMTB_25)
        {
        	if(meanrgb(diffmeanofmeansTop[vStripInd]) < meanrgb(diffmeanofmeansBottom[vStripInd]))
        		incval=meanrgb(diffmeanofmeansTop[vStripInd])*1.0/MAX_COMPRGB;
        	else
        		incval=meanrgb(diffmeanofmeansBottom[vStripInd])*1.0/MAX_COMPRGB;
            score[vStripInd] += 2.0*(1-incval);
        }
        // 10
        if(islowVT==0 && islowVB==0 && islowVP1 && islowVP2 && isdiffmoreMT_30 && isdiffmoreMB_30)
        {
        	if(meanrgb(diffmeanofmeansTop[vStripInd]) > meanrgb(diffmeanofmeansBottom[vStripInd]))
        		incval=meanrgb(diffmeanofmeansTop[vStripInd])*1.0/MAX_COMPRGB;
        	else
        		incval=meanrgb(diffmeanofmeansBottom[vStripInd])*1.0/MAX_COMPRGB;
        	if(isdifflessMP1P2_15)
        	{
                incval2 = fabs(meanrgb(meanofmeansPatch1[vStripInd])-meanrgb(meanofmeansPatch2[vStripInd]))*1.0 / MAX_COMPRGB;
                score[vStripInd] -= 6.0*incval*(1-incval2);
            }
            else
                score[vStripInd] -= 4.0*incval;
        }

        k=0;
    }

    fin_score = 0.0;
    for(int int_i =0; int_i < vstripsnum; int_i++)
    {
        fin_score += score[int_i];
    }

    return fin_score;
}

double horTraversScore(PPMImage *img1, int cw1, int ch1){
    int threshSqrWidth=10;
    int part1=BRDR_WIDTH/threshSqrWidth;
    int part2=((PSL/2)/threshSqrWidth);
    int k;

    // across a single vertical traversal (column)
    PPMPixel_double meanTop[part1];
    PPMPixel_double meanPatch1[part2];
    PPMPixel_double meanPatch2[part2];
    PPMPixel_double meanBottom[part1];
    
	// each element for a single vertical; across all vertical strips, 1 array for 1 window
    int vstripsnum=PSL/threshSqrWidth;
    PPMPixel_double meanofmeansTop[vstripsnum];
    PPMPixel_double varofmeansTop[vstripsnum];
    PPMPixel_double meanofmeansBottom[vstripsnum];
    PPMPixel_double varofmeansBottom[vstripsnum];
    PPMPixel_double meanofmeansPatch1[vstripsnum];
    PPMPixel_double varofmeansPatch1[vstripsnum];
    PPMPixel_double meanofmeansPatch2[vstripsnum];
    PPMPixel_double varofmeansPatch2[vstripsnum];
    PPMPixel_double diffmeanofmeansTop[vstripsnum];
    PPMPixel_double diffmeanofmeansBottom[vstripsnum];

    double score[vstripsnum];
    double fin_score;
    double incval, incval2;
    bool islowVT, islowVP1, islowVP2, islowVB, isdifflessMT_12, isdifflessMB_12, isdifflessMP1P2_15, isdiffmoreMT_30, isdiffmoreMB_30, isdifflessMTB_20, isdifflessMT_15, isdifflessMB_15, isdifflessMTB_25;
    
    k=0;
    // initialize all means
    for(int k1=0; k1 < 2*(part1 + part2); k1++)
    {
        if(k1<part1)
        {
            meanTop[k1].red = 0;meanTop[k1].green = 0;meanTop[k1].blue = 0;
        }
        else if(k1 < part1+part2)
        {
        	int fu=k1-part1;
            meanPatch1[fu].red=0;meanPatch1[fu].green=0;meanPatch1[fu].blue=0;
        }
        else if(k1 < part1+2*part2)
        {
        	int fu1=k1-part1-part2;
            meanPatch2[fu1].red = 0;meanPatch2[fu1].green = 0;meanPatch2[fu1].blue = 0;
        }
        else
        {
        	int fu2=k1-part1-2*part2;
            meanBottom[fu2].red=0;meanBottom[fu2].green = 0;meanBottom[fu2].blue=0;
        }
    }
    // go horizontally: as if vertical looking at 90 degrees
    for(int i=ch1-PSL/2,vStripInd=0;i<ch1+PSL/2;i+=threshSqrWidth,vStripInd++) 
    {
        for (int j=cw1-(PSL/2)-BRDR_WIDTH;j<cw1+(PSL/2)+BRDR_WIDTH;j+=threshSqrWidth)
        {
            for(int si=i;si<=i+threshSqrWidth;si++)
            {
                for(int sj=j;sj<=j+threshSqrWidth;sj++)
                {
                    if(k<part1)
                    {
                        meanTop[k].red+=img1->data[si][sj].red;meanTop[k].green+=img1->data[si][sj].green;meanTop[k].blue+=img1->data[si][sj].blue;
                    }
                    else if(k<part1+part2)
                    {
                    	int fu=k-part1;
                        meanPatch1[fu].red +=img1->data[si][sj].red;meanPatch1[fu].green +=img1->data[si][sj].green;meanPatch1[fu].blue +=img1->data[si][sj].blue;
                    }
                    else if(k<part1+2*part2) 
                    {
                    	int fu2=k-part1-part2;
                        meanPatch2[fu2].red += img1->data[si][sj].red;meanPatch2[fu2].green += img1->data[si][sj].green;meanPatch2[fu2].blue += img1->data[si][sj].blue;
                    }
                    else
                    {
                    	int fu1=k-part1-2*part2;
                        meanBottom[fu1].red += img1->data[si][sj].red;meanBottom[fu1].green += img1->data[si][sj].green;meanBottom[fu1].blue += img1->data[si][sj].blue;
                    }
                }
            }
            k++;
        }

        meanofmeansTop[vStripInd].red=0; varofmeansTop[vStripInd].red = 0;
        meanofmeansTop[vStripInd].green=0;varofmeansTop[vStripInd].green = 0;
        meanofmeansTop[vStripInd].blue=0;varofmeansTop[vStripInd].blue = 0;
       
        meanofmeansBottom[vStripInd].red=0;varofmeansBottom[vStripInd].red = 0;
        meanofmeansBottom[vStripInd].green=0;varofmeansBottom[vStripInd].green = 0;
        meanofmeansBottom[vStripInd].blue=0;varofmeansBottom[vStripInd].blue = 0;
        
        meanofmeansPatch1[vStripInd].red=0;varofmeansPatch1[vStripInd].red = 0;
        meanofmeansPatch1[vStripInd].green=0;varofmeansPatch1[vStripInd].green = 0;
        meanofmeansPatch1[vStripInd].blue=0;varofmeansPatch1[vStripInd].blue = 0;
        
        meanofmeansPatch2[vStripInd].red=0;varofmeansPatch2[vStripInd].red = 0;
        meanofmeansPatch2[vStripInd].green=0;varofmeansPatch2[vStripInd].green = 0;
        meanofmeansPatch2[vStripInd].blue=0;varofmeansPatch2[vStripInd].blue = 0;
        
        // limits means' rgb's to (0-255)
        // variance varies from 0 to (255/2)^2 for even #elements and ( ((255^2)*n*(n+1)) / ((2n+1)^2) ) for (2n+1) elements
        int index;
        for(int k1=0;k1<2*(part1+part2);k1++)
        {
            if(k1<part1)
            {
            	int sqs=threshSqrWidth*threshSqrWidth;
                meanTop[k1].red /=(sqs*1.0);meanTop[k1].green /=(sqs*1.0);meanTop[k1].blue /=(sqs*1.0);
                // mean = (Summation_X)/N
                meanofmeansTop[vStripInd].red +=meanTop[k1].red;
                meanofmeansTop[vStripInd].green +=meanTop[k1].green;
                meanofmeansTop[vStripInd].blue +=meanTop[k1].blue;
                // variance = (Summation_X^2)/N - mean^2
                varofmeansTop[vStripInd].red +=meanTop[k1].red * meanTop[k1].red;
                varofmeansTop[vStripInd].green +=meanTop[k1].green * meanTop[k1].green;
                varofmeansTop[vStripInd].blue +=meanTop[k1].blue * meanTop[k1].blue;
            }
            else if(k1 < part1+part2)
            {
                index=k1-part1;
                int sqs=threshSqrWidth*threshSqrWidth;
                meanPatch1[index].red /=(sqs*1.0);meanPatch1[index].green /=(sqs*1.0);meanPatch1[index].blue /=(sqs*1.0);
                meanofmeansPatch1[vStripInd].red +=meanPatch1[index].red;
                meanofmeansPatch1[vStripInd].green +=meanPatch1[index].green;
                meanofmeansPatch1[vStripInd].blue +=meanPatch1[index].blue;
                varofmeansPatch1[vStripInd].red +=meanPatch1[index].red * meanPatch1[index].red;
                varofmeansPatch1[vStripInd].green +=meanPatch1[index].green * meanPatch1[index].green;
                varofmeansPatch1[vStripInd].blue +=meanPatch1[index].blue * meanPatch1[index].blue;
            }
            else if(k1 < part1 + 2*part2)
            {
                index = k1-part1-part2;
                int sqs=threshSqrWidth*threshSqrWidth;
                meanPatch2[index].red /= (sqs*1.0);meanPatch2[index].green /= (sqs*1.0);meanPatch2[index].blue /= (sqs*1.0);
                meanofmeansPatch2[vStripInd].red +=meanPatch2[index].red;
                meanofmeansPatch2[vStripInd].green +=meanPatch2[index].green;
                meanofmeansPatch2[vStripInd].blue +=meanPatch2[index].blue;
                varofmeansPatch2[vStripInd].red +=meanPatch2[index].red * meanPatch2[index].red;
                varofmeansPatch2[vStripInd].green +=meanPatch2[index].green * meanPatch2[index].green;
                varofmeansPatch2[vStripInd].blue +=meanPatch2[index].blue * meanPatch2[index].blue;
            }
            else
            {
                index=k1-part1-2*part2;
                int sqs=threshSqrWidth*threshSqrWidth;
                meanBottom[index].red /= (sqs*1.0);meanBottom[index].green /= (sqs*1.0);meanBottom[index].blue /= (sqs*1.0);
                meanofmeansBottom[vStripInd].red += meanBottom[index].red;
                meanofmeansBottom[vStripInd].green += meanBottom[index].green;
                meanofmeansBottom[vStripInd].blue += meanBottom[index].blue;
                varofmeansBottom[vStripInd].red += meanBottom[index].red*meanBottom[index].red;
                varofmeansBottom[vStripInd].green += meanBottom[index].green*meanBottom[index].green;
                varofmeansBottom[vStripInd].blue += meanBottom[index].blue*meanBottom[index].blue;
            }
        }

        // limits all the mean_mean*'s to (0-255)
        // diffmean's - (0-255). Then diffmeans will be normalized. So, finally (0,1)

        meanofmeansTop[vStripInd].red /= part1*1.0;
        meanofmeansTop[vStripInd].green /= part1*1.0;
        meanofmeansTop[vStripInd].blue /= part1*1.0;
        varofmeansTop[vStripInd].red = fabs(varofmeansTop[vStripInd].red/part1 - meanofmeansTop[vStripInd].red*meanofmeansTop[vStripInd].red);
        varofmeansTop[vStripInd].green = fabs(varofmeansTop[vStripInd].green/part1 - meanofmeansTop[vStripInd].green*meanofmeansTop[vStripInd].green);
        varofmeansTop[vStripInd].blue = fabs(varofmeansTop[vStripInd].blue/part1 - meanofmeansTop[vStripInd].blue*meanofmeansTop[vStripInd].blue);

        meanofmeansBottom[vStripInd].red /= part1*1.0;
        meanofmeansBottom[vStripInd].green /= part1*1.0;
        meanofmeansBottom[vStripInd].blue /= part1*1.0;
        varofmeansBottom[vStripInd].red = fabs(varofmeansBottom[vStripInd].red/part1 - meanofmeansBottom[vStripInd].red*meanofmeansBottom[vStripInd].red);
        varofmeansBottom[vStripInd].green = fabs(varofmeansBottom[vStripInd].green/part1 - meanofmeansBottom[vStripInd].green*meanofmeansBottom[vStripInd].green);
        varofmeansBottom[vStripInd].blue = fabs(varofmeansBottom[vStripInd].blue/part1 - meanofmeansBottom[vStripInd].blue*meanofmeansBottom[vStripInd].blue);

        meanofmeansPatch1[vStripInd].red /= part2*1.0;
        meanofmeansPatch1[vStripInd].green /= part2*1.0;
        meanofmeansPatch1[vStripInd].blue /= part2*1.0;
        diffmeanofmeansTop[vStripInd].red = fabs(meanofmeansTop[vStripInd].red-meanofmeansPatch1[vStripInd].red);
        diffmeanofmeansTop[vStripInd].green = fabs(meanofmeansTop[vStripInd].green-meanofmeansPatch1[vStripInd].green);
        diffmeanofmeansTop[vStripInd].blue = fabs(meanofmeansTop[vStripInd].blue-meanofmeansPatch1[vStripInd].blue);
        varofmeansPatch1[vStripInd].red = fabs(varofmeansPatch1[vStripInd].red/part2 - meanofmeansPatch1[vStripInd].red*meanofmeansPatch1[vStripInd].red);
        varofmeansPatch1[vStripInd].green = fabs(varofmeansPatch1[vStripInd].green/part2 - meanofmeansPatch1[vStripInd].green*meanofmeansPatch1[vStripInd].green);
        varofmeansPatch1[vStripInd].blue = fabs(varofmeansPatch1[vStripInd].blue/part2 - meanofmeansPatch1[vStripInd].blue*meanofmeansPatch1[vStripInd].blue);

        meanofmeansPatch2[vStripInd].red /= part2*1.0;
        meanofmeansPatch2[vStripInd].green /= part2*1.0;
        meanofmeansPatch2[vStripInd].blue /= part2*1.0;
        diffmeanofmeansBottom[vStripInd].red = fabs(meanofmeansBottom[vStripInd].red-meanofmeansPatch2[vStripInd].red);
        diffmeanofmeansBottom[vStripInd].green = fabs(meanofmeansBottom[vStripInd].green-meanofmeansPatch2[vStripInd].green);
        diffmeanofmeansBottom[vStripInd].blue = fabs(meanofmeansBottom[vStripInd].blue-meanofmeansPatch2[vStripInd].blue);
        varofmeansPatch2[vStripInd].red = fabs(varofmeansPatch2[vStripInd].red/part2 - meanofmeansPatch2[vStripInd].red*meanofmeansPatch2[vStripInd].red);
        varofmeansPatch2[vStripInd].green = fabs(varofmeansPatch2[vStripInd].green/part2 - meanofmeansPatch2[vStripInd].green*meanofmeansPatch2[vStripInd].green);
        varofmeansPatch2[vStripInd].blue = fabs(varofmeansPatch2[vStripInd].blue/part2 - meanofmeansPatch2[vStripInd].blue*meanofmeansPatch2[vStripInd].blue);

        // Calculated variances used for all cases and weightages: variance - each - acceptable value (for border strips and patch strips - threshold = 90)- below or above; each strip, consider each mean if variance is 'low'

        islowVT = islowVar(varofmeansTop[vStripInd], 90);
        islowVP1 = islowVar(varofmeansPatch1[vStripInd], 90);
        islowVP2 = islowVar(varofmeansPatch2[vStripInd], 90);
        islowVB = islowVar(varofmeansBottom[vStripInd], 90);
        isdifflessMT_12 = meanrgb(diffmeanofmeansTop[vStripInd]) < 12;
        isdifflessMB_12 = meanrgb(diffmeanofmeansBottom[vStripInd]) < 12;
        isdifflessMP1P2_15 = fabs(meanrgb(meanofmeansPatch1[vStripInd]) - meanrgb(meanofmeansPatch2[vStripInd])) < 15;
        isdiffmoreMT_30 = meanrgb(diffmeanofmeansTop[vStripInd]) > 30;
        isdiffmoreMB_30 = meanrgb(diffmeanofmeansBottom[vStripInd]) > 30;
        isdifflessMTB_20 = fabs(meanrgb(meanofmeansTop[vStripInd]) - meanrgb(meanofmeansBottom[vStripInd])) < 20;
        isdifflessMT_15 = meanrgb(diffmeanofmeansTop[vStripInd]) < 15;
        isdifflessMB_15 = meanrgb(diffmeanofmeansBottom[vStripInd]) < 15;
        isdifflessMTB_25 = fabs(meanrgb(meanofmeansTop[vStripInd]) - meanrgb(meanofmeansBottom[vStripInd])) < 20;

        score[vStripInd]=0;
        // points also proportional to diffmean* and mean differences- also normalized
        // top continuity
        // 1
        if(islowVT && islowVP1 && isdifflessMT_12)
        {
            incval = meanrgb(diffmeanofmeansTop[vStripInd])*1.0/MAX_COMPRGB;
            score[vStripInd] += 4.0 * (1 - incval);
        }
        // bottom continuity
        // 2
        if(islowVB && islowVP2 && isdifflessMB_12)
        {
            incval = meanrgb(diffmeanofmeansBottom[vStripInd])*1.0/MAX_COMPRGB;
            score[vStripInd] += 4.0 * (1 - incval);
        }
        // 3
        if(islowVT && islowVP1 && isdifflessMT_12 && islowVB && islowVP2 && isdifflessMB_12 && isdifflessMP1P2_15)
        {
            incval =fabs(meanrgb(meanofmeansPatch1[vStripInd]) - meanrgb(meanofmeansPatch2[vStripInd]))*1.0 / MAX_COMPRGB;
            score[vStripInd] += 8.0 * (1 - incval); //POINTS_TP1P2B;
        }
        // 4
        if(islowVT && islowVP1 && isdiffmoreMT_30)
        {
            incval = meanrgb(diffmeanofmeansTop[vStripInd])*1.0/MAX_COMPRGB;
            score[vStripInd] -= 4.0*incval;
        }
        // 5
        if(islowVB && islowVP2 && isdiffmoreMB_30)
        {
            incval = meanrgb(diffmeanofmeansBottom[vStripInd])*1.0/MAX_COMPRGB;
            score[vStripInd] -= 4.0*incval;
        }
        // 6
        if(islowVT && islowVB && (isdiffmoreMT_30 || isdiffmoreMB_30) && isdifflessMTB_20)
        {
        	if(meanrgb(diffmeanofmeansTop[vStripInd]) > meanrgb(diffmeanofmeansBottom[vStripInd]))
        		incval=meanrgb(diffmeanofmeansTop[vStripInd])*1.0/MAX_COMPRGB;
        	else
        		incval=meanrgb(diffmeanofmeansBottom[vStripInd])*1.0/MAX_COMPRGB;
            if(islowVP1 && islowVP2)
                score[vStripInd] -= 10.0*incval;
            
            else if(islowVP1 || islowVP2)
                score[vStripInd] -= 9.0*incval;
            
            else
                score[vStripInd] -= 8.0*incval;
            
        }
        // 7
        if(!islowVT && isdifflessMT_15)
        {
            incval = meanrgb(diffmeanofmeansTop[vStripInd])*1.0/MAX_COMPRGB;
            score[vStripInd] += 2.0*(1-incval);
        }
        // 8
        if(!islowVB && isdifflessMB_15)
        {
            incval = meanrgb(diffmeanofmeansBottom[vStripInd])*1.0/MAX_COMPRGB;
            score[vStripInd] += 2.0*(1-incval);
        }
        // 9
        if(!islowVT && !islowVB && isdifflessMTB_25 && isdifflessMT_15 && isdifflessMB_15)
        {
        	if(meanrgb(diffmeanofmeansTop[vStripInd]) < meanrgb(diffmeanofmeansBottom[vStripInd]))
        		incval=meanrgb(diffmeanofmeansTop[vStripInd])*1.0/MAX_COMPRGB;
        	else
        		incval=meanrgb(diffmeanofmeansBottom[vStripInd])*1.0/MAX_COMPRGB;
            score[vStripInd] += 2.0*(1-incval);
        }
        // 10
        if(!islowVT && !islowVB && islowVP1 && islowVP2 && isdiffmoreMT_30 && isdiffmoreMB_30)
        {
        	if(meanrgb(diffmeanofmeansTop[vStripInd]) > meanrgb(diffmeanofmeansBottom[vStripInd]))
        		incval=meanrgb(diffmeanofmeansTop[vStripInd])*1.0/MAX_COMPRGB;
        	else
        		incval=meanrgb(diffmeanofmeansBottom[vStripInd])*1.0/MAX_COMPRGB;
            if(isdifflessMP1P2_15)
            {
                incval2 = fabs(meanrgb(meanofmeansPatch1[vStripInd]) - meanrgb(meanofmeansPatch2[vStripInd]))*1.0 / MAX_COMPRGB;
                score[vStripInd] -= 6.0*incval*(1-incval2);
            }
            else
                score[vStripInd] -= 4.0*incval;
        }

        k=0;
    }

    fin_score = 0.0;
    for(int int_i =0; int_i < vstripsnum; int_i++)
    {
        fin_score += score[int_i];
    }

    return fin_score;
}

void finTraversCheck(PPMImage *img1, string f_name[], int n, int cw1, int ch1, answer &ans)
{
	double scoreMax=-100.0;
    double vertTraversScore;
    double horiTraversScore;
    double finTScore;
	ifstream fin;
    char buf[200];
	PPMImage *img2;
    int cw2, ch2;
	int margin=PSL/2;
    string tempOutf_name;
	for (int imgIndex = 0; imgIndex < n; imgIndex++) 
    {
    	cout<<'\n';
        cout<<"Processing patches in "<< f_name[imgIndex].c_str()<<'\n';
        fin.open(f_name[imgIndex].c_str());
        if (!fin.good()) 
        {
            fprintf(stderr, "Error in opening file '%s' !!\n", f_name[imgIndex].c_str());
            exit(1);
        }
		if (imgIndex !=0) 
        {
            buf[0]='a';   //so that by default fails the correct format
			// Check for comments
            while ('#' == fin.peek()) 
            {
                fin.getline(buf, 200);
            }

            // Check file format
            fin >> buf;
            if (!(toupper(buf[0]) == 'P' && buf[1] == '3')) 
            {
                fprintf(stderr, "This is an invalid format! %s here should be P3 instead in %s\n", buf, f_name[imgIndex].c_str());
                exit(1);
            }
        }
        fin.close();

        img2=readPPM_P3(f_name[imgIndex].c_str());
        cw2=margin;
        ch2=margin;
        string tempname;
        while(ch2 < 720-margin)
        {
            while(cw2 < 1280-margin)
            {
                if(BASE_SOURCE_FILE_INDEX == imgIndex && (ch2<=ch1+PSL && ch2 >= ch1-PSL && cw2>=cw1-PSL && cw2<=cw1+PSL))
                {

                    // for nftyp window
                    cw2 += 5;
                    continue;
                }
                // First, add/fit new image patch into hole

                addPatch(img1, cw1, ch1, img2, cw2, ch2);
                vertTraversScore = vertTraversalScore(img1, cw1, ch1);
                horiTraversScore = horTraversScore(img1, cw1, ch1);
                if(horiTraversScore < vertTraversScore)
                	finTScore = vertTraversScore;
                else
                    finTScore = 0.7*vertTraversScore + 0.3*horiTraversScore;

                // has a higher score than the previous highest scorer in combined continuity
                if(scoreMax < finTScore)
               	{
                    scoreMax=finTScore;
                    ans.ch=ch2;ans.cw=cw2;
                    ans.f_nameFinal = f_name[imgIndex];

                    for(int i0=ch1-PSL/2, i1= ans.ch-PSL/2; i0<= ch1+PSL/2; i0++, i1++) 
                    { 
                        for (int j0=cw1-PSL/2, j1=ans.cw-PSL/2; j0<= cw1+PSL/2; j0++,j1++) 
                        {
                            img1->data[i0][j0].red = img2->data[i1][j1].red;img1->data[i0][j0].green = img2->data[i1][j1].green;img1->data[i0][j0].blue = img2->data[i1][j1].blue;
                        }
                    }
                }cw2=cw2+5;
            }
            cw2=margin;
            ch2=ch2+5;
        }
    }
}

int main() 
{

    int n;
    cout<<"Enter the value of number of source files: ";//first file should be the base file for creating the patch
    cin>>n;
    getchar();
    string f_name[n];
    cout<<"Enter the file names with proper ftypensions:"<<'\n';
    for (int i = 0;i < n;i++) 
        getline(cin, f_name[i]);

    // To Convert all source files into required p3 ppm image file
    string ftyp, cmd;
    ifstream fin;
    char buf[100];
    int rm[n];
    for(int i=0; i<n; i++)
    {
        size_t ind=f_name[i].rfind('.', f_name[i].length());
        //cout<<"index of . is :"<<ind<<'\n';
        if (ind!=string::npos) 
        {
            ftyp=f_name[i].substr(f_name[i].find_last_of('.')+1);
            //cout<<"ftyp of the f_name is:"<<ftyp<<'\n';
        }
        else
        {
            cout<<"This is an invalid image!"<<'\n';
            return 1;
        }
        if(strcmp(ftyp.c_str(), "png")==0 || strcmp(ftyp.c_str(), "jpeg")==0 || strcmp(ftyp.c_str(), "jpg")==0)
        {
           cmd = "convert "+f_name[i]+" -compress none ";
            f_name[i]=f_name[i].substr(0, f_name[i].rfind('.'))+".ppm";
            cmd=cmd+ f_name[i];
            system(cmd.c_str());
            rm[i]=1;
        }
        else if(strcmp(ftyp.c_str(),"ppm")==0)
        {
            fin.open(f_name[i].c_str());
            if (fin.good()==0) 
            {
                fprintf(stderr, "Error in opening input file '%s' !! \n", f_name[0].c_str());
                fin.close();
                exit(1);
            }

            // Check for comments
            while ('#' == fin.peek()) 
            {
                fin.getline(buf, 100);
            }

            fin >> buf;
            if (buf[0] == 'P' && buf[1] == '6') 
            {
                cmd = "convert "+f_name[i]+" -compress none ";
                f_name[i]=f_name[i].substr(0, f_name[i].rfind('.'))+"_p3.ppm";
                cmd=cmd+ f_name[i];
                system(cmd.c_str());
                rm[i]=1;
            }
            else if (buf[0] == 'P' && buf[1] == '3') 
                rm[i]=0;
            else
            {
                cout << f_name[i] << "- invalid format !" <<'\n';
                 return 1;
            }
            fin.close();
        }
        else
        {
            cout << f_name[i] << "- format not allowed !" <<'\n';
            return 1;
        }
    }
    
    // To check for the validity of the input images
    for(int i=0; i < n; i++)
    {
        chk_readPPM_P3(f_name[i].c_str());
    }

    PPMImage *image;
    cout<<'\n';
    string src_file=f_name[0].c_str();
    cout<<"Image file in which the patch is to be created : "<<src_file<<'\n';

    // Read the base source file
    image = readPPM_P3(f_name[0].c_str());
    int margin=(2*BRDR_WIDTH + PSL)/2;  // 100

    int cw, ch;
    int badCenter=0;
    do 
    {
    	cout<<'\n';
	    cout<<"Centre the patch such that centre of 120 X 120 square patch is (j, i) where j >= "<< margin<<", j <= "<< 1280 - margin<<", i >= "<<margin<<", i <= "<<720-margin<<'\n';
	    cin>>cw;
	    cin>>ch;     
    
        if (((cw < margin) || (cw > 1280 - margin) || (ch < margin) || (ch > 720 - margin)) )
        {
            cout<<"This is a bad centre position !"<<'\n';
            badCenter = 1;
        }
        else
            badCenter = 0;
    }while(badCenter);

    blacken(image, cw, ch, PSL); // Remove 120 X 120 square around (cw, ch)

    string tempname="noPatch.ppm";
    writePPM_P3(tempname.c_str(), image);

    cmd = "convert " + tempname + " ";
    string finaltempname = tempname.substr(0, tempname.rfind('.'))+".jpg";
    cmd += finaltempname;
    system(cmd.c_str());
    answer ans;
    cmd = "rm " + tempname;
    system(cmd.c_str());
    cout<<'\n';
    cout << "See the file noPatch.jpg to view the patch location in the image\n";
    
    finTraversCheck(image, f_name, n, cw, ch, ans);
    PPMImage *img2 = readPPM_P3(ans.f_nameFinal.c_str());
    for(int i0=ch-PSL/2, i1 = ans.ch-PSL/2;i0<=ch+PSL/2;i0++,i1++)
    {
        for(int j0 = cw-PSL/2, j1 = ans.cw-PSL/2; j0 <= cw+PSL/2; j0++, j1++)
        {
            image->data[i0][j0].red= img2->data[i1][j1].red;image->data[i0][j0].green= img2->data[i1][j1].green;image->data[i0][j0].blue= img2->data[i1][j1].blue;
        }
    }
    tempname = "finale.ppm";
    writePPM_P3(tempname.c_str(), image);

    cmd = "convert " + tempname + " ";
    finaltempname = tempname.substr(0, tempname.rfind('.'))+".jpg";
    cmd += finaltempname;
    system(cmd.c_str());

    cmd = "rm " + tempname;
    system(cmd.c_str());
    cout<<'\n';
    cout <<"The final output image 'finale.jpg' showing the filled patch is made\n";
    
    for(int i=0; i < n; i++)
    {
        if(rm[i])
        {
            cmd="rm "+f_name[i];
            system(cmd.c_str());
        }
    }
    
    return 0;
}
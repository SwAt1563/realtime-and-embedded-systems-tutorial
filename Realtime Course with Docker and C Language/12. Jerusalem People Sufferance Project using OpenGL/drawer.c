
#include "header.h"
#include "processing_times.h"
#include "shared_memories.h"
#include "person_header.h"
#include "message_queues.h"


// functions for openGl
void display(void);
void reshape(int w, int h);
void timer(int z);
void drawReceptionHall();
void drawEnterOfQueues();
void drawQueue();
void drawWattingHall();
void drawMetalDetector();
void drawChairs();
void drawTellers();
void drawText();
void renderText(int, int, void *, char *);
void readDataFromSharedMemory();

// parameter that you want to change by reading from shared memory
int peopleInTheInnerHall = 0,
    maleQueueCount = 0,
    femaleQueueCount = 0,
    receptionHallMaleCount = 0,
    receptionHallFemaleCount = 0,
    birthCertificateCnt = 0,
    travelDocumentCnt = 0,
    familyReunionCnt = 0,
    idRelatedCnt = 0,
    unservedCnt=0,
    unhappyCnt=0,
    satisfiedCnt=0,
    fram_rate = 1000 / 120;



char *drawer_current_time = "";


int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutInitWindowSize(1800, 1000);
    glutCreateWindow("OIM");
    glLineWidth(5);
    glutReshapeFunc(reshape);
    glutDisplayFunc(display);

    glutTimerFunc(0, timer, 0);
    glutMainLoop();
    return 0;
}
void display(void)
{
    // glClearColor(1.0, 1.0, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3ub(255, 0, 255);

    drawReceptionHall();
    drawEnterOfQueues();
    drawQueue();
    drawWattingHall();
    drawMetalDetector();
    drawChairs();
    drawTellers();
    drawText();
    glutSwapBuffers();
}
void reshape(int w, int h)
{
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-20, 20, -20, 20);
    glMatrixMode(GL_MODELVIEW);
}
void timer(int z)
{
    glutTimerFunc(fram_rate, timer, 0);
    glutPostRedisplay();
    readDataFromSharedMemory();
}
void drawReceptionHall()
{
    glBegin(GL_LINES);
    glVertex2f(-20.0, -13.0);
    glVertex2f(-10.0, -13.0);
    glEnd();

    glBegin(GL_LINES);
    glVertex2f(-20.0, 13.0);
    glVertex2f(-10.0, 13.0);
    glEnd();

    glBegin(GL_LINES);
    glVertex2f(-10.0, -13.0);
    glVertex2f(-10.0, 13.0);
    glEnd();
    glFlush();
}

void drawEnterOfQueues()
{
    glPointSize(20);
    glBegin(GL_POINTS);
    glColor3f(0.5, 0, 0);
    glVertex2f(-10.0, -6.0);
    glEnd();

    glPointSize(20);
    glBegin(GL_POINTS);
    glColor3f(0.5, 0, 0);
    glVertex2f(-10.0, 6.0);
    glEnd();
    glFlush();
}
void drawQueue()
{
    glBegin(GL_LINES);
    glVertex2f(-10.0, 8.0);
    glVertex2f(-2.0, 8.0);
    glEnd();

    glBegin(GL_LINES);
    glVertex2f(-10.0, 4.0);
    glVertex2f(-2.0, 4.0);
    glEnd();

    glBegin(GL_LINES);
    glVertex2f(-10.0, -8.0);
    glVertex2f(-2.0, -8.0);
    glEnd();

    glBegin(GL_LINES);
    glVertex2f(-10.0, -4.0);
    glVertex2f(-2.0, -4.0);
    glEnd();
    glFlush();
}

void drawWattingHall()
{
    glColor3ub(255, 0, 255);
    glBegin(GL_LINES);
    glVertex2f(-2.0, -13.0);
    glVertex2f(-2.0, 13.0);
    glEnd();

    glBegin(GL_LINES);
    glVertex2f(-2.0, 13.0);
    glVertex2f(20.0, 13.0);
    glEnd();

    glBegin(GL_LINES);
    glVertex2f(-2.0, -13.0);
    glVertex2f(20.0, -13.0);
    glEnd();
    glFlush();
}
void drawMetalDetector()
{
    glPointSize(20);
    glBegin(GL_POINTS);
    glColor3f(0.5, 0.5, 0.5);
    glVertex2f(-2.0, -6.0);
    glEnd();

    glPointSize(20);
    glBegin(GL_POINTS);
    glColor3f(0.5, 0.5, 0.5);
    glVertex2f(-2.0, 6.0);
    glEnd();
    glFlush();
}

// y : -9-9 ,,,, x : 0-8
void drawChairs()
{

    int peopleCount = 0, jTemp = 0, iTemp = -9;

    glPointSize(5);
    glBegin(GL_POINTS);
    glColor3f(1.0, 0.1, 0.1);
    for (int i = 8; i > -1; i--)
    {
        iTemp = i;
        int f = 0;
        for (int j = -9; j < 8; j++)
        {
            jTemp = j;
            glVertex2i(i, j);
            peopleCount++;
            if (peopleCount == peopleInTheInnerHall)
            {
                f = 1;
                break;
            }
        }
        if (f)
            break;
    }
    glEnd();



    glPointSize(5);
    glBegin(GL_POINTS);
    glColor3f(0.0, 0.0, 1.0);
    for (int i = iTemp; i > -1; i--)
    {

        for (int j = jTemp + 1; j < 8; j++)
        {
            glVertex2i(i, j);
        }
        jTemp = -10;
    }
    glEnd();
    glFlush();
}
void drawTellers()
{
    glBegin(GL_POLYGON_BIT);
    glColor3f(0.1, 0.9, 0.4);
    glVertex2i(20, 12);
    glVertex2i(12, 12);
    glVertex2i(20, 6);
    glVertex2i(12, 6);
    glEnd();

    glBegin(GL_POLYGON_BIT);
    glColor3f(0.6, 0.4, 0.9);
    glVertex2i(20, 6);
    glVertex2i(12, 6);
    glVertex2i(20, 0);
    glVertex2i(12, 0);
    glEnd();

    glBegin(GL_POLYGON_BIT);
    glColor3f(0.3, 0.8, 0.2);
    glVertex2i(20, 0);
    glVertex2i(12, 0);
    glVertex2i(20, -6);
    glVertex2i(12, -6);
    glEnd();

    glBegin(GL_POLYGON_BIT);
    glColor3f(0.356, 0.514, 0.999);
    glVertex2i(20, -6);
    glVertex2i(12, -6);
    glVertex2i(20, -12);
    glVertex2i(12, -12);
    glEnd();
}

void drawText()
{
    glColor3f(1.0, 0.8, 0.5);
    char
        reseptionHallMale[30] = "males : ",
        reseptionHallFemale[30] = "females : ",
        maleQueue[30] = "male queue : ",
        femaleQueue[30] = "female queue : ",
        birthCirtifecate[30] = "birth certificate : ",
        travelDocument[30] = "travel document : ",
        familyReunion[30] = "family reunion : ",
        idRelated[30] = "ID-related : ",
        unserved[30]="unserved : ",
        unhappy[30]="unhappy : ",
        satisfied[30]="satisfied : ",
        numberOfPeopleInMaleQueue[5] = "",
        numberOfPeopleInFemalQueue[5] = "",
        numberOfMaleInTheReceptionHall[5] = "",
        numberOfFemaleInTheReceptionHall[5] = "",
        numberOfBusyTellersForBirthCertificate[3]="",
        numberOfBusyTellersForTravelDocument[3]="",
        numberOfBusyTellersForFamilyReunion[3]="",
        numberOfBusyTellersForIdRelated[3]="",
        numberOfUnserved[3]="",
        numberOfUnhappy[3]="",
        numberOfSatisfied[3]="";

    sprintf(numberOfPeopleInMaleQueue, "%d", maleQueueCount);
    sprintf(numberOfPeopleInFemalQueue, "%d", femaleQueueCount);
    sprintf(numberOfFemaleInTheReceptionHall, "%d", receptionHallFemaleCount);
    sprintf(numberOfMaleInTheReceptionHall, "%d", receptionHallMaleCount);

    sprintf(numberOfBusyTellersForBirthCertificate, "%d", birthCertificateCnt);
    sprintf(numberOfBusyTellersForTravelDocument, "%d", travelDocumentCnt);
    sprintf(numberOfBusyTellersForFamilyReunion, "%d", familyReunionCnt);
    sprintf(numberOfBusyTellersForIdRelated, "%d", idRelatedCnt);

    sprintf(numberOfUnserved, "%d", unservedCnt);
    sprintf(numberOfUnhappy, "%d", unhappyCnt);
    sprintf(numberOfSatisfied, "%d", satisfiedCnt);


    strcat(maleQueue, numberOfPeopleInMaleQueue);
    strcat(femaleQueue, numberOfPeopleInFemalQueue);
    strcat(reseptionHallMale, numberOfMaleInTheReceptionHall);
    strcat(reseptionHallFemale, numberOfFemaleInTheReceptionHall);

    strcat(birthCirtifecate, numberOfBusyTellersForBirthCertificate);
    strcat(travelDocument, numberOfBusyTellersForTravelDocument);
    strcat(familyReunion, numberOfBusyTellersForFamilyReunion);
    strcat(idRelated, numberOfBusyTellersForIdRelated);

    strcat(unserved,numberOfUnserved);
    strcat(unhappy, numberOfUnhappy);
    strcat(satisfied, numberOfSatisfied);

    renderText(-8, 6, GLUT_BITMAP_TIMES_ROMAN_24, maleQueue);
    renderText(-8, -6, GLUT_BITMAP_TIMES_ROMAN_24, femaleQueue);
    renderText(-18, 2, GLUT_BITMAP_TIMES_ROMAN_24, reseptionHallMale);
    renderText(-18, -2, GLUT_BITMAP_TIMES_ROMAN_24, reseptionHallFemale);
    renderText(14, 9, GLUT_BITMAP_TIMES_ROMAN_24, birthCirtifecate);
    renderText(14, 3, GLUT_BITMAP_TIMES_ROMAN_24, travelDocument);
    renderText(14, -3, GLUT_BITMAP_TIMES_ROMAN_24, familyReunion);
    renderText(14, -9, GLUT_BITMAP_TIMES_ROMAN_24, idRelated);
    renderText(13, 18, GLUT_BITMAP_TIMES_ROMAN_24, drawer_current_time);
    renderText(-8, 18, GLUT_BITMAP_TIMES_ROMAN_24, unserved);
    renderText(-8, 15, GLUT_BITMAP_TIMES_ROMAN_24, unhappy);
    renderText(-8, 12, GLUT_BITMAP_TIMES_ROMAN_24, satisfied);
}

void renderText(int x, int y, void *font, char *string)
{
    glRasterPos2f(x, y);
    int len, i;
    len = strlen(string);
    for (i = 0; i < len && string[i] != '\0'; i++)
    {
        glutBitmapCharacter(font, string[i]);
    }
}

// read data from shared memory in this function
void readDataFromSharedMemory()
{


    peopleInTheInnerHall = read_queue_size(INSIDE, MALE);
    maleQueueCount = read_queue_size(MIDDLE, MALE);
    femaleQueueCount = read_queue_size(MIDDLE, FEMALE);
    receptionHallMaleCount = read_queue_size(OUTSIDE, MALE) ;
    receptionHallFemaleCount =read_queue_size(OUTSIDE, FEMALE);
    birthCertificateCnt = read_tellers_numbers(B);
    travelDocumentCnt = read_tellers_numbers(T);
    familyReunionCnt = read_tellers_numbers(R);
    idRelatedCnt = read_tellers_numbers(I);
    drawer_current_time = time_to_string();
    unhappyCnt= read_ends(UNHAPPY);
    unservedCnt= read_ends(UNSERVED);
    satisfiedCnt= read_ends(SATISFIED);


}
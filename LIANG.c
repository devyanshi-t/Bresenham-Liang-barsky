#include<stdio.h>
#include<GL/gl.h>
#include<GL/glut.h>
#include<string.h>
#include<math.h>
/* Function that returns -1,0,1 depending on whether x */
/* is <0, =0, >0 respectively */
#define sign(x) ((x>0)?1:((x<0)?-1:0))
int ww = 640, wh = 480;
int first = 0;
int xi, yi, xf, yf;
int count=0;
int xmin=-50,ymin=-50,xmax=50,ymax=50;//view port dimensions

void setPixel(GLint x, GLint y) {
    glBegin(GL_POINTS);
    glVertex2i(x,y);
    glEnd();
}
void breshmans(int x1, int y1, int x2, int y2)
{
    int dx,dy,Po;
    int k=0;
    dx=(x2-x1);
    dy=(y2-y1);
    if(dy<=dx&&dy>0)
    {
        dx=abs(dx);
        dy=abs(dy);
        Po=(2*dy)-dx;
        setPixel(x1,y1);
        int xk=x1;
        int yk=y1;
        for(k=x1;k<x2;k++)
        {
            if(Po<0)
            {
                setPixel(++xk,yk);
                Po=Po+(2*dy);
            }
            else{
                setPixel(++xk,++yk);
                Po=Po+(2*dy)-(2*dx);
            }
        }
    }
    else if(dy>dx&&dy>0)
    {
        dx=abs(dx);
        dy=abs(dy);
        Po=(2*dx)-dy;
        setPixel(x1,y1);
        int xk=x1; int yk=y1;
        for(k=y1;k<y2;k++)
        {
            if(Po<0)
            {
                setPixel(xk,++yk);
                Po=Po+(2*dx);
            }
            else{
                setPixel(++xk,++yk);
                Po=Po+(2*dx)-(2*dy);
            }
        }
    }
    else if(dy>=-dx)
    {
        dx=abs(dx);
        dy=abs(dy);
        Po=(2*dy)-dx;
        setPixel(x1,y1);
        int xk=x1;
        int yk=y1;
        for(k=x1;k<x2;k++)
        {
            if(Po<0)
            {
                setPixel(++xk,yk);
                Po=Po+(2*dy);
            }
            else{
                setPixel(++xk,--yk);
                Po=Po+(2*dy)-(2*dx);
            }
        }
    }
    else if(dy<-dx)
    {
        dx=abs(dx);
        dy=abs(dy);
        Po=(2*dy)-dx;
        setPixel(x1,y1);
        int xk=x1;
        int yk=y1;
        for(k=y1;k>y2;k--)
        {
            if(Po<0)
            {
                setPixel(xk,--yk);
                Po=Po+(2*dx);
            }
            else{
                setPixel(++xk,--yk);
                Po=Po+(2*dx)-(2*dy);
            }
        }
    }
    glFlush();
}

int test(float p,float q,float *t1,float *t2)
{
    float r;
    int ret = true;
    
    if(p<0.0){
        r=q/p;
        if(r>*t2)
            ret= false;
        else if(r>*t1)
        {*t1 = r;}
    }
    
    else if(p>0.0){
        r= q/p;
        if(r<*t1)
            ret = false;
        else if(r<*t2)
        {*t2 = r;}
    }
    
    else
    {
        if(q<0.0)
        {ret=false;
        }
    }
    return ret;
    
}

void lineclip(int xmin,int ymin,int xmax,int ymax, int x1,int y1,int x2,int y2)
{
    float t1=0.0,t2=1.0,dx=x2-x1;
    //intilaizeP( dx, dy);
    //intilaizeQ(x1,y1);
    
    if(test(-dx,x1-xmin,&t1,&t2))
        if(test(dx,xmax-x1,&t1,&t2)){
            float dy=y2-y1;
            if(test(-dy,y1-ymin,&t1,&t2))
                if(test(dy,ymax-y1,&t1,&t2)){
                    if(t2<1.0)
                    {
                        x2=x1+t2*dx;
                        y2=y1+t2*dy;
                    }
                    if(t1>0.0){
                        x1=x1+t1*dx;
                        y1=y1+t1*dy;
                    }
                    glColor3f(0.0,1.0,0.0);
                    breshmans(x1,y1,x2,y2);
                }
        }
}



int X1, Y1, X2, Y2;

void *font = GLUT_BITMAP_TIMES_ROMAN_10;
void *fonts[] = {
    GLUT_BITMAP_9_BY_15,
    GLUT_BITMAP_TIMES_ROMAN_10,
    GLUT_BITMAP_TIMES_ROMAN_24
};

void show(int x, int y, char *string){
    int len, i;
    glRasterPos2f(x, y);
    len = (int) strlen(string);
    for (i = 0; i < len; i++)
        glutBitmapCharacter(font, string[i]);
}

void draw(void)
{
    glClear(GL_COLOR_BUFFER_BIT);
    int i;
    for(i=-320;i<320;i=i+10)
    {
        setPixel(i,0);
        setPixel(0,i);
    }
    
    
    glFlush();
}
int k;
void Keypress(unsigned char key,int x,int y)
{
    glClear(GL_COLOR_BUFFER_BIT);
    if(key=='c')
    {
        glClear(GL_COLOR_BUFFER_BIT);
        glFlush();
        glColor3f(1.0,0.0,0.0);
        glBegin(GL_LINES);
        glVertex2i(-50,-50);
        glVertex2i(-50,50);
        glVertex2i(-50,50);
        glVertex2i(50,50);
        glVertex2i(50,50);
        glVertex2i(50,-50);
        glVertex2i(50,-50);
        glVertex2i(-50,-50);
        glEnd();
        glFlush();
        
        lineclip(xmin,ymin,xmax,ymax,xi,yi,xf,yf);
        
    }
    
    
}
void mouse(int btn, int state, int x, int y)
{
    if(btn==GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        char initial[10];
        char final[10];
        switch(first)
        {
            case 0:
                xi = x-320;
                yi = 240-y;
                first = 1;
                sprintf(initial,"X=%d,Y=%d",xi,yi);
                show(xi+5,yi,initial);
                break;
            case 1:
                xf = x-320;
                yf = 240-y;
                sprintf(final,"X=%d,Y=%d",xf,yf);
                show(xf+5,yf,final);
                if(xi>xf)
                {
                    int temp=xi;
                    xi=xf;
                    xf=temp;
                    temp=yi;
                    yi=yf;
                    yf=temp;
                }
                
                breshmans( xi, yi, xf, yf);
                first = 0;
                break;
        }
    }
}

void init() {
    glViewport(-320,0,320,0);
    glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB);
    glutInitWindowPosition(0,0);
    glutInitWindowSize(ww, wh);
    glutCreateWindow("assignment 2 ");
    glClearColor(0.0,0.0,0.0,0);
    glColor3f(1.0,1.0,1.0);
    gluOrtho2D(-320,320,-240,240);
    
}
int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    init();
    glutDisplayFunc(draw);
    glutMouseFunc(mouse);
    glutKeyboardFunc(Keypress);
    glutMainLoop();
    return 0;
}


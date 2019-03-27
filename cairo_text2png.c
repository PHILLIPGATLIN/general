#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>
#include <X11/keysym.h>
#include <X11/cursorfont.h>
#include <cairo/cairo.h>
#include <cairo/cairo-xlib.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

cairo_surface_t* cairo_image = 0;
cairo_t* cairo_image_context = 0;


//FILE* caption_log;

enum
{
    alignment_leftright = 0,
    alignment_centerright,
    alignment_rightright,
    alignment_leftcenter,
    alignment_centercenter,
    alignment_rightcenter,
    alignment_leftleft,
    alignment_centerleft,
    alignment_rightleft
};

char* alignment_names[9] =   {  "left_right",
                                "center_right",
                                "right_right",
                                "left_center",
                                "center_center",
                                "right_center",
                                "left_left",
                                "center_left",
                                "right_left"};
                                
typedef struct
{
    int     flag_parameter_image;
    int     flag_parameter_fonttype;
    int     flag_parameter_fontscale;
    int     flag_parameter_sourcecolor_red;
    int     flag_parameter_sourcecolor_green;
    int     flag_parameter_sourcecolor_blue;
    int     flag_parameter_sourcecolor_alpha;
    int     flag_parameter_arbitraryposition_X;
    int     flag_parameter_arbitraryposition_Y;
    int     flag_parameter_text_alignment;
    char*   parameter_fonttype;
    double  parameter_fontscale;
    double  parameter_sourcecolor_red;
    double  parameter_sourcecolor_green;
    double  parameter_sourcecolor_blue;
    double  parameter_sourcecolor_alpha;
    double  parameter_arbitraryposition_X;
    double  parameter_arbitraryposition_Y;
    int     parameter_text_alignment;
}parameters;

void end(int);
void end(int error)
{
    switch (error)
    {
        case -1:
            printf("Insufficient number of paramaeters. This program requires at least text input, contained in double quotes, to execute.\n");
            exit(-1);
        case -2:
            printf("A connection to the X server was not able to be established!\n");
            exit(-2);
        default:
            printf("caption executed and terminated without errors.\n");
            exit(0);
    }
}

void fstrpper(char*);
void fstrpper(char* fnme)
{
    int index,index2;
    char tmp[255];
    char c;
    strcpy(tmp,fnme);
    memset(fnme,0,strlen(fnme));
    
    for (index = 0,index2 = 0;(c = tmp[index]) != '\0';index++)
    {
        if (!isalnum(c))
            continue;
        else
            *(fnme+(index2++)) = c;
    }
    return;
}

char* argcpy(int,char*[],char*);
char* argcpy(int arglim,char* arvecs[],char*argray)
{
    char c;
    int index,index2,index3;
    
    for (index = 0, index2 = 0, index3 = 0; index < arglim; index++)
    {
        while ((c = *(arvecs[index]+index2)) != '\0') *(argray+(index3++)) = *((arvecs[index])+(index2++));
        *(argray+(index3++)) = '\0';
        index2 = index2^index2;
    }
    return argray;
}

void parse(int,char*[],parameters*);
void parse(int argc,char* argv[],parameters* options)
{
    char c = 0;
    char argtoken = 0;
    char* argstream = 0;
    int txtal_index = 0;
    int index = 0;
    size_t arraysize = 0;
    
    //"caption 'lmao' -r 255 -g 255 -b 255 -l center-center -f monospace -S 100"
    
    for (index = 0; index < argc; index++) arraysize += strlen(argv[index]) + 1;
    argstream = calloc(arraysize,sizeof(c));
    argcpy(argc,argv,argstream);
    
    //fprintf(caption_log,"line 130:\n");
    //for (index = 0; index < arraysize; index++)fprintf(caption_log,"%c",*(argstream+index));
    //fputc('\n',caption_log);
    index = 0;
    
    do
    {
        c = *(argstream+index);
        
        //fprintf(caption_log,"line 139: %c\n",c);
        
        if (c == '-')
        {
            argtoken = *(argstream+(++index));
            ++index;
            //fprintf(caption_log,"line 145: %c\n",argtoken);
            while (!isprint((c = *(argstream+(index))))) ++index;//{fprintf(caption_log,"line 148: %c\n",c);++index;}
            switch (argtoken)
            {
                case 'f':
                    options -> parameter_fonttype = calloc(strlen(argstream+index),sizeof(c));
                    strcpy(options -> parameter_fonttype,argstream+index);
                    options -> flag_parameter_fonttype = ~(options -> flag_parameter_fonttype);
                    break;
                case 'i':
                    cairo_image = cairo_image_surface_create_from_png(argstream+index);
                    cairo_image_context = cairo_create(cairo_image);
                    options -> flag_parameter_image = ~(options -> flag_parameter_image);
                    break;
                case 'S':
                    options -> parameter_fontscale = atof(argstream+index);
                    options -> flag_parameter_fontscale = ~(options -> flag_parameter_fontscale);
                    break;
                case 'a':
                    options -> parameter_sourcecolor_alpha = atof(argstream+index);
                    options -> flag_parameter_sourcecolor_alpha = ~(options -> flag_parameter_sourcecolor_alpha);
                    break;
                case 'r':
                    options -> parameter_sourcecolor_red = atof(argstream+index);
                    options -> flag_parameter_sourcecolor_red = ~(options -> flag_parameter_sourcecolor_red);
                    break;
                case 'g':
                    options -> parameter_sourcecolor_green = atof(argstream+index);
                    options -> flag_parameter_sourcecolor_green = ~(options -> flag_parameter_sourcecolor_green);
                    break;
                case 'b':
                    options -> parameter_sourcecolor_blue = atof(argstream+index);
                    options -> flag_parameter_sourcecolor_blue = ~(options -> flag_parameter_sourcecolor_blue);
                    break;
                case 'X':
                    //fprintf(caption_log,"text of -X: %s\n",argstream+index);
                    options -> parameter_arbitraryposition_X = 0;
                    //fprintf(caption_log,"value of arbitrary X: %d\n",options -> parameter_arbitraryposition_X);
                    options -> flag_parameter_arbitraryposition_X = ~(options -> flag_parameter_arbitraryposition_X);
                    break;
                case 'Y':
                    //fprintf(caption_log,"text of -Y: %s\n",argstream+index);
                    options -> parameter_arbitraryposition_Y = 0;
                    //fprintf(caption_log,"value of arbitrary Y: %d\n",options -> parameter_arbitraryposition_Y);
                    options -> flag_parameter_arbitraryposition_Y = ~(options -> flag_parameter_arbitraryposition_Y);
                    break;
                case 'l':
                    for (txtal_index = 0;txtal_index < 9;txtal_index++)
                    {
                        if (strcmp(argstream+index,alignment_names[txtal_index]) == 0)
                        {options -> parameter_text_alignment = txtal_index;break;}
                        else
                            continue;
                    }
                    options -> flag_parameter_text_alignment = ~(options -> flag_parameter_text_alignment);
                    break;
                default:
                    break;
            }
            while (!isprint((c = *(argstream+(index))))) ++index;//{fprintf(caption_log,"line 206: %c\n",c);++index;}
        }
        ++index;
            
    } while (index < arraysize);
    
    free(argstream);
    
    return;
}

int main(int argc,char* argv[])
{ 
    if (argc < 2) end(-1);
    
    //caption_log = fopen("log","w");
    
    typedef struct
    {
        double position_X;
        double position_Y;
    }alignment;
    
    parameters OPTIONS = {0,0,0,0,0,0,0,0,0,0,"monospace",140,1,1,1,1,0,0,4};
    
    if (argc > 2) parse(argc-2,argv+2, &OPTIONS);
    
    alignment  POSITION;
    alignment  text_positions[9];
    int index;
    Display* MAIN_DISPLAY;
    Screen* screen;
    Window MAIN_DISPLAY_WINDOW;
    XEvent INPUT_EVENT;
    KeySym XKEY;
    char XKEYBUFF[10];
    char filename[255];
    unsigned int DEFAULT_SCREEN;
    int MAIN_DISPLAY_WINDOW_ORIGIN_X,MAIN_DISPLAY_WINDOW_ORIGIN_Y = 0;
    unsigned int MAIN_DISPLAY_WINDOW_MAX_WIDTH,MAIN_DISPLAY_WINDOW_MAX_HEIGHT;
    unsigned long MAIN_DISPLAY_WINDOW_BORDER_WIDTH = 0;
    unsigned long MAIN_DISPLAY_WINDOW_BORDER = 0;
    unsigned long MAIN_DISPLAY_WINDOW_BACKGROUND = 0;
    
    cairo_surface_t* cairo_drawing_surface;
    cairo_t* cairo_context;
    cairo_text_extents_t cairo_text;
    cairo_text_extents_t cairo_image_text;
    
    if ((MAIN_DISPLAY = XOpenDisplay((char*)0)) == NULL) end(-2);
    
    DEFAULT_SCREEN = DefaultScreen(MAIN_DISPLAY);
    screen = DefaultScreenOfDisplay(MAIN_DISPLAY);
    
    if (!OPTIONS.flag_parameter_image)
    {
        MAIN_DISPLAY_WINDOW_MAX_WIDTH = WidthOfScreen(screen);
        MAIN_DISPLAY_WINDOW_MAX_HEIGHT = HeightOfScreen(screen);
    }
    else
    {
        MAIN_DISPLAY_WINDOW_MAX_WIDTH = cairo_image_surface_get_width(cairo_image);
        MAIN_DISPLAY_WINDOW_MAX_HEIGHT = cairo_image_surface_get_height(cairo_image);
    }
    
    MAIN_DISPLAY_WINDOW = XCreateSimpleWindow(
                                                MAIN_DISPLAY,
                                                DefaultRootWindow(MAIN_DISPLAY),
                                                MAIN_DISPLAY_WINDOW_ORIGIN_X,
                                                MAIN_DISPLAY_WINDOW_ORIGIN_Y,
                                                MAIN_DISPLAY_WINDOW_MAX_WIDTH,
                                                MAIN_DISPLAY_WINDOW_MAX_HEIGHT,
                                                MAIN_DISPLAY_WINDOW_BORDER_WIDTH,
                                                MAIN_DISPLAY_WINDOW_BORDER,
                                                MAIN_DISPLAY_WINDOW_BACKGROUND
                                             );
    
    XSelectInput(MAIN_DISPLAY,MAIN_DISPLAY_WINDOW,ButtonPressMask|KeyPressMask);

    XMapWindow(MAIN_DISPLAY,MAIN_DISPLAY_WINDOW);
    
    cairo_drawing_surface = cairo_xlib_surface_create(
                                                        MAIN_DISPLAY,
                                                        MAIN_DISPLAY_WINDOW,
                                                        DefaultVisual(MAIN_DISPLAY,DEFAULT_SCREEN),
                                                        MAIN_DISPLAY_WINDOW_ORIGIN_X,
                                                        MAIN_DISPLAY_WINDOW_ORIGIN_Y
                                                     );
    
    cairo_xlib_surface_set_size(cairo_drawing_surface,MAIN_DISPLAY_WINDOW_MAX_WIDTH,MAIN_DISPLAY_WINDOW_MAX_HEIGHT);
    cairo_context = cairo_create(cairo_drawing_surface);
    cairo_scale(cairo_context,1,1);
    
    cairo_select_font_face(cairo_context,OPTIONS.parameter_fonttype,CAIRO_FONT_SLANT_NORMAL,CAIRO_FONT_WEIGHT_BOLD);
    cairo_set_source_rgba(cairo_context,OPTIONS.parameter_sourcecolor_red,OPTIONS.parameter_sourcecolor_green,OPTIONS.parameter_sourcecolor_blue,OPTIONS.parameter_sourcecolor_alpha);
    cairo_set_font_size(cairo_context,OPTIONS.parameter_fontscale);
    cairo_text_extents(cairo_context,argv[1],&cairo_text);
    
    POSITION.position_X = 0+((MAIN_DISPLAY_WINDOW_MAX_WIDTH/3)/2 - cairo_text.width/2),POSITION.position_Y = 0+(MAIN_DISPLAY_WINDOW_MAX_HEIGHT/3/2);
    text_positions[alignment_leftright] = POSITION;
    
    POSITION.position_X = 0+(MAIN_DISPLAY_WINDOW_MAX_WIDTH/3)+(MAIN_DISPLAY_WINDOW_MAX_WIDTH/3/2)-cairo_text.width/2-cairo_text.x_bearing,POSITION.position_Y = 0+(MAIN_DISPLAY_WINDOW_MAX_HEIGHT/3/2);
    text_positions[alignment_centerright] = POSITION;
    
    POSITION.position_X = (0+(MAIN_DISPLAY_WINDOW_MAX_WIDTH - (cairo_text.width/2)))-MAIN_DISPLAY_WINDOW_MAX_WIDTH/3/2,POSITION.position_Y = 0+(MAIN_DISPLAY_WINDOW_MAX_HEIGHT/3/2);
    text_positions[alignment_rightright] = POSITION;
    
    POSITION.position_X = 0+((MAIN_DISPLAY_WINDOW_MAX_WIDTH/3)/2 - cairo_text.width/2),POSITION.position_Y = 0+(MAIN_DISPLAY_WINDOW_MAX_HEIGHT/3/2)+(MAIN_DISPLAY_WINDOW_MAX_HEIGHT/3);
    text_positions[alignment_leftcenter] = POSITION;
    
    OPTIONS.parameter_arbitraryposition_X= POSITION.position_X = 0+(MAIN_DISPLAY_WINDOW_MAX_WIDTH/3)+(MAIN_DISPLAY_WINDOW_MAX_WIDTH/3/2)-cairo_text.width/2-cairo_text.x_bearing,OPTIONS.parameter_arbitraryposition_Y = POSITION.position_Y = 0+(MAIN_DISPLAY_WINDOW_MAX_HEIGHT/3/2)+(MAIN_DISPLAY_WINDOW_MAX_HEIGHT/3);
    text_positions[alignment_centercenter] = POSITION;
    
    POSITION.position_X = 0+(MAIN_DISPLAY_WINDOW_MAX_WIDTH - (cairo_text.width/2))-MAIN_DISPLAY_WINDOW_MAX_WIDTH/3/2,POSITION.position_Y = 0+(MAIN_DISPLAY_WINDOW_MAX_HEIGHT/3/2)+(MAIN_DISPLAY_WINDOW_MAX_HEIGHT/3);
    text_positions[alignment_rightcenter] = POSITION;
        
    POSITION.position_X = 0+((MAIN_DISPLAY_WINDOW_MAX_WIDTH/3)/2 - cairo_text.width/2),POSITION.position_Y = (((0+cairo_text.height/2)+MAIN_DISPLAY_WINDOW_MAX_HEIGHT)-MAIN_DISPLAY_WINDOW_MAX_HEIGHT/3/2);
    text_positions[alignment_leftleft] = POSITION;
    
    POSITION.position_X = 0+(MAIN_DISPLAY_WINDOW_MAX_WIDTH/3)+(MAIN_DISPLAY_WINDOW_MAX_WIDTH/3/2)-cairo_text.width/2-cairo_text.x_bearing,POSITION.position_Y = (((0+cairo_text.height/2)+MAIN_DISPLAY_WINDOW_MAX_HEIGHT)-MAIN_DISPLAY_WINDOW_MAX_HEIGHT/3/2);
    text_positions[alignment_centerleft] = POSITION;
    
    POSITION.position_X = 0+(MAIN_DISPLAY_WINDOW_MAX_WIDTH - (cairo_text.width/2))-MAIN_DISPLAY_WINDOW_MAX_WIDTH/3/2,POSITION.position_Y = (((0+cairo_text.height/2)+MAIN_DISPLAY_WINDOW_MAX_HEIGHT)-MAIN_DISPLAY_WINDOW_MAX_HEIGHT/3/2);
    text_positions[alignment_rightleft] = POSITION;
    
    
    if (!OPTIONS.flag_parameter_image)
    {   
        if (OPTIONS.flag_parameter_text_alignment)
            cairo_move_to(cairo_context,(text_positions+OPTIONS.parameter_text_alignment) -> position_X,0+(text_positions+OPTIONS.parameter_text_alignment) -> position_Y);
        else
            cairo_move_to(cairo_context,OPTIONS.parameter_arbitraryposition_X,OPTIONS.parameter_arbitraryposition_Y);
        
        cairo_show_text(cairo_context,argv[1]);
    }
    
    if (OPTIONS.flag_parameter_image)
    {
        cairo_scale(cairo_image_context,1,1);
        cairo_select_font_face(cairo_image_context,OPTIONS.parameter_fonttype,CAIRO_FONT_SLANT_NORMAL,CAIRO_FONT_WEIGHT_BOLD);
        cairo_set_source_rgba(cairo_image_context,OPTIONS.parameter_sourcecolor_red,OPTIONS.parameter_sourcecolor_green,OPTIONS.parameter_sourcecolor_blue,OPTIONS.parameter_sourcecolor_alpha);
        cairo_set_font_size(cairo_image_context,OPTIONS.parameter_fontscale);
        cairo_text_extents(cairo_image_context,argv[1],&cairo_image_text);
        
        if (OPTIONS.flag_parameter_text_alignment)
            cairo_move_to(cairo_image_context,(text_positions+OPTIONS.parameter_text_alignment) -> position_X,0+(text_positions+OPTIONS.parameter_text_alignment) -> position_Y);
        else
            cairo_move_to(cairo_image_context,OPTIONS.parameter_arbitraryposition_X,OPTIONS.parameter_arbitraryposition_Y);
        
        cairo_show_text(cairo_image_context,argv[1]);
        cairo_set_source_surface(cairo_context,cairo_image,0,0);
        cairo_paint(cairo_context);
    }
    
    while (1)
    {
        if (XPending(cairo_xlib_surface_get_display(cairo_drawing_surface)))
            XNextEvent(cairo_xlib_surface_get_display(cairo_drawing_surface),&INPUT_EVENT);
        else
            continue;
        
        switch (INPUT_EVENT.type)
        {
            case ButtonPress:
                break;
            case KeyPress:
                XKEY = XKeycodeToKeysym(MAIN_DISPLAY,INPUT_EVENT.xkey.keycode,0);
                switch (XKEY)
                {
                    case XK_q:
                        //fclose(caption_log);
                        strcpy(filename,argv[1]);
                        fstrpper(filename);
                        strcat(filename,".png");
                        
                        if (OPTIONS.flag_parameter_fonttype)
                            free(OPTIONS.parameter_fonttype);
                        
                        if (!OPTIONS.flag_parameter_image)
                        {
                            cairo_surface_write_to_png(cairo_drawing_surface,filename);
                            cairo_destroy(cairo_context);
                            cairo_surface_destroy(cairo_drawing_surface);
                        }
                        else
                        {
                            cairo_surface_write_to_png(cairo_image,filename);
                            cairo_surface_destroy(cairo_image);
                            cairo_destroy(cairo_image_context);
                        }
                        
                        XCloseDisplay(MAIN_DISPLAY);
                        end(0);
                    case XK_d:
                        //fclose(caption_log);
                        if (OPTIONS.flag_parameter_fonttype)
                            free(OPTIONS.parameter_fonttype);
                        
                        if (!OPTIONS.flag_parameter_image)
                        {
                            cairo_destroy(cairo_context);
                            cairo_surface_destroy(cairo_drawing_surface);
                        }
                        else
                        {
                            cairo_surface_destroy(cairo_image);
                            cairo_destroy(cairo_image_context);
                        }

                        XCloseDisplay(MAIN_DISPLAY);
                        end(0);
                    default:
                        break;
                }
                break;
            default:
                break;
        }
    }
    
}

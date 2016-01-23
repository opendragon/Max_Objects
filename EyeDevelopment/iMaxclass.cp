#include <string.h>
#include "maxincludes.h"
#include "iMaxclass.h"

//#include "iMaxdefines.h"
#include "iSensor.h"
#include "Eyes.h"
#include "iSensorsClass.h"
#include "imEyes.h"
#include "imDisplayUtils.h"

void iMAX::iMAX_Constructor () {

    mIncommingImage = false;
    mDisplayIncommingImage = false;
    doimageout = true;
    mOnOff=1;
    maximage = NULL;
    mKeepAspect = false;
    maximage = NULL;
    scale = 1;
    displaycolor = _gray;
    op = NULL;
    mSensors = NULL;
    src_internalmemory = false;
    res_internalmemory = false;
    output_old_flag = false;
    src = NULL;
    res_of = NULL;
    //srcTotrg.constructor ();
}
void iMAX::iMAX_Destructor () {        
    
}

iMAX::iMAX (Eyes *x, Symbol *s, short ac, Atom *av) 
        : iMAXimage ()
{
    post ("iMAX::construct----------------------------------");
    // initialize class variables.
    iMAX_Constructor ();
    post ("iMAX::do allocates: Start:%d  AC:%d",((ac>=PSTART)?(PSTART):(0)),ac);

    for (int i=0;i< ac;i++) {
        if (ArgIsName(i,av)) {
            post ("-n%d->%s",i,ArgName(i,av));
        }
        if (ArgIsLong(i,av)) {
            post ("-d%d->%d",i,ArgLong(i,av));
        }
        if (ArgIsFloat(i,av)) {
            post ("-f%d->%lf",i,ArgFloat(i,av));
        }
    }
    post ("--------------------------------------------------->");
    
    

    // allocate the image, processing operator, and a window.
    // if ac > PSTART then some bin buf information has been saved about the display window.????
    int offset;
    if (ArgIsName (0,av) && !strcmp (ArgName(0,av),"_binbuf_")) {
        offset = PSTART;
    }
    else { 
        offset = 0;
    }
    // restore from a bin buf, or create new.
    // offset is set to the end of the bin buf data.
    doallocate(x,offset,ac,av);
    post ("--------------------------------------------------->");
}
iMAX::~iMAX () {
        dodeallocate ();
        // free up class pointers.
        iMAX_Destructor ();
}

void iMAX::doinit (Eyes *x, short ac, Atom *av)
{
    int i;

     // the type of operator determines the number of outlets and inlets to be created.
    // number scheme is as follows.  First input is the image input and is numbered as 0, this inlet
    // also accepts the sync bang event.
    // mNumInlets represents inlets 1 through mNumInlets-1 inclusive.  So the total number of
    // allocated inlets is mNumInlets.
    // THESE ARE CREATED IN LEFT TO RIGHT ORDER, LAST TO FIRST.
    //post ("doinit::create inlets::");

    //post ("doinit::create inlets:: %d downto %d",mNumInlets-1,0);

    for (i=mNumInlets-1;i>=0;i--) {
        m_inlet[i] = proxy_new (x,i,&mWhich);
    }
    //post ("doinit::create outlets:: %d downto %d",mNumOutlets-1,NUMGENERALOUTLETS);

    listoutlet = listout (x);
    for (i=mNumOutlets-1;i>=NUMGENERALOUTLETS;i--) {
        //post ("doinit::general outlet %d",i);
        m_outlet[i] = outlet_new (x,0L);    // create general outlets for processing results.
    }
    // first two inlets are these:
    // post ("create bang out::");

    //post ("image doinit::create image outlet");
    //bang m_banger = bangout (this);                // syncronization of image processing.
    if (doimageout) m_imageout = outlet_new (x,"image");    // image results.

    //sendQelem = qelem_new(this, (method)Eyes_sendTick);    // create qelems

    // images.
    maximage = NULL;// SOURCE...This is the image passed to us.
     //size issue...
     // if the size is not given, then the operator matches the size of the incomming image.
     // if the size is given, then it maintains the same proportions to the first image it sees.
    //post ("image doinit::look for size::");
    lookfor_and_setsize (ac,av);
    
    // have to see if any parameters change, x,y,xsize, or ysize.
    // assume that the right size is the current target image size.  
    // eventually, when after the first image comes int, m_cx etc.
    // will track the source image size.
    // m_nx=kUnknown;m_ny=kUnknown;m_nxsize=kUnknown;m_nysize=kUnknown;// set the next x,y,xsize,ysize to unknown.
    //post ("doinit::");
    //post ("MAX CLass doinit TARGET::x:%d y:%d xs:%d ys:%d ",m_x,m_y,m_xsize,m_ysize);

    imageinit (m_xorig,m_yorig,m_xsizeorig,m_ysizeorig);
    allocate_offbounds ();
    firstimage = true;
    // image is resized upon receipt of first image, or when image size changes in process image.
}
void iMAX::dofree () 
{
    short i;

    //post ("image free.");
    imagefree ();
    //post ("free outlets ");
    for (i=mNumInlets-1;i>=0;i--) {
        freeobject((object *)m_inlet[i]);
    }
    delete_offbounds ();
    //post ("free qelem");
    //qelem_free ((qelem*)sendQelem);    // create qelems
    //post ("free done");
}

#define check_percentage(it)\
    if (it > 100.0) it = 100.0;\
    else if (it < 0.0) it = 0.0;

bool iMAX::lookfor_and_setsize (int ac,Atom *av)
{
    short count=0;
    m_keepproportion=false;// use this as afound flag.  
    //if a size is given then keep size proportional to first incomming image.
    // otherwise match the incomming image's size.
    short i;
    //actual size
    m_xorig = m_x = kDefaultX; m_yorig = m_y = kDefaultY; 
    m_xsizeorig = m_xsize = kDefaultXsize; m_ysizeorig = m_ysize = kDefaultYsize;
    m_sizeorig = m_size = m_xsize*m_ysize; 
    //percentage of original size.
    m_xper = 0.0; m_yper = 0.0;
    m_xsizeper = 100.0; m_ysizeper = 100.0;
    if (!op) return m_keepproportion;
    
    //post ("x:%d y:%d xs:%d ys:%d ",m_x,m_y,m_xsize,m_ysize);
    for (i=0;i<ac;i++) {
        if (ArgIsName(i,av)) {
            //post ("look for:%s",ArgName(i,av));
            if (op->isString (ArgName(i,av),"xs","xsize","sizex","sx") && i+1<ac) {
                post ("Found x size");
                i++;
                if (i < ac && ArgIsLong(i,av)) {
                    m_xsizeper = ArgLong(i,av);
                }
                else if (i < ac && ArgIsFloat(i,av)) {
                    m_xsizeper = ArgFloat(i,av);
                }
                else { break; }
                check_percentage(m_xsizeper);
            }
    
            else if (op->isString (ArgName(i,av),"ys","ysize","sizey","sy") && i+1<ac) {
                post ("Found y size");
                i++;
                if (i < ac && ArgIsLong(i,av)) {
                    m_ysizeper = ArgLong(i,av);
                }
                else if (i < ac && ArgIsFloat(i,av)) {
                    m_ysizeper = ArgFloat(i,av);
                }
                else { break; }
                check_percentage(m_ysizeper);
            }
            
            else if (op->isString (ArgName(i,av),"size","sz","","s") && i+1<ac) {
                post ("Found size");

                i++;
                if (i < ac && ArgIsLongOrFloat(i,av)) {
                    m_xsizeper = ArgIfLongOrFloat(i++,av);
                    check_percentage(m_xsizeper);
                    m_ysizeper = ArgIfLongOrFloat(i,av);
                    check_percentage(m_ysizeper);
                }
                else { break; }
            }
            else if (!strcmp (ArgName(i,av),"x") && i+1<ac) {
                post ("Found x");
                i++;
                if (i < ac && ArgIsLongOrFloat(i,av)) {
                    m_xper = ArgIfLongOrFloat(i,av);
                    check_percentage(m_xper);
                }
                else { break; }
            }
            else if (!strcmp (ArgName(i,av),"y") && i+1<ac) {
                post ("Found y");

                i++;
                if (i < ac && ArgIsLongOrFloat(i,av)) {
                    m_yper = ArgIfLongOrFloat(i,av);
                    check_percentage(m_yper);
                }
                else { break; }
            }
            
        }
    }    
    //post ("lookfor_and_setsize::percents:%lf %lf %lf %lf  ",m_xper,m_yper,m_xsizeper,m_ysizeper);

    set_window_size (m_xper,m_yper,m_xsizeper,m_ysizeper);
    //post ("lookfor_and_setsize::exit size: x:%d y:%d xs:%d ys:%d %d",m_x,m_y,m_xsize,m_ysize,m_keepproportion);
    return m_keepproportion;
}
/* deferred and utility routines */



/*
void iMAX::resizeimage ()
{
        //post ("resize image to:%d %d %d %d",maximage->m_x,maximage->m_y,maximage->m_xsize,maximage->m_ysize);
        //imagefree ();
        // initialize this objects image according to its window settings.
        // maximage is the incomming image.
        imageinit (maximage->m_x,maximage->m_y,maximage->m_xsize,maximage->m_ysize);
        allocate_offbounds ();
}

void iMAX::checkImageSize () {
    //post ("checkimagesize::");
    //post ("keep proportion:%d",m_keepproportion);
    //post ("WINDOW::x:%d y:%d xs:%d ys:%d ",m_x,m_y,m_xsize,m_ysize);
    //post ("INCOMMING::x:%d y:%d xs:%d ys:%d ",maximage->m_x,maximage->m_y,maximage->m_xsize,maximage->m_ysize);
    //post ("ORIGINAL::x:%d y:%d xs:%d ys:%d ",m_xorig,m_yorig,m_xsizeorig,m_ysizeorig);

    // if the images are not the same size, then make them so...
    if (maximage->m_x!=m_xorig || maximage->m_y != m_yorig || maximage->m_xsize!=m_xsizeorig || maximage->m_ysize!=m_ysizeorig) {
        
        resizeimage ();

        //post ("leaving 1....checkimagesize::");
        //post ("WINDOW::x:%d y:%d xs:%d ys:%d ",m_x,m_y,m_xsize,m_ysize);
        //post ("INCOMMING::x:%d y:%d xs:%d ys:%d ",maximage->m_x,maximage->m_y,maximage->m_xsize,maximage->m_ysize);
        //post ("ORIGINAL::x:%d y:%d xs:%d ys:%d ",m_xorig,m_yorig,m_xsizeorig,m_ysizeorig);
        //post ("thiswindow::x:%d y:%d xs:%d ys:%d ",im->x,im->y,im->xsize,im->ysize);
        //post ("incomming image::x:%d y:%d xs:%d ys:%d ",maximage->im->x,maximage->im->y,maximage->im->xsize,maximage->im->ysize);

    }
}
*/
void iMAX::dobang()
{    
}

void iMAX::dodump(Symbol *s, short ac, Atom *av)
{
    //post ("iMAX::dump image: inlet:%d",m_which);
    //qelem_set(sendQelem);
}    

// a quelm that looks to see if an image can be sent after processing.
void iMAX::sendImage ()
{
    Atom anImage;

    short prevLock = lockout_set (1);

    anImage.a_type = A_GIMME;
    if (mIncommingImage) {
        //post ("send incomming");
        anImage.a_w.w_obj = (object *)this->maximage;
    }
    else {
        //post ("send outgoing");
        anImage.a_w.w_obj = (object *)this;
    }
    //post ("iMAX::send image:%d  %u",anImage.a_type,anImage.a_w.w_obj);
    outlet_anything (m_imageout,sym_im,1,&anImage);
    //rel qelem_unset(sendQelem);
    

    lockout_set (prevLock);
}

void iMAX::sendResult ()
{
    //post ("iMAX::send result:");
}

// a quelm that looks to see if an image can be sent after processing.
void iMAX::sendImages ()
{
    Atom anImage;

    short prevLock = lockout_set (1);

    anImage.a_type = A_GIMME;
    if (mIncommingImage) {
        //post ("send incomming");
        anImage.a_w.w_obj = (object *)this->src;
    }
    else {
        //post ("send outgoing");
        anImage.a_w.w_obj = (object *)this->res_of;
    }
    //post ("iMAX::send image:%d  %u",anImage.a_type,anImage.a_w.w_obj);
    outlet_anything (m_imageout,sym_ims,1,&anImage);
    //rel qelem_unset(sendQelem);

    lockout_set (prevLock);
}

void iMAX::sendResults ()
{
    //post ("iMAX::send results:");
}


void iMAX::outlet_long_or_float (void *anOutlet,Atom *mResult) 
{
    short prevLock = lockout_set (1);
//post("output a long:");
//postatom (mResult);
    switch (ArgPtrIsLong(mResult)) {
        case true: outlet_int (anOutlet,ArgPtrLong(mResult));break;
        case false: outlet_float (anOutlet,ArgPtrFloat(mResult));break;
    }
    lockout_set (prevLock);

}

/*

bool iMAX::isString (char *inOpName,char *name1,char *name2,char *name3,char *name4) 
{
    if (strlen (inOpName) == 0) return false;
    char *c = inOpName;

    // if the first two characters are a "im" then strip it off. 
    if (strlen (inOpName) >= 2 && *c == 'i' && *(c+1) == 'm') {
        for (c = inOpName;c-inOpName+2 < strlen (inOpName); c++) {
            *c = *(c+2);
        }
        *c = '\0';
    }
    // if the first character is a lower case i, then strip it off.
    else if (*c == 'i') {
        for (c = inOpName;c-inOpName+2 < strlen (inOpName); c++) {
            *c = *(c+1);
        }
        *c = '\0';
    }
    // make opName lower case.
    for (c = inOpName;c-inOpName < strlen (inOpName);c++) {
        if (*c < 'a' && *c >= 'A' && *c <= 'Z') *c = *c - 'A' + 'a';
    }

    // see if any of the names match.

    if (!strcmp (inOpName,name1)) return true;
    if (!strcmp (inOpName,name2)) return true;
    if (!strcmp (inOpName,name3)) return true;
    if (!strcmp (inOpName,name4)) return true;
    
    //try adding an '=' at the end of the input name.
    char newname[100];// = new char[strlen(inOpName)+2];
    newname[strlen(inOpName)] = '=';
    newname[strlen(inOpName)+1] = '\0';
    if (!strcmp (inOpName,name1)) return true;
    if (!strcmp (inOpName,name2)) return true;
    if (!strcmp (inOpName,name3)) return true;
    if (!strcmp (inOpName,name4)) return true;

    //try adding a '-' at the end of the input name.
    newname[strlen(inOpName)] = '-';    
    if (!strcmp (inOpName,name1)) return true;
    if (!strcmp (inOpName,name2)) return true;
    if (!strcmp (inOpName,name3)) return true;
    if (!strcmp (inOpName,name4)) return true;
    

    //delete newname;
    return false;
}
bool iMAX::isString (char *inOpName,short num,char **inNames) 
{
    if (strlen (inOpName) == 0) return false;
    char *c = inOpName;
    //post ("op name:%s   %c",inOpName,*c);

    // if the first two characters are a "im" then strip it off. 
    if (strlen (inOpName) >= 2 && *c == 'i' && *(c+1) == 'm') {
        for (c = inOpName;c-inOpName+2 < strlen (inOpName); c++) {
            *c = *(c+2);
        }
        *c = '\0';
    }
    // if the first character is a lower case i, then strip it off.
    else if (*c == 'i') {
        for (c = inOpName;c-inOpName+2 < strlen (inOpName); c++) {
            *c = *(c+1);
        }
        *c = '\0';
    }
    // make opName lower case.
    for (c = inOpName;c-inOpName < strlen (inOpName);c++) {
    //    post ("%d < %d  A:%d  a:%d  new:%d %c",*c,'a','A','a',*c - 'A' + 'a',*c - 'A' + 'a');
        if (*c < 'a' && *c >= 'A' && *c <= 'Z') *c = *c - 'A' + 'a';
    }

    // see if any of the names match.
    for (short i = 0; i < num; i++ ) {
        //post ("op name:%s == %s?",inOpName,*(inNames+i));
        if (!strcmp (inOpName,*(inNames+i))) return true;
    }
    return false;
}
*/
#include "imThreshold.h"
//-----------------------------------------------------------------------------------
void iMAX::makewindow (Eyes  *e, long x, long y, long xs, long ys, long flag)
{
    post ("make window IMAX:");
    displayWindpub = (Windpub *) wind_new (e,x,y,xs,ys,WVIS | WCOLOR);
}
void iMAX::doallocate (Eyes *x,int offset, short ac, Atom *av)
{
    short i;
    op = NULL;

    /* sets mNumInlets and mNumOutlets.*/
    //offset pushes us past the bin buf saved items if they exist.
    allocateOperator (x,offset,ac,av);/* allocates the operator variable.*/
    allocate_resultimages (MAXRESULTS);
    /* set the operator threshold and control variables.*/
 
    //post ("iMAX::middle %d %d",mNumInlets,mNumOutlets);

    // allocate image.
    doinit (x,ac,av);
    //REL? doafter_image_resize ();
    SetRect(&imageRect, 0, 0, kDefaultXsize, kDefaultYsize);
    //post ("offset:%d",offset);
    if (offset != 0 && ac >= PSTART) {
        if (ArgIsLongOrFloat(2,av) && ArgIsLongOrFloat(3,av) && ArgIsLongOrFloat(4,av) && ArgIsLongOrFloat(5,av) ) {
            /*     displayWindpub = (Windpub*) wind_new(x,kDefaultRectLeft, kDefaultRectTop, 
                                            kDefaultRectLeft + kDefaultXsize, kDefaultRectTop + kDefaultYsize, 
                                            WCOLOR  | WSGROW);*/
            post ("iMAX::create window:%d %d %d %d   %d %d", (long)( ArgIfLongOrFloat(2,av) ),
                                             (long)( ArgIfLongOrFloat(3,av) ), 
                                             (long) (ArgIfLongOrFloat(2,av) + ArgIfLongOrFloat(4,av) ), 
                                             (long)( ArgIfLongOrFloat(3,av) + ArgIfLongOrFloat(5,av) ),
                                             (long)ArgIfLongOrFloat(4,av), (long)ArgIfLongOrFloat(5,av));


            //displayWindpub = (Windpub*) wind_new
            makewindow (x,  (long)( ArgIfLongOrFloat(2,av) ),(long)( ArgIfLongOrFloat(3,av) ), 
                                            (long) (ArgIfLongOrFloat(2,av) + (long)ArgIfLongOrFloat(4,av) ), 
                                            (long)( ArgIfLongOrFloat(3,av) + (long)ArgIfLongOrFloat(5,av) ), 
                                             WVIS | WCOLOR | WSGROW);
            //ChangeDisplayPosition(x, NULL, 2, av+2);
            //ChangeDisplayDimensions(x, NULL, 2, av+4);
  
              short xloc = (short) ArgIfLongOrFloat(2,av);
            short yloc = (short) ArgIfLongOrFloat(3,av);
            MoveWindow((WindowPtr) &displayWindpub->w_gp, xloc, yloc, FALSE);

             short width = (short) ArgIfLongOrFloat(4,av);
            short height = (short) ArgIfLongOrFloat(5,av);
            // resize windpub window, copy dimensions into windpub's internal values
            SizeWindow((WindowPtr) &displayWindpub->w_gp, width, height, FALSE);
            displayWindpub->w_x2 = displayWindpub->w_x1+displayWindpub->w_gp.portRect.right;
            displayWindpub->w_y2 = displayWindpub->w_y1+displayWindpub->w_gp.portRect.bottom;
        }
        else {
            //displayWindpub = (Windpub*) wind_new
            makewindow (x,kDefaultRectLeft, kDefaultRectTop, 
                                            kDefaultRectLeft + kDefaultXsize, kDefaultRectTop + kDefaultYsize, 
                                            WVIS | WCOLOR | WSGROW);
        }
        mHidden = (bool) ArgIfLongOrFloat(6,av);
        if (mHidden) {
            wind_invis ((Wind*) displayWindpub);
        }
        else {
            wind_vis ((Wind*) displayWindpub);
        }
        mKeepAspect = ArgIfLongOrFloat(7,av);
        mFull = ArgIfLongOrFloat (8,av);
        mOnOff = ArgIfLongOrFloat (9,av);
        //post ("onoff:%d",mOnOff);
        scale = ArgIfLongOrFloat(10,av);
        post ("%d %d %d %d   hide:%d ka:%d full:%d on:%d scale:%f",            displayWindpub->w_x1,
            displayWindpub->w_y1,
            displayWindpub->w_x2-displayWindpub->w_x1,
            displayWindpub->w_y2-displayWindpub->w_y1,
            mHidden,mKeepAspect,mFull,mOnOff,scale);
            if (scale <= 0) scale = 1;
    }
    else {
        post ("iMAX::create window:%d %d %d %d", kDefaultRectLeft, kDefaultRectTop, 
                                            kDefaultRectLeft + kDefaultXsize, kDefaultRectTop + kDefaultYsize);
        //displayWindpub = (Windpub*) wind_new
        makewindow (x,kDefaultRectLeft, kDefaultRectTop, 
                                            kDefaultRectLeft + kDefaultXsize, kDefaultRectTop + kDefaultYsize, 
                                            WVIS | WCOLOR  | WSGROW);
        //mHidden = false; // the show_hide changes the hidden flag. so assign the opposite meaning.
        //show_hide_Windows (x);// change to the correct stored value and set the window to the correct config.
        wind_invis ((Wind*) displayWindpub);
        mHidden = true;
    }
    if (op) {
        //post ("OP NAME:%s",op->myName ());
        wind_settitle((Wind*) displayWindpub, op->myName (), 0);//zzplugin
#if 0
//BEGIN-EYE-PATCH
        PluginDescriptorPtr descriptor = xx->fActiveEye;

        if (descriptor)
        {
            Pchar    aName = static_cast<Pchar>(CallUniversalProc(descriptor->fDoMyNameUpp,
                                                                                    uppDoMyNameProcInfo, EYE_FIXED_ARGUMENTS));

              wind_settitle((Wind*) displayWindpub, aName, 0);
        }
//END-EYE-PATCH
#endif // 0        
    }
    else {
        wind_settitle((Wind*) displayWindpub, /*op->myName ()*/"Display", 0);
    }
    //short off = PSTART;
    //doextrarestore (off,av);
} 
void iMAX::dodeallocate ()
{

    /* free windpubs*/
    // free the window.
    if (displayWindpub) {
        wind_free((Object*) displayWindpub);
        displayWindpub = NULL;
    }
    // free the image.
    dofree ();
    deallocate_resultimages ();
    // free the image processing operator.
    deallocateOperator ();/* allocates the operator variable.*/
    /* set the operator threshold and control variables.*/
}
void iMAX::allocateOperator (Eyes *x,int &offset, short ac,Atom *av)
{
    // default inlet and outlet configuration.
    mNumInlets = 1;// the image input (not created for us), threshold, low value, and high value.
    mNumOutlets = NUMGENERALOUTLETS;// the image result and syncronization.
    post ("iMAX::allocateOperator:allocate imEyesOpFactory..............................");
    //imEyesOpFactory eyesfactory;
    post ("iMAX::allocateOperator:offset:%d",offset);
    op = NULL;
    //eyesfactory = new imEyesOpFactory ();
    //op = imHighThreshold::ifMeThenMake (offset,ac,av); //
    //op = imEyesOpFactory::MakeOp (offset,ac,av,mNumInlets,mNumOutlets);
    if (offset < ac && ArgIsName (offset,av)) {
        op = imEyesOpFactory::MakeOp (ArgName(offset,av),mNumInlets,mNumOutlets);
    }
    if (op == NULL) {
        mIncommingImage = true;
        mDisplayIncommingImage = true;
        post ("op is NULL");
    }
    else {
        //post ("THE TYPE:::::%s  inlets:%d  outlets:%d",op->myName (),mNumInlets,mNumOutlets);
        post ("set parameters:%d %d",offset,ac);
        offset++;
        op->setParameters(offset,ac,av);    // zzplugin
#if 0
//BEGIN-EYE-PATCH
        PluginDescriptorPtr descriptor = xx->fActiveEye;

        if (descriptor)
        {
            CallUniversalProc(descriptor->fDoSetParametersUpp, uppDoSetParametersProcInfo,
                                EYE_FIXED_ARGUMENTS, ac, av);
        }
//END-EYE-PATCH
#endif // 0        
        mIncommingImage = false;
        mDisplayIncommingImage = false;
    }
}
void iMAX::deallocateOperator ()
{
    delete op;
    op = NULL;
}
void iMAX::dosave(short &ac,Atom *av) { 
    // save the x,y,xs,ys parametwrs
    // set the rest of the parameters:
    SetArgSym (ac,av,gensym("x"));ac++;
    SetArgFloat (ac,av,m_xper);ac++;
    SetArgSym (ac,av,gensym("y"));ac++;
    SetArgFloat (ac,av,m_yper);ac++;
    SetArgSym (ac,av,gensym("xs"));ac++;
    SetArgFloat (ac,av,m_xsizeper);ac++;
    SetArgSym (ac,av,gensym("ys"));ac++;
    SetArgFloat (ac,av,m_ysizeper);ac++;
    
    // save the objects thresholds.
    if (op) {
        op->dosave (ac,av);// zzplugin
#if 0
//BEGIN-EYE-PATCH
        PluginDescriptorPtr descriptor = xx->fActiveEye;

        if (descriptor)
        {
            CallUniversalProc(descriptor->fDoSaveUpp, uppDoSaveProcInfo, EYE_FIXED_ARGUMENTS, ac, av);
        }
//END-EYE-PATCH
#endif // 0        
    }
}


//    Add assists for each new inlet and outlet that the processing class uses.

void iMAX::doassist(void * , long type, long which, char *outAssistStr)
{
    #define        kAssistInlet            1
    #define        kAssistOutlet            2

    switch (type) {
        case kAssistInlet:
            switch (which) {
                case 0:
                    strcpy(outAssistStr, "Image data input/Sync Bang");
                    break;
            }
            break;
        case kAssistOutlet:
            switch (which) {
                case 0:
                    strcpy(outAssistStr, "Image data output");
                    break;
                case 1:
                    strcpy(outAssistStr, "Sync Bang=>Signal to send more data.");
                    break;
            }
            break;
    }
}



void iMAX::doint(long inInt)
{
    switch (mWhich) {
        case 0: mOnOff = inInt; break;
        default:
            op->setParameter (mWhich-1,inInt);  // zzplugin
#if 0
//BEGIN-EYE-PATCH
        PluginDescriptorPtr descriptor = xx->fActiveEye;

        if (descriptor)
        {
            CallUniversalProc(descriptor->fDoSetParameterLongUpp, uppDoSetParameterLongProcInfo,
                                EYE_FIXED_ARGUMENTS, inInt);
        }
//END-EYE-PATCH
#endif // 0
        break;
    }
    //post ("do int:%d  which:%d",inInt,mWhich);
}
void iMAX::dofloat(float inFloat)
{
    switch (mWhich) {
        case 0: mOnOff = inInt; break;
        default: op->setParameter (mWhich-1,(double)inFloat); // zzplugin
#if 0
//BEGIN-EYE-PATCH
        PluginDescriptorPtr descriptor = xx->fActiveEye;

        if (descriptor)
        {
            CallUniversalProc(descriptor->fDoSetParameterFloatUpp, uppDoSetParameterFloatProcInfo,
                                EYE_FIXED_ARGUMENTS, inFloat);
        }
//END-EYE-PATCH
#endif // 0
        break;
    }
}

void iMAX::doimage(Symbol *s, short ac, Atom *av)
{
    post ("iMAX::received image: inlet:%d  %u",mWhich,av->a_w.w_obj);
    if (av) {
        maximage = (iMAXimage *)av->a_w.w_obj;
        deallocate_sourceimages ();// sets internal memory to 0, deallocates if necessary.
        //allocate_sourceimages (1);// sets internal memory to 1.
        //srcimage[1] = maximage;// don't need to delete maximage.
        
        post ("iMAX::image in :%d  %u",av->a_type,av->a_w.w_obj);
        if (!maximage->im) {
            post ("MAXIMAGE->IM is NULL");
        }
        else {
            post ("iMAX::image size: %d %d",maximage->im->xsize,maximage->im->ysize);
        }
        post ("onoff:%d",mOnOff);
        //imWindowWrite ((CGrafPtr) &displayWindpub->w_gp,*maximage->ix); 
    }
    /*if (mOnOff)
        processImage (s,ac,av);
        //defer (this,(method)Eyes_processImage,NULL,0,NULL);*/
}
void iMAX::doimages(Symbol *s, short ac, Atom *av)
{
    post ("iMAX::received imageS: inlet:%d  %u",mWhich,av->a_w.w_obj);
    if (!maximage) {
        // if no maximage, then don't know the area covered by the incoming images.
        // need to allocatge image that is greator than the maximum size for all images.
        //????
        post ("many images, no display area can be allocated. ");
    }
    if (av) {
        deallocate_sourceimages ();// sets internal memory to 0, deallocates if necessary.
        src = (iMAXimages *)av->a_w.w_obj;
    }
}
void iMAX::allocate_sourceimages (int howmany)
{
    deallocate_sourceimages ();
    src_internalmemory = 1;
    short prevLock;
    prevLock = lockout_set (1);
    src = new iMAXimages (howmany);
    lockout_set (prevLock);
}
void iMAX::deallocate_sourceimages ()
{
    if (src_internalmemory) {
        if (src) {
            short prevLock;
            prevLock = lockout_set (1);
            delete src;
            lockout_set (prevLock);
            src = NULL;
        }
    }
    src_internalmemory = 0;
}
void iMAX::allocate_resultimages (int howmany)
{
    deallocate_resultimages ();
    res_internalmemory = 1;
    short prevLock;
    prevLock = lockout_set (1);
    res_of = new iMAXimages(howmany);
    lockout_set (prevLock);
}
void iMAX::deallocate_resultimages ()
{
    if (res_internalmemory) {
        if (res_of) {
            short prevLock;
            prevLock = lockout_set (1);
            delete res_of;
            lockout_set (prevLock);
            res_of = NULL;
        }
    }
    res_internalmemory = 0;
}
void iMAX::dosensormap(Symbol *s, short ac, Atom *av)
{
    post ("sensor map received:ac:%d",ac);
    if (av) {
        
        mSensors = (iSensors *) av->a_w.w_obj;
       
//        post ("receive sensors num:%d  %u",mSensors->num,mSensors);
        if (mSensors) {
            if (mSensors->mSensors->num == 0) {
                mSensors = NULL;
            }
        
        /*
            for (iSensor **i = mSensors->mSensor;i< mSensors->mSensor+mSensors->num;i++) {
                post ("%u %u",i,*i);
                if ((*i)->mType == rect_) {
                    post ("sensor:%lf %lf %lf %lf ",(*i)->mRect.getleft (),(*i)->mRect.getright (),
                            (*i)->mRect.gettop (),(*i)->mRect.getbottom ());
                    post ("sensor:%lf %lf %lf %lf ",(*i)->mRect2->getleft (),(*i)->mRect2->getright (),
                            (*i)->mRect2->gettop (),(*i)->mRect2->getbottom ());
                }
            }
             */
        }
       
    }
}
void iMAX::doamessage(Symbol *s, short ac, Atom *av)
{
    post ("do message:::::::::::::");
    post ("message %s",s->s_name);
    for (int i=0;i< ac; i++) {
        if (ArgIsName(i,av)) {
            post ("-n%d->%s",i,ArgName(i,av));
        }
        if (ArgIsLong(i,av)) {
            post ("-d%d->%d",i,ArgLong(i,av));
        }
        if (ArgIsFloat(i,av)) {
            post ("-f%d->%lf",i,ArgFloat(i,av));
        }
    }        
    post (":::::::::::::::::::::::");
}
/*void iMAX::doDisplay ()
{
    //        post ("display image %d %d %d %d",displayWindpub,mHidden,mDisplayIncommingImage,maximage);
    if (displayWindpub && !mHidden) {  
        post ("write to window");
        if (mDisplayIncommingImage) {
            imWindowWrite ((CGrafPtr) &displayWindpub->w_gp,*maximage->ix); //rel in  maximage->ix);
        }
        else if (maximage) {
            imWindowWrite ((CGrafPtr) &displayWindpub->w_gp,*ix);
        }
    }
}*/

void iMAX::doGraphics ()
{
//post ("imax graphics.");
}
void iMAX::doDisplay ()
{
    doDisplay (maximage,this);
}
void iMAX::doDisplay (iMAXimage *src,iMAXimage *trg)
{
    post ("do display:%u %u  ",src,trg);
    post ("do display:%u %u  ",src->ix,trg->ix);
    if (displayWindpub && !mHidden && backGWorld && *offpixmap) {
        short prevLock;
        prevLock = lockout_set(1);    
        if (offBounds.right > 0 && offBounds.bottom > 0 ) {
            CGrafPtr savePort;
            GDHandle saveGD;
            //PixMapHandle bufferPM,savePM;
            //CGrafPtr tempPort = (CGrafPtr ) &displayWindpub->w_gp;
            GetGWorld (&savePort,&saveGD);
            
            SetGWorld (trg->backGWorld,NULL);
            //docolor (_black);
            docolor (displaycolor);
            EraseRect (&trg->backGWorld->portRect);
            if (mDisplayIncommingImage) {
                if (src && src->ix) {
                    post ("display incomming");
                    imPixMapWrite (src->ix,trg->backGWorld,trg->offpixmap,displaycolor);
                }
            }
            else {
                if (trg && trg->ix) {
                    post ("display outgoing");
                    imPixMapWrite (trg->ix,trg->backGWorld,trg->offpixmap,displaycolor);
                }
            }
            docolor (_black);

            //post ("CALL DO GRAPHICS:");
            doGraphics ();
            
            SetGWorld (savePort,saveGD);
        }
        //mKeepAspect = 1;
        if (mKeepAspect ) {
            Rect offWindowBounds;
            int xsize = (displayWindpub->w_gp.portRect.right-displayWindpub->w_gp.portRect.left)*scale;
            int ysize = (displayWindpub->w_gp.portRect.bottom-displayWindpub->w_gp.portRect.top)*scale;
            post ("viewport:%d %d %d %d",displayWindpub->w_gp.portRect.left,displayWindpub->w_gp.portRect.top,
                                    displayWindpub->w_gp.portRect.right,displayWindpub->w_gp.portRect.bottom);
            post ("target image:%d %d %d %d",trg->ix->getx(),trg->ix->gety(),
                                trg->ix->getx()+trg->ix->getxsize(),trg->ix->gety()+trg->ix->getysize());
            post ("xsize:%d  ysize:%d  scale:%lf",xsize,ysize,(double)scale);
            offWindowBounds.top = (float)trg->ix->gety()/(float)ysize*
                                (float)(displayWindpub->w_gp.portRect.bottom-displayWindpub->w_gp.portRect.top);
            offWindowBounds.left = (float)trg->ix->getx()/(float)xsize*
                                (float)(displayWindpub->w_gp.portRect.right-displayWindpub->w_gp.portRect.left);
            offWindowBounds.bottom = (float)(trg->ix->gety()+trg->ix->getysize()-1)/(float)ysize*
                                (float)(displayWindpub->w_gp.portRect.bottom-displayWindpub->w_gp.portRect.top);
            offWindowBounds.right = (float)(trg->ix->getx()+trg->ix->getxsize()-1)/(float)xsize*
                                (float)(displayWindpub->w_gp.portRect.right-displayWindpub->w_gp.portRect.left);    
            post ("aspect bounds: %d %d %d %d",offWindowBounds.left,
                        offWindowBounds.top,offWindowBounds.right,offWindowBounds.bottom);
            rawRectWrite ((CGrafPtr) &displayWindpub->w_gp, (PixMapHandle)trg->offpixmap,trg->backGWorld->portRect,offWindowBounds,
                                (PixMapHandle)trg->offmaskmap,&trg->maskWorld->portRect);
        }
        else {
            //post ("copy to screen:%d %d %d %d",backGWorld->portRect.left,backGWorld->portRect.right,backGWorld->portRect.top,backGWorld->portRect.bottom);
            
            rawWindowWrite ((CGrafPtr) &displayWindpub->w_gp, (PixMapHandle)trg->offpixmap,trg->backGWorld->portRect,
                                (PixMapHandle)trg->offmaskmap,&trg->maskWorld->portRect);
        }
        
        lockout_set(prevLock);
    }
    else {  
        if (!backGWorld) post ("no backgworld");
        if (!displayWindpub) post ("no display window.");
    }
}

/*
void iMAX::collect_results (int &num,Atom *(&results))
{
    op->getValues (num,results);// results have to be unpacked in max.
}

*/
void iMAX::output_results_to_outlets (short &num,Atom *results)
{
     for (short i=0;i<num && i<mNumOutlets-NUMGENERALOUTLETS;i++) {    
        outlet_long_or_float (m_outlet[i+NUMGENERALOUTLETS],&results[i]);    
    }                                                        
}
// code each result for each sensor as a number:
// that way it can be routed!!!!
// image/sensor  256 images/ 256 sensors.==> 16 bit number.
// image*256+sensor.  
// each operation has a known output result.
// each operation could publish the format of this list, introspection?


void iMAX::output_results_as_list (int imagenum, int sensornum, short &num,Atom *results)
{
    // code a number into the two bytes of the list that represents the image and sensor
    // of the result data.
    // image*256+sensor.
    Atom codedresults[256];
    SETLONG(codedresults+0,imagenum);
    SETLONG(codedresults+1,sensornum);
    //post ("outputting results:%d",num+2);
    for (int i=0;i<num && i<MAXRESULTS-2;i++) {
        SETLONG((codedresults+i+2),((long)ArgIfLongOrFloat(i,results)));
    }
    //outlet_list (m_outlet[0+NUMGENERALOUTLETS],NULL,num+2,codedresults);                                                        
    outlet_list (listoutlet,NULL,num+2,codedresults);                                                        
}
void iMAX::process_one_image (ImageIx *src,ImageIx *trg,iSensor *sensor,short &num,Atom *results)
{
    post ("THE TYPE:::::%s",op->myName ());        //zzplugin            
#if 0
//BEGIN-EYE-PATCH
    PluginDescriptorPtr descriptor = xx->fActiveEye;

    if (descriptor)
    {
        Pchar    aName = static_cast<Pchar>(CallUniversalProc(descriptor->fDoMyNameUpp, uppDoMyNameProcInfo,
                                                    EYE_FIXED_ARGUMENTS));
                                                    
        post("THE TYPE:::::%s", aName);
   }
//END-EYE-PATCH
#endif // 0        
    op->Source (src);   //zzplugin????                        
    op->Target (trg);   //zzplugin????
    op->Index (sensor); //zzplugin                
#if 0
//BEGIN-EYE-PATCH
    if (descriptor)
    {
        CallUniversalProc(descriptor->fDoIndexUpp, uppDoIndexProcInfo, EYE_FIXED_ARGUMENTS, sensor);
   }
//END-EYE-PATCH
#endif // 0        
                    
    post ("iMAX::process src:%u trg:%u  op:%u",src,trg,op);            
    op->go ();        //zzplugin
#if 0
//BEGIN-EYE-PATCH
    if (descriptor)
    {
        CallUniversalProc(descriptor->fDoGoUpp, uppDoGoProcInfo, EYE_FIXED_ARGUMENTS);
   }
//END-EYE-PATCH
#endif // 0        
    post ("get values:::");
    op->getValues (num,results);//zzplugin   // results have to be unpacked in max.
#if 0
//BEGIN-EYE-PATCH
    if (descriptor)
    {
        CallUniversalProc(descriptor->fDoGetValuesUpp, uppDoGetValuesProcInfo, EYE_FIXED_ARGUMENTS, &num, results);
   }
//END-EYE-PATCH
#endif // 0        
    post ("results:%d  %u",num,results);
}
bool iMAX::processSensor (int imix,int senix,iMAXimage *src,iMAXimage *trg,iSensor *sensor)
{
    bool success=false;            
    // if input image changes size, change the result image size.    
    if (!trg) {
        post ("!!!!no target maximage");
        return false;
    }
    if (!src) {
        post ("!!!!no source maximage");
        return false;
    }
    if (!src->ix) {
        post ("!!!no source image");
        return false;
    }
    if (!trg->ix) {
        post ("!!!no target image");
        return false;
    }
    // wait for the resize to be scheduled and completed.    
    //REL if (doafter_image_resize ()) // could be removed in theory. REL.    always false????
    //    return success;// don't process yet, wait for resize to get scheduled.
    post ("check op");
    if (op) {
        process_one_image (src->ix,trg->ix,sensor,mNumResults,mResult);
        //collect_results (mNumResults,mResult);        
/*        post("output results");        

//rel output results.
        if (output_old_flag) {// for backward compatibility.
            output_results_to_outlets (mNumResults,mResult);
        }
        else {        
            output_results_as_list (imix,senix,mNumResults,mResult);
        }
  */
        success = true;        
    }    
    //post ("call the display:");
    doDisplay (src,trg);
    return success;
}            
// One to one and one to many.
bool iMAX::processImage (int imix,iMAXimage *src,iMAXimage **trg)
{                                                        
    /*processing of the image occurs here.*/                        
    post ("iMAX::process image: %u  %u    src:%dx%d    trg:%dx%d",op,src,trg,src->im->xsize,src->im->ysize,
                                    (*trg)->im->xsize,(*trg)->im->ysize);    
    bool success=false;            
    if (src != NULL) {                                                    
        if (mSensors && mSensors->mSensor) {
            post ("process many sensors:");
            if (trg) { // could be null the first time around. && *trg) {
                for (iSensor **i = mSensors->mSensor;i< mSensors->mSensor+mSensors->num;i++,trg++,res_of->num++) {
                    if (!*trg) return success;
                    post ("check the image size for sensor:%u %u  src:%dx%d    trg:%dx%d",src,*trg,
                                    src->im->xsize,src->im->ysize,
                                    (*trg)->im->xsize,(*trg)->im->ysize);
                    if ((*trg)->checkImageSize (src,*i)) {    
                          (*trg)->allocate_offbounds ();
                      }   
                      post ("process a sensor:");
                    if (!processSensor (imix,i-mSensors->mSensor,src,*trg,*i)) {
                        return false;
                    }
                    
                }
            }
            else {
                post ("target images not allocated");
            }
            post ("finished many sensors:");
        }
        else {
            if ((*trg)->checkImageSize (src)) {    
                  (*trg)->allocate_offbounds ();
              }            
              post ("process arg dimensions.:");
            if (!processSensor (imix,0,src,*trg,NULL)) {
                return false; 
            }
            post ("finished arg dimensions:");
            trg++;res_of->num++;
        }
    }
    return success;
}
/*
    Connection through a defer to the one to one and one to many.
 */
bool iMAX::processImage ()                    
{    
    res_of->num = 0;
    //post ("Global check the image size:%u %u",maximage,ix);

    if (src != NULL) {
        /*
            Many to many, either one sensor or many.
            
            This is where many images come into the object for processing.  In the case where there 
            many sensors, all sensors are applied to each image (as is one sensor).
         */
        bool success=false;            
        if (src != NULL && src->image != NULL) {
            if (res_of != NULL) {    
                post ("process many images:");
                for (iMAXimage **i=src->image,**j=res_of->image;i<src->image+src->num && 
                     j-res_of->image < MAXRESULTS && src->num < MAXRESULTS;
                     i++) {
                    success = processImage (i-src->image,*i,j);
                    if (!success) return success;
                }    
            }
            else {
                post("result image is null.");
            }                
        }
        return success;
    }
    else {
        // one to one and one to many.    
        iMAXimage **trg;
        iMAXimage *tmp=this;
        
        if (mSensors) {
            if (res_of) { 
                if (res_of->image) {
                    trg = res_of->image;
                }
                else {
                    post ("no results images.");
                    return false;
                }
            }
            else {
                post ("results of image holder not allocated....");
                return false;
            }   
        }
        else {
            trg = &tmp;
        }
        if (!maximage) {
            post ("no maximage");
            return false;
        }
        if (!maximage->im) {
            post ("maximage->im is NULL:"); 
            return false;
        }
        post ("process one image: x:%d y:%d",maximage->im->xsize,maximage->im->ysize);
        bool result = processImage (0,maximage,trg);
        //if (mSensors && res_of ) res_of->num++;// why???
        post ("finished processing:%d",result);
        return result;
    }
}
/*                        
bool iMAX::processImages ()                    
{                                                                                                    
    //processing of the image occurs here.                            
    bool success=false;            
    if (srcimage != NULL) {    
        for (iMAXimage **i=srcimage,**j=resimage;i<srcimage+numsrc;i++,j++) {
            processimage (i-srcimage,*i,*j);
        }                    
    }
}    
        if (mSensors) {// many sensors.
            for (iSensor **i = mSensors->mSensor;i< mSensors->mSensor+mSensors->num;i++) {
            }
        }
        else {// one sensor.
            //post ("check the image size:");                
            checkImageSize ();                                        
            if (doafter_image_resize ()) 
                return success;
            if (op) {
                //post ("THE TYPE:::::%s",op->myName ());                    
                op->Source (maximage->ix);                                
                op->Target (ix);                 
                                
                //post ("iMAX::process ix: %d  op:%d",ix,op);            
                op->go ();                                                
                // post ("iMAX::getvalues");                    
                 op->getValues (mNumResults,mResult);                        
                //    post ("values:%d    outlets:%d ",mNumResults,mNumOutlets);
                 for (short i=0;i<mNumResults && i<mNumOutlets-NUMGENERALOUTLETS;i++) {    
                    outlet_long_or_float (m_outlet[i+NUMGENERALOUTLETS],&mResult[i]);    
                }                                                        
                //post ("set send qelem");            
                // send results if ready.                                
                //qelem_set(sendQelem);        
                success = true;        
            }                    
        }
        // post ("iMAX::dodisplay %d",maximage);
        //post ("do display");        
        doDisplay ();
    }
    return success;
}
*/

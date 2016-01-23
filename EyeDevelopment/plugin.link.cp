#include "imPixelOp.h"
#include "iSensor.h"

static void
Index(imPixelOp *x, 
      iSensor *s)
{
    x->Index(s);
}

static void
go(imPixelOp *x)
{
    x->go();
}

static char *
myName(imPixelOp *x)
{
    return x->myName();
}

//static double
//getParameter(imPixelOp *x,
//             short v)
//{
//    return x->getParameter(v);
//}

//static short
//getNumParameters(imPixelOp *x)
//{
//    return x->getNumParameters();
//}

static void
setParameter(imPixelOp *x,
             short which,
             short ac,
             Atom *av)
{
    x->setParameter(which, ac, av);
}

static void
setParameter(imPixelOp *x,
             short which,
             long val)
{
    x->setParameter(which, val);
}

static void
setParameter(imPixelOp *x,
             short which,
             double val)
{
    x->setParameter(which, val);
}

static void
setParameters(imPixelOp *x,
              int &base,
              short ac,
              Atom *av)
{
    x->setParameters(base, ac, av);
}

//static int
//isParameter(imPixelOp *x,
//            char *c,
//            int &num)
//{
//    return x->isParameter(c, num);
//}

static void
dosave(imPixelOp *x,
       short &ac,
       Atom *av)
{
    x->dosave(ac, av);
}

static void
getValues(imPixelOp *x,
          short &ac,
          Atom *av)
{
    x->getValues(ac, av);
}


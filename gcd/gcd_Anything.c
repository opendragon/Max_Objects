/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       gcd_Anything.c                                                          */
/*                                                                                      */
/*  Contains:   The routine cmd_Anything().                                             */
/*                                                                                      */
/*  Written by: Norman Jaffe                                                            */
/*                                                                                      */
/*  Copyright:  © 1999 by K. Gregory, G. I. Gregson, N. Jaffe and T. H. Schiphorst.     */
/*              All rights reserved. This software and documentation may be distributed */
/*              for non-commercial use only. Any commercial use, without the written    */
/*              permission of N. Jaffe and T. H. Schiphorst, is strictly prohibited.    */
/*                                                                                      */
/*  Change History (most recent first):                                                 */
/*                                                                                      */
/*  Version Author  Date        Status                                                  */
/*  1.0.1   NJ      2000/05/28  Modified for separate compilation                       */
/*  1.0.0   NJ      1999/12/05  Created                                                 */
/*                                                                                      */
/*  To Do:                                                                              */
/*                                                                                      */
/*--------------------------------------------------------------------------------------*/

#include "gcd.h"

/*------------------------------------ cmd_Anything ---*/
Pvoid cmd_Anything
    (GcdPtr  xx,
     PSymbol message,
     short   argc,
     PAtom   argv)
{
#pragma unused(xx)
    short ii = 0;

    EnterCallback();
    error(OUTPUT_PREFIX "unknown message '%s' seen", message->s_name);
    for (;
        ii < argc;
        ii++)
    {
        switch (argv[ii].a_type)
        {
            case A_LONG:
                post("  argument %hd is a long: %ld", ii, argv[ii].a_w.w_long);
                break;
                
            case A_SYM:
                post("  argument %hd is a symbol: '%s'", ii, argv[ii].a_w.w_sym->s_name);
                break;

            case A_FLOAT:
                post("  argument %hd is a float: %f", ii, asDouble(argv[ii].a_w.w_float));
                break;

        }
    }
    ExitMaxMessageHandler()
} /* cmd_Anything */

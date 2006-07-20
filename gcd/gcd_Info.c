/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       gcd_Info.c                                                              */
/*                                                                                      */
/*  Contains:   The routine cmd_Info().                                                 */
/*                                                                                      */
/*  Written by: Norman Jaffe                                                            */
/*                                                                                      */
/*  Copyright:  © 2000 by K. Gregory, G. I. Gregson, N. Jaffe and T. H. Schiphorst.     */
/*              All rights reserved. This software and documentation may be distributed */
/*              for non-commercial use only. Any commercial use, without the written    */
/*              permission of N. Jaffe and T. H. Schiphorst, is strictly prohibited.    */
/*                                                                                      */
/*  Change History (most recent first):                                                 */
/*                                                                                      */
/*  Version Author  Date        Status                                                  */
/*  1.0.0   NJ      2000/06/04  Created                                                 */
/*                                                                                      */
/*  To Do:                                                                              */
/*                                                                                      */
/*--------------------------------------------------------------------------------------*/

#include "gcd.h"
#include "Common_DisplayAboutBox.h"

/*------------------------------------ cmd_Info ---*/
Pvoid cmd_Info
    (GcdPtr   xx,
     PPatcher pp,
     PBox     bb)
{
#pragma unused(xx, pp, bb)
    EnterCallback();
    Common_DisplayAboutBox(OUR_RES_NUMB, gVersionNumber);
    ExitMaxMessageHandler()
} /* cmd_Info */

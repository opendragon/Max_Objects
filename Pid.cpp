/* Copyright @ 1992 Aurel Systems, Inc. All rights reserved.                */

/* File:            PID.C                                                                            */
/* Author(s):    L. Wasik & T. Casavant                                                    */
/* Created:        92/06/02                                                                        */
/* Modified:                                                                                    */
/*                    93/09/17:    merged Perform() and Warmup()                            */
/*                    92/10/27:    added ShutDown()                                            */
/*                    92/06/02:    added GetDefaultValue()                                    */

// Following 2 precompiled header lines must appear at the top of all files.
#include "dlm_pch.h"
#pragma hdrstop

#include "cntrl.h"

        /*=============================================================*/
        /*                                                                                    */
        /*            Description, Requirements and Recognition                    */
        /*                                                                                    */
        /*=============================================================*/

/*                                                                    */
/* Controls output to make input match a setpoint    */
/*                                                                    */

/* CONTROLLED_VAR_MAX is the upper limit beyond which                                */
/* NEW_CONTROLLED_VARIABLE should not be taken; CONTROLLED_VAR_MIN is the    */
/* lower limit beyond which NEW_CONTROLLED_VARIABLE should not be taken;    */
/* DELTA_CONTROLLED_VARIABLE is the change in NEW_CONTROLLED_VARIABLE;        */
/* DIFFERENTIAL_TIME_CONSTANT is the differential time constant;                */
/* ERROR_FROM_SETPOINT is the difference between SETPOINT and                    */
/* MEASURED_VARIABLE; INTEGRAL_TIME_CONSTANT is the integral time constant    */
/* or reset rate; MAX_CONTROL_CHANGE is the maximum change that should        */
/* ever be made in one control period; MEASURED_VARIABLE is the value of    */
/* the output variable of a process to be controlled;                                */
/* NEW_CONTROLLED_VARIABLE is the calculated value for the input variable    */
/* of the process to be controlled; PROPORTIONAL_GAIN is the controller        */
/* proportional gain constant; SETPOINT is the desired setpoint for            */
/* MEASURED_VARIABLE; VARIABLE_GAIN is the amount the MEASURED_VARIABLE        */
/* will change per unit increase of NEW_CONTROLLED_VARIABLE                        */

#define kModuleDescription    "PID"
#define kVendorNumber        kVendorAurelCadsimPlus
#define kFileTag                PID

static Qchar    kKeywordNames[] =
  {
    PID_NAME,
    END_OF_LIST
  };

static T_NameValue    kFreeControlVariableTable[] =
  {
    { "MEASURED_VARIABLE",                5.0 },
    { "SETPOINT",                            5.0 },
    { "VARIABLE_RATIO",                    1.0 },
    { "PROPORTIONAL_GAIN",                0.125 },
    { "INTEGRAL_TIME_CONSTANT",        5.0 },
    { "DIFFERENTIAL_TIME_CONSTANT",    0.0 },
    { "CONTROLLED_VAR_MIN",                1.0e-10 },
    { "CONTROLLED_VAR_MAX",                50.0 },    /* max & max change scaled */
    { "MAX_CONTROL_CHANGE",                0.1 },    /* by var_ratio & conv. factors */
    { "CONTROLLED_VAR_CURRENT",        0.0 },    /*    max always > 1000 */
    END_OF_NAMES_VALUES                                /*    max_change always > 1 */
  };

enum EFreeVarIndices
  {
    indMeasuredVariable,                /* 0 */
    indSetpoint,                        /* 1 */
    indVariableRatio,                    /* 2 */
    indProportionalGain,                /* 3 */
    indIntegralTimeConstant,        /* 4 */
    indDifferentialTimeConstant,    /* 5 */
    indControlledVarMin,                /* 6 */
    indControlledVarMax,                /* 7 */
    indMaxControlChange,                /* 8 */
    indControlledVarCurrent            /* 9 */
  };    /* EFreeVarIndices */

static Qchar    kCalcVariables[] =
  {
    ERROR_FROM_SETPOINT_NAME,
    "DELTA_CONTROLLED_VARIABLE",
    "NEW_CONTROLLED_VARIABLE",
    RELATIVE_ERROR_NAME,
    END_OF_LIST
  };

enum ECalcVarIndices
  {
    indErrorFromSetpoint,            /* 0 */
    indDeltaControlledVariable,    /* 1 */
    indNewControlledVariable,        /* 2 */
    indRelativeErrorFromSetpoint    /* 3 */
  };    /* ECalcVarIndices */

Begin_private_data(kFileTag)
    bool                checkLimits;
    double            lastErrorFromSetpoint;
    double            lastMeasured;
    double            secondLastMeasured;
    double            manualSetting;
    TIndex            countNoResponse;
    TIndex            countOnTarget;
    EControlState    controlState;
    EControlState    lastControlState;
End_private_data(kFileTag)

Standard_control_description
(kFileTag,                                    /* source file name */
 kVendorNumber,                            /* vendor number */
 kModuleDescription,                        /* module description */
 kKeywordNames,                            /* module keywords */
 kFreeControlVariableTable,            /* names of input parameters/default values */
 kCalcVariables,                            /* names of calculated variables */
 indNewControlledVariable + 1,        /* index of controlled variable */
 indMeasuredVariable + 1,                /* index of measured variable */
 indSetpoint + 1,                            /* index of setpoint variable */
 indVariableRatio + 1,                    /* fVariableGainIndex */
 indProportionalGain + 1,                /* fProportionalGainIndex */
 indErrorFromSetpoint + 1,                /* fErrorVariableIndex */
 indDeltaControlledVariable + 1,        /* fDeltaControlledVariableIndex */
 indIntegralTimeConstant + 1,            /* fIntegralTimeConstantIndex */
 indDifferentialTimeConstant + 1,    /* fDifferentialTimeConstantIndex */
 indControlledVarMin + 1,                /* fControlledVariableMinimumIndex */
 indControlledVarMax + 1,                /* fControlledVariableMaximumIndex */
 indMaxControlChange + 1,                /* fMaximumControlChangeIndex */
 indControlledVarCurrent + 1,            /* fControlledVariableCurrentIndex */
 indRelativeErrorFromSetpoint + 1)    /* fRelativeErrorVariableIndex */

        /*=============================================================*/
        /*                                                                                    */
        /*            Creation and Initialization                                    */
        /*                                                                                    */
        /*=============================================================*/

/*-------------------------------- SetUp ----------------------------------*/
Standard_SetUp_function

/*-------------------------------- Initialize -----------------------------*/
Initialize_function
  {
    RecognizeAllArguments(Initialize)
    local->manualSetting = local->lastErrorFromSetpoint = 0.0;
    local->countNoResponse = local->countOnTarget = 0;
    local->checkLimits = true;
    local->lastControlState = kControlStateManual;
    local->controlState = kControlStateAutomatic;
  }    /* Initialize */

        /*=============================================================*/
        /*                                                                                    */
        /*            Getting and Setting Information                                */
        /*                                                                                    */
        /*=============================================================*/

/*-------------------------------- GetState -------------------------------*/
GetState_function
  {
    RecognizeAllArguments(GetState)
    *currentState = local->controlState;
    return local->manualSetting;
  }    /* GetState */

/*-------------------------------- SetState -------------------------------*/
SetState_function
  {
    RecognizeAllArguments(SetState)
    local->controlState = newState;
    if (newState == kControlStateManual)
        local->manualSetting = value;
  }    /* SetState */

/*-------------------------------- GetTypicalValue ------------------------*/
Standard_GetTypicalValue_function

        /*=============================================================*/
        /*                                                                                    */
        /*            Calculations                                                        */
        /*                                                                                    */
        /*=============================================================*/

/*-------------------------------- Perform --------------------------------*/
Perform_function
  {
    double    change, changeLimit, controlledVariable, down;
    double    errorFromSetpoint, maxChange, newControlled, measuredVar;
    double    newDifferential, newIntegral, proportionalChange;
    double    realGain, setpoint, timeStep, up, relError, resetTime;
    TIndex    noRespMax;

    RecognizeAllArguments(Perform)
    setpoint = FreeControlVariable(indSetpoint);
    measuredVar = FreeControlVariable(indMeasuredVariable);
    errorFromSetpoint = measuredVar - setpoint;
    CalcVariable(indErrorFromSetpoint) = relError = errorFromSetpoint;
    if (fabs(setpoint) > kZeroWannaBe)
        relError /= setpoint;
    CalcVariable(indRelativeErrorFromSetpoint) = relError;
    newControlled = controlledVariable =
                                            FreeControlVariable(indControlledVarCurrent);
    realGain = FreeControlVariable(indVariableRatio);
    if (fabs(realGain) > kZeroWannaBe)
        realGain = FreeControlVariable(indProportionalGain) / realGain;
     else
        realGain = FreeControlVariable(indProportionalGain);
    if (isWarmup)
      {
        if (verbose)
            PrintSelf("- warming up on manual ", " -\n");
        local->countNoResponse = local->countOnTarget = 0;
        local->lastControlState = local->controlState;
        local->lastErrorFromSetpoint = 0.0;
        CalcVariable(indDeltaControlledVariable) = 0.0;
        CalcVariable(indNewControlledVariable) = controlledVariable;
        local->lastMeasured = local->secondLastMeasured = measuredVar;
        if (verbose)
            PrintDouble(" New controlled variable is ", controlledVariable, "\n");
      }
     else
      {
        if (local->controlState == kControlStateManual)
          {                                                            /* Manual Control */
            if (verbose)
                PrintSelf("- performing on manual ", " -\n");
            newControlled = local->manualSetting;
            change = newControlled - controlledVariable;
            local->countNoResponse = local->countOnTarget = 0;
            local->checkLimits = true;
          }
         else
          {                                                            /* Automatic Control */
            resetTime = FreeControlVariable(indIntegralTimeConstant);
            if (verbose)
              {
                PrintSelf("- performing on automatic ", " -\n");
                PrintDouble("\tSetpoint: ", setpoint, NULL);
                PrintDouble("\tMeasured Variable: ", measuredVar, "\n");
                PrintDouble("\tProportional Gain: ",
                                FreeControlVariable(indProportionalGain), NULL);
                PrintDouble("\tVariable Action Ratio: ",
                                FreeControlVariable(indVariableRatio), NULL);
                PrintDouble("\tOverall Gain: ", realGain, "\n");
                PrintDouble("\tReset Time: ", resetTime, NULL);
                PrintDouble("\tDerivative Time: ",
                                FreeControlVariable(indDifferentialTimeConstant),
                                "\n");
              }
            timeStep = GetDeltaTime();    /* minutes */
            maxChange = FreeControlVariable(indMaxControlChange);
            proportionalChange = errorFromSetpoint -
                                            local->lastErrorFromSetpoint;
            if (resetTime < kZeroWannaBe)
                newIntegral = 0.0;
             else
                newIntegral = errorFromSetpoint * timeStep / resetTime;
            newDifferential = measuredVar - 2.0 * local->lastMeasured +
                                    local->secondLastMeasured;
            newDifferential *= FreeControlVariable(indDifferentialTimeConstant) /
                                        timeStep;

                /* For a forward action controller, a negative error */
                /* (measured value too low) requires a positive control */
                /* action--> this action must be ADDED to the current */
                /* controlled value so that the new controlled value is HIGHER. */

            change = - realGain * (proportionalChange + newIntegral +
                            newDifferential);
            if (verbose)
              {
                PrintDouble("\tError: ", errorFromSetpoint, NULL);
                PrintDouble("\tRelative error: ", relError, "\n");
                PrintDouble("\tLast error: ", local->lastErrorFromSetpoint, NULL);
                PrintDouble("\tTime step: ", timeStep, NULL);
                PrintDouble("\tMax_change: ", maxChange, "\n");
                PrintDouble("\tProportional part: ", proportionalChange, NULL);
                PrintDouble("\tIntegral part: ", newIntegral, NULL);
                PrintDouble("\tDerivative part: ", newDifferential, "\n");
                PrintDouble("\tControl action: ", change, NULL);
                PrintDouble("\tCurrent controlledVariable: ", controlledVariable,
                                "\n");
              }
            down = FreeControlVariable(indControlledVarMin);
            up = FreeControlVariable(indControlledVarMax);
            if (local->checkLimits)    /*    true here if last pass was MANUAL */
              {
                if ((controlledVariable < down) &&
                    ((controlledVariable + change) < down))
                  {
                    newControlled = down;
                        /* change = gap between minimum and current */
                    change = down - controlledVariable;
                    if (verbose)
                        PrintDouble("\tnewControlled set to lower limit: ",
                                        newControlled, "\n");
                  }
                 else if ((controlledVariable > up) &&
                            ((controlledVariable + change) > up))
                  {
                    newControlled = up;
                        /* change = gap between maximum and current */
                    change = up - controlledVariable;
                    if (verbose)
                        PrintDouble("\tnewControlled set to upper limit: ",
                                        newControlled, "\n");
                  }
                 else
                    local->checkLimits = false;
              }
            if (local->checkLimits)
                local->checkLimits = false;
             else if (change < 0.0)
              {
                if (change < (- maxChange))
                    change = (- maxChange);
                newControlled = controlledVariable + change;
                if (newControlled < down)
                  {
                        /* changeLimit = -gap between minimum and current */
                    changeLimit = down - controlledVariable;
                    if ((changeLimit < 0.0) && (changeLimit >= change))
                        change = changeLimit;
                     else
                      {
                            /*    controlledVariable is already below minimum */
                            /*    AND the calc'd change would make it even lower */
                        if (local->lastControlState == kControlStateAutomatic)
                          {
                            Pchar mes1 = TranslateMessage0("Control is shut.");

                            SignalAlarmAndFree1(kAlarmImportant, mes1);
                          }
                        change = 0.0;
                      }
                    newControlled = controlledVariable + change;
                  }
              }
             else if (change > 0.0)
              {
                if (change > maxChange)
                    change = maxChange;
                newControlled = controlledVariable + change;
                if (newControlled > up)
                  {
                        /* changeLimit = gap between maximum and current */
                    changeLimit = up - controlledVariable;
                    if ((changeLimit > 0.0) && (changeLimit <= change))
                        change = changeLimit;
                     else
                      {
                            /*    controlledVariable is already above maximum */
                            /*    AND the calc'd change would make it even higher */
                        if (local->lastControlState == kControlStateAutomatic)
                          {
                            Pchar mes1 = TranslateMessage0("Control is wide open.");

                            SignalAlarmAndFree1(kAlarmImportant, mes1);
                          }
                        change = 0.0;
                      }
                    newControlled = controlledVariable + change;
                  }
              }
             else    /* the rare case where it EXACTLY matches */
                newControlled = controlledVariable;
            if (verbose)
              {
                PrintDouble("\tNew_controlled: ", newControlled, NULL);
                PrintDouble("\tminimum: ", down, NULL);
                PrintDouble("\tmaximum: ", up, "\n");
              }
            local->manualSetting = newControlled;    /* track manual control */
            if (((errorFromSetpoint > 0.01) &&
                    (errorFromSetpoint >= local->lastErrorFromSetpoint)) ||
                    ((errorFromSetpoint < -0.01) &&
                    (errorFromSetpoint <= local->lastErrorFromSetpoint)))
              {
                noRespMax = 200 + static_cast<TIndex>(3.0 * resetTime / timeStep);
                if (local->countNoResponse++ > noRespMax)
                  {
                    Pchar mes1 = TranslateMessage0("Control is not responding.");

                    SignalAlarmAndFree1(kAlarmImportant, mes1);
                  }
              }
             else
                local->countNoResponse = 0;
          }
        local->lastControlState = local->controlState;
        local->lastErrorFromSetpoint = errorFromSetpoint;
        local->secondLastMeasured = local->lastMeasured;
        local->lastMeasured = measuredVar;
        CalcVariable(indDeltaControlledVariable) = change;
        CalcVariable(indNewControlledVariable) = newControlled;
        if (verbose)
            PrintDouble("\t---> New controlled variable:", newControlled, "\n");
      }
  }    /* Perform */

        /*=============================================================*/
        /*                                                                                    */
        /*            Reading/Writing                                                    */
        /*                                                                                    */
        /*=============================================================*/

/*-------------------------------- WriteDataToMemory ----------------------*/
WriteDataToMemory_function
  {
    T_Size    howMuch = 0;

    RecognizeAllArguments(WriteDataToMemory);
    COPY_TO_MEMORY_HANDLE(startOfData, local->checkLimits, howMuch)
    COPY_TO_MEMORY_HANDLE(startOfData, local->lastErrorFromSetpoint, howMuch)
    COPY_TO_MEMORY_HANDLE(startOfData, local->lastMeasured, howMuch)
    COPY_TO_MEMORY_HANDLE(startOfData, local->secondLastMeasured, howMuch)
    COPY_TO_MEMORY_HANDLE(startOfData, local->manualSetting, howMuch)
    COPY_TO_MEMORY_HANDLE(startOfData, local->countNoResponse, howMuch)
    COPY_TO_MEMORY_HANDLE(startOfData, local->countOnTarget, howMuch)
    COPY_TO_MEMORY_HANDLE(startOfData, local->controlState, howMuch)
    COPY_TO_MEMORY_HANDLE(startOfData, local->lastControlState, howMuch)
    return howMuch;
  }    /* WriteDataToMemory */

/*-------------------------------- ReadDataFromMemory ---------------------*/
ReadDataFromMemory_function
  {
    T_Size    howMuch = 0;

    RecognizeAllArguments(ReadDataFromMemory);
    COPY_FROM_MEMORY_HANDLE(startOfData, local->checkLimits, howMuch)
    COPY_FROM_MEMORY_HANDLE(startOfData, local->lastErrorFromSetpoint, howMuch)
    COPY_FROM_MEMORY_HANDLE(startOfData, local->lastMeasured, howMuch)
    COPY_FROM_MEMORY_HANDLE(startOfData, local->secondLastMeasured, howMuch)
    COPY_FROM_MEMORY_HANDLE(startOfData, local->manualSetting, howMuch)
    COPY_FROM_MEMORY_HANDLE(startOfData, local->countNoResponse, howMuch)
    COPY_FROM_MEMORY_HANDLE(startOfData, local->countOnTarget, howMuch)
    COPY_FROM_MEMORY_HANDLE(startOfData, local->controlState, howMuch)
    COPY_FROM_MEMORY_HANDLE(startOfData, local->lastControlState, howMuch)
    return howMuch;
  }    /* ReadDataFromMemory */

/*-------------------------------- WriteDataToFile ------------------------*/
Standard_WriteDataToFile_function

        /*=============================================================*/
        /*                                                                                    */
        /*            Termination                                                            */
        /*                                                                                    */
        /*=============================================================*/

/*-------------------------------- Terminate ------------------------------*/
Standard_Terminate_function

/*-------------------------------- ShutDown -------------------------------*/
Standard_ShutDown_function


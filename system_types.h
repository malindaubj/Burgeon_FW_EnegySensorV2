/* Microchip Technology Inc. and its subsidiaries.  You may use this software 
 * and any derivatives exclusively with Microchip products. 
 * 
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS".  NO WARRANTIES, WHETHER 
 * EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED 
 * WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A 
 * PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION 
 * WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION. 
 *
 * IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
 * INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
 * WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS 
 * BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE 
 * FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS 
 * IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF 
 * ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 * MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE 
 * TERMS. 
 */

/* 
 * File:   
 * Author: 
 * Comments:
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef SYSTEM_TYPES_H
#define	SYSTEM_TYPES_H

#include <xc.h> // include processor files - each processor file is guarded.  

typedef enum tagCORCON_MODE_TYPE
{ 
    CORCON_MODE_PORVALUES   = 0x0020,                       /** Use POR values of CORCON */
    CORCON_MODE_ENABLEALLSATNORMAL_ROUNDBIASED = 0x00E2,    /** Enable saturation for ACCA, ACCB
                                                             *  and Dataspace write, enable normal
                                                             *  ACCA/ACCB saturation mode and set
                                                             *  rounding to Biased (conventional)
                                                             *  mode. Rest of CORCON settings are
                                                             *  set to the default POR values.
                                                             *  */
    CORCON_MODE_ENABLEALLSATNORMAL_ROUNDUNBIASED = 0x00E0,  /** Enable saturation for ACCA, ACCB
                                                             *  and Dataspace write, enable normal
                                                             *  ACCA/ACCB saturation mode and set
                                                             *  rounding to Unbiased (convergent)
                                                             *  mode. Rest of CORCON settings are
                                                             *  set to the default POR values.
                                                             *  */
    CORCON_MODE_DISABLEALLSAT_ROUNDBIASED = 0x0022,         /** Disable saturation for ACCA, ACCB
                                                             *  and Dataspace write and set
                                                             *  rounding to Biased (conventional)
                                                             *  mode. Rest of CORCON settings are
                                                             *  set to the default POR values.
                                                             *  */
    CORCON_MODE_DISABLEALLSAT_ROUNDUNBIASED = 0x0020,       /** Disable saturation for ACCA, ACCB
                                                             *  and Dataspace write and set
                                                             *  rounding to Unbiased (convergent)
                                                             *  mode. Rest of CORCON settings are
                                                             *  set to the default POR values.
                                                             *  */
    CORCON_MODE_ENABLEALLSATSUPER_ROUNDBIASED = 0x00F2,    /** Enable saturation for ACCA, ACCB
                                                             *  and Dataspace write, enable super
                                                             *  ACCA/ACCB saturation mode and set
                                                             *  rounding to Biased (conventional)
                                                             *  mode. Rest of CORCON settings are
                                                             *  set to the default POR values.
                                                             *  */
    CORCON_MODE_ENABLEALLSATSUPER_ROUNDUNBIASED = 0x00F0,  /** Enable saturation for ACCA, ACCB
                                                             *  and Dataspace write, enable super
                                                             *  ACCA/ACCB saturation mode and set
                                                             *  rounding to Unbiased (convergent)
                                                             *  mode. Rest of CORCON settings are
                                                             *  set to the default POR values.
                                                             *  */
} SYSTEM_CORCON_MODES;
// TODO Insert appropriate #include <>

// TODO Insert C++ class definitions if appropriate

// TODO Insert declarations

// Comment a function and leverage automatic documentation with slash star star
/**
    <p><b>Function prototype:</b></p>
  
    <p><b>Summary:</b></p>

    <p><b>Description:</b></p>

    <p><b>Precondition:</b></p>

    <p><b>Parameters:</b></p>

    <p><b>Returns:</b></p>

    <p><b>Example:</b></p>
    <code>
 
    </code>

    <p><b>Remarks:</b></p>
 */
// TODO Insert declarations or function prototypes (right here) to leverage 
// live documentation

#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */

    // TODO If C++ is being used, regular C code needs function names to have C 
    // linkage so the functions can be used by the c code. 

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif	/* XC_HEADER_TEMPLATE_H */


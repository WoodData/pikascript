/*!
 * @file       apm32e10x_eint.c
 *
 * @brief      This file provides all the EINT firmware functions
 *
 * @version    V1.0.0
 *
 * @date       2021-07-26
 *
 */

#include "apm32e10x_eint.h"

/** @addtogroup Peripherals_Library Standard Peripheral Library
  @{
*/

/** @addtogroup EINT_Driver EINT Driver
  @{
*/

/** @addtogroup EINT_Fuctions Fuctions
  @{
*/

/*!
 * @brief        Reset the EINT peripheral registers to their default reset values.
 *
 * @param        None
 *
 * @retval       None
 */
void EINT_Reset(void)
{
    EINT->IMASK = 0x00000000;
    EINT->EMASK = 0x00000000;
    EINT->RTEN  = 0x00000000;
    EINT->FTEN  = 0x00000000;
    EINT->IPEND = 0x000FFFFF;
}

/*!
 * @brief        Configure the EINT
 *
 * @param        eintConfig: pointer to a EINT_Config_T structure.
 *
 * @retval       None
 */
void EINT_Config(EINT_Config_T* eintConfig)
{
    uint32_t temp = 0;
    temp = (uint32_t)EINT_BASE;

    if(eintConfig->lineCmd != DISABLE)
    {
        EINT->IMASK &= ~eintConfig->line;
        EINT->EMASK &= ~eintConfig->line;

        temp += eintConfig->mode;
        *(__IOM uint32_t *) temp |= eintConfig->line;

        EINT->RTEN &= ~eintConfig->line;
        EINT->FTEN &= ~eintConfig->line;

        if (eintConfig->trigger == EINT_TRIGGER_RISING_FALLING)
        {
            EINT->RTEN |= eintConfig->line;
            EINT->FTEN |= eintConfig->line;
        }
        else
        {
            temp = (uint32_t)EINT_BASE;
            temp += eintConfig->trigger;

            *(__IOM uint32_t *) temp |= eintConfig->line;
        }
    }
    else
    {
        temp += eintConfig->mode;

        *(__IOM uint32_t *) temp &= ~eintConfig->line;
    }
}

/*!
 * @brief    Select Software interrupt on EINT line
 *
 * @param    line: specifies the EINT lines.
 *                 This parameter can be any combination of EINT_LINE_T(can be from 0 to 18)
 *
 * @retval   None
 */
void EINT_SelectSWInterrupt(uint32_t line)
{
    EINT->SWINTE |= line;
}

/*!
 * @brief        Read the specified EINT_Line flag
 *
 * @param        line: Select the EINT_Line.
 *                     This parameter can be one of EINT_LINE_T(can be from 0 to 18)
 *
 * @retval       status: The new state of flag (SET or RESET)
 */
uint8_t EINT_ReadStatusFlag(EINT_LINE_T line)
{
    uint8_t status = RESET;

    if((EINT->IPEND & line) != (uint32_t)RESET)
    {
        status = SET;
    }
    else
    {
        status = RESET;
    }
    return status;
}

/*!
 * @brief        Clears the EINT_Line pending bits
 *
 * @param        line: Select the EINT_Line.
 *                     This parameter can be any combination of EINT_LINE_T(can be from 0 to 18)
 *
 * @retval        None
 */
void EINT_ClearStatusFlag(uint32_t line)
{
    EINT->IPEND = line;
}

/*!
 * @brief        Read the specified EINT_Line Interrupt Flag.
 *
 * @param        line: Select the EINT_Line.
 *                     This parameter can be one of EINT_LINE_T(can be from 0 to 18)
 *
 * @retval        None
 */
uint8_t EINT_ReadIntFlag(EINT_LINE_T line)
{
    uint8_t status = RESET;
    uint32_t enablestatus = 0;

    enablestatus = EINT->IMASK & line;

    if((EINT->IPEND & line) != ((uint32_t)RESET) && (enablestatus != (uint32_t)RESET))
    {
        status = SET;
    }
    else
    {
        status = RESET;
    }
    return status;
}

/*!
 * @brief        Clears the EINT_Line pending bits
 *
 * @param        line: Select the EINT_Line
 *                     This parameter can be any combination of EINT_LINE_T(can be from 0 to 18)
 *
 * @retval        None
 */
void EINT_ClearIntFlag(uint32_t line)
{
    EINT->IPEND = line;
}

/**@} end of group EINT_Fuctions*/
/**@} end of group EINT_Driver */
/**@} end of group Peripherals_Library*/

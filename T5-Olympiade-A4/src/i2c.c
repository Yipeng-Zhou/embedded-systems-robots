// Dies ist eine verlinkte Datei

/*

 * i2c.c
 *
 *  Created on: 13.12.2013
 *      Author: Coy
 *		Copyright: TUM MiMed 2013
 *		beaglebone EGR
 *		Version: 1.0
 */
#include <hw_types.h>
#include <hw_hsi2c.h>
#include "delay_ms.h"
#include "i2c.h"
#include "EGR_Cape.h"
#include "Conf_mod.h"

volatile unsigned int transCount;
volatile unsigned int recvCount;
volatile unsigned char dataSnd[50];
volatile unsigned char dataRecv[50];

void
initi2c()
{
  //Enable interrupts
  IntMasterIRQEnable();

  IntAINTCInit();

  IntRegister(SYS_INT_I2C1INT, I2CIsr);

  IntPrioritySet(SYS_INT_I2C1INT, 0, AINTC_HOSTINT_ROUTE_IRQ);

  IntSystemEnable(SYS_INT_I2C1INT);

  //Pinmux I2C_SCL and I2C_SDA
  PinMuxing(CONF_I2C_SCL, PULL_DISABLE, PULL_UP, 2);
  PinMuxing(CONF_I2C_SDA, PULL_DISABLE, PULL_UP, 2);

  //Enable I2C1 Module Clock
  HWREG(SOC_PRCM_REGS + CM_PER_I2C1_CLKCTRL) |= CM_PER_I2C1_CLKCTRL_MODULEMODE_ENABLE;

  //Disable I2C Module for Configuration
  HWREG(SOC_I2C_1_REGS + I2C_CON) &= ~(1<<15);

  //Prescale functional clock to internal i2c clock
  HWREG(SOC_I2C_1_REGS + I2C_PSC) |= (3<<0);

  //Comparematch for low-time
  HWREG(SOC_I2C_1_REGS + I2C_SCLL) |= (8<<0);

  //Comparematch for high-time
  HWREG(SOC_I2C_1_REGS + I2C_SCLH) |= (10<<0);

  //Enable I2C Module again
  HWREG(SOC_I2C_1_REGS + I2C_CON) |= (1<<15);

}

void
writetoi2c(unsigned int sla_address, unsigned char*data, unsigned char count, char stp)
{
  int i = 0;

  HWREG(SOC_I2C_1_REGS + I2C_SA) = sla_address;

  for (i = 0; i < count; ++i)
    {
      dataSnd[i] = data[i];
    }

  //set datalength
  HWREG(SOC_I2C_1_REGS + I2C_CNT) = (unsigned char) count;

  //reset golbal variable
  transCount = 0;

  //enable master transmit Mode
  HWREG(SOC_I2C_1_REGS + I2C_CON) = I2C_CON_TRX | I2C_CON_MST | I2C_CON_I2C_EN;

  //enable transmit interrupt-trigger
  HWREG(SOC_I2C_1_REGS + I2C_IRQENABLE_SET) |= I2C_IRQSTATUS_XRDY;

  //Start communiction with Startcondition
  HWREG(SOC_I2C_1_REGS + I2C_CON) |= I2C_CON_STT;
  HWREG(SOC_I2C_1_REGS + I2C_CON) |= ((stp & 0x1) << I2C_CON_STP_SHIFT);

  //Wait for all data to be transmitted
  while (transCount < count){};

  //Disable Interrupt
  HWREG(SOC_I2C_1_REGS + I2C_IRQENABLE_CLR) = I2C_IRQSTATUS_XRDY;

  //Wait for Bus to close Communication
  delay_us(50);

}

void
readfromi2c(unsigned int sla_address,unsigned char*data, unsigned char count, char stp)
{
	int i = 0;;
  // TODO: implement read function here

  //set slave address
	HWREG(SOC_I2C_1_REGS + I2C_SA) = sla_address;
 
  //set length of expected data
	HWREG(SOC_I2C_1_REGS + I2C_CNT) = (unsigned char) count;
  
  //reset global variables
	recvCount = 0;

  //configure I2C controller in Master Receiver mode
	HWREG(SOC_I2C_1_REGS + I2C_CON) = I2C_CON_MST | I2C_CON_I2C_EN;

  //enable receive interrupt-trigger
	HWREG(SOC_I2C_1_REGS + I2C_IRQENABLE_SET) |= I2C_IRQSTATUS_RRDY;

  //generate Start Condition over I2C bus
	HWREG(SOC_I2C_1_REGS + I2C_CON) |= I2C_CON_STT;
	HWREG(SOC_I2C_1_REGS + I2C_CON) |= ((stp & 0x1) << I2C_CON_STP_SHIFT);

  //wait till data read
  while (recvCount < count){};

  //disable receive interrupt
  	HWREG(SOC_I2C_1_REGS + I2C_IRQENABLE_CLR) = I2C_IRQSTATUS_RRDY;
	
  // copy received data from slave
  for (i = 0; i < count; i++)
    {
	  data[i] = dataRecv[i];
    }
	
  //Wait till Bus is free
  while ((HWREG(SOC_I2C_1_REGS + I2C_IRQSTATUS_RAW) & I2C_IRQSTATUS_RAW_BB)!= 0);

  delay_us(50);
}

void
CleanUpInterrupts(void)
{
  //clear and disable all Interrupts
  HWREG(SOC_I2C_1_REGS + I2C_IRQENABLE_SET) |= 0x7FF;
  HWREG(SOC_I2C_1_REGS + I2C_IRQSTATUS) = 0x7FF;
  HWREG(SOC_I2C_1_REGS + I2C_IRQENABLE_CLR) = 0x7FF;
}

void
I2CIsr(void)
{
  unsigned int status = 0;

  /* Get only Enabled interrupt status */
  status = ((HWREG(SOC_I2C_1_REGS + I2C_IRQSTATUS) ));

  /*
   ** Clear all enabled interrupt status except receive ready and
   ** transmit ready interrupt status
   */HWREG(SOC_I2C_1_REGS + I2C_IRQSTATUS) = (status
      & ~(I2C_IRQSTATUS_RRDY | I2C_IRQSTATUS_XRDY));

  if (status & I2C_IRQSTATUS_XRDY)
    {
      /* Put data to data transmit register of i2c */
      HWREG(SOC_I2C_1_REGS + I2C_DATA) = dataSnd[transCount++];
      /* Clear Transmit interrupt status */
      HWREG(SOC_I2C_1_REGS + I2C_IRQSTATUS) = I2C_IRQSTATUS_XRDY;
    }
	
  if (status & I2C_IRQSTATUS_RRDY)
    {
	  // TODO: implement I2CIsr for data receive
	  /* Receive data from data receive register */
	  dataRecv[recvCount++] = HWREG(SOC_I2C_1_REGS + I2C_DATA);
	  /* Clear receive ready interrupt status */
	  HWREG(SOC_I2C_1_REGS + I2C_IRQSTATUS) = I2C_IRQSTATUS_RRDY;

    }
}


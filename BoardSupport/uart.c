#include "lpc177x_8x_uart.h"
#include "lpc177x_8x_pinsel.h"
#include "lpc177x_8x_timer.h"
#include "lpc177x_8x_gpdma.h"
#include "uart.h"
#include "timer.h"
#include "dma.h"
#include <ucos_ii.h>
uint8_t Buffer0[23]  = "uart0 init success\n\r";
uint8_t Buffer2[23]  = "uart2 init success!\n\r"; 

// __IO uint8_t UART_RX[128];//DMA_UART�Ĵ��ڽ��ջ�����
//  OS_EVENT *QSem;//������Ϣ����ָ��
//  OS_MEM   *PartitionPt;
//extern void *MsgQeueTb[10];//������Ϣָ�����飬���г���Ϊ10
//extern uint8_t Partition[10][128];//[SIZE];
void lpc1788_Uart_Init(int port)//LPC1788_Uart_Init
{
   UART_CFG_Type UARTConfigStruct;	/* UART���� */
   UART_FIFO_CFG_Type UARTFIFOConfigStruct;	/* UART FIFO���� */
   switch(port)
   {
    case 0:
       PINSEL_ConfigPin(0,2,1);
       PINSEL_ConfigPin(0,3,1);break;
    case 2:
       PINSEL_ConfigPin(0,10,1);
       PINSEL_ConfigPin(0,11,1);break;
    default:break;
	}
	UART_ConfigStructInit(&UARTConfigStruct);/* ��ʼ��UARTConfigStruct�ṹ��Ϊ��������Ϊ115200��8λ���ݣ�����żУ�飬1λֹͣλ */
	UARTConfigStruct.Baud_rate = 115200;
	UART_Init((UART_ID_Type)port, &UARTConfigStruct);	/* ��UARTConfigStruct�����Ĳ�����ʼ��UART���� */
	UART_FIFOConfigStructInit(&UARTFIFOConfigStruct);

	UARTFIFOConfigStruct.FIFO_DMAMode = ENABLE;//ΪUARTʹ��DMA
	UART_FIFOConfig(UART_0, &UARTFIFOConfigStruct);// ΪUART0�����ʼ��FIFO

	UART_TxCmd((UART_ID_Type)port, ENABLE);/* ʹ��UART���� */
	
	UART_IntConfig(UART_0, UART_INTCFG_RBR, ENABLE);
	UART_IntConfig(UART_0, UART_INTCFG_RLS, ENABLE);
	NVIC_DisableIRQ(UART0_IRQn);
		
}


 void UART_Config(unsigned char channel)
{
   UART_CFG_Type UARTConfigStruct1;
   UART_FIFO_CFG_Type UARTFIFOConfigStruct1;
   PINSEL_ConfigPin(0,10,1);
   PINSEL_ConfigPin(0,11,1);
   UART_ConfigStructInit(&UARTConfigStruct1);
   UARTConfigStruct1.Baud_rate = 38400;
   UART_Init((UART_ID_Type)channel, &UARTConfigStruct1);
   
   UART_FIFOConfigStructInit(&UARTFIFOConfigStruct1);
   UARTFIFOConfigStruct1.FIFO_DMAMode = ENABLE;
   UART_FIFOConfig((UART_ID_Type)channel, &UARTFIFOConfigStruct1);
   UART_TxCmd((UART_ID_Type)channel, ENABLE);
}

// static void DMA_Config(unsigned char channel)
// {
// 	GPDMA_Channel_CFG_Type GPDMACfg; 
// // 	GPDMA_Init();
// 	NVIC_DisableIRQ (DMA_IRQn);
// 	NVIC_SetPriority(DMA_IRQn, ((0x01<<3)|0x01));
// 	GPDMACfg.ChannelNum = 1;
// 	GPDMACfg.SrcMemAddr = 0;// Source memory - don't care
// 	GPDMACfg.DstMemAddr = (uint32_t) &UART2_RX;// Destination memory
// 	GPDMACfg.TransferSize = sizeof(UART2_RX);// Transfer size
// 	GPDMACfg.TransferWidth = 0;// Transfer width - don't care
// 	GPDMACfg.TransferType = GPDMA_TRANSFERTYPE_P2M;// Transfer type
// 	GPDMACfg.SrcConn = GPDMA_CONN_UART2_Rx;// Source connection
// 	GPDMACfg.DstConn = 0;// Destination connection - don't care
// 	GPDMACfg.DMALLI = 0;// Linker List Item - unused
// 	GPDMA_Setup(&GPDMACfg);
// 	NVIC_EnableIRQ (DMA_IRQn);
// 	GPDMA_ChannelCmd(1, ENABLE);
// }

void USER_Init(void)
{	
	GPDMA_Init();
	lpc1788_Uart_Init(0);/* ��ʼ������Uart0 ʹ��DMAͨ��0*/	
 lpc1788_DMA_Init();  
//	printf("uart0 init success!\n\r\r");	
	UART_Send((UART_ID_Type)0, Buffer0,sizeof(Buffer0),BLOCKING);

     
//	UART_Send((UART_ID_Type)2,Buffer2,sizeof(Buffer2),BLOCKING);///* ��ӡһ���ַ�������Uart2 */
////	lpc1788_TIMER_Init();
}





PUTCHAR_PROTOTYPE//�ض���C printf������Uart0
{
	UART_Send((UART_ID_Type)UART_0, (uint8_t*) &ch, 1, BLOCKING);  /* ����һ���ַ���UART */
	while (UART_CheckBusy((UART_ID_Type)UART_0) == SET);/* ���ڷ�����ɣ�THR����Ϊ��*/
  return ch;
}

/************************************* End *************************************/

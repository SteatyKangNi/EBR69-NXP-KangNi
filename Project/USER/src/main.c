/*********************************************************************************************************************
 * COPYRIGHT NOTICE
 * Copyright (c) 2020,逐飞科技
 * All rights reserved.
 * 技术讨论QQ群：一群：179029047(已满)  二群：244861897(已满)  三群：824575535
 *
 * 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
 * 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
 *
 * @file       		main
 * @company	   		成都逐飞科技有限公司
 * @author     		逐飞科技(QQ790875685)
 * @version    		查看doc内version文件 版本说明
 * @Software 		MDK FOR C251 V5.60
 * @Target core		STC32G12K128
 * @Taobao   		https://seekfree.taobao.com/
 * @date       		2020-12-18
 ********************************************************************************************************************/

#include "headfile.h"


/*
 *关于内核频率的设定，可以查看board.h文件
 *在board_init中,已经将P54引脚设置为复位
 *如果需要使用P54引脚,可以在board.c文件中的board_init()函数中删除SET_P54_RESRT即可
 */
 
//该例程仅支持带方向的编码器解码

//定义脉冲引脚
#define SPEEDL_PLUSE   CTIM0_P34
#define SPEEDR_PLUSE   CTIM3_P04
//定义方向引脚
#define SPEEDL_DIR     P35
#define SPEEDR_DIR     P53

#define DIR_1 P63
#define DIR_2 P61
#define PWM_1 PWMA_CH2P_P62
#define PWM_2 PWMA_CH1P_P60

int16 templ_pluse = 0;
int16 tempr_pluse = 0;
uint8  dir = 0;
int16 dutyl = 2000;
int16 dutyr = 2000;
int16 spd_set = 1700;
int16 errorr = 0;
int16 errorl =  0;
int16 errorr_last = 0;
int16 errorl_last =  0;
int16 errorr_i = 0;
int16 errorl_i =  0;
int16 errorr_d = 0;
int16 errorl_d =  0;
uint8 data_to_send[20];
uint16 adc_data[12];

void main()
{
    WTST = 0;               //设置程序代码等待参数，赋值为0可将CPU执行程序的速度设置为最快
    
	DisableGlobalIRQ();		//关闭总中断
	
    //sys_clk可选值:35000000，30000000, 27000000. 24000000, 22118400, 20000000, 18432000, 12000000, 11059200, 6000000, 5529600。
    //设置系统频率，此频率需要跟STC-ISP软件中的 <输入用户程序运行的IRC频率>选项的频率一致。
    //如果频率设置不对，将会导致串口的数据不正常,PWM的工作不正常等等。
    sys_clk = 35000000;     //设置系统频率为35MHz
    
	board_init();		//初始化
	
	pwm_init(PWM_1, 17000, 0); //初始化PWM1  使用P60引脚  初始化频率为17Khz
	pwm_init(PWM_2, 17000, 0); //初始化PWM2  使用P62引脚  初始化频率为17Khz
    
	ctimer_count_init(SPEEDL_PLUSE);	//初始化定时器0作为外部计数
	ctimer_count_init(SPEEDR_PLUSE);	//初始化定时器3作为外部计数
//	lcd_init();							//1.8寸TFT初始化
	ips114_init();						//初始化1.14寸IPS屏幕
//	oled_init_spi();					//OLED初始化
	adc_init(ADC_P01, ADC_SYSclk_DIV_2);	
    adc_init(ADC_P02, ADC_SYSclk_DIV_2);	
    adc_init(ADC_P03, ADC_SYSclk_DIV_2);
	adc_init(ADC_P04, ADC_SYSclk_DIV_2);	
    adc_init(ADC_P05, ADC_SYSclk_DIV_2);	
    adc_init(ADC_P06, ADC_SYSclk_DIV_2);		
	
	EnableGlobalIRQ();					//开启总中断
    while(1)
	{
        //读取采集到的编码器脉冲数
        templ_pluse = ctimer_count_read(SPEEDL_PLUSE);
		tempr_pluse = ctimer_count_read(SPEEDR_PLUSE);

        //计数器清零
        ctimer_count_clean(SPEEDL_PLUSE);
		ctimer_count_clean(SPEEDR_PLUSE);

		
		//在TFT上显示，需要初始化1.8寸TFT屏幕，才能使用。
//		lcd_showstr(0, 0, "pluse_l =");			lcd_showint16(8*10, 0, templ_pluse);
//		lcd_showstr(0, 1, "pluse_r =");			lcd_showint16(8*10, 1, tempr_pluse);

		//在1.14IPS屏幕上显示，需要初始化1.14寸ips屏幕，才能使用。
		// ips114_showstr(0, 0, "pluse_l =");		ips114_showint16(8*10, 0, templ_pluse);
		// ips114_showstr(0, 1, "pluse_r =");		ips114_showint16(8*10, 1, tempr_pluse);
		ips114_showint16(0,1, adc_data[0]);
		ips114_showint16(8*10,1, adc_data[1]);
		ips114_showint16(0,2, adc_data[2]);
		ips114_showint16(8*10,2, adc_data[3]);
		ips114_showint16(0,3, adc_data[4]);
		ips114_showint16(8*10,3, adc_data[5]);
		
		//在OLED屏幕上显示，需要初始化OLED屏幕，才能使用。
//		oled_p6x8str_spi(0, 0, "pluse_l =");	oled_int16_spi(8*10, 0, templ_pluse);
//		oled_p6x8str_spi(0, 1, "pluse_r =");	oled_int16_spi(8*10, 1, tempr_pluse);、
		//延时100ms
		adc_data[0] = adc_once(ADC_P10, ADC_10BIT)/82;	//采集一次ADC，精度10位
		adc_data[1] = adc_once(ADC_P11, ADC_10BIT)/82;	//采集一次ADC，精度10位
		adc_data[2] = adc_once(ADC_P54, ADC_10BIT)/82;	//采集一次ADC，精度10位
		adc_data[3] = adc_once(ADC_P13, ADC_10BIT)/82;	//采集一次ADC，精度10位
		adc_data[4] = adc_once(ADC_P14, ADC_10BIT)/82;	//采集一次ADC，精度10位
		adc_data[5] = adc_once(ADC_P15, ADC_10BIT)/82;	//采集一次ADC，精度10位
		delay_ms(100);
		
		// errorr_last = errorr;
        // errorl_last =  errorl;
		// errorl = spd_set - templ_pluse;
		// errorr = spd_set - tempr_pluse;
		// errorl_i = errorl_i + errorl;
		// errorr_i = errorr_i + errorr;
		// errorr_d = errorr_last - errorr;
		// errorl_d = errorl_last - errorl;
		// dutyl = dutyl + 0.02*errorl + 0.001*errorl_i + 0.01*errorl_d;
		// dutyr = dutyr + 0.02*errorr + 0.001*errorr_i + 0.01*errorr_d;
		// DIR_1 = 0;
		// pwm_duty(PWM_2, dutyl);
		// DIR_2 = 0;
		// pwm_duty(PWM_1, dutyr);
		// ips114_showint16(0, 2, dutyl);
		// ips114_showint16(0, 3, dutyr);
	    // printf("%d,%d\n",templ_pluse,tempr_pluse);
	}
}
                      

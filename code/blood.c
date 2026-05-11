#include "blood.h"
#include "algorithm.h"
#include "MAX30102.h"  // 必须包含底层的头文件，我们要调用它的读写函数
#include <math.h>
#include <stdio.h>

// --- 大水池和标志位 (加了 static，锁死在这个文件里) ---
#define FFT_N 512
static struct compx s1[FFT_N+16]; 
static struct compx s2[FFT_N+16]; 

static volatile uint16_t g_fft_index = 0;  
static volatile uint8_t  fft_ready_flag = 0; 

static int   my_heart_rate = 0;     
static float my_spo2 = 0.0;       

// --- 接口实现 ---

void Blood_App_Init(void)
{
    Health_Init(); // 调用 MAX30102 的初始化
}

// 专门给中断调用的“收数据”函数
void Blood_Collect_Data_ISR(void)
{
    uint8_t  wr_ptr=0, rd_ptr=0;
    int8_t   num_samples=0;

    Health_ReadData(REG_FIFO_WR_PTR, &wr_ptr);
    Health_ReadData(REG_FIFO_RD_PTR, &rd_ptr);
    
    num_samples = wr_ptr - rd_ptr;
    if(num_samples < 0)  num_samples = num_samples + 32;

    if(num_samples > 0)
    {
        uint8_t temp_buf[6]; 
        for (int i = 0; i < num_samples; i++) 
        {
            Health_ReadDatas(REG_FIFO_DATA, temp_buf, 6); 

            uint32_t red_val = ((uint32_t)temp_buf[0]<<16 | (uint32_t)temp_buf[1]<<8 | (uint32_t)temp_buf[2]) & 0x03FFFF;
            uint32_t ir_val  = ((uint32_t)temp_buf[3]<<16 | (uint32_t)temp_buf[4]<<8 | (uint32_t)temp_buf[5]) & 0x03FFFF;

            // 存入大水池
            if(g_fft_index < FFT_N && fft_ready_flag == 0)
            {
                s1[g_fft_index].real = red_val;
                s1[g_fft_index].imag = 0;
                s2[g_fft_index].real = ir_val;
                s2[g_fft_index].imag = 0;
                g_fft_index++; 
                
                if(g_fft_index >= FFT_N)
                {
                    fft_ready_flag = 1; // 凑齐 512 个，举旗！
                }
            }
        }
    }
    // 清除中断标志位
    uint8_t temp;
    Health_ReadData(REG_INTR_STATUS_1, &temp);
}

// 专门在 while(1) 里算心率的函数
void Blood_Process_Task(void)
{
    if(fft_ready_flag == 1)
    {
        float dc_red = 0, dc_ir = 0;
        float ac_red = 0, ac_ir = 0;
        int i;
        
        for (i = 0; i < FFT_N; i++) {
            dc_red += s1[i].real;
            dc_ir  += s2[i].real;
        }
        dc_red = dc_red / FFT_N;
        dc_ir  = dc_ir / FFT_N;
        
        for (i = 0; i < FFT_N; i++) {
            s1[i].real -= dc_red; 
            s2[i].real -= dc_ir; 
        }
        
        FFT(s1);
        FFT(s2);
        
        for(i = 0; i < FFT_N; i++) {
            s1[i].real = sqrtf(s1[i].real * s1[i].real + s1[i].imag * s1[i].imag);
            s2[i].real = sqrtf(s2[i].real * s2[i].real + s2[i].imag * s2[i].imag); 
        }
        
        for (i = 1; i < FFT_N; i++) {
            ac_red += s1[i].real;
            ac_ir  += s2[i].real;
        }
        
        int s1_max_index = find_max_num_index(s1, 30);
        
        my_heart_rate = (int)(60.0 * ((100.0 * s1_max_index) / 512.0));
        
        float R = (ac_ir * dc_red) / (ac_red * dc_ir);
        my_spo2 = -45.060 * R * R + 30.354 * R + 94.845;
        if(my_spo2 > 99.99) my_spo2 = 99.99;
        if(dc_red < 30000) 
        {
            printf("传感器空闲：请将手指平放在传感器上...\r\n");
            // 将历史心率清零，防止界面残留旧数据
            my_heart_rate = 0;
            my_spo2 = 0.0;
        }
        else 
        {
            // 手指确实放上去了，并且算出了合理的心率
            if(my_heart_rate > 40 && my_heart_rate < 180) {
                printf("? 测算成功! 心率: %d BPM, 血氧: %.2f %%\r\n", my_heart_rate, my_spo2);
            } else {
                printf("?? 正在测算，请保持手指平稳...\r\n");
            }
        }
        if(my_heart_rate > 40 && my_heart_rate < 180) {
            printf("心率: %d BPM, 血氧: %.2f %%\r\n", my_heart_rate, my_spo2);
        }
        
        // 清空重置
        g_fft_index = 0;
        fft_ready_flag = 0;
    }
}

// --- 提供给 LVGL 屏幕显示用的函数 ---
int Get_HeartRate(void) { return my_heart_rate; }
float Get_SpO2(void)    { return my_spo2; }
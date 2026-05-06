#ifndef __LISTENAI_CONFIG_MGR_H__
#define __LISTENAI_CONFIG_MGR_H__

#include "ls_app_uart.h"
#include "ls_app_cfg_def.h"

#define APP_CFG_RES_ADDR            (8)

typedef struct
{
	int16_t adc_again;  // 模拟增益
    int16_t adc_dgain;  // 数字增益
} adc_gain_t;

typedef struct
{
    uint8_t *audios;        // 音频
	uint8_t audio_size;     // 需要播放的音频列表的大小
} audio_info_t;

typedef struct
{
    uint8_t level[10];      // 音量等级，0-100
	uint8_t level_size;     // 音量等级数量
    uint8_t default_vol;    // 上电默认音量
    audio_info_t max_vol_tip;
    audio_info_t min_vol_tip;
    audio_info_t med_vol_tip;
    audio_info_t up_vol_tip;
    audio_info_t down_vol_tip;
    audio_info_t adjmax_vol_tip;
    audio_info_t adjmin_vol_tip;
} volume_info_t;

typedef struct
{
	uint8_t *addr;
	uint32_t size;
} tone_info_t;

// 持久化信息
typedef struct
{
    uint8_t volume;                   // 音量是否需要持久化
    uint8_t voice;                    // 语音开关是否需要持久化
    uint8_t wakeup;                   // 唤醒词是否需要持久化
} persisted_info_t;

typedef struct
{
    uint8_t wake_sensitivity;       // 0 1 2
    uint8_t wake_min_words;         //
    uint8_t wake_max_words;         //
    uint8_t wake_reg_max;
    uint8_t wake_repeat_count;         //
    uint8_t wake_retry_count;         //
    uint8_t cmd_sensitivity;       // 0 1 2
    uint8_t cmd_min_words;         //
    uint8_t cmd_max_words;         //
    uint8_t cmd_reg_max;
    uint8_t cmd_repeat_count;         //
    uint8_t cmd_retry_count;         //
} study_user_cfg_t;

// 配置解析初始化
int cfg_parser_init();

// 获取配置中日志等级
const char *cfg_parser_get_version();

// 获取配置中adc增益
adc_gain_t cfg_parser_get_adc_gain();

// 获取配置中日志等级
int cfg_parser_get_loglev();

// 获取配置中音量信息
int cfg_parser_get_vol_info(volume_info_t *vol_info);

// 获取配置中超时时间(单位s)
int cfg_parser_get_timeout_time();

// 获取配置中持久化值
persisted_info_t cfg_parser_get_persisted_info();

// 播放开机提示语和发送开机协议
int cfg_parser_welcome_handler();

// 超时处理函数
int cfg_parser_timeout_handler();

// 串口接收消息处理函数
int cfg_parser_recv_data_handler(const ls_uart_msg_t *uart_msg);

// 唤醒识别处理函数
int cfg_parser_keyword_handler(const char *keyword);

// 注册过程中唤醒识别处理函数
int cfg_parser_register_inprogress_keyword_handler(const char *keyword);

// 获取超时提示音
int cfg_parser_get_timeout_audio(audio_info_t *audio_info);

// 获取自学习功能开关
int cfg_parser_get_study_switch();

int cfg_parser_get_study_user_cfg(study_user_cfg_t *user_cfg);

int cfg_parser_study_reply_by_label(reply_label_t reply_label);

int cfg_parser_study_get_audio_by_label(reply_label_t reply_label, audio_info_t *audio_info);

typedef struct reg_word_info
{
    uint8_t keyword_size;   // 退出识别模式命令词拼音大小
	uint8_t audio_size;     // 需要播放的音频列表的大小
    char *keywords_ptr;     // 唤醒词拼音偏移地址
    char *play_audio_ptr;  // 需要播放的音频列表的偏移地址
} reg_word_info_t;

int cfg_parser_study_get_reg_wake_words(reg_word_info_t *reg_word_info);

int cfg_parser_study_get_reg_cmd_count();

int cfg_parser_study_get_reg_cmd_words_by_index(int index, reg_word_info_t *reg_word_info);

void clear_voice_reg_flag(int reset_waked);

int cfg_parser_get_voice_reg_type();
int cfg_parser_get_voice_reg_word_cnt();
int cfg_parser_get_voice_reg_cmd_cnt();
int cfg_parser_switch_wakeword_info_to_send_protocol();
int cfg_parser_get_default_keyword_id();
pa_config_item_t cfg_parser_get_pa_config();

uart_config_t cfg_parser_get_uart_config();

#endif
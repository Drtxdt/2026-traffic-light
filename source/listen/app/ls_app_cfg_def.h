#ifndef __LISTENAI_CONFIG_DEF_H__
#define __LISTENAI_CONFIG_DEF_H__
#include <stdint.h>

#define VOLUME_MAX_LEVEL           	(10)    // 音量等级最大值
#define VOLUME_MIN_LEVEL           	(0)     // 音量等级最小值
#define VOLUME_DEFAULT_LEVEL        (4)     // 音量默认等级
#define ALGO_KEYWORD_MAX_LEN        (64)    // 唤醒or命令词最大长度
#define PROTOCOL_DATA_LEN_MAX       (32)    // 协议数据最大长度
#define PLAY_AUDIO_COUNT_MAX        (10)    // 最大播放音频数量
#define VERSION_STRING_LEN_MAX      (128)    // 配置版本字符串最大长度
#define VOLUME_CTRL_CMD_CNT_MAX     (10)     // 音量控制命令最大数量
#define WAKEWORD_SWITCH_LIST_MAX    (20)     // 唤醒词切换列表最大数量
#define VOICEREG_CONTROL_CMD_COUNT_MAX  (20)    // 语音注册控制命令最大数量
#define VOICEREG_OTHER_REPLY_COUNT_MAX  (30)    // 语音注册其他播报提示音最大数量
#define VOICEREG_REGWORD_COUNT_MAX      (30)    // 语音注册命令词或唤醒词最大数量

/** pa功能配置 **/
#define CONTROL_IO_PAD_MAX_PIN (12)
typedef enum {
    CONTROL_IO_PAD_A = 0,
    CONTROL_IO_PAD_B
} ctl_io_pad_t;

typedef struct pa_config_item
{
    uint8_t enable;         // 是否使能pa功能
    ctl_io_pad_t ctl_io_pad;// 控制io所在pad
    uint8_t ctl_io_num;     // 控制io编号
    uint8_t enable_level;   // 使能电平
    uint32_t hold_time;     // 保持时间，单位ms
} pa_config_item_t;

// @brief 串口配置项
typedef struct
{
	uint8_t trace_uart;     // 日志串口号 0:uart0 1:uart1
	uint8_t uport_uart;     // 通信串口号 0:uart0 1:uart1
	uint32_t trace_baud;    // 日志串口波特率
	uint32_t uport_baud;    // 通信串口波特率
} uart_config_t;

/** 通用配置 **/
typedef enum
{
    REPLY_SWITCH_OFF = 0,   // 播报开关关
    REPLY_SWITCH_ON = 1     // 播报开关开
} reply_switch_t;

typedef struct general_item
{
    uint8_t version[VERSION_STRING_LEN_MAX];
    uint8_t log_level;                          // 日志等级0-4
    uint8_t persisted_volume;                   // 音量是否需要持久化 1:需要 0:不需要
    uint8_t persisted_voice;                    // 语音开关是否需要持久化
    uint8_t persisted_wakeup;                   // 唤醒词是否需要持久化
    reply_switch_t reply_switch;                // 播报开关
	int16_t adc_again;							// 模拟增益
    int16_t adc_dgain;							// 数字增益
} general_item_t;

/** 欢迎语配置 **/
typedef struct welcome_item
{
    uint8_t send_pro_len;       // 发送协议大小
    uint8_t recv_pro_len;       // 接收协议大小
	uint8_t audio_size;         // 需要播放的音频列表的大小
    uint8_t play_type;          // 主动还是被动0-主动，非0-被动
	int8_t send_pro_buf[PROTOCOL_DATA_LEN_MAX];    // 发送协议
	int8_t recv_pro_buf[PROTOCOL_DATA_LEN_MAX];    // 接收协议
    uint8_t play_audio[PLAY_AUDIO_COUNT_MAX];      // 需要播放的音频列表
} welcome_item_t;

/** 超时配置 **/
typedef struct timeout_item
{
    uint8_t send_pro_len;   // 发送协议大小
    uint8_t recv_pro_len;   // 接收协议大小
	uint8_t audio_size;     // 需要播放的音频列表的大小
    uint8_t play_type;      // 主动还是被动0-主动，非0-被动
    uint32_t time;          // 超时时间，单位秒
    uint8_t play_audio[PLAY_AUDIO_COUNT_MAX];      // 需要播放的音频列表
	int8_t send_pro_buf[PROTOCOL_DATA_LEN_MAX];    // 发送协议
	int8_t recv_pro_buf[PROTOCOL_DATA_LEN_MAX];    // 接收协议
} timeout_item_t;

/** 唤醒词配置 **/
typedef struct wakeup_item
{
    uint8_t keyword_size;   // 唤醒词拼音大小
    uint8_t send_pro_len;   // 发送协议大小
	uint8_t audio_size;     // 需要播放的音频随机列表的大小
    uint8_t play_type;      // 主动还是被动0-主动，非0-被动
	int8_t keywords[ALGO_KEYWORD_MAX_LEN];         // 唤醒词拼音
    int8_t send_pro_buf[PROTOCOL_DATA_LEN_MAX];    // 发送协议
    uint8_t play_audio[PLAY_AUDIO_COUNT_MAX];      // 需要播放的音频随机列表
} wakeup_item_t;

/** 命令词配置 **/
typedef struct command_item
{
    uint8_t keyword_size;   // 唤醒词拼音大小
    uint8_t send_pro_len;   // 发送协议大小
    uint8_t recv_pro_len;   // 接收协议大小
	uint8_t audio_size;     // 需要播放的音频列表的大小
    uint8_t play_type;      // 主动还是被动0-主动，非0-被动
    int8_t keywords[ALGO_KEYWORD_MAX_LEN];         // 唤醒词拼音
    int8_t send_pro_buf[PROTOCOL_DATA_LEN_MAX];    // 发送协议
    int8_t recv_pro_buf[PROTOCOL_DATA_LEN_MAX];    // 接收协议
    uint8_t play_audio[PLAY_AUDIO_COUNT_MAX];      // 需要播放的音频列表
} command_item_t;

/** 音量配置 **/
typedef enum {
    CTRL_MAXIMUM_VOLUME,   // 最大音量
    CTRL_MINIMUM_VOLUME,   // 最小音量
    CTRL_MEDIMUM_VOLUME,   // 中等音量
    CTRL_TURN_UP_VOLUME,   // 增加音量
    CTRL_TURN_DOWN_VOLUME, // 减小音量
} vol_ctrl_t;

typedef struct volume_cmd_item
{
    uint8_t keyword_size;   // 音量命令词拼音大小
    uint8_t send_pro_len;   // 发送协议大小
    uint8_t recv_pro_len;   // 接收协议大小
	uint8_t audio_size;     // 需要播放的音频列表的大小
	vol_ctrl_t ctrl_type;   // 音量操作类型
    uint8_t play_type;      // 主动还是被动0-主动，非0-被动
	int8_t keywords[ALGO_KEYWORD_MAX_LEN];         // 音量命令词拼音
    int8_t send_pro_buf[PROTOCOL_DATA_LEN_MAX];    // 发送协议
    int8_t recv_pro_buf[PROTOCOL_DATA_LEN_MAX];    // 接收协议
    uint8_t play_audio[PLAY_AUDIO_COUNT_MAX];      // 需要播放的音频列表
} volume_cmd_item_t;

typedef struct volume_item
{
    uint8_t level[VOLUME_MAX_LEVEL];            // 音量等级0-100进行划分
	uint8_t level_size;							// 音量等级数量 1~10
    uint8_t default_vol;						// 上电默认音量
    uint8_t adjmax_play_audio[PLAY_AUDIO_COUNT_MAX];// 调节到最大音量需要播放的音频
	uint8_t adjmax_play_audio_size;                 // 调节到最大音量需要播放的音频大小
    uint8_t adjmin_play_audio[PLAY_AUDIO_COUNT_MAX];// 调节到最小音量需要播放的音频
	uint8_t adjmin_play_audio_size;                 // 调节到最小音量需要播放的音频大小
	volume_cmd_item_t vol_cmd_items[VOLUME_CTRL_CMD_CNT_MAX];// 音量控制相关命令列表
	uint8_t vol_cmd_item_size;						// 音量控制相关命令列表数量
} volume_item_t;

// @brief 退出识别模式配置项
typedef struct quit_asr_item
{
    uint8_t keyword_size;   // 退出识别模式命令词拼音大小
	uint8_t audio_size;     // 需要播放的音频列表的大小
    uint8_t send_pro_len;   // 发送协议大小
    uint8_t recv_pro_len;   // 接收协议大小
    uint8_t play_type;      // 主动还是被动0-主动，非0-被动
	int8_t keywords[ALGO_KEYWORD_MAX_LEN];     // 退出识别模式拼音
    uint8_t play_audio[PLAY_AUDIO_COUNT_MAX];  // 需要播放的音频列表
    int8_t send_pro_buf[PROTOCOL_DATA_LEN_MAX];// 发送协议
    int8_t recv_pro_buf[PROTOCOL_DATA_LEN_MAX];// 接收协议
} quit_asr_item_t;

// @brief 恢复出厂设置配置项
typedef struct reset_item
{
    uint8_t keyword_size;   // 恢复出厂设置开关命令词拼音大小
	uint8_t audio_size;     // 需要播放的音频列表的大小
    uint8_t send_pro_len;   // 发送协议大小
    uint8_t recv_pro_len;   // 接收协议大小
    uint8_t play_type;      // 主动还是被动0-主动，非0-被动
	int8_t keywords[ALGO_KEYWORD_MAX_LEN];     // 恢复出厂设置命令词拼音
    uint8_t play_audio[PLAY_AUDIO_COUNT_MAX];  // 需要播放的音频列表
    int8_t send_pro_buf[PROTOCOL_DATA_LEN_MAX];// 发送协议
    int8_t recv_pro_buf[PROTOCOL_DATA_LEN_MAX];// 接收协议
} reset_item_t;

// @brief 语音功能配置项
typedef enum
{
    VOICE_FUNCTION_OPEN,    // 语音功能开
    VOICE_FUNCTION_CLOSE,   // 语音功能关
} voice_type_t;

typedef struct voice_item
{
    uint8_t keyword_size;   // 语音开关命令词拼音大小
	uint8_t audio_size;     // 需要播放的音频列表的大小
	voice_type_t type;      // 语音操作类型 voice_type_t
    uint8_t send_pro_len;   // 发送协议大小
    uint8_t recv_pro_len;   // 接收协议大小
    uint8_t play_type;      // 主动还是被动0-主动，非0-被动
	int8_t keywords[ALGO_KEYWORD_MAX_LEN];     // 语音开关命令词拼音
    uint8_t play_audio[PLAY_AUDIO_COUNT_MAX];  // 需要播放的音频列表
    int8_t send_pro_buf[PROTOCOL_DATA_LEN_MAX];// 发送协议
    int8_t recv_pro_buf[PROTOCOL_DATA_LEN_MAX];// 接收协议
} voice_item_t;

// @brief 心跳配置项
typedef struct heartbeat_item
{
    uint8_t send_pro_len;   // 心跳发送协议大小
    uint8_t recv_pro_len;   // 心跳接收协议大小
    int8_t send_pro_buf[PROTOCOL_DATA_LEN_MAX];    // 心跳发送协议
    int8_t recv_pro_buf[PROTOCOL_DATA_LEN_MAX];    // 心跳接收协议
} heartbeat_item_t;

/**********************************************************/
/********************  唤醒词切换配置  *********************/
/**********************************************************/

// @brief 唤醒词切换类型
typedef enum
{
    WAKEUP_SWITCH_TYPE_UNKNOW = 0,
    WAKEUP_SWITCH_TYPE_VOICE,       // 语音切换
    WAKEUP_SWITCH_TYPE_PROTOCOL     // 协议切换
} wakeup_switch_type_t;

// @brief 唤醒词切换模式
typedef enum
{
    WAKEUP_SWITCH_MODE_UNKNOW = 0,
    WAKEUP_SWITCH_MODE_LOOP,    // 循环切换
    WAKEUP_SWITCH_MODE_ROLLCALL //指定切换
} wakeup_switch_mode_t;

// @brief 多唤醒词切换具体切换命令
typedef struct
{
    uint8_t keyword_size;   // 唤醒词拼音大小
    uint8_t send_pro_len;   // 发送协议大小
    uint8_t recv_pro_len;   // 接收协议大小
	uint8_t audio_size;     // 需要播放的音频列表的大小
    uint8_t play_type;      // 主动还是被动0-主动，非0-被动
    int8_t keywords[ALGO_KEYWORD_MAX_LEN];         // 唤醒词拼音
    int8_t send_pro_buf[PROTOCOL_DATA_LEN_MAX];    // 发送协议
    int8_t recv_pro_buf[PROTOCOL_DATA_LEN_MAX];    // 接收协议
    uint8_t play_audio[PLAY_AUDIO_COUNT_MAX];      // 需要播放的音频列表
} wakeup_switch_info_t;

// @brief 多唤醒词切换查询命令
typedef struct
{
    uint8_t keyword_size;   // 唤醒词拼音大小
    uint8_t send_pro_len;   // 发送协议大小
    uint8_t recv_pro_len;   // 接收协议大小
	uint8_t audio_size;     // 需要播放的音频列表的大小
    uint8_t play_type;      // 主动还是被动0-主动，非0-被动
    int8_t keywords[ALGO_KEYWORD_MAX_LEN];         // 唤醒词拼音
    int8_t send_pro_buf[PROTOCOL_DATA_LEN_MAX];    // 发送协议
    int8_t recv_pro_buf[PROTOCOL_DATA_LEN_MAX];    // 接收协议
    uint8_t play_audio[PLAY_AUDIO_COUNT_MAX];      // 需要播放的音频列表
} wakeup_query_info_t;

// @brief 多唤醒词切换恢复命令
typedef struct
{
    uint8_t keyword_size;   // 唤醒词拼音大小
    uint8_t send_pro_len;   // 发送协议大小
    uint8_t recv_pro_len;   // 接收协议大小
	uint8_t audio_size;     // 需要播放的音频列表的大小
    uint8_t play_type;      // 主动还是被动0-主动，非0-被动
    int8_t keywords[ALGO_KEYWORD_MAX_LEN];     // 唤醒词拼音
    int8_t send_pro_buf[PROTOCOL_DATA_LEN_MAX];// 发送协议
    int8_t recv_pro_buf[PROTOCOL_DATA_LEN_MAX];// 接收协议
    uint8_t play_audio[PLAY_AUDIO_COUNT_MAX];  // 需要播放的音频列表
} wakeup_restore_info_t;

// @brief 单个唤醒词切换项(是否为默认唤醒词、是否常驻等)
typedef struct
{
    uint8_t default_keyword;    //是否是默认唤醒词
    uint8_t keyword_size;       // 唤醒词拼音大小
    uint8_t send_pro_len;       // 发送协议大小
    uint8_t recv_pro_len;       // 接收协议大小
	uint8_t audio_size;         // 需要播放的音频列表的大小
    uint8_t play_type;          // 主动还是被动0-主动，非0-被动
    uint8_t frozen;             // 是否常驻
    int8_t keywords[ALGO_KEYWORD_MAX_LEN];     // 唤醒词拼音
    int8_t send_pro_buf[PROTOCOL_DATA_LEN_MAX];// 发送协议
    int8_t recv_pro_buf[PROTOCOL_DATA_LEN_MAX];// 接收协议
    uint8_t play_audio[PLAY_AUDIO_COUNT_MAX];  // 需要播放的音频列表
} wakeup_switch_item_t;

// @brief 多唤醒词切换信息结构体
typedef struct wakeup_switch_item
{
    uint8_t enable;
    wakeup_switch_type_t sw_type;
    wakeup_switch_mode_t sw_mode;
    uint8_t switch_list_size;
    wakeup_switch_info_t switch_info;
    wakeup_query_info_t query_info;
    wakeup_restore_info_t restore_info;
    wakeup_switch_item_t switch_list[WAKEWORD_SWITCH_LIST_MAX];
} multi_wakeup_item_t;

/**********************************************************/
/*********************  语音注册功能  **********************/
/**********************************************************/

// @brief 语音注册意图类型
typedef enum {
    ACTION_TYPE_UNKNOW = -1,
    ACTION_TYPE_STUDY_WAKE,     // 学习唤醒词
    ACTION_TYPE_STUDY_CMD,      // 学习命令词
    ACTION_TYPE_STUDY_NEXT,     // 学习下一个
    ACTION_TYPE_STUDY_RELEARN,  // 重新学习
    ACTION_TYPE_STUDY_QUIT,     // 退出学习
    ACTION_TYPE_DEL_WAKE,       // 删除唤醒词
    ACTION_TYPE_DEL_CMD,        // 删除命令词
    ACTION_TYPE_DEL_ALL_CMD,    // 删除所有命令词
    ACTION_TYPE_DEL_ALL,        // 删除所有词
    ACTION_TYPE_DEL_QUIT        // 退出删除
} action_type_t;

// @brief 播报提示标签
typedef enum {
    REPLY_LABEL_TYPE_TIP_UNKNOW = -1,
    REPLY_LABEL_TYPE_TIP_RECORD_SUCCESS,    //当录入成功时
    REPLY_LABEL_TYPE_TIP_RECORD_FAILED,     //当录入失败时
    REPLY_LABEL_TYPE_TIP_STUDY_SUCCESS,     //当学习成功时
    REPLY_LABEL_TYPE_TIP_STUDY_COMPLETE,    //当学习完成时
    REPLY_LABEL_TYPE_TIP_STUDY_FAILED,      //当学习失败时
    REPLY_LABEL_TYPE_TIP_TEMPLATE_FULL,     //当超过数量时
    REPLY_LABEL_TYPE_TIP_LENGTH_ERROR,      //当长度异常时
    REPLY_LABEL_TYPE_TIP_SPEED_ERROR,       //当语速异常时
    REPLY_LABEL_TYPE_TIP_SIMILA_ERROR,      //当前后不一致时
    REPLY_LABEL_TYPE_TIP_COMMAND_CONFLICT,  //当指令冲突时
    REPLY_LABEL_TYPE_TIP_DELETE_FAILED,     //当删除错误时
    REPLY_LABEL_TYPE_TIP_DELETE_WAKE_START, //删除唤醒词开始
    REPLY_LABEL_TYPE_TIP_DELETE_WAKE_SUCCESS,//删除唤醒词成功
    REPLY_LABEL_TYPE_TIP_DELETE_CMD_START,      //删除命令词开始
    REPLY_LABEL_TYPE_TIP_DELETE_CMD_SUCCESS,    //删除命令词成功
    REPLY_LABEL_TYPE_TIP_DELETE_ALL_START,      //删除所有开始
    REPLY_LABEL_TYPE_TIP_DELETE_ALL_SUCCESS,    //删除所有成功
    REPLY_LABEL_TYPE_TIP_ASR_OVV,               //该指令不支持学习或删除，请换个指令
    REPLY_LABEL_TYPE_TIP_DELETE_ALL_CMD_SUCCESS,//命令词全部删除成功
    REPLY_LABEL_TYPE_TIP_ASR_START,         //
    REPLY_LABEL_TYPE_TIP_WAKEUP_START,      //
    REPLY_LABEL_TYPE_TIP_ASR_OVER_TAIL,     //
    REPLY_LABEL_TYPE_TIP_ASR_OVER_HEAD,     //
} reply_label_t;

// @brief 语音注册用户配置项
typedef struct user_cfg_item
{
    uint8_t wake_sensitivity;   // 注册的唤醒词灵敏度 0 1 2
    uint8_t wake_min_words;     // 注册的唤醒词长度最小值
    uint8_t wake_max_words;     // 注册的唤醒词长度最大值
    uint8_t wake_reg_max;       // 最大可注册唤醒词数量
    uint8_t wake_repeat_count;  // 注册唤醒词重复次数
    uint8_t wake_retry_count;   // 注册唤醒词错误重试次数
    uint8_t cmd_sensitivity;    // 注册的命令词灵敏度 0 1 2
    uint8_t cmd_min_words;      // 注册的命令词长度最小值
    uint8_t cmd_max_words;      // 注册的命令词长度最大值
    uint8_t cmd_reg_max;        // 最大可注册命令词数量
    uint8_t cmd_repeat_count;   // 注册命令词重复次数
    uint8_t cmd_retry_count;    // 注册命令词错误重试次数
} user_cfg_item_t;

// @brief 其他播报提示音项
typedef struct other_replys_item
{
    reply_label_t label_type;   // 播报提示标签
    uint8_t audio_size;         // 音频列表的大小
    uint8_t play_audio[PLAY_AUDIO_COUNT_MAX];  // 音频列表
} other_replys_item_t;

// @brief 语音注册动作项
typedef struct control_cmd_item
{
	action_type_t action_type;  // 动作类型
    uint8_t keyword_size;       // 命令词拼音大小
	uint8_t audio_size;         // 音频列表的大小
    int8_t keywords[ALGO_KEYWORD_MAX_LEN];     // 命令词拼音
    uint8_t play_audio[PLAY_AUDIO_COUNT_MAX];  // 音频列表
} control_cmd_item_t;

// @brief 语音注册词项
typedef struct reg_word_item
{
    uint8_t keyword_size;   // 命令词拼音大小
	uint8_t audio_size;     // 音频列表的大小
	uint8_t del_audio_size; // 删除词音频列表的大小
    int8_t keywords[ALGO_KEYWORD_MAX_LEN];     // 命令词拼音
    uint8_t play_audio[PLAY_AUDIO_COUNT_MAX];  // 音频列表
    uint32_t del_play_audio[PLAY_AUDIO_COUNT_MAX];  // 删除词音频列表
} reg_word_item_t;

// @brief 语音注册配置项
typedef struct study_item
{
    uint8_t enable;                 // 是否使能语音注册功能
    uint8_t type;                   // 1 连续学习 2 指定学习
    uint8_t mode;                   // 预留
    user_cfg_item_t user_cfg;       // 用户配置项
	uint8_t control_cmd_item_size;  // 提示音长度
    uint8_t other_replys_item_size; // 其他播报提示音长度
    uint8_t reg_wakewords_item_size;// 注册唤醒词长度
    uint8_t reg_commands_item_size; // 注册命令词长度
	control_cmd_item_t control_cmd_items[VOICEREG_CONTROL_CMD_COUNT_MAX]; // 语音注册动作列表
	other_replys_item_t other_replys_items[VOICEREG_OTHER_REPLY_COUNT_MAX]; // 其他播报提示音列表
    reg_word_item_t reg_wakewords_items[VOICEREG_REGWORD_COUNT_MAX]; // 注册唤醒词列表
    reg_word_item_t reg_commands_items[VOICEREG_REGWORD_COUNT_MAX];  // 注册命令词列表
} study_item_t;

#endif
#ifndef __LISTENAI_CONFIG_USER_H__
#define __LISTENAI_CONFIG_USER_H__

#include "ls_app_cfg_def.h"
#include "../tone/tone.h"

/* PA 配置 */
const pa_config_item_t pa_config = {
    .enable = 0,
    .ctl_io_pad = CONTROL_IO_PAD_B,        /* PB */
    .ctl_io_num = 11,
    .enable_level = 1,                      /* high */
    .hold_time = 20000
};

/* 串口配置 */
const uart_config_t uart_config = {
    .trace_uart = 0,
    .uport_uart = 1,
    .trace_baud = 115200,
    .uport_baud = 9600
};

/* 通用配置 */
const general_item_t general_config = {
    .version = "V-2026.04.21_20.",
    .log_level = 1,
    .persisted_volume = 0,
    .persisted_voice = 1,
    .persisted_wakeup = 0,
    .reply_switch = REPLY_SWITCH_ON,        /* 1 */
    .adc_again = 36,
    .adc_dgain = 6
};

/* 欢迎语配置（空） */
const welcome_item_t welcome = {};

/* 超时配置（空） */
const timeout_item_t timeout = {};

/* 唤醒词配置数组 */
const wakeup_item_t wakeup_items[] = {
    {
        .keyword_size = 16,                  /* "xiao ai tong xue" 长度 */
        .send_pro_len = 0,
        .audio_size = 1,
        .play_type = 0,
        .keywords = "xiao ai tong xue",
        .send_pro_buf = {},
        .play_audio = {TONE_ID_0}
    },
};

/* 命令词配置数组 */
const command_item_t command_items[] = {
    {
        .keyword_size = 18,                    /* "bian wei hong deng" 长度 */
        .send_pro_len = 0,
        .recv_pro_len = 0,
        .audio_size = 1,
        .play_type = 0,
        .keywords = "bian wei hong deng",
        .send_pro_buf = {},
        .recv_pro_buf = {},
        .play_audio = {TONE_ID_1}
    },
    {
        .keyword_size = 17,                    /* "bian wei zhi xing" 长度 */
        .send_pro_len = 0,
        .recv_pro_len = 0,
        .audio_size = 1,
        .play_type = 0,
        .keywords = "bian wei zhi xing",
        .send_pro_buf = {},
        .recv_pro_buf = {},
        .play_audio = {TONE_ID_2}
    },
    {
        .keyword_size = 18,                    /* "bian wei zuo zhuan" 长度 */
        .send_pro_len = 0,
        .recv_pro_len = 0,
        .audio_size = 1,
        .play_type = 0,
        .keywords = "bian wei zuo zhuan",
        .send_pro_buf = {},
        .recv_pro_buf = {},
        .play_audio = {TONE_ID_3}
    },
    {
        .keyword_size = 18,                    /* "bian wei you zhuan" 长度 */
        .send_pro_len = 0,
        .recv_pro_len = 0,
        .audio_size = 1,
        .play_type = 0,
        .keywords = "bian wei you zhuan",
        .send_pro_buf = {},
        .recv_pro_buf = {},
        .play_audio = {TONE_ID_4}
    },
};

/* 音量配置 */
const volume_item_t volume = {
    .level = {10, 37, 58, 79, 100},
    .level_size = 5,
    .default_vol = 4,
    .adjmax_play_audio = {TONE_ID_8},
    .adjmax_play_audio_size = 1,
    .adjmin_play_audio = {TONE_ID_9},
    .adjmin_play_audio_size = 1,
    .vol_cmd_items = {
        {
            .keyword_size = 13,                  /* "da dian sheng" 长度 */
            .send_pro_len = 0,
            .recv_pro_len = 0,
            .audio_size = 1,
            .ctrl_type = CTRL_TURN_UP_VOLUME,
            .play_type = 0,
            .keywords = "da dian sheng",
            .send_pro_buf = {},
            .recv_pro_buf = {},
            .play_audio = {TONE_ID_5}
        },
        {
            .keyword_size = 15,                  /* "xiao dian sheng" 长度 */
            .send_pro_len = 0,
            .recv_pro_len = 0,
            .audio_size = 1,
            .ctrl_type = CTRL_TURN_DOWN_VOLUME,
            .play_type = 0,
            .keywords = "xiao dian sheng",
            .send_pro_buf = {},
            .recv_pro_buf = {},
            .play_audio = {TONE_ID_6}
        },
    },
    .vol_cmd_item_size = 2
};

/* 退出识别模式配置数组 */
const quit_asr_item_t quit_asr_items[] = {
    {
        .keyword_size = 15,                  /* "tui chu shi bie" 长度 */
        .audio_size = 1,
        .send_pro_len = 0,
        .recv_pro_len = 0,
        .play_type = 0,
        .keywords = "tui chu shi bie",
        .play_audio = {TONE_ID_7},
        .send_pro_buf = {},
        .recv_pro_buf = {}
    },
};

/* 恢复出厂设置配置数组（空） */
const reset_item_t reset_items[] = {};

/* 语音功能配置数组（空） */
const voice_item_t voice_items[] = {};

/* 心跳配置数组（空） */
const heartbeat_item_t heartbeat_items[] = {};

/* 多唤醒词切换配置 */
const multi_wakeup_item_t multi_wakeup = {
    .enable = 0,
    .sw_type = (0),      /* 0 */
    .sw_mode = (0),       /* 0 */
    .switch_info = {
        .keyword_size = 0,                    /* "" 长度 */
        .send_pro_len = 0,
        .recv_pro_len = 0,
        .audio_size = 0,
        .play_type = 0,
        .keywords = "",
        .send_pro_buf = {},
        .recv_pro_buf = {},
        .play_audio = {}
    },
    .query_info = {
        .keyword_size = 0,                    /* "" 长度 */
        .send_pro_len = 0,
        .recv_pro_len = 0,
        .audio_size = 0,
        .play_type = 0,
        .keywords = "",
        .send_pro_buf = {},
        .recv_pro_buf = {},
        .play_audio = {}
    },
    .restore_info = {
        .keyword_size = 0,                    /* "" 长度 */
        .send_pro_len = 0,
        .recv_pro_len = 0,
        .audio_size = 0,
        .play_type = 0,
        .keywords = "",
        .send_pro_buf = {},
        .recv_pro_buf = {},
        .play_audio = {}
    },
    .switch_list_size = 0,
    .switch_list = {
    }
};

/* 语音注册配置 */
const study_item_t study_config = {
    .enable = 0,
    .type = 0,                          /* "" → 0 */
    .mode = 0,                          /* "" → 0 */
    .user_cfg = {
        .wake_sensitivity = 1,           /* "中" → 1 */
        .wake_min_words = 0,
        .wake_max_words = 0,
        .wake_reg_max = 0,
        .wake_repeat_count = 0,
        .wake_retry_count = 0,
        .cmd_sensitivity = 1,            /* "中" → 1 */
        .cmd_min_words = 0,
        .cmd_max_words = 0,
        .cmd_reg_max = 0,
        .cmd_repeat_count = 0,
        .cmd_retry_count = 0
    },
    .control_cmd_item_size = 0,
    .other_replys_item_size = 0,
    .reg_wakewords_item_size = 0,
    .reg_commands_item_size = 0,
    .control_cmd_items = {
    },
    .other_replys_items = {
    },
    .reg_wakewords_items = {
    },
    .reg_commands_items = {
    }
};

#endif /* __LISTENAI_CONFIG_USER_H__ */
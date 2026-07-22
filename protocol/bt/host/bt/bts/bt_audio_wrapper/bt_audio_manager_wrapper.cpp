/*
 * Copyright (c) @CompanyNameMagicTag 2022. All rights reserved.
 * Description: audio manager wrapper api.
 */

#include "bt_audio_manager_wrapper.h"

#include <iostream>
#include <memory>

#include "audio_manager.h"
#include "audio_stream_in.h"
#include "audio_stream_out.h"
#include "audio_base_type.h"

using Audio::AudioManager;
using Audio::AudioStreamIn;
using Audio::AudioStreamOut;
using Audio::InterruptListener;
using Audio::AudioInterrupt;

#ifdef __cplusplus
extern "C" {
#endif

const int SUCCESS = 0;
const int FAILURE = (-1);

const int AUDIO_CHANNEL_1 = 1;
const int AUDIO_SAMPLE_RATE_16K = 16000;
const int AUDIO_SAMPLE_RATE_8K = 8000;
const int AUDIO_STREAM_MAX_INSTANCE_NUM = 2;

static AudioManager& g_am_instance = AudioManager::GetInstance();
static std::shared_ptr<AudioStreamIn> g_stream_in[AUDIO_STREAM_MAX_INSTANCE_NUM];
static std::shared_ptr<AudioStreamOut> g_stream_out[AUDIO_STREAM_MAX_INSTANCE_NUM];
static AudioSession g_session_id;
static AudioInterrupt g_interrupt;
static AudioStreamType g_stream_type = AUDIO_STREAM_INVALID;

static bool check_param(int instance_num)
{
    if (instance_num == 0 || instance_num == 1) {
        return true;
    }
    return false;
}

class SampleInterruptListener : public InterruptListener {
public:
    SampleInterruptListener(){};
    ~SampleInterruptListener(){};

    void OnInterrupt(int32_t type, int32_t hint) override
    {
        printf("Callback type: %d hint: %d\n", type, hint);
    }
};

int audio_manager_init(AudioStreamType stream_type)
{
    /* audio manager init */
    bool success = g_am_instance.Initialize(); // load audio adapter
    if (!success) {
        printf("func:%s, line:%d, audio manager init failed\n", __FUNCTION__, __LINE__);
        return FAILURE;
    }

    printf("func:%s, line:%d, success\n", __FUNCTION__, __LINE__);
    return SUCCESS;
}

int audio_manager_activate_interrupt(AudioStreamType stream_type)
{
    /* request audio focus */
    g_session_id = g_am_instance.MakeSessionId();
    if (g_session_id == AUDIO_SESSION_ID_NONE) {
        printf("func:%s, line:%d, make session id failed\n", __FUNCTION__, __LINE__);
        return FAILURE;
    }
    printf("func:%s, line:%d, session id = %u\n", __FUNCTION__, __LINE__, g_session_id);
    std::shared_ptr<SampleInterruptListener> sample_interrupt_listener = std::make_shared<SampleInterruptListener>();
    g_stream_type = stream_type;
    g_interrupt = { stream_type, g_session_id, sample_interrupt_listener };
    if (g_am_instance.ActivateAudioInterrupt(g_interrupt) == INTERRUPT_FAILED) {
        printf("func:%s, line:%d, ActivateAudioInterrupt faild\n", __FUNCTION__, __LINE__);
        return FAILURE;
    }
    printf("func:%s, line:%d, success\n", __FUNCTION__, __LINE__);
    return SUCCESS;
}

int audio_manager_set_device_connection_state(AudioDeviceType device_type, AudioDeviceConnectState connect_state)
{
    int ret = g_am_instance.SetDeviceConnectionState(device_type, connect_state);
    if (ret != SUCCESS) {
        printf("func:%s, line:%d, device 0x%x connect %d failed: 0x%x\n", __FUNCTION__, __LINE__, device_type,
            connect_state, ret);
        return FAILURE;
    }

    printf("func:%s, line:%d, device 0x%x connect %d success\n", __FUNCTION__, __LINE__, device_type, connect_state);
    return SUCCESS;
}

bool audio_manager_set_volume(AudioStreamType stream_type, int32_t volume)
{
    return g_am_instance.SetVolume(stream_type, volume);
}

int32_t audio_manager_get_volume(AudioStreamType stream_type)
{
    return g_am_instance.GetVolume(stream_type);
}

int audio_stream_in_init(AudioLinkDirection link_dir, AudioCodecFormat codec)
{
    int instance_num = -1;

    /* create ASI instance UP_LINK: index 0,  DOWN_LINK: index 1 */
    if (link_dir == AUDIO_UP_LINK) {
        g_stream_in[0] = std::make_shared<AudioStreamIn>();
        instance_num = 0;
    } else if (link_dir == AUDIO_DOWN_LINK) {
        g_stream_in[1] = std::make_shared<AudioStreamIn>();
        instance_num = 1;
    } else {
        printf("func:%s, line:%d, invalid link_dir %d\n", __FUNCTION__, __LINE__, link_dir);
        return FAILURE;
    }

    if (g_stream_in[instance_num] == nullptr || g_stream_in[instance_num].get() == nullptr) {
        printf("func:%s, line:%d, make_shared AudioStreamIn failed\n", __FUNCTION__, __LINE__);
        return FAILURE;
    }

    /* init ASI */
    CapturerInputConfig capture_config = {};
    capture_config.audioFormat = codec;
    if (codec == PCM) {
        capture_config.sampleRate = AUDIO_SAMPLE_RATE_8K;
    } else if (codec == mSBC) {
        capture_config.sampleRate = AUDIO_SAMPLE_RATE_16K;
    }
    capture_config.channelCount = AUDIO_CHANNEL_1;
    capture_config.sessionID = (int32_t)g_session_id;
    capture_config.streamType = g_stream_type;
    capture_config.linkDir = link_dir;
    int ret = g_stream_in[instance_num]->Init(capture_config);
    if (ret != SUCCESS) {
        printf("func:%s, line:%d, streamIn init failed: 0x%x\n", __FUNCTION__, __LINE__, ret);
        return FAILURE;
    }

    printf("func:%s, line:%d success\n", __FUNCTION__, __LINE__);
    return instance_num;
}

int audio_stream_out_init(AudioLinkDirection link_dir, AudioCodecFormat codec)
{
    int instance_num = -1;

    /* create ASO instance UP_LINK: index 0,  DOWN_LINK: index 1 */
    if (link_dir == AUDIO_UP_LINK) {
        g_stream_out[0] = std::make_shared<AudioStreamOut>();
        instance_num = 0;
    } else if (link_dir == AUDIO_DOWN_LINK) {
        g_stream_out[1] = std::make_shared<AudioStreamOut>();
        instance_num = 1;
    } else {
        printf("func:%s, line:%d, invalid link_dir %d\n", __FUNCTION__, __LINE__, link_dir);
        return FAILURE;
    }

    if (g_stream_out[instance_num] == nullptr || g_stream_out[instance_num].get() == nullptr) {
        printf("func:%s, line:%d, make_shared AudioStreamOut failed\n", __FUNCTION__, __LINE__);
        return FAILURE;
    }

    /* init ASO */
    AudioRendererConfig render_config = {};
    render_config.audioFormat = codec;
    if (codec == PCM) {
        render_config.sampleRate = AUDIO_SAMPLE_RATE_8K;
    } else if (codec == mSBC) {
        render_config.sampleRate = AUDIO_SAMPLE_RATE_16K;
    }
    render_config.channelCount = AUDIO_CHANNEL_1;
    render_config.sessionID = (int32_t)g_session_id;
    render_config.streamType = g_stream_type;
    render_config.linkDir = link_dir;
    int ret = g_stream_out[instance_num]->Init(render_config);
    if (ret != SUCCESS) {
        printf("func:%s, line:%d, streamOut init failed: 0x%x\n", __FUNCTION__, __LINE__, ret);
        return FAILURE;
    }
    /* dump stream */
    g_stream_out[instance_num]->DumpInfo();

    printf("func:%s, line:%d success\n", __FUNCTION__, __LINE__);
    return instance_num;
}

int audio_stream_in_start(int instance_num)
{
    bool is_valid = check_param(instance_num);
    if (!is_valid) {
        printf("func:%s, line:%d instance_num %d is invalid\n", __FUNCTION__, __LINE__, instance_num);
        return FAILURE;
    }

    /* start ASI */
    int ret = g_stream_in[instance_num]->Start();
    if (ret != SUCCESS) {
        printf("func:%s, line:%d, streamIn Start failed: 0x%x\n", __FUNCTION__, __LINE__, ret);
        return FAILURE;
    }

    printf("func:%s, line:%d success\n", __FUNCTION__, __LINE__);
    return SUCCESS;
}

int audio_stream_out_start(int instance_num)
{
    bool is_valid = check_param(instance_num);
    if (!is_valid) {
        printf("func:%s, line:%d instance_num %d is invalid\n", __FUNCTION__, __LINE__, instance_num);
        return FAILURE;
    }

    /* start ASO */
    int ret = g_stream_out[instance_num]->Play();
    if (ret != SUCCESS) {
        printf("func:%s, line:%d, streamOut Play failed: 0x%x\n", __FUNCTION__, __LINE__, ret);
        return FAILURE;
    }

    printf("func:%s, line:%d success\n", __FUNCTION__, __LINE__);
    return SUCCESS;
}

int audio_stream_in_get_channel_id(int instance_num, uint32_t *shm_id)
{
    bool is_valid = check_param(instance_num);
    if (!is_valid) {
        printf("func:%s, line:%d instance_num %d is invalid\n", __FUNCTION__, __LINE__, instance_num);
        return FAILURE;
    }

    /* obtain ASI channel id(adp handle) */
    uint32_t channel_id;
    int ret = g_stream_in[instance_num]->GetCurrentChannelId(channel_id);
    if (ret != SUCCESS) {
        printf("func:%s, line:%d, streamIn GetCurrentChannelId failed: 0x%x\n", __FUNCTION__, __LINE__, ret);
        return FAILURE;
    }

    printf("func:%s, line:%d, stream in channel_id: 0x%x\n", __FUNCTION__, __LINE__, channel_id);

    *shm_id = channel_id;
    printf("func:%s, line:%d success\n", __FUNCTION__, __LINE__);
    return SUCCESS;
}

int audio_stream_out_get_channel_id(int instance_num, uint32_t *shm_id)
{
    bool is_valid = check_param(instance_num);
    if (!is_valid) {
        printf("func:%s, line:%d instance_num %d is invalid\n", __FUNCTION__, __LINE__, instance_num);
        return FAILURE;
    }

    /* obtain ASO channel id(adp handle) */
    uint32_t channel_id;
    int ret = g_stream_out[instance_num]->GetCurrentChannelId(channel_id);
    if (ret != SUCCESS) {
        printf("func:%s, line:%d, streamOut GetCurrentChannelId failed: 0x%x\n", __FUNCTION__, __LINE__, ret);
        return FAILURE;
    }

    printf("func:%s, line:%d, stream out channel_id: 0x%x\n", __FUNCTION__, __LINE__, channel_id);

    *shm_id = channel_id;
    printf("func:%s, line:%d success\n", __FUNCTION__, __LINE__);
    return SUCCESS;
}

int audio_stream_in_attach_backend(int instance_num, uint32_t backend_id)
{
    bool is_valid = check_param(instance_num);
    if (!is_valid) {
        printf("func:%s, line:%d instance_num %d is invalid\n", __FUNCTION__, __LINE__, instance_num);
        return FAILURE;
    }

    /* ASI set shm id */
    int ret = g_stream_in[instance_num]->AttachBackend(backend_id);
    if (ret != SUCCESS) {
        printf("func:%s, line:%d, streamIn AttachBackend failed: 0x%x\n", __FUNCTION__, __LINE__, ret);
        return FAILURE;
    }

    printf("func:%s, line:%d, stream in backend_id: 0x%x\n", __FUNCTION__, __LINE__, backend_id);

    printf("func:%s, line:%d success\n", __FUNCTION__, __LINE__);
    return SUCCESS;
}

int audio_stream_out_attach_frontend(int instance_num, uint32_t frontend_id)
{
    bool is_valid = check_param(instance_num);
    if (!is_valid) {
        printf("func:%s, line:%d instance_num %d is invalid\n", __FUNCTION__, __LINE__, instance_num);
        return FAILURE;
    }

    /* ASO set shm id */
    int ret = g_stream_out[instance_num]->AttachFrontend(frontend_id);
    if (ret != SUCCESS) {
        printf("func:%s, line:%d, streamOut AttachFrontend failed: 0x%x\n", __FUNCTION__, __LINE__, ret);
        return FAILURE;
    }

    printf("func:%s, line:%d, stream out frontend_id: 0x%x\n", __FUNCTION__, __LINE__, frontend_id);

    printf("func:%s, line:%d success\n", __FUNCTION__, __LINE__);
    return SUCCESS;
}

int audio_stream_in_stop(int instance_num)
{
    bool is_valid = check_param(instance_num);
    if (!is_valid) {
        printf("func:%s, line:%d instance_num %d is invalid\n", __FUNCTION__, __LINE__, instance_num);
        return FAILURE;
    }

    /* stop ASI */
    int ret = g_stream_in[instance_num]->Stop();
    if (ret != SUCCESS) {
        printf("func:%s, line:%d, streamIn Stop failed: 0x%x\n", __FUNCTION__, __LINE__, ret);
    }

    printf("func:%s, line:%d success\n", __FUNCTION__, __LINE__);
    return SUCCESS;
}

int audio_stream_out_stop(int instance_num)
{
    bool is_valid = check_param(instance_num);
    if (!is_valid) {
        printf("func:%s, line:%d instance_num %d is invalid\n", __FUNCTION__, __LINE__, instance_num);
        return FAILURE;
    }

    /* stop ASO */
    int ret = g_stream_out[instance_num]->Stop();
    if (ret != SUCCESS) {
        printf("func:%s, line:%d, streamOut Stop failed: 0x%x\n", __FUNCTION__, __LINE__, ret);
    }

    printf("func:%s, line:%d success\n", __FUNCTION__, __LINE__);
    return SUCCESS;
}

int audio_stream_in_deinit(int instance_num)
{
    bool is_valid = check_param(instance_num);
    if (!is_valid) {
        printf("func:%s, line:%d instance_num %d is invalid\n", __FUNCTION__, __LINE__, instance_num);
        return FAILURE;
    }

    /* deinit ASI */
    if (g_stream_in[instance_num] != nullptr) {
        g_stream_in[instance_num]->Release();
        g_stream_in[instance_num] = nullptr;
    }

    printf("func:%s, line:%d success\n", __FUNCTION__, __LINE__);
    return SUCCESS;
}

int audio_stream_out_deinit(int instance_num)
{
    bool is_valid = check_param(instance_num);
    if (!is_valid) {
        printf("func:%s, line:%d instance_num %d is invalid\n", __FUNCTION__, __LINE__, instance_num);
        return FAILURE;
    }

    /* deinit ASO */
    if (g_stream_out[instance_num] != nullptr) {
        g_stream_out[instance_num]->Release();
        g_stream_out[instance_num] = nullptr;
    }

    printf("func:%s, line:%d success\n", __FUNCTION__, __LINE__);
    return SUCCESS;
}

int audio_manager_deactivate_interrupt(void)
{
    /* release audio focus */
    if (g_am_instance.DeactivateAudioInterrupt(g_interrupt) != SUCCESS) {
        return FAILURE;
    }
    printf("func:%s, line:%d, success\n", __FUNCTION__, __LINE__);
    return SUCCESS;
}

int audio_manager_deinit(void)
{
    printf("func:%s, line:%d success\n", __FUNCTION__, __LINE__);
    return SUCCESS;
}

bool audio_manager_mute(AudioStreamType streamType)
{
    return g_am_instance.Mute(streamType);
}

bool audio_manager_unmute(AudioStreamType streamType)
{
    return g_am_instance.UnMute(streamType);
}

#ifdef __cplusplus
}
#endif

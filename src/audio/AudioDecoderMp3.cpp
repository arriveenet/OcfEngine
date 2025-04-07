#include "AudioDecoderMp3.h"
#define MINIMP3_IMPLEMENTATION
#include "minimp3/minimp3_ex.h"
#include <stdio.h>

struct Mp3DecImpl {
    mp3dec_ex_t mp3dec;
    mp3dec_io_t mp3decIO;
};

static size_t readMiniMp3(void* buf, size_t size, void* user_data)
{
    auto stream = static_cast<std::ifstream*>(user_data);
    stream->read(static_cast<char*>(buf), size);

    return stream->gcount();
}

static int seekMiniMp3(uint64_t position, void* user_data)
{
    auto stream = static_cast<std::ifstream*>(user_data);
    // EOFの場合、clear()を呼び出して状態をリセットする
    // これにより、次の読み取り操作が正常に動作するようになります
    if (stream->eof()) {
        stream->clear();
    }

    stream->seekg(position, std::ios::beg);

    return 0;
}

NS_OCF_BEGIN

bool AudioDecoderMp3::open(std::string_view filename)
{
    do {
        m_fileStream = std::make_unique<std::ifstream>();
        m_fileStream->open(filename.data(), std::ios::binary);
        if (!m_fileStream->is_open()) {
            OCFLOG("Failed to open file: %s", filename.data());
            break;
        }

        auto handle = new Mp3DecImpl();

        handle->mp3decIO.read = readMiniMp3;
        handle->mp3decIO.seek = seekMiniMp3;
        handle->mp3decIO.read_data = m_fileStream.get();
        handle->mp3decIO.seek_data = m_fileStream.get();

        if (mp3dec_ex_open_cb(&handle->mp3dec, &handle->mp3decIO, MP3D_SEEK_TO_SAMPLE) != 0) {
            OCFLOG("Failed to open MP3 decoder");
            delete handle;
            break;
        }

        auto& info = handle->mp3dec.info;
        m_channelCount = info.channels;
        m_sampleRate = info.hz;
        m_sourceFormat = AudioSourceFormat::PCM_16;
        m_bytesPerBlock = m_channelCount * sizeof(int16_t);

        m_totalFrames = handle->mp3dec.samples / m_channelCount;

        m_handle = handle;

        m_isOpened = true;

        return true;
    } while (false);

    return false;
}

void AudioDecoderMp3::close()
{
    if (isOpened()) {
        if (m_handle != nullptr) {
            mp3dec_ex_close(&m_handle->mp3dec);
            delete m_handle;
            m_handle = nullptr;
            m_fileStream.reset();
        }

        m_isOpened = false;
    }
}

uint32_t AudioDecoderMp3::read(uint32_t framesToRead, char* pcmBuffer)
{
    auto samplesToRead = framesToRead * m_channelCount;
    auto samplesRead = mp3dec_ex_read(&m_handle->mp3dec,
                                      reinterpret_cast<mp3d_sample_t*>(pcmBuffer),
                                      samplesToRead);

    return static_cast<uint32_t>(samplesRead / m_channelCount);
}

bool AudioDecoderMp3::seek(uint32_t frameOffset)
{
    return mp3dec_ex_seek(&m_handle->mp3dec, frameOffset) == 0;
}

AudioDecoderMp3::AudioDecoderMp3()
    : m_handle(nullptr)
    , m_fileStream(nullptr)
{
}

AudioDecoderMp3::~AudioDecoderMp3()
{
    close();
}

NS_OCF_END
